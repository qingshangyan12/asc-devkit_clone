/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#if !defined(ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#warning                                                                                                               \
    "impl/tensor_api/utils/print_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "tensor_api/utils/print.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
 * \file print_impl.h
 * \brief Tensor API debug print helpers implementation.
 */
#ifndef IMPL_TENSOR_API_UTILS_PRINT_IMPL_H
#define IMPL_TENSOR_API_UTILS_PRINT_IMPL_H

#include "include/tensor_api/utils/utils.h"
#include "impl/tensor_api/tensor/tensor_impl.h"
#include "utils/debug/asc_printf.h"

#if defined(__DAV_CUBE__) && (__NPU_ARCH__ == 3510)
#include "impl/utils/debug/npu_arch_3510/asc_aicore_dump_utils.h"
#endif

namespace asc {
namespace te {
namespace detail {

template <typename T>
struct is_print_char_pointer : Std::false_type {};

template <>
struct is_print_char_pointer<char*> : Std::true_type {};

template <>
struct is_print_char_pointer<const char*> : Std::true_type {};

template <typename T>
constexpr bool is_print_string_v = is_print_char_pointer<Std::decay_t<T>>::value;

__aicore__ inline int num_digits(long long x)
{
    unsigned long long value = x < 0 ? 0ULL - static_cast<unsigned long long>(x) : static_cast<unsigned long long>(x);
    int digits = x < 0 ? 2 : 1;
    while (value >= 10) {
        value /= 10;
        ++digits;
    }
    return digits;
}

template <typename... Args>
__aicore__ inline void print_format(__gm__ const char* fmt, Args&&... args)
{
#if !(defined(ASCENDC_DUMP) && (ASCENDC_DUMP == 0))
    __asc_aicore::printf(fmt, static_cast<Args&&>(args)...);
#else
    (void)fmt;
    ((void)args, ...);
#endif
}

struct print_session {
    bool first_fragment = true;
};

__aicore__ static __attribute__((noinline)) void print_fragment(print_session& session, __gm__ const char* fmt)
{
#if !(defined(ASCENDC_DUMP) && (ASCENDC_DUMP == 0))
    if (session.first_fragment) {
        session.first_fragment = false;
        __asc_aicore::printf(fmt);
    } else {
        __asc_aicore::scalar_printf_impl(__asc_aicore::DumpType::DUMP_SCALAR, fmt, (__gm__ const char*)"");
    }
#else
    (void)session;
    (void)fmt;
#endif
}

template <typename ArgType>
__aicore__ static __attribute__((noinline)) void print_fragment(print_session& session, __gm__ const char* fmt,
                                                                ArgType arg)
{
#if !(defined(ASCENDC_DUMP) && (ASCENDC_DUMP == 0))
    if (session.first_fragment) {
        session.first_fragment = false;
        __asc_aicore::printf(fmt, arg);
    } else {
        __asc_aicore::scalar_printf_impl(__asc_aicore::DumpType::DUMP_SCALAR, fmt, (__gm__ const char*)"", arg);
    }
#else
    (void)session;
    (void)fmt;
    (void)arg;
#endif
}

__aicore__ inline void emit_spaces(print_session& session, int count)
{
    for (int i = 0; i < count; ++i) {
        print_fragment(session, (__gm__ const char*)" ");
    }
}

__aicore__ inline void emit_padded_int(print_session& session, long long value, int width)
{
    emit_spaces(session, width - num_digits(value));
    print_fragment(session, (__gm__ const char*)"%lld", value);
}

template <typename T>
__aicore__ inline void emit_tuple_element(print_session& session, const T& value);

template <typename Tuple, size_t I, size_t N>
struct tuple_printer {
    __aicore__ inline static void print(print_session& session, const Tuple& tuple)
    {
        emit_tuple_element(session, Std::get<I>(tuple));
        if constexpr (I + 1 < N) {
            print_fragment(session, (__gm__ const char*)", ");
            tuple_printer<Tuple, I + 1, N>::print(session, tuple);
        }
    }
};

template <typename Tuple, size_t N>
struct tuple_printer<Tuple, N, N> {
    __aicore__ inline static void print(print_session&, const Tuple&) {}
};

template <typename T>
__aicore__ inline void emit_tuple_element(print_session& session, const T& value)
{
    using value_type = Std::remove_cvref_t<T>;
    if constexpr (Std::is_tuple_v<value_type>) {
        print_fragment(session, (__gm__ const char*)"(");
        tuple_printer<value_type, 0, Std::tuple_size_v<value_type>>::print(session, value);
        print_fragment(session, (__gm__ const char*)")");
    } else if constexpr (is_print_string_v<T>) {
        print_fragment(session, (__gm__ const char*)"%s", value);
    } else if constexpr (is_one_of_attr_v<value_type, half, bfloat16_t>) {
        print_fragment(session, (__gm__ const char*)"%f", static_cast<float>(value));
    } else if constexpr (is_one_of_attr_v<value_type, float>) {
        print_fragment(session, (__gm__ const char*)"%f", static_cast<float>(value));
    } else if constexpr (Std::is_floating_point_v<value_type>) {
        print_fragment(session, (__gm__ const char*)"%f", static_cast<double>(value));
    } else if constexpr (is_one_of_attr_v<value_type, uint8_t, uint16_t, uint32_t, uint64_t>) {
        print_fragment(session, (__gm__ const char*)"%llu", static_cast<unsigned long long>(value));
    } else if constexpr (Std::is_integral_v<value_type>) {
        print_fragment(session, (__gm__ const char*)"%lld", static_cast<long long>(value));
    } else {
        print_fragment(session, (__gm__ const char*)"%lld", static_cast<long long>(value));
    }
}

template <typename T>
__aicore__ inline void emit_value(print_session& session, const T& value)
{
    if constexpr (Std::is_tuple_v<Std::remove_cvref_t<T>>) {
        emit_tuple_element(session, value);
    } else if constexpr (is_layout_v<T>) {
        print_fragment(session, (__gm__ const char*)"layout(shape=");
        emit_tuple_element(session, value.shape());
        print_fragment(session, (__gm__ const char*)", stride=");
        emit_tuple_element(session, value.stride());
        print_fragment(session, (__gm__ const char*)")");
    } else if constexpr (is_attr_tensor_v<T>) {
        using element_type = typename Std::remove_cvref_t<T>::element_type;
        using data_type = get_attribute_element_type<element_type*>;
        constexpr uint32_t ELEMENT_BITS = is_b4_type<data_type> ? 4U : sizeof(data_type) * 8U;
        print_fragment(session, (__gm__ const char*)"ptr[%ub](", ELEMENT_BITS);
        print_fragment(session, (__gm__ const char*)"%p", value.data().get());
        print_fragment(session, (__gm__ const char*)") o ");
        emit_value(session, value.layout());
    } else {
        emit_tuple_element(session, value);
    }
}

template <typename ValueType, typename T>
__aicore__ inline void emit_element_as(print_session& session, const T& value)
{
    if constexpr (Std::is_same_v<ValueType, float> || Std::is_same_v<ValueType, half>
                  || Std::is_same_v<ValueType, bfloat16_t>) {
        print_fragment(session, (__gm__ const char*)"%f  ", static_cast<float>(value));
    } else if constexpr (Std::is_same_v<ValueType, int8_t> || Std::is_same_v<ValueType, int16_t>
                         || Std::is_same_v<ValueType, int32_t>) {
        print_fragment(session, (__gm__ const char*)"%d  ", static_cast<int32_t>(value));
    } else if constexpr (Std::is_same_v<ValueType, int64_t>) {
        print_fragment(session, (__gm__ const char*)"%lld  ", static_cast<long long>(value));
    } else if constexpr (Std::is_same_v<ValueType, uint8_t> || Std::is_same_v<ValueType, uint16_t>
                         || Std::is_same_v<ValueType, uint32_t>) {
        print_fragment(session, (__gm__ const char*)"%u  ", static_cast<uint32_t>(value));
    } else if constexpr (Std::is_same_v<ValueType, uint64_t>) {
        print_fragment(session, (__gm__ const char*)"%llu  ", static_cast<unsigned long long>(value));
    } else {
        print_fragment(session, (__gm__ const char*)"  ");
        emit_value(session, value);
    }
}

template <typename DataType, typename TensorType>
struct addressable_tensor_reader {
    const TensorType& tensor;

    template <typename CoordType>
    __aicore__ inline void emit(print_session& session, const CoordType& coord)
    {
        emit_element_as<DataType>(session, tensor[coord]);
    }
};

#if defined(__DAV_CUBE__) && (__NPU_ARCH__ == 3510)
constexpr uint32_t DEBUG_BUS_MAX_WAIT_COUNT = 5U;
constexpr uint32_t DEBUG_BUS_READY_MASK = 0x80000000U;
constexpr uint64_t DEBUG_BUS_WAIT_CYCLES = 100U;

struct debug_bus_chunk {
    uint32_t word0 = 0U;
    uint32_t word1 = 0U;
    uint32_t word2 = 0U;
    uint32_t word3 = 0U;
    uint32_t word4 = 0U;
    uint32_t word5 = 0U;
    uint32_t word6 = 0U;
    uint32_t word7 = 0U;

    __aicore__ inline uint32_t word(uint32_t index) const
    {
        switch (index) {
        case 0:
            return word0;
        case 1:
            return word1;
        case 2:
            return word2;
        case 3:
            return word3;
        case 4:
            return word4;
        case 5:
            return word5;
        case 6:
            return word6;
        default:
            return word7;
        }
    }

    __aicore__ inline uint64_t bits(uint32_t bit_offset, uint32_t bit_count) const
    {
        uint64_t value = 0U;
        uint32_t byte_offset = bit_offset >> 3;
        uint32_t byte_count = (bit_count + 7U) >> 3;
        for (uint32_t i = 0; i < byte_count; ++i) {
            uint32_t byte_index = byte_offset + i;
            uint32_t byte_value = (word(byte_index >> 2) >> ((byte_index & 3U) << 3)) & 0xFFU;
            value |= static_cast<uint64_t>(byte_value) << (i << 3);
        }
        return (value >> (bit_offset & 7U))
               & (bit_count == 64U ? ~0ULL : ((static_cast<uint64_t>(1U) << bit_count) - 1U));
    }
};

template <AscendC::Hardware HardwareType>
__aicore__ inline uint32_t get_debug_bus_local_offset(uint64_t local_address)
{
    return static_cast<uint32_t>(local_address >> __asc_aicore::get_debug_bus_local_addr_shift<HardwareType>());
}

template <AscendC::Hardware HardwareType>
__aicore__ inline void configure_debug_bus_read(uint64_t debug_bus_address, uint64_t local_address)
{
    __asc_aicore::sync_all();
    __asc_aicore::debug_bus_write_reg(debug_bus_address, __asc_aicore::ASC_DEBUG_BUS_UNLOCK_OFFSET,
                                      __asc_aicore::ASC_DEBUG_BUS_UNLOCK_VALUE);
    __asc_aicore::debug_bus_write_reg(debug_bus_address, __asc_aicore::ASC_DEBUG_BUS_ADDR_LOW_OFFSET,
                                      get_debug_bus_local_offset<HardwareType>(local_address));
    __asc_aicore::debug_bus_write_reg(debug_bus_address, __asc_aicore::ASC_DEBUG_BUS_MODEL_BASE_OFFSET,
                                      __asc_aicore::get_debug_bus_model_base<HardwareType>());
    __asc_aicore::debug_bus_write_reg(debug_bus_address, __asc_aicore::ASC_DEBUG_BUS_CTRL_OFFSET,
                                      __asc_aicore::ASC_DEBUG_BUS_READ_MASK);
}

__aicore__ inline void read_debug_bus_data(uint64_t debug_bus_address, debug_bus_chunk& chunk)
{
    chunk.word0 = __asc_aicore::debug_bus_read_reg(debug_bus_address, __asc_aicore::ASC_DEBUG_BUS_DATA_OFFSET);
    chunk.word1 = __asc_aicore::debug_bus_read_reg(debug_bus_address, __asc_aicore::ASC_DEBUG_BUS_DATA_OFFSET + 4U);
    chunk.word2 = __asc_aicore::debug_bus_read_reg(debug_bus_address, __asc_aicore::ASC_DEBUG_BUS_DATA_OFFSET + 8U);
    chunk.word3 = __asc_aicore::debug_bus_read_reg(debug_bus_address, __asc_aicore::ASC_DEBUG_BUS_DATA_OFFSET + 12U);
    chunk.word4 = __asc_aicore::debug_bus_read_reg(debug_bus_address, __asc_aicore::ASC_DEBUG_BUS_DATA_OFFSET + 16U);
    chunk.word5 = __asc_aicore::debug_bus_read_reg(debug_bus_address, __asc_aicore::ASC_DEBUG_BUS_DATA_OFFSET + 20U);
    chunk.word6 = __asc_aicore::debug_bus_read_reg(debug_bus_address, __asc_aicore::ASC_DEBUG_BUS_DATA_OFFSET + 24U);
    chunk.word7 = __asc_aicore::debug_bus_read_reg(debug_bus_address, __asc_aicore::ASC_DEBUG_BUS_DATA_OFFSET + 28U);
}

template <AscendC::Hardware HardwareType>
__aicore__ inline bool read_debug_bus_chunk(uint64_t local_address, debug_bus_chunk& chunk)
{
    uint64_t debug_bus_address = __asc_aicore::get_debug_bus_addr_impl();
    if (debug_bus_address == 0U) {
        return false;
    }

    configure_debug_bus_read<HardwareType>(debug_bus_address, local_address);

    uint32_t debug_status = 0U;
    for (uint32_t counter = 0U; counter <= DEBUG_BUS_MAX_WAIT_COUNT; ++counter) {
        __asc_aicore::ringbuf_wait_rts_sync();
        __asc_aicore::sync_all();
        debug_status = __asc_aicore::debug_bus_read_reg(debug_bus_address, __asc_aicore::ASC_DEBUG_BUS_BUSY_OFFSET);
        if ((debug_status & DEBUG_BUS_READY_MASK) != 0U) {
            break;
        }
    }
    if ((debug_status & DEBUG_BUS_READY_MASK) == 0U) {
        return false;
    }

    __asc_aicore::ringbuf_wait_rts_sync<DEBUG_BUS_WAIT_CYCLES>();
    read_debug_bus_data(debug_bus_address, chunk);
    __asc_aicore::sync_all();
    return true;
}

template <typename DataType>
__aicore__ inline DataType debug_value_from_bits(uint64_t bits)
{
    if constexpr (sizeof(DataType) == 1U) {
        return __builtin_bit_cast(DataType, static_cast<uint8_t>(bits));
    } else if constexpr (sizeof(DataType) == 2U) {
        return __builtin_bit_cast(DataType, static_cast<uint16_t>(bits));
    } else if constexpr (sizeof(DataType) == 4U) {
        return __builtin_bit_cast(DataType, static_cast<uint32_t>(bits));
    } else {
        return __builtin_bit_cast(DataType, bits);
    }
}

template <typename DataType, typename TensorType, AscendC::Hardware HardwareType>
struct debug_bus_tensor_reader {
    const TensorType& tensor;
    debug_bus_chunk chunk;
    uint64_t cached_chunk_address = ~0ULL;
    bool read_succeeded = true;

    template <typename CoordType>
    __aicore__ inline void emit(print_session& session, const CoordType& coord)
    {
        constexpr uint32_t ELEMENT_BITS = is_b4_type<DataType> ? 4U : sizeof(DataType) * 8U;
        uint64_t element_index = static_cast<uint64_t>(tensor.layout()(coord));
        uint64_t bit_address = reinterpret_cast<uint64_t>(tensor.data().get()) * 8U + element_index * ELEMENT_BITS;
        uint64_t chunk_address =
            (bit_address >> 3) & ~(static_cast<uint64_t>(__asc_aicore::ASC_DEBUG_BUS_CHUNK_BYTES) - 1U);
        if (chunk_address != cached_chunk_address) {
            read_succeeded = read_debug_bus_chunk<HardwareType>(chunk_address, chunk);
            cached_chunk_address = chunk_address;
        }
        if (!read_succeeded) {
            print_fragment(session, (__gm__ const char*)"?  ");
            return;
        }
        uint32_t bit_offset = static_cast<uint32_t>(bit_address - chunk_address * 8U);
        emit_element_as<DataType>(session, debug_value_from_bits<DataType>(chunk.bits(bit_offset, ELEMENT_BITS)));
    }
};
#endif

template <typename T>
__aicore__ inline void emit_layout_table(print_session& session, const T& layout)
{
    static_assert(is_layout_v<T>, "print_layout expects a Tensor API layout.");
    static_assert(Std::remove_cvref_t<T>::rank_size == 2, "print_layout only supports rank-2 layouts.");

    int idx_width = num_digits(static_cast<long long>(cosize(layout))) + 2;
    emit_value(session, layout);
    print_fragment(session, (__gm__ const char*)"\n");

    print_fragment(session, (__gm__ const char*)"    ");
    for (int n = 0; n < static_cast<int>(size<1>(layout)); ++n) {
        print_fragment(session, (__gm__ const char*)"  ");
        emit_padded_int(session, n, idx_width - 2);
        print_fragment(session, (__gm__ const char*)" ");
    }
    print_fragment(session, (__gm__ const char*)"\n");

    for (int m = 0; m < static_cast<int>(size<0>(layout)); ++m) {
        print_fragment(session, (__gm__ const char*)"    ");
        for (int n = 0; n < static_cast<int>(size<1>(layout)); ++n) {
            print_fragment(session, (__gm__ const char*)"+");
            for (int i = 0; i < idx_width; ++i) {
                print_fragment(session, (__gm__ const char*)"-");
            }
        }
        print_fragment(session, (__gm__ const char*)"+\n");

        emit_padded_int(session, m, 2);
        print_fragment(session, (__gm__ const char*)"  ");
        for (int n = 0; n < static_cast<int>(size<1>(layout)); ++n) {
            print_fragment(session, (__gm__ const char*)"| ");
            emit_padded_int(session, static_cast<long long>(layout(make_coord(m, n))), idx_width - 2);
            print_fragment(session, (__gm__ const char*)" ");
        }
        print_fragment(session, (__gm__ const char*)"|\n");
    }

    print_fragment(session, (__gm__ const char*)"    ");
    for (int n = 0; n < static_cast<int>(size<1>(layout)); ++n) {
        print_fragment(session, (__gm__ const char*)"+");
        for (int i = 0; i < idx_width; ++i) {
            print_fragment(session, (__gm__ const char*)"-");
        }
    }
    print_fragment(session, (__gm__ const char*)"+\n");
}

__aicore__ inline void emit_tensor_separator(print_session& session, int width, __gm__ const char* separator)
{
    for (int i = 0; i < width; ++i) {
        print_fragment(session, separator);
    }
    print_fragment(session, (__gm__ const char*)"\n");
}

template <typename ReaderType, typename T>
__aicore__ inline void emit_rank_one_tensor(print_session& session, const T& tensor, ReaderType& reader)
{
    for (int m = 0; m < static_cast<int>(size<0>(tensor.layout())); ++m) {
        reader.emit(session, make_coord(m));
        print_fragment(session, (__gm__ const char*)"\n");
    }
}

template <typename ReaderType, typename T>
__aicore__ inline void emit_rank_two_tensor(print_session& session, const T& tensor, ReaderType& reader)
{
    for (int m = 0; m < static_cast<int>(size<0>(tensor.layout())); ++m) {
        for (int n = 0; n < static_cast<int>(size<1>(tensor.layout())); ++n) {
            reader.emit(session, make_coord(m, n));
        }
        print_fragment(session, (__gm__ const char*)"\n");
    }
}

template <typename ReaderType, typename T>
__aicore__ inline void emit_rank_three_plane(print_session& session, const T& tensor, ReaderType& reader, int k)
{
    for (int m = 0; m < static_cast<int>(size<0>(tensor.layout())); ++m) {
        for (int n = 0; n < static_cast<int>(size<1>(tensor.layout())); ++n) {
            reader.emit(session, make_coord(m, n, k));
        }
        print_fragment(session, (__gm__ const char*)"\n");
    }
}

template <typename ReaderType, typename T>
__aicore__ inline void emit_rank_three_tensor(print_session& session, const T& tensor, ReaderType& reader)
{
    int separator_width = 5 * static_cast<int>(size<1>(tensor.layout()));
    for (int k = 0; k < static_cast<int>(size<2>(tensor.layout())); ++k) {
        if (k != 0) {
            emit_tensor_separator(session, separator_width, (__gm__ const char*)"-");
        }
        emit_rank_three_plane(session, tensor, reader, k);
    }
}

template <typename ReaderType, typename T>
__aicore__ inline void emit_rank_four_plane(print_session& session, const T& tensor, ReaderType& reader, int k, int p)
{
    for (int m = 0; m < static_cast<int>(size<0>(tensor.layout())); ++m) {
        for (int n = 0; n < static_cast<int>(size<1>(tensor.layout())); ++n) {
            reader.emit(session, make_coord(m, n, k, p));
        }
        print_fragment(session, (__gm__ const char*)"\n");
    }
}

template <typename ReaderType, typename T>
__aicore__ inline void emit_rank_four_tensor(print_session& session, const T& tensor, ReaderType& reader)
{
    int separator_width = 5 * static_cast<int>(size<1>(tensor.layout()));
    for (int p = 0; p < static_cast<int>(size<3>(tensor.layout())); ++p) {
        if (p != 0) {
            emit_tensor_separator(session, separator_width, (__gm__ const char*)"=");
        }
        for (int k = 0; k < static_cast<int>(size<2>(tensor.layout())); ++k) {
            if (k != 0) {
                emit_tensor_separator(session, separator_width, (__gm__ const char*)"-");
            }
            emit_rank_four_plane(session, tensor, reader, k, p);
        }
    }
}

template <typename Location>
__aicore__ inline void emit_unsupported_tensor_location(print_session& session)
{
    print_fragment(session, (__gm__ const char*)"[print_tensor] tensor elements cannot be printed from ");
    if constexpr (Std::is_same_v<Location, location::l1>) {
        print_fragment(session, (__gm__ const char*)"L1 (__cbuf__)");
    } else if constexpr (Std::is_same_v<Location, location::l0a>) {
        print_fragment(session, (__gm__ const char*)"L0A (__ca__)");
    } else if constexpr (Std::is_same_v<Location, location::l0b>) {
        print_fragment(session, (__gm__ const char*)"L0B (__cb__)");
    } else if constexpr (Std::is_same_v<Location, location::l0c>) {
        print_fragment(session, (__gm__ const char*)"L0C (__cc__)");
    } else if constexpr (Std::is_same_v<Location, location::l0scalea>) {
        print_fragment(session, (__gm__ const char*)"L0ScaleA");
    } else if constexpr (Std::is_same_v<Location, location::l0scaleb>) {
        print_fragment(session, (__gm__ const char*)"L0ScaleB");
    } else if constexpr (Std::is_same_v<Location, location::bias>) {
        print_fragment(session, (__gm__ const char*)"BiasTable (__biasbuf__)");
    } else if constexpr (Std::is_same_v<Location, location::fixbuf>) {
        print_fragment(session, (__gm__ const char*)"FixBuf (__fbuf__)");
    } else if constexpr (Std::is_same_v<Location, location::ssbuf>) {
        print_fragment(session, (__gm__ const char*)"SSBuf (__ssbuf__)");
    } else {
        print_fragment(session, (__gm__ const char*)"this memory location");
    }
#if defined(__DAV_CUBE__) && (__NPU_ARCH__ == 3510)
    print_fragment(session,
                   (__gm__ const char*)"; only GM, UB, ordinary local memory, L1, L0A, and L0B are supported.\n");
#else
    print_fragment(session, (__gm__ const char*)"; only GM, UB, and ordinary local memory are supported.\n");
#endif
}

template <typename ReaderType, typename T>
__aicore__ inline void emit_tensor_with_reader(print_session& session, const T& tensor, ReaderType& reader)
{
    using layout_type = typename Std::remove_cvref_t<T>::layout_type;
    if constexpr (layout_type::rank_size == 1) {
        emit_rank_one_tensor(session, tensor, reader);
    } else if constexpr (layout_type::rank_size == 2) {
        emit_rank_two_tensor(session, tensor, reader);
    } else if constexpr (layout_type::rank_size == 3) {
        emit_rank_three_tensor(session, tensor, reader);
    } else if constexpr (layout_type::rank_size == 4) {
        emit_rank_four_tensor(session, tensor, reader);
    }
}

template <typename DataType, typename T>
__aicore__ inline void emit_addressable_tensor_content(print_session& session, const T& tensor)
{
    addressable_tensor_reader<DataType, T> reader{tensor};
    emit_tensor_with_reader(session, tensor, reader);
}

#if defined(__DAV_CUBE__) && (__NPU_ARCH__ == 3510)
template <typename DataType, AscendC::Hardware HardwareType, typename T>
__aicore__ inline void emit_debug_bus_tensor_content(print_session& session, const T& tensor)
{
    debug_bus_tensor_reader<DataType, T, HardwareType> reader{tensor};
    emit_tensor_with_reader(session, tensor, reader);
}
#endif

template <typename T>
__aicore__ inline void emit_tensor_content(print_session& session, const T& tensor, bool print_type)
{
    static_assert(is_attr_tensor_v<T>, "print_tensor expects a Tensor API tensor created by make_tensor.");
    using tensor_type = Std::remove_cvref_t<T>;
    using layout_type = typename tensor_type::layout_type;
    using element_type = typename tensor_type::element_type;
    using data_type = get_attribute_element_type<element_type*>;
    using tensor_location = get_mem_location<tensor_type>;
    static_assert(layout_type::rank_size >= 1 && layout_type::rank_size <= 4,
                  "print_tensor only supports rank 1 to 4 tensors.");

    if (print_type) {
        emit_value(session, tensor);
        print_fragment(session, (__gm__ const char*)":\n");
    }
    if constexpr (Std::is_same_v<tensor_location, location::gm> || Std::is_same_v<tensor_location, location::ub>
                  || Std::is_same_v<tensor_location, Std::ignore_t>) {
        emit_addressable_tensor_content<data_type>(session, tensor);
#if defined(__DAV_CUBE__) && (__NPU_ARCH__ == 3510)
    } else if constexpr (Std::is_same_v<tensor_location, location::l1>) {
        emit_debug_bus_tensor_content<data_type, AscendC::Hardware::L1>(session, tensor);
    } else if constexpr (Std::is_same_v<tensor_location, location::l0a>) {
        emit_debug_bus_tensor_content<data_type, AscendC::Hardware::L0A>(session, tensor);
    } else if constexpr (Std::is_same_v<tensor_location, location::l0b>) {
        emit_debug_bus_tensor_content<data_type, AscendC::Hardware::L0B>(session, tensor);
#endif
    } else {
        emit_unsupported_tensor_location<tensor_location>(session);
    }
}

} // namespace detail

__aicore__ inline void print(__gm__ const char* fmt)
{
    detail::print_format(fmt);
}

template <typename Arg0, typename... Args>
__aicore__ inline void print(__gm__ const char* fmt, Arg0&& arg0, Args&&... args)
{
    detail::print_format(fmt, static_cast<Arg0&&>(arg0), static_cast<Args&&>(args)...);
}

template <typename T, typename Std::enable_if_t<!detail::is_print_string_v<T>, int> = 0>
__aicore__ inline void print(const T& value)
{
#if !(defined(ASCENDC_DUMP) && (ASCENDC_DUMP == 0))
    detail::print_session session;
    detail::emit_value(session, value);
    detail::print_fragment(session, (__gm__ const char*)"\n");
#else
    (void)value;
#endif
}

template <typename LayoutType>
__aicore__ inline void print_layout(const LayoutType& layout)
{
#if !(defined(ASCENDC_DUMP) && (ASCENDC_DUMP == 0))
    detail::print_session session;
    detail::emit_layout_table(session, layout);
#else
    (void)layout;
#endif
}

template <typename TensorType>
__aicore__ inline void print_tensor(const TensorType& tensor, bool print_type = true)
{
#if !(defined(ASCENDC_DUMP) && (ASCENDC_DUMP == 0))
    detail::print_session session;
    detail::emit_tensor_content(session, tensor, print_type);
#else
    (void)tensor;
    (void)print_type;
#endif
}

} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_UTILS_PRINT_IMPL_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
