#ifndef C_API_CALL_EXAMPLES_COMMON_SAMPLE_HOST_H
#define C_API_CALL_EXAMPLES_COMMON_SAMPLE_HOST_H

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

#include "acl/acl.h"

namespace CApiExample {

constexpr int32_t DEVICE_ID = 0;
constexpr size_t PRINT_COUNT = 8;

inline bool CheckAcl(aclError status, const char* operation)
{
    if (status == ACL_SUCCESS) {
        return true;
    }
    std::cerr << "[ERROR] " << operation << " failed, error code " << status << std::endl;
    return false;
}

template <typename T>
void Print(const char* label, const std::vector<T>& values)
{
    std::cout << label << ":";
    const size_t count = std::min(values.size(), PRINT_COUNT);
    for (size_t i = 0; i < count; ++i) {
        if constexpr (std::is_integral_v<T> && sizeof(T) == 1) {
            std::cout << ' ' << +values[i];
        } else {
            std::cout << ' ' << values[i];
        }
    }
    if (values.size() > count) {
        std::cout << " ...";
    }
    std::cout << std::endl;
}

inline void PrintHalf(const char* label, const std::vector<aclFloat16>& values)
{
    std::cout << label << ":";
    const size_t count = std::min(values.size(), PRINT_COUNT);
    for (size_t i = 0; i < count; ++i) {
        std::cout << ' ' << aclFloat16ToFloat(values[i]);
    }
    if (values.size() > count) {
        std::cout << " ...";
    }
    std::cout << std::endl;
}

inline bool CompareHalf(
    const std::vector<aclFloat16>& output, const std::vector<aclFloat16>& golden, float tolerance)
{
    if (output.size() != golden.size()) {
        return false;
    }
    for (size_t i = 0; i < output.size(); ++i) {
        const float actual = aclFloat16ToFloat(output[i]);
        const float expected = aclFloat16ToFloat(golden[i]);
        if (std::fabs(actual - expected) > tolerance) {
            std::cerr << "[ERROR] mismatch at " << i << ": output=" << actual << ", golden=" << expected
                      << std::endl;
            return false;
        }
    }
    return true;
}

template <typename T>
bool Compare(const std::vector<T>& output, const std::vector<T>& golden, double tolerance = 0.0)
{
    if (output.size() != golden.size()) {
        return false;
    }
    for (size_t i = 0; i < output.size(); ++i) {
        if constexpr (std::is_floating_point_v<T>) {
            if (std::fabs(static_cast<double>(output[i]) - static_cast<double>(golden[i])) > tolerance) {
                std::cerr << "[ERROR] mismatch at " << i << ": output=" << output[i]
                          << ", golden=" << golden[i] << std::endl;
                return false;
            }
        } else if (output[i] != golden[i]) {
            std::cerr << "[ERROR] mismatch at " << i << ": output=" << +output[i]
                      << ", golden=" << +golden[i] << std::endl;
            return false;
        }
    }
    return true;
}

template <typename T>
bool CompareRange(const std::vector<T>& output, const std::vector<T>& golden, size_t begin, size_t end)
{
    if (output.size() != golden.size() || end > output.size() || begin > end) {
        return false;
    }
    for (size_t i = begin; i < end; ++i) {
        if (output[i] != golden[i]) {
            std::cerr << "[ERROR] mismatch at " << i << ": output=" << +output[i]
                      << ", golden=" << +golden[i] << std::endl;
            return false;
        }
    }
    return true;
}

template <typename T>
class DeviceBuffer {
public:
    explicit DeviceBuffer(size_t count) : byte_size_(count * sizeof(T)) {}

    bool Allocate(const char* name)
    {
        name_ = name;
        return CheckAcl(aclrtMalloc(reinterpret_cast<void**>(&data_), byte_size_, ACL_MEM_MALLOC_HUGE_FIRST), name_);
    }

    bool CopyFrom(const std::vector<T>& host) const
    {
        return CheckAcl(
            aclrtMemcpy(data_, byte_size_, host.data(), byte_size_, ACL_MEMCPY_HOST_TO_DEVICE), "Host to Device copy");
    }

    bool CopyTo(std::vector<T>& host) const
    {
        return CheckAcl(
            aclrtMemcpy(host.data(), byte_size_, data_, byte_size_, ACL_MEMCPY_DEVICE_TO_HOST), "Device to Host copy");
    }

    T* Get() const { return data_; }

    ~DeviceBuffer()
    {
        if (data_ != nullptr) {
            (void)aclrtFree(data_);
        }
    }

    DeviceBuffer(const DeviceBuffer&) = delete;
    DeviceBuffer& operator=(const DeviceBuffer&) = delete;

private:
    T* data_ = nullptr;
    size_t byte_size_ = 0;
    const char* name_ = "aclrtMalloc";
};

class Runtime {
public:
    bool Init()
    {
        initialized_ = CheckAcl(aclInit(nullptr), "aclInit");
        device_set_ = initialized_ && CheckAcl(aclrtSetDevice(DEVICE_ID), "aclrtSetDevice");
        return device_set_;
    }

    ~Runtime()
    {
        if (device_set_) {
            (void)aclrtResetDevice(DEVICE_ID);
        }
        if (initialized_) {
            (void)aclFinalize();
        }
    }

    Runtime(const Runtime&) = delete;
    Runtime& operator=(const Runtime&) = delete;
    Runtime() = default;

private:
    bool initialized_ = false;
    bool device_set_ = false;
};

} // namespace CApiExample

#endif
