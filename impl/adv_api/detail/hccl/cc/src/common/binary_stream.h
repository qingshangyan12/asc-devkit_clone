/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef HCCL_SERIALIZATION
#define HCCL_SERIALIZATION

#include <vector>
#include <cstdint>
#include <sstream>
#include <algorithm>
#include <limits>
#include <map>
#include <type_traits>
#include <utility>

#include "log.h"

namespace mc2_ops_hccl {

class BinaryStream {
public:
    static constexpr std::ios_base::openmode DEFAULT_IOS_MODE = std::ios_base::in | std::ios_base::out;
    static constexpr size_t MAX_CONTAINER_ELEMENTS = 1024U * 1024U;
    static constexpr size_t MAX_CONTAINER_BYTES = 64U * 1024U * 1024U;

    explicit BinaryStream(std::ios_base::openmode mode = DEFAULT_IOS_MODE) : stream(mode | std::ios_base::binary){};

    explicit BinaryStream(std::vector<char>& buf, std::ios_base::openmode mode = DEFAULT_IOS_MODE)
        : stream(mode | std::ios_base::binary), inputSize(buf.size()), hasInputLimit(true)
    {
        stream.rdbuf()->pubsetbuf(buf.data(), buf.size());
    }

    template <typename T>
    BinaryStream& operator<<(const T& t)
    {
        stream.write(reinterpret_cast<const char*>(&t), sizeof(T));
        return *this;
    }

    // 多级vector递归序列化
    template <typename T>
    BinaryStream& operator<<(const std::vector<T>& vec)
    {
        size_t size = vec.size();
        *this << size;
        for (const auto& elem : vec) {
            *this << elem;
        }
        return *this;
    }

    // 对string的输入函数
    BinaryStream& operator<<(const std::string& s)
    {
        size_t size = s.size();
        stream.write(reinterpret_cast<const char*>(&size), sizeof(size_t)); // 写入长度
        stream.write(s.data(), size);                                       // 写入字符数据
        return *this;
    }

    template <typename T>
    BinaryStream& operator>>(T& t)
    {
        ReadExact(reinterpret_cast<char*>(&t), sizeof(T));
        return *this;
    }

    // 对string的读取函数
    BinaryStream& operator>>(std::string& s)
    {
        size_t size = 0U;
        *this >> size;
        const size_t remaining = RemainingBytes();
        if (!Good() || size > MAX_CONTAINER_BYTES || size > remaining) {
            MarkFailed();
            return *this;
        }

        std::string value;
        value.resize(size);
        if ((size > 0U) && !ReadExact(&value[0], size)) {
            return *this;
        }
        s.swap(value);
        return *this;
    }

    // 多级vector递归反序列化，失败时不提交部分结果
    template <typename T>
    BinaryStream& operator>>(std::vector<T>& vec)
    {
        size_t size = 0U;
        *this >> size;
        constexpr size_t minSerializedSize =
            (std::is_arithmetic<T>::value || std::is_enum<T>::value) ? sizeof(T) : 1U;
        if (!Good() || !ValidateContainerSize(size, sizeof(T), minSerializedSize)) {
            return *this;
        }

        std::vector<T> value(size);
        for (auto& elem : value) {
            *this >> elem;
            if (!Good()) {
                return *this;
            }
        }
        vec.swap(value);
        return *this;
    }

    // map序列化
    template <typename T1, typename T2>
    BinaryStream& operator<<(const std::map<T1, T2>& m)
    {
        size_t size = m.size();
        *this << size;
        for (const auto& elem : m) {
            *this << elem.first;
            *this << elem.second;
        }
        return *this;
    }

    // map反序列化
    template <typename T1, typename T2>
    BinaryStream& operator>>(std::map<T1, T2>& m)
    {
        size_t size = 0U;
        *this >> size;
        if (!Good() || !ValidateContainerSize(size, sizeof(std::pair<const T1, T2>), 2U)) {
            return *this;
        }

        std::map<T1, T2> value;
        for (size_t i = 0; i < size; i++) {
            T1 key{};
            *this >> key;
            if (!Good()) {
                return *this;
            }
            T2 mappedValue{};
            *this >> mappedValue;
            if (!Good()) {
                return *this;
            }
            value[key] = std::move(mappedValue);
        }
        m.swap(value);
        return *this;
    }

    bool Good() const { return stream.good(); }

    explicit operator bool() const { return Good(); }

    void Dump(std::vector<char>& vec)
    {
        std::for_each(std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>(), [&vec](const char c) {
            vec.push_back(c);
        });
    }

    void DumpWithRevert(std::vector<char>& vec)
    {
        std::streampos originalPos = stream.tellg(); // 保存原始位置
        std::for_each(std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>(), [&vec](const char c) {
            vec.push_back(c);
        });
        stream.seekg(originalPos); // 恢复原始位置
    }

    std::uint64_t GetSize() { return stream.str().size(); }

    std::string GetString() { return stream.str(); }

    std::string SplictStream(u64& start, u64& end)
    {
        std::string temp = stream.str();
        if (start >= temp.length()) {
            HCCL_ERROR("[SplictStream]start[%llu] is bigger than stream length[%llu]", start, temp.length());
            return "";
        }

        // 截取子串
        std::string result = temp.substr(start, end - start);

        // 返回新的 string
        return result;
    }

    void Clear() { stream.clear(); }

private:
    size_t RemainingBytes()
    {
        if (!stream.good()) {
            return 0U;
        }
        const std::streampos position = stream.tellg();
        if (position == std::streampos(-1)) {
            MarkFailed();
            return 0U;
        }
        const std::streamoff offset = static_cast<std::streamoff>(position);
        const size_t limit = hasInputLimit ? inputSize : stream.str().size();
        if ((offset < 0) || (static_cast<uintmax_t>(offset) > static_cast<uintmax_t>(limit))) {
            MarkFailed();
            return 0U;
        }
        return limit - static_cast<size_t>(offset);
    }

    bool ReadExact(char* dst, size_t size)
    {
        const size_t remaining = RemainingBytes();
        if (!stream.good() || size > remaining ||
            size > static_cast<size_t>((std::numeric_limits<std::streamsize>::max)())) {
            MarkFailed();
            return false;
        }
        if (size == 0U) {
            return true;
        }
        stream.read(dst, static_cast<std::streamsize>(size));
        if (!stream.good()) {
            MarkFailed();
            return false;
        }
        return true;
    }

    bool ValidateContainerSize(size_t count, size_t elementSize, size_t minSerializedSize)
    {
        if (count > MAX_CONTAINER_ELEMENTS ||
            (elementSize > 0U && count > MAX_CONTAINER_BYTES / elementSize) ||
            (minSerializedSize > 0U && count > RemainingBytes() / minSerializedSize)) {
            MarkFailed();
            return false;
        }
        return stream.good();
    }

    void MarkFailed() { stream.setstate(std::ios_base::failbit); }

    std::stringstream stream;
    size_t inputSize = 0U;
    bool hasInputLimit = false;
};

} // namespace mc2_ops_hccl

#endif // HCCL_SERIALIZATION
