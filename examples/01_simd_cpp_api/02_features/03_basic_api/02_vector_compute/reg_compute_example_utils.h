/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the license.
 */

#ifndef EXAMPLES_REG_COMPUTE_EXAMPLE_UTILS_H
#define EXAMPLES_REG_COMPUTE_EXAMPLE_UTILS_H

#include <fstream>
#include <iostream>
#include <string>

#include "acl/acl.h"

namespace example {

inline bool check_acl(aclError result, const char* api)
{
    if (result == ACL_SUCCESS) {
        return true;
    }
    std::cerr << api << " failed, error code: " << result << std::endl;
    return false;
}

class runtime {
public:
    runtime() = default;
    runtime(const runtime&) = delete;
    runtime& operator=(const runtime&) = delete;

    bool init()
    {
        if (!check_acl(aclInit(nullptr), "aclInit")) {
            return false;
        }
        initialized_ = true;
        if (!check_acl(aclrtSetDevice(device_id_), "aclrtSetDevice")) {
            return false;
        }
        device_set_ = true;
        return check_acl(aclrtCreateStream(&stream_), "aclrtCreateStream");
    }

    ~runtime()
    {
        if (stream_ != nullptr) {
            (void)aclrtDestroyStream(stream_);
        }
        if (device_set_) {
            (void)aclrtResetDevice(device_id_);
        }
        if (initialized_) {
            (void)aclFinalize();
        }
    }

    aclrtStream stream() const
    {
        return stream_;
    }

    bool synchronize() const
    {
        return check_acl(aclrtSynchronizeStream(stream_), "aclrtSynchronizeStream");
    }

private:
    int32_t device_id_ = 0;
    bool initialized_ = false;
    bool device_set_ = false;
    aclrtStream stream_ = nullptr;
};

template <typename T>
class buffer {
public:
    buffer() = default;
    buffer(const buffer&) = delete;
    buffer& operator=(const buffer&) = delete;

    bool allocate(size_t count)
    {
        bytes_ = count * sizeof(T);
        if (!check_acl(aclrtMallocHost(reinterpret_cast<void**>(&host_), bytes_), "aclrtMallocHost")) {
            return false;
        }
        return check_acl(
            aclrtMalloc(reinterpret_cast<void**>(&device_), bytes_, ACL_MEM_MALLOC_HUGE_FIRST), "aclrtMalloc");
    }

    ~buffer()
    {
        if (device_ != nullptr) {
            (void)aclrtFree(device_);
        }
        if (host_ != nullptr) {
            (void)aclrtFreeHost(host_);
        }
    }

    bool load(const std::string& path)
    {
        std::ifstream file(path, std::ios::binary | std::ios::ate);
        if (!file.is_open() || static_cast<size_t>(file.tellg()) != bytes_) {
            std::cerr << "Unexpected input file: " << path << std::endl;
            return false;
        }
        file.seekg(0);
        return static_cast<bool>(file.read(reinterpret_cast<char*>(host_), bytes_));
    }

    bool save(const std::string& path) const
    {
        std::ofstream file(path, std::ios::binary | std::ios::trunc);
        return file.is_open() && static_cast<bool>(file.write(reinterpret_cast<const char*>(host_), bytes_));
    }

    bool copy_to_device() const
    {
        return check_acl(aclrtMemcpy(device_, bytes_, host_, bytes_, ACL_MEMCPY_HOST_TO_DEVICE),
            "aclrtMemcpy host to device");
    }

    bool copy_to_host() const
    {
        return check_acl(aclrtMemcpy(host_, bytes_, device_, bytes_, ACL_MEMCPY_DEVICE_TO_HOST),
            "aclrtMemcpy device to host");
    }

    bool load_to_device(const std::string& path, size_t count)
    {
        return allocate(count) && load(path) && copy_to_device();
    }

    bool save_from_device(const std::string& path) const
    {
        return copy_to_host() && save(path);
    }

    T* device() const
    {
        return device_;
    }

private:
    T* host_ = nullptr;
    T* device_ = nullptr;
    size_t bytes_ = 0;
};

} // namespace example

#endif // EXAMPLES_REG_COMPUTE_EXAMPLE_UTILS_H
