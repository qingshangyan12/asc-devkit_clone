#ifndef DATA_UTILS_H
#define DATA_UTILS_H
#include <cstdint>
#include <cstdio>
#include <cstdlib>

inline int32_t ReadFile(const char* filePath, void* buffer, uint32_t bufSize)
{
    FILE* fp = fopen(filePath, "rb");
    if (fp == nullptr) { printf("ReadFile: failed to open %s\n", filePath); return -1; }
    size_t readSize = fread(buffer, 1, bufSize, fp);
    fclose(fp);
    return static_cast<int32_t>(readSize);
}

inline int32_t WriteFile(const char* filePath, const void* buffer, uint32_t bufSize)
{
    FILE* fp = fopen(filePath, "wb");
    if (fp == nullptr) { printf("WriteFile: failed to open %s\n", filePath); return -1; }
    size_t writeSize = fwrite(buffer, 1, bufSize, fp);
    fclose(fp);
    return static_cast<int32_t>(writeSize);
}
#endif
