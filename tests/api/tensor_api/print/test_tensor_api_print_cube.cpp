/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <algorithm>
#include <cstring>

#include <gtest/gtest.h>

#include "tensor_api/stub/cce_stub.h"

template <typename DstType, typename SrcType>
DstType tensor_api_print_test_bit_cast(const SrcType& src)
{
    static_assert(sizeof(DstType) == sizeof(SrcType));
    DstType dst;
    std::memcpy(&dst, &src, sizeof(dst));
    return dst;
}

#define __builtin_bit_cast(dst_type, src) tensor_api_print_test_bit_cast<dst_type>(src)
#include "include/tensor_api/tensor.h"
#undef __builtin_bit_cast

namespace {

using tensor_api_print_debug_bus_test::data;
using tensor_api_print_debug_bus_test::data_read_count;
using tensor_api_print_debug_bus_test::debug_bus_address;
using tensor_api_print_debug_bus_test::last_local_offset;
using tensor_api_print_debug_bus_test::last_model_base;
using tensor_api_print_debug_bus_test::ready;
using tensor_api_print_test::output;
using tensor_api_print_test::records;
using tensor_api_print_test::reset_records;

void reset_test_state()
{
    reset_records();
    tensor_api_print_debug_bus_test::reset();
}

class TensorApiPrintCubeTest : public testing::Test {
protected:
    void SetUp() override
    {
        reset_test_state();
    }
};

void expect_single_head()
{
    ASSERT_FALSE(records().empty());
    EXPECT_EQ(records().front().head, "[AIC Block 0/1] ");
    EXPECT_TRUE(
        std::all_of(records().begin() + 1, records().end(), [](const auto& record) { return record.head.empty(); }));
}

template <typename Location>
void expect_debug_bus_values(uint32_t expected_model_base, uint32_t local_address_shift)
{
    using namespace asc::te;

    alignas(32) int32_t local_data[2] = {};
    auto tensor = make_tensor(make_mem_ptr<Location>(local_data), make_layout(make_shape(1, 2)));
    data[0] = 11U;
    data[1] = 22U;

    print_tensor(tensor, false);

    EXPECT_EQ(output(), "[AIC Block 0/1] 11  22  \n");
    EXPECT_EQ(last_model_base, expected_model_base);
    EXPECT_EQ(last_local_offset, static_cast<uint32_t>(reinterpret_cast<uint64_t>(local_data) >> local_address_shift));
    EXPECT_EQ(data_read_count, 8U);
    expect_single_head();
}

TEST_F(TensorApiPrintCubeTest, PrintTensorReadsL1L0aAndL0bThroughDebugBus)
{
    expect_debug_bus_values<asc::te::location::l1>(0x620000U, 5U);

    reset_test_state();
    expect_debug_bus_values<asc::te::location::l0a>(0x410000U, 0U);

    reset_test_state();
    expect_debug_bus_values<asc::te::location::l0b>(0x420000U, 0U);
}

TEST_F(TensorApiPrintCubeTest, PrintTensorMarksElementsWhenDebugBusIsUnavailable)
{
    using namespace asc::te;

    alignas(32) int32_t local_data[2] = {};
    auto tensor = make_tensor(make_mem_ptr<location::l1>(local_data), make_layout(make_shape(1, 2)));
    debug_bus_address = 0U;

    print_tensor(tensor, false);

    EXPECT_EQ(output(), "[AIC Block 0/1] ?  ?  \n");
    EXPECT_EQ(data_read_count, 0U);
    expect_single_head();
}

TEST_F(TensorApiPrintCubeTest, PrintTensorMarksElementsWhenDebugBusTimesOut)
{
    using namespace asc::te;

    alignas(32) int32_t local_data[1] = {};
    auto tensor = make_tensor(make_mem_ptr<location::l0a>(local_data), make_layout(make_shape(1)));
    ready = false;

    print_tensor(tensor, false);

    EXPECT_EQ(output(), "[AIC Block 0/1] ?  \n");
    EXPECT_EQ(tensor_api_print_debug_bus_test::busy_read_count, 6U);
    EXPECT_EQ(data_read_count, 0U);
    expect_single_head();
}

TEST_F(TensorApiPrintCubeTest, PrintTensorExplainsUnsupportedCubeMemoryLocations)
{
    using namespace asc::te;

    int32_t local_data[1] = {};
    auto layout = make_layout(make_shape(1));
    auto l0scalea_tensor = make_tensor(make_mem_ptr<location::l0scalea>(local_data), layout);
    auto l0scaleb_tensor = make_tensor(make_mem_ptr<location::l0scaleb>(local_data), layout);
    auto l0c_tensor = make_tensor(make_mem_ptr<location::l0c>(local_data), layout);
    auto bias_tensor = make_tensor(make_mem_ptr<location::bias>(local_data), layout);
    auto fixbuf_tensor = make_tensor(make_mem_ptr<location::fixbuf>(local_data), layout);
    auto ssbuf_tensor = make_tensor(make_mem_ptr<location::ssbuf>(local_data), layout);
    auto invalid_tensor = make_tensor(make_mem_ptr<location::invalid>(local_data), layout);

    const auto expect_unsupported = [](const auto& tensor, const char* location_name) {
        reset_records();
        print_tensor(tensor, false);
        EXPECT_EQ(output(), std::string("[AIC Block 0/1] [print_tensor] tensor elements cannot be printed from ")
                                + location_name
                                + "; only GM, UB, ordinary local memory, L1, L0A, and L0B are supported.\n");
        expect_single_head();
    };

    expect_unsupported(l0scalea_tensor, "L0ScaleA");
    expect_unsupported(l0scaleb_tensor, "L0ScaleB");
    expect_unsupported(l0c_tensor, "L0C (__cc__)");
    expect_unsupported(bias_tensor, "BiasTable (__biasbuf__)");
    expect_unsupported(fixbuf_tensor, "FixBuf (__fbuf__)");
    expect_unsupported(ssbuf_tensor, "SSBuf (__ssbuf__)");
    expect_unsupported(invalid_tensor, "this memory location");
}

} // namespace
