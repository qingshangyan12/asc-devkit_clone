#ifndef COPY_INTERFACE_CUBE_LOAD_DATA_UTILS_H
#define COPY_INTERFACE_CUBE_LOAD_DATA_UTILS_H
#include <fcntl.h>
#include <unistd.h>
#include <fstream>
#include <string>
inline bool read_file(const std::string& p, void* b, size_t n) { std::ifstream f(p, std::ios::binary); if (!f) return false; f.read(static_cast<char*>(b), n); return static_cast<size_t>(f.gcount()) == n; }
inline bool write_file(const std::string& p, const void* b, size_t n) { int f=open(p.c_str(),O_WRONLY|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR); if(f<0)return false; auto w=write(f,b,n); close(f); return w==static_cast<ssize_t>(n); }
#endif
