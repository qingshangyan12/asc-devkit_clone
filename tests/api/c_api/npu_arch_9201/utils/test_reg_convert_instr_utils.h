/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef TESTS_API_C_API_NPU_ARCH_9201_UTILS_TEST_REG_CONVERT_INSTR_UTILS_H
#define TESTS_API_C_API_NPU_ARCH_9201_UTILS_TEST_REG_CONVERT_INSTR_UTILS_H

#include <gtest/gtest.h>
#include <mockcpp/mockcpp.hpp>
#include "tests/api/c_api/stub/cce_stub.h"
#include "include/c_api/reg_compute/reg_convert.h"

// CPU stub verification: exercise the C API path that calls into empty tikicpulib stubs
// (same approach as 9201 cube_datamove UT when mockcpp cannot bind overloads).

#define TEST_9201_REG_CONVERT_POS(class_name, c_api_name, dst_data_type, src_data_type, pos_arg, index) \
                                                                                                        \
    class Test9201RegConvert##class_name##_##index : public testing::Test {                             \
    protected:                                                                                          \
        void SetUp() {}                                                                                 \
        void TearDown() {}                                                                              \
    };                                                                                                  \
                                                                                                        \
    TEST_F(Test9201RegConvert##class_name##_##index, c_api_name##_Succ)                                 \
    {                                                                                                   \
        dst_data_type dst;                                                                              \
        src_data_type src0;                                                                             \
        vector_bool mask;                                                                               \
                                                                                                        \
        c_api_name(dst, src0, mask, pos_arg);                                                           \
        GlobalMockObject::verify();                                                                     \
    }

#define TEST_9201_REG_CONVERT_RETURN_POS(class_name, c_api_name, dst_data_type, src_data_type, pos_arg, index) \
                                                                                                               \
    class Test9201RegConvertRet##class_name##_##index : public testing::Test {                                 \
    protected:                                                                                                 \
        void SetUp() {}                                                                                        \
        void TearDown() {}                                                                                     \
    };                                                                                                         \
                                                                                                               \
    TEST_F(Test9201RegConvertRet##class_name##_##index, c_api_name##_return_Succ)                              \
    {                                                                                                          \
        src_data_type src0;                                                                                    \
        vector_bool mask;                                                                                      \
                                                                                                               \
        dst_data_type dst = c_api_name(src0, mask, pos_arg);                                                   \
        (void)dst;                                                                                             \
        GlobalMockObject::verify();                                                                            \
    }

// Void asc_cast_merge with trailing tag args (round/sat/pos as needed).
#define TEST_9201_ASC_CAST_MERGE(class_name, dst_data_type, src_data_type, index, ...) \
                                                                                       \
    class Test9201AscCastMerge##class_name##_##index : public testing::Test {          \
    protected:                                                                         \
        void SetUp() {}                                                                \
        void TearDown() {}                                                             \
    };                                                                                 \
                                                                                       \
    TEST_F(Test9201AscCastMerge##class_name##_##index, asc_cast_merge_Succ)            \
    {                                                                                  \
        dst_data_type dst;                                                             \
        src_data_type src0;                                                            \
        vector_bool mask;                                                              \
                                                                                       \
        asc_cast_merge(dst, src0, mask, __VA_ARGS__);                                  \
        GlobalMockObject::verify();                                                    \
    }

#endif
