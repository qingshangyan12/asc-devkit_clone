/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 */
#ifndef DATA_REORDER_API_DATA_UTILS_H
#define DATA_REORDER_API_DATA_UTILS_H

#include <fstream>
#include <string>

inline bool ReadFile(const std::string& path, void* buffer, size_t expectedSize)
{
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file.is_open() || static_cast<size_t>(file.tellg()) != expectedSize) {
        return false;
    }
    file.seekg(0, std::ios::beg);
    return static_cast<bool>(file.read(static_cast<char*>(buffer), expectedSize));
}

inline bool WriteFile(const std::string& path, const void* buffer, size_t size)
{
    std::ofstream file(path, std::ios::binary | std::ios::trunc);
    return file.is_open() && static_cast<bool>(file.write(static_cast<const char*>(buffer), size));
}

#endif

