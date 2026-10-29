/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <gtest/gtest.h>
#include <array>
#include <cstring>
#include <mockcpp/mockcpp.hpp>
#include "tensor_api/stub/cce_stub.h"
#include "tensor_api/tensor.h"
#include "tensor_api/experimental/arch/vector/reg_data_load.h"
#include "tensor_api/experimental/arch/vector/reg_data_store.h"

namespace {

Literal expected_load_mode = NORM;
Literal expected_store_mode = NORM_B8;
vector_bool expected_store_mask = 0;
uint8_t store_mask_pattern = 0xFF;
uint32_t load_call_count = 0;
uint32_t store_call_count = 0;

void load_packed_stub(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, int32_t offset, Literal mode)
{
    EXPECT_EQ(mode, expected_load_mode);
    EXPECT_EQ(offset, 0);
    ++load_call_count;
    const auto* source = reinterpret_cast<const uint8_t*>(src);
    auto* destination = reinterpret_cast<uint8_t*>(&dst);
    for (size_t index = 0; index < sizeof(dst); ++index) {
        if (mode == UNPK_B8) {
            destination[index] = index % 2 == 0 ? source[index / 2] : 0;
        } else if (mode == UNPK4_B8) {
            destination[index] = index % 4 == 0 ? source[index / 4] : 0;
        } else if (mode == DS_B8) {
            destination[index] = source[index * 2];
        } else if (mode == US_B8) {
            destination[index] = source[index / 2];
        } else if (mode == BRC_B8) {
            destination[index] = source[0];
        } else if (mode == BLK) {
            destination[index] = source[index % 32];
        } else {
            destination[index] = source[index];
        }
    }
}

void load_interleaved_stub(
    vector_fp4x2_e1m2_t& first, vector_fp4x2_e1m2_t& second, __ubuf__ fp4x2_e1m2_t* src, int32_t offset, int32_t mode)
{
    EXPECT_EQ(mode, DINTLV_B8);
    EXPECT_EQ(offset, 0);
    ++load_call_count;
    const auto* source = reinterpret_cast<const uint8_t*>(src);
    auto* even = reinterpret_cast<uint8_t*>(&first);
    auto* odd = reinterpret_cast<uint8_t*>(&second);
    for (size_t index = 0; index < sizeof(first); ++index) {
        even[index] = source[index * 2];
        odd[index] = source[index * 2 + 1];
    }
}

void store_packed_stub(
    vector_fp4x2_e1m2_t src, __ubuf__ fp4x2_e1m2_t* dst, int32_t offset, Literal mode, vector_bool mask)
{
    EXPECT_EQ(mode, expected_store_mode);
    EXPECT_EQ(offset, 0);
    ++store_call_count;
    auto* destination = reinterpret_cast<uint8_t*>(dst);
    const auto* source = reinterpret_cast<const uint8_t*>(&src);
    if (mode == ONEPT_B8) {
        destination[0] = source[0];
    } else {
        EXPECT_EQ(mask, expected_store_mask);
        for (size_t index = 0; index < sizeof(src); ++index) {
            if ((store_mask_pattern & (1U << (index % 8))) != 0) {
                destination[index] = source[index];
            }
        }
    }
}

void store_interleaved_stub(
    vector_fp4x2_e1m2_t first, vector_fp4x2_e1m2_t second, __ubuf__ fp4x2_e1m2_t* dst, int32_t offset, int32_t mode,
    vector_bool)
{
    EXPECT_EQ(mode, INTLV_B8);
    EXPECT_EQ(offset, 0);
    ++store_call_count;
    auto* destination = reinterpret_cast<uint8_t*>(dst);
    const auto* even = reinterpret_cast<const uint8_t*>(&first);
    const auto* odd = reinterpret_cast<const uint8_t*>(&second);
    for (size_t index = 0; index < sizeof(first); ++index) {
        destination[index * 2] = even[index];
        destination[index * 2 + 1] = odd[index];
    }
}

template <typename Tensor, asc::te::experimental::load_sideband_mode Mode>
__simd_callee__ inline void compile_int4_load(Tensor& tensor)
{
    using namespace asc::te::experimental;
    auto coordinate = asc::te::make_coord(64);
    auto value = load<Mode>(tensor, coordinate);
    auto value_without_coord = load<Mode>(tensor);
    if constexpr (Mode == load_sideband_mode::deintlv) {
        static_assert(AscendC::Std::is_same_v<decltype(value), reg_pair<int4x2_t>>);
        static_assert(AscendC::Std::is_same_v<decltype(value_without_coord), reg_pair<int4x2_t>>);
        store<store_sideband_mode::intlv>(tensor, coordinate, value.first, value.second);
        store<store_sideband_mode::intlv>(tensor, value_without_coord.first, value_without_coord.second);
    } else {
        static_assert(AscendC::Std::is_same_v<decltype(value), reg_tensor<int4x2_t>>);
        static_assert(AscendC::Std::is_same_v<decltype(value_without_coord), reg_tensor<int4x2_t>>);
        store(tensor, coordinate, value);
        store(tensor, value_without_coord);
        store<store_sideband_mode::store_1st>(tensor, coordinate, value);
        store<store_sideband_mode::store_1st>(tensor, value_without_coord);
    }
}

template <typename Tensor, asc::te::experimental::broadcast_mode Mode>
__simd_callee__ inline void compile_int4_broadcast(Tensor& tensor)
{
    using namespace asc::te::experimental;
    auto value = load_broadcast<Mode>(tensor, asc::te::make_coord(64));
    auto value_without_coord = load_broadcast<Mode>(tensor);
    static_assert(AscendC::Std::is_same_v<decltype(value), reg_tensor<int4x2_t>>);
    static_assert(AscendC::Std::is_same_v<decltype(value_without_coord), reg_tensor<int4x2_t>>);
}

template <typename Tensor>
__simd_vf__ inline void compile_int4_interfaces(Tensor tensor)
{
    using namespace asc::te::experimental;
    compile_int4_load<Tensor, load_sideband_mode::direct>(tensor);
    compile_int4_load<Tensor, load_sideband_mode::downsample>(tensor);
    compile_int4_load<Tensor, load_sideband_mode::unpack>(tensor);
    compile_int4_load<Tensor, load_sideband_mode::unpack4>(tensor);
    compile_int4_load<Tensor, load_sideband_mode::upsample>(tensor);
    compile_int4_load<Tensor, load_sideband_mode::deintlv>(tensor);
    compile_int4_broadcast<Tensor, broadcast_mode::elem>(tensor);
    compile_int4_broadcast<Tensor, broadcast_mode::datablock>(tensor);
}

template <typename DataType>
__aicore__ inline void instantiate_int4_interfaces(__ubuf__ DataType* source)
{
    auto layout = asc::te::make_layout(asc::te::make_shape(2048));
    auto tensor = asc::te::make_tensor(asc::te::make_mem_ptr<asc::te::location::ub>(source), layout);
    static_assert(AscendC::Std::is_same_v<typename decltype(tensor)::data_type, DataType>);
    compile_int4_interfaces(tensor);
}

} // namespace

TEST(TensorApiRegInt4, CompileLogicalInt4Interfaces)
{
    auto interfaces = &instantiate_int4_interfaces<int4b_t>;
    (void)interfaces;
}

TEST(TensorApiRegInt4, LogicalCoordinateToPackedAddress)
{
    __ubuf__ int4b_t storage[128];
    auto layout = asc::te::make_layout(asc::te::make_shape(4, 64), asc::te::make_stride(64, 1));
    auto tensor = asc::te::make_tensor(asc::te::make_mem_ptr<asc::te::location::ub>(storage), layout);

    auto engine00 = tensor.engine() + tensor.layout()(asc::te::make_coord(0, 0));
    auto engine02 = tensor.engine() + tensor.layout()(asc::te::make_coord(0, 2));
    auto engine10 = tensor.engine() + tensor.layout()(asc::te::make_coord(1, 0));
    auto engine12 = tensor.engine() + tensor.layout()(asc::te::make_coord(1, 2));

    EXPECT_EQ(engine00.begin().get(), storage);
    EXPECT_EQ(engine02.begin().get(), storage + 1);
    EXPECT_EQ(engine10.begin().get(), storage + 32);
    EXPECT_EQ(engine12.begin().get(), storage + 33);
}

class TensorApiRegInt4Runtime : public testing::Test {
protected:
    void SetUp() override
    {
        AscendC::SetGCoreType(2);
        load_call_count = 0;
        store_call_count = 0;
        expected_load_mode = NORM;
        expected_store_mode = NORM_B8;
        expected_store_mask = 0;
        store_mask_pattern = 0xFF;
        std::memset(&expected_store_mask, 0xFF, sizeof(expected_store_mask));
        MOCKER(static_cast<void (*)(vector_fp4x2_e1m2_t&, __ubuf__ fp4x2_e1m2_t*, int32_t, Literal)>(vlds))
            .stubs()
            .will(invoke(load_packed_stub));
        MOCKER(
            static_cast<void (*)(vector_fp4x2_e1m2_t&, vector_fp4x2_e1m2_t&, __ubuf__ fp4x2_e1m2_t*, int32_t, int32_t)>(
                vlds))
            .stubs()
            .will(invoke(load_interleaved_stub));
        MOCKER(static_cast<void (*)(vector_fp4x2_e1m2_t, __ubuf__ fp4x2_e1m2_t*, int32_t, Literal, vector_bool)>(vsts))
            .stubs()
            .will(invoke(store_packed_stub));
        MOCKER(
            static_cast<void (*)(
                vector_fp4x2_e1m2_t, vector_fp4x2_e1m2_t, __ubuf__ fp4x2_e1m2_t*, int32_t, int32_t, vector_bool)>(vsts))
            .stubs()
            .will(invoke(store_interleaved_stub));
        for (size_t index = 0; index < source.size(); ++index) {
            source[index] = static_cast<uint8_t>(index * 73 + 19);
        }
        destination.fill(0xA5);
    }

    void TearDown() override
    {
        GlobalMockObject::verify();
        AscendC::SetGCoreType(0);
    }

    auto make_tensor(uint8_t* storage)
    {
        return asc::te::make_tensor(
            asc::te::make_mem_ptr<asc::te::location::ub>(reinterpret_cast<__ubuf__ int4b_t*>(storage)),
            asc::te::make_layout(asc::te::make_shape(4096)));
    }

    template <asc::te::experimental::load_sideband_mode Mode>
    void check_load()
    {
        using namespace asc::te::experimental;
        if constexpr (Mode == load_sideband_mode::downsample) {
            expected_load_mode = DS_B8;
        } else if constexpr (Mode == load_sideband_mode::unpack) {
            expected_load_mode = UNPK_B8;
        } else if constexpr (Mode == load_sideband_mode::unpack4) {
            expected_load_mode = UNPK4_B8;
        } else if constexpr (Mode == load_sideband_mode::upsample) {
            expected_load_mode = US_B8;
        }
        auto tensor = make_tensor(source.data());
        auto first = load<Mode>(tensor);
        auto offset = load<Mode>(tensor, asc::te::make_coord(64));
        static_assert(AscendC::Std::is_same_v<decltype(first), reg_tensor<int4x2_t>>);
        const auto* first_bytes = reinterpret_cast<const uint8_t*>(&first.reg);
        const auto* offset_bytes = reinterpret_cast<const uint8_t*>(&offset.reg);
        for (size_t index = 0; index < sizeof(first.reg); ++index) {
            size_t source_index = index;
            bool padding = false;
            if constexpr (Mode == load_sideband_mode::downsample) {
                source_index = index * 2;
            } else if constexpr (Mode == load_sideband_mode::unpack) {
                source_index = index / 2;
                padding = index % 2 != 0;
            } else if constexpr (Mode == load_sideband_mode::unpack4) {
                source_index = index / 4;
                padding = index % 4 != 0;
            } else if constexpr (Mode == load_sideband_mode::upsample) {
                source_index = index / 2;
            }
            EXPECT_EQ(first_bytes[index], padding ? 0 : source[source_index]) << index;
            EXPECT_EQ(offset_bytes[index], padding ? 0 : source[source_index + 32]) << index;
        }
        EXPECT_EQ(load_call_count, 2);
    }

    alignas(32) std::array<uint8_t, 2048> source{};
    alignas(32) std::array<uint8_t, 2048> destination{};
};

class TensorApiRegInt4MaskedStore : public TensorApiRegInt4Runtime, public testing::WithParamInterface<uint8_t> {};

TEST_P(TensorApiRegInt4MaskedStore, PreservesMaskedBytes)
{
    using namespace asc::te::experimental;
    auto tensor = make_tensor(destination.data());
    reg_tensor<int4x2_t> value;
    std::memcpy(&value.reg, source.data(), sizeof(value.reg));
    store_mask_pattern = GetParam();
    value.mask = GetParam();
    expected_store_mask = value.mask;
    for (size_t byte_offset : {size_t{0}, size_t{32}}) {
        destination.fill(0xA5);
        if (byte_offset == 0) {
            store(tensor, value);
        } else {
            store(tensor, asc::te::make_coord(64), value);
        }
        for (size_t index = 0; index < destination.size(); ++index) {
            uint8_t expected = 0xA5;
            if (index >= byte_offset && index < byte_offset + sizeof(value.reg)) {
                size_t register_index = index - byte_offset;
                if ((GetParam() & (1U << (register_index % 8))) != 0) {
                    expected = source[register_index];
                }
            }
            EXPECT_EQ(destination[index], expected) << index;
        }
    }
    EXPECT_EQ(store_call_count, 2);
}

INSTANTIATE_TEST_SUITE_P(Masks, TensorApiRegInt4MaskedStore, testing::Values(0x00, 0x55, 0x80, 0xFF));

TEST_F(TensorApiRegInt4Runtime, TwoDimensionalCoordinateRoundTrip)
{
    using namespace asc::te::experimental;
    auto layout = asc::te::make_layout(asc::te::make_shape(4, 1024), asc::te::make_stride(1024, 1));
    auto src_tensor = asc::te::make_tensor(
        asc::te::make_mem_ptr<asc::te::location::ub>(reinterpret_cast<__ubuf__ int4b_t*>(source.data())), layout);
    auto dst_tensor = asc::te::make_tensor(
        asc::te::make_mem_ptr<asc::te::location::ub>(reinterpret_cast<__ubuf__ int4b_t*>(destination.data())), layout);
    auto coordinate = asc::te::make_coord(1, 64);
    constexpr size_t byte_offset = (1024 + 64) / 2;
    auto value = load(src_tensor, coordinate);
    static_assert(AscendC::Std::is_same_v<decltype(value), reg_tensor<int4x2_t>>);
    EXPECT_EQ(std::memcmp(&value.reg, source.data() + byte_offset, sizeof(value.reg)), 0);
    std::memset(&value.mask, 0xFF, sizeof(value.mask));
    store(dst_tensor, coordinate, value);
    for (size_t index = 0; index < destination.size(); ++index) {
        uint8_t expected = index >= byte_offset && index < byte_offset + sizeof(value.reg) ? source[index] : 0xA5;
        EXPECT_EQ(destination[index], expected) << index;
    }
    EXPECT_EQ(load_call_count, 1);
    EXPECT_EQ(store_call_count, 1);
}

TEST_F(TensorApiRegInt4Runtime, InterleaveStoreIgnoresSourceMasks)
{
    using namespace asc::te::experimental;
    auto tensor = make_tensor(destination.data());
    reg_tensor<int4x2_t> first;
    reg_tensor<int4x2_t> second;
    std::memcpy(&first.reg, source.data(), sizeof(first.reg));
    std::memcpy(&second.reg, source.data() + sizeof(first.reg), sizeof(second.reg));
    std::memset(&first.mask, 0, sizeof(first.mask));
    std::memset(&second.mask, 0, sizeof(second.mask));
    for (size_t byte_offset : {size_t{0}, size_t{32}}) {
        destination.fill(0xA5);
        if (byte_offset == 0) {
            store(tensor, first, second);
        } else {
            store(tensor, asc::te::make_coord(64), first, second);
        }
        for (size_t index = 0; index < destination.size(); ++index) {
            uint8_t expected = 0xA5;
            if (index >= byte_offset && index < byte_offset + sizeof(first.reg) * 2) {
                size_t register_index = index - byte_offset;
                expected = source[register_index / 2 + (register_index % 2) * sizeof(first.reg)];
            }
            EXPECT_EQ(destination[index], expected) << index;
        }
    }
    EXPECT_EQ(store_call_count, 2);
}

TEST_F(TensorApiRegInt4Runtime, DirectLoad) { check_load<asc::te::experimental::load_sideband_mode::direct>(); }

TEST_F(TensorApiRegInt4Runtime, DownsampleLoad) { check_load<asc::te::experimental::load_sideband_mode::downsample>(); }

TEST_F(TensorApiRegInt4Runtime, UnpackLoad) { check_load<asc::te::experimental::load_sideband_mode::unpack>(); }

TEST_F(TensorApiRegInt4Runtime, Unpack4Load) { check_load<asc::te::experimental::load_sideband_mode::unpack4>(); }

TEST_F(TensorApiRegInt4Runtime, UpsampleLoad) { check_load<asc::te::experimental::load_sideband_mode::upsample>(); }

TEST_F(TensorApiRegInt4Runtime, DeinterleaveLoadAndInterleaveStore)
{
    using namespace asc::te::experimental;
    auto src_tensor = make_tensor(source.data());
    auto dst_tensor = make_tensor(destination.data());
    auto first = load<load_sideband_mode::deintlv>(src_tensor);
    auto offset = load<load_sideband_mode::deintlv>(src_tensor, asc::te::make_coord(64));
    static_assert(AscendC::Std::is_same_v<decltype(first), reg_pair<int4x2_t>>);
    const auto* even = reinterpret_cast<const uint8_t*>(&first.first.reg);
    const auto* odd = reinterpret_cast<const uint8_t*>(&first.second.reg);
    const auto* offset_even = reinterpret_cast<const uint8_t*>(&offset.first.reg);
    const auto* offset_odd = reinterpret_cast<const uint8_t*>(&offset.second.reg);
    for (size_t index = 0; index < sizeof(first.first.reg); ++index) {
        EXPECT_EQ(even[index], source[index * 2]);
        EXPECT_EQ(odd[index], source[index * 2 + 1]);
        EXPECT_EQ(offset_even[index], source[index * 2 + 32]);
        EXPECT_EQ(offset_odd[index], source[index * 2 + 33]);
    }
    store<store_sideband_mode::intlv>(dst_tensor, first.first, first.second);
    EXPECT_EQ(std::memcmp(destination.data(), source.data(), sizeof(first.first.reg) * 2), 0);
    destination.fill(0xA5);
    store<store_sideband_mode::intlv>(dst_tensor, asc::te::make_coord(64), offset.first, offset.second);
    EXPECT_EQ(std::memcmp(destination.data() + 32, source.data() + 32, sizeof(first.first.reg) * 2), 0);
    EXPECT_EQ(destination[31], 0xA5);
    EXPECT_EQ(load_call_count, 2);
    EXPECT_EQ(store_call_count, 2);
}

TEST_F(TensorApiRegInt4Runtime, ElementBroadcast)
{
    using namespace asc::te::experimental;
    expected_load_mode = BRC_B8;
    auto tensor = make_tensor(source.data());
    auto first = load_broadcast<broadcast_mode::elem>(tensor);
    auto offset = load_broadcast<broadcast_mode::elem>(tensor, asc::te::make_coord(64));
    const auto* first_bytes = reinterpret_cast<const uint8_t*>(&first.reg);
    const auto* offset_bytes = reinterpret_cast<const uint8_t*>(&offset.reg);
    for (size_t index = 0; index < sizeof(first.reg); ++index) {
        EXPECT_EQ(first_bytes[index], source[0]);
        EXPECT_EQ(offset_bytes[index], source[32]);
    }
    EXPECT_EQ(load_call_count, 2);
}

TEST_F(TensorApiRegInt4Runtime, DataBlockBroadcast)
{
    using namespace asc::te::experimental;
    expected_load_mode = BLK;
    auto tensor = make_tensor(source.data());
    auto first = load_broadcast<broadcast_mode::datablock>(tensor);
    auto offset = load_broadcast<broadcast_mode::datablock>(tensor, asc::te::make_coord(64));
    const auto* first_bytes = reinterpret_cast<const uint8_t*>(&first.reg);
    const auto* offset_bytes = reinterpret_cast<const uint8_t*>(&offset.reg);
    for (size_t index = 0; index < sizeof(first.reg); ++index) {
        EXPECT_EQ(first_bytes[index], source[index % 32]);
        EXPECT_EQ(offset_bytes[index], source[index % 32 + 32]);
    }
    EXPECT_EQ(load_call_count, 2);
}

TEST_F(TensorApiRegInt4Runtime, DirectStore)
{
    using namespace asc::te::experimental;
    auto tensor = make_tensor(destination.data());
    reg_tensor<int4x2_t> value;
    std::memcpy(&value.reg, source.data(), sizeof(value.reg));
    std::memset(&value.mask, 0xFF, sizeof(value.mask));
    store(tensor, value);
    EXPECT_EQ(std::memcmp(destination.data(), source.data(), sizeof(value.reg)), 0);
    EXPECT_EQ(destination[sizeof(value.reg)], 0xA5);
    destination.fill(0xA5);
    store(tensor, asc::te::make_coord(64), value);
    EXPECT_EQ(std::memcmp(destination.data() + 32, source.data(), sizeof(value.reg)), 0);
    EXPECT_EQ(destination[31], 0xA5);
    EXPECT_EQ(destination[32 + sizeof(value.reg)], 0xA5);
    EXPECT_EQ(store_call_count, 2);
}

TEST_F(TensorApiRegInt4Runtime, StoreFirstPackedByte)
{
    using namespace asc::te::experimental;
    expected_store_mode = ONEPT_B8;
    auto tensor = make_tensor(destination.data());
    reg_tensor<int4x2_t> value;
    std::memcpy(&value.reg, source.data(), sizeof(value.reg));
    std::memset(&value.mask, 0, sizeof(value.mask));
    store<store_sideband_mode::store_1st>(tensor, value);
    EXPECT_EQ(destination[0], source[0]);
    EXPECT_EQ(destination[1], 0xA5);
    store<store_sideband_mode::store_1st>(tensor, asc::te::make_coord(64), value);
    EXPECT_EQ(destination[31], 0xA5);
    EXPECT_EQ(destination[32], source[0]);
    EXPECT_EQ(destination[33], 0xA5);
    EXPECT_EQ(store_call_count, 2);
}
