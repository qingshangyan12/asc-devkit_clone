#ifndef COPY_INTERFACE_VECTOR_DATA_UTILS_H
#define COPY_INTERFACE_VECTOR_DATA_UTILS_H
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fstream>
#include <string>

inline bool read_file(const std::string& path, void* buffer, size_t size)
{
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) return false;
    file.read(static_cast<char*>(buffer), size);
    return static_cast<size_t>(file.gcount()) == size;
}

inline bool write_file(const std::string& path, const void* buffer, size_t size)
{
    int fd = open(path.c_str(), O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd < 0) return false;
    auto written = write(fd, buffer, size);
    close(fd);
    return written == static_cast<ssize_t>(size);
}
#endif
