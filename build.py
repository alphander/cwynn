# Version 1.8

# --> build without arguments for debug build

# arg option: debug            (default) option for debug build
# arg option: release          optimized for release

# arg: f or fast        uses multiple threads to build but silences warnings and errors
# arg: l or lib         creates library files in bin folder (does not need main function)
# arg: c or clean       removes all folders and files that were built
# arg: r or run         runs the program after build is complete


# dirs
SRC = "src"
LIB = "lib"
INC = "inc"
OBJ = "obj"
BIN = "bin"

# compiler and linker
CC = "clang"
AR = "ar"

# build output
OUT = "a"
RCS = ""
LIB_FOLDER = ""

# file endings
SRC_FILE_ENDING = ".c"
HEADER_FILE_ENDING = ".h"
OBJ_FILE_ENDING = ".o"
LIB_FILE_ENDING = ".a"

# include outside of project
INCLUDES = ""
LIBS = ""

# library flags 
LDFLAGS = {
    "all" : "-ltoolkit", 
    "nt" : "-lwininet",
    "posix" : "-pthread -lcurl"
}

# "all" is appended
# top one (besides "all") is default
CFLAGS = {
    "all" : "-Wall -Wno-unused-function -std=c17",
    "debug" : "-O0 -g -DDEBUG -fsanitize=address",
    "release" : "-O3 -DNDEBUG",
}

##############################################################################
# Code
#
#
#
#
#
#
#
#
#
##############################################################################

import os
import subprocess
import threading
import glob
import shutil
import re
import sys

##############################################################################
# Parameter post processing
#
#
##############################################################################

INCLUDES = f"-I./{INC} -I./{SRC} " + " ".join(INCLUDES.split())
LIBS += " ".join(["-L./" + l for l in glob.glob(f"{LIB}/**/", recursive=True)]) + " ".join(LIBS.split())
LDFLAGS = LDFLAGS["all"] + " " + LDFLAGS[os.name]

OUT = OUT + {"nt" : ".exe", "posix" : ".out"}[os.name]
RCS = f"{BIN}/{LIB}/{LIB_FOLDER}/{RCS}{LIB_FILE_ENDING}"

# CFLAGS
cflag_all = CFLAGS.pop("all")
for key, value in CFLAGS.items():
    CFLAGS[key] = f"{cflag_all} {value}"
        

##############################################################################
# File access
#
#
##############################################################################

def needs_recompile(resulting_file, dependency_files) -> bool:
    if not os.path.isfile(resulting_file):
        return True

    file_time = os.path.getmtime(resulting_file)
    for file in dependency_files:
        if os.path.getmtime(file) > file_time:
            return True

    return False


def find_files():
    object_files = []
    source_files = glob.glob(f"{SRC}/**/*{SRC_FILE_ENDING}", recursive=True)
    for file in source_files:
        object_file = os.path.splitext(os.path.basename(file))[0]
        object_files.append("".join([OBJ, os.path.sep, object_file, OBJ_FILE_ENDING]))
    return source_files, object_files

##############################################################################
# Compilation
#
#
##############################################################################

def compile(source_file, object_file, cflags):
    check_command = f"{CC} -MM {source_file} {cflags} {INCLUDES}"
    try:
        raw = subprocess.check_output(check_command).decode().split(": ")[1].split(SRC_FILE_ENDING)
    except subprocess.CalledProcessError as e:
        exit()

    sources = raw[0] + SRC_FILE_ENDING
    headers = raw[1].replace("\\\r\n  ", "").replace("\r\n", "").split(HEADER_FILE_ENDING)
    headers = [header.replace("\\", "").strip() + HEADER_FILE_ENDING for header in headers if header != ""]

    dependencies = [sources] + headers
    if not needs_recompile(object_file, dependencies):
        return

    command = f"{CC} -c {source_file} -o {object_file} {cflags} {INCLUDES}"
    print(command + "\n")
    result = subprocess.run(command)
    if result.returncode != 0:
        exit(0)
    

def link_exe(out, object_files, cflags, ldflags):
    if not needs_recompile(out, object_files):
        return

    object_files_string = " ".join(object_files)
    command = f"{CC} -o {out} {object_files_string} {cflags} {LIBS} {ldflags}"
    print(command)
    result = subprocess.run(command)
    if result.returncode != 0:
        exit(0)


def link_lib(rcs, object_files, ldflags):
    if not needs_recompile(rcs, object_files):
        return

    os.makedirs(os.path.dirname(rcs), exist_ok=True)

    object_files_string = " ".join(object_files)
    command = f"{AR} rcs {rcs} {object_files_string}"
    print(command)
    result = subprocess.run(command)
    if result.returncode != 0:
        exit(0)

##############################################################################
# Utility
#
#
##############################################################################

def clean():
    rcs = f"{BIN}/{LIB}/{LIB_FOLDER}"
    header_dir = f"{BIN}/{LIB}/{LIB_FOLDER}"

    object_files = glob.glob("".join([OBJ, "/*", OBJ_FILE_ENDING]))
    for file in object_files:
        if os.path.isfile(file):
            os.remove(file)
            print("- " + file)


    header_files = glob.glob("".join([f"{BIN}/{INC}/{LIB_FOLDER}", "/*", HEADER_FILE_ENDING]))
    for header in header_files:
        if os.path.isfile(file):
            os.remove(file)
            print("- " + file)
            
    if os.path.isfile(f"{BIN}/{OUT}"):
        os.remove(f"{BIN}/{OUT}")
        print("- " + f"{BIN}/{OUT}")
    
    if os.path.isfile(RCS):
        os.remove(RCS)
        print("- " + RCS)

    try:
        shutil.rmtree(f"{BIN}/{INC}")
        shutil.rmtree(f"{BIN}/{LIB}")
    except FileNotFoundError:
        pass


def copy_headers(dst):
    inc_path = f"{BIN}/{INC}/{LIB_FOLDER}"

    src_header_files = glob.glob(f"{SRC}/**/*{HEADER_FILE_ENDING}", recursive=True)
    header_files = []
    for src_header in src_header_files:
        dst_path = os.path.join(dst, os.path.relpath(src_header, SRC))
        dst_dir = os.path.dirname(dst_path)

        if not needs_recompile(dst_path, [src_header]):
            continue
    
        os.makedirs(dst_dir, exist_ok=True)
        shutil.copy(src_header, dst_path)

        header_files.append(dst_path)
    
    for header in header_files:
        with open(header, "r") as file:
            header_contents = file.read()

        offset = 0
        for match in re.finditer(r'#include\s*"([^"]*.h)"', header_contents):
            start, end = match.start(1) + offset, match.end(1) + offset
            header_path = match.group(1)
            include_replacement = os.path.join(os.path.dirname(header), header_path)
            if not os.path.isfile(include_replacement):
                include_replacement = os.path.join(inc_path, header_path)

                if not os.path.isfile(include_replacement):
                    print("Invalid include path!", header, match.group(0))
                    exit(1)

            include_replacement = os.path.relpath(include_replacement, f"{BIN}/{INC}")
            header_contents = header_contents[:start] + include_replacement + header_contents[end:]
            offset += len(include_replacement) - (end - start)

        with open(header, "w") as file:
            file.write(header_contents)


def make_directories(dirs):
    for dir in dirs:
        os.makedirs(dir, exist_ok=True);

##############################################################################
# Main
#
#
##############################################################################

if __name__ == "__main__":
    args = sys.argv

    # Flags
    cflags = list(CFLAGS.values())[0]
    for arg in args:
        if not arg in CFLAGS.keys():
            continue
        cflags = CFLAGS[arg]
        break

    # Running
    if "r" in args or "run" in args:
        print("Running program...")
        command = f"{BIN}/{OUT}"
        print(command, "\n")
        result = subprocess.run(command); # Learn to handle this error.
        print(f"\nProgram ended with error code: {result.returncode}")
        exit()

    # Default files
    if not os.path.exists(".gitignore"):
        with open(".gitignore", "w") as file:
            file.write("*.o\n*.exe\nbin/*")

    os.makedirs(SRC, exist_ok=True);
    for dir in [LIB, INC, OBJ, BIN]:
        os.makedirs(dir, exist_ok=True);
        open(os.path.sep.join([dir, ".gitkeep"]), 'a').close()

    # Cleaning
    if "c" in args or "clean" in args:
        print("Removing temporary files...")
        clean()
        print("Temporary files removed!")
        exit()
    
    print("Starting build...")

    source_files, object_files = find_files()

    # Compile
    if "f" in args or "fast" in args:
        cflags += " -w"
        threads = []
        for source_file, object_file in zip(source_files, object_files):
            thread = threading.Thread(target=compile, args=(source_file, object_file, cflags))
            threads.append(thread)
            thread.start()

        for thread in threads:
            thread.join()
    else:
        for source_file, object_file in zip(source_files, object_files):
            compile(source_file, object_file, cflags)

    # Link
    if "l" in args or "lib" in args:
        link_lib(RCS, object_files, LDFLAGS)
        copy_headers(f"{BIN}/{INC}/{LIB_FOLDER}")
    else:
        link_exe(f"{BIN}/{OUT}", object_files, cflags, LDFLAGS)
        
    print("Build complete!")