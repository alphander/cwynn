#ifndef DATAIO_H
#define DATAIO_H

#include <stdlib.h>
#include <stdbool.h>
#include "LTK\platform_defines.h"
#include "LTK\containers.h"

/// @brief Checks if path file exists
/// @param[in] path File path
/// @return If there IS a file
bool dataio_isfile(const char* path);

/// @brief Reads bulk data from path (will always include a NULL terminator)
/// @param[in] path File path
/// @param[out] pSizeOut Out parameter size in bytes for output data
/// @return Heap allocated data (will always include a NULL terminator)
uint8_t* dataio_read(const char* path, size_t* pSizeOut);

/// @brief Writes bulk data to path
/// @param[in] path File path
/// @param[in] pData Input data
/// @param size Input data size in bytes
void dataio_write(const char* path, uint8_t* pData, size_t size);

/// @brief Appends bulk data to path
/// @param[in] path File path
/// @param[in] pData Input data
/// @param size Input data size in bytes
void dataio_append(const char* path, uint8_t* pData, size_t size);

/// @brief Gets data from https, http or ftp address (will always include a NULL terminator)
/// @param[in] url Internet URL (https http or ftp only)
/// @param[out] pSizeOut Out parameter size in bytes for output data
/// @return Heap allocated data (will always include a NULL terminator)
uint8_t* dataio_get(const char* url, size_t* pSizeOut);

#endif // DATAIO_H