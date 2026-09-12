/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef TESTS_API_C_API_NPU_ARCH_9201_UTILS_TEST_COPY_L0C2L1_INSTR_UTILS_H
#define TESTS_API_C_API_NPU_ARCH_9201_UTILS_TEST_COPY_L0C2L1_INSTR_UTILS_H

#include <gtest/gtest.h>
#include <mockcpp/mockcpp.hpp>
#include "tests/api/c_api/stub/cce_stub.h"
#include "impl/c_api/instr_impl/npu_arch_9201/utils_impl/utils_impl.h"

#ifndef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#endif

// Basic overload: no quant_pre_rnd / enable_nz2dn.
// copy_matrix_cc_to_cbuf arity exceeds mockcpp hook limit; exercise empty CPU stubs without MOCKER.
#define TEST_CUBE_DATAMOVE_L0C2L1_BASIC(class_name, c_api_name, cce_name, dst_data_type, src_data_type, index)      \
                                                                                                                    \
    class TestCubeDatamove##class_name##_##dst_data_type##_##src_data_type##_Basic_##index : public testing::Test { \
    protected:                                                                                                      \
        void SetUp() { g_coreType = C_API_AIC_TYPE; }                                                               \
        void TearDown() { g_coreType = C_API_AIV_TYPE; }                                                            \
    };                                                                                                              \
                                                                                                                    \
    TEST_F(                                                                                                         \
        TestCubeDatamove##class_name##_##dst_data_type##_##src_data_type##_Basic_##index,                           \
        c_api_name##_##dst_data_type##_##src_data_type##_Basic_Succ)                                                \
    {                                                                                                               \
        __cbuf__ dst_data_type* dst = reinterpret_cast<__cbuf__ dst_data_type*>(1);                                 \
        __cc__ src_data_type* src = reinterpret_cast<__cc__ src_data_type*>(2);                                     \
        uint16_t n_size = 4;                                                                                        \
        uint16_t m_size = 5;                                                                                        \
        uint32_t dst_stride = 6;                                                                                    \
        uint16_t src_stride = 7;                                                                                    \
        uint8_t enable_clip_relu_pre = 8;                                                                           \
        uint8_t unit_flag_mode = 10;                                                                                \
        uint64_t quant_pre_mode = 11;                                                                               \
        uint8_t relu_pre_mode = 12;                                                                                 \
        bool enable_channel_split = true;                                                                           \
        bool enable_nz2nd = true;                                                                                   \
        uint64_t quant_post = 13;                                                                                   \
        uint8_t relu_post = 14;                                                                                     \
        bool clip_relu_post = true;                                                                                 \
        uint8_t eltwise_op = 15;                                                                                    \
        uint8_t eltwise_antq_en = 1;                                                                                \
        bool c0_pad_en = true;                                                                                      \
                                                                                                                    \
        c_api_name(                                                                                                 \
            dst, src, n_size, m_size, dst_stride, src_stride, enable_clip_relu_pre, unit_flag_mode, quant_pre_mode, \
            relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post, clip_relu_post, eltwise_op,   \
            eltwise_antq_en, c0_pad_en);                                                                            \
        GlobalMockObject::verify();                                                                                 \
    }

// Overload with bool quant_pre_rnd (no enable_nz2dn)
#define TEST_CUBE_DATAMOVE_L0C2L1_RND(class_name, c_api_name, cce_name, dst_data_type, src_data_type, index)          \
                                                                                                                      \
    class TestCubeDatamove##class_name##_##dst_data_type##_##src_data_type##_Rnd_##index : public testing::Test {     \
    protected:                                                                                                        \
        void SetUp() { g_coreType = C_API_AIC_TYPE; }                                                                 \
        void TearDown() { g_coreType = C_API_AIV_TYPE; }                                                              \
    };                                                                                                                \
                                                                                                                      \
    TEST_F(                                                                                                           \
        TestCubeDatamove##class_name##_##dst_data_type##_##src_data_type##_Rnd_##index,                               \
        c_api_name##_##dst_data_type##_##src_data_type##_Rnd_Succ)                                                    \
    {                                                                                                                 \
        __cbuf__ dst_data_type* dst = reinterpret_cast<__cbuf__ dst_data_type*>(1);                                   \
        __cc__ src_data_type* src = reinterpret_cast<__cc__ src_data_type*>(2);                                       \
        uint16_t n_size = 4;                                                                                          \
        uint16_t m_size = 5;                                                                                          \
        uint32_t dst_stride = 6;                                                                                      \
        uint16_t src_stride = 7;                                                                                      \
        bool quant_pre_rnd = true;                                                                                    \
        uint8_t enable_clip_relu_pre = 8;                                                                             \
        uint8_t unit_flag_mode = 10;                                                                                  \
        uint64_t quant_pre_mode = 11;                                                                                 \
        uint8_t relu_pre_mode = 12;                                                                                   \
        bool enable_channel_split = true;                                                                             \
        bool enable_nz2nd = true;                                                                                     \
        uint64_t quant_post = 13;                                                                                     \
        uint8_t relu_post = 14;                                                                                       \
        bool clip_relu_post = true;                                                                                   \
        uint8_t eltwise_op = 15;                                                                                      \
        uint8_t eltwise_antq_en = 1;                                                                                  \
        bool c0_pad_en = true;                                                                                        \
                                                                                                                      \
        c_api_name(                                                                                                   \
            dst, src, n_size, m_size, dst_stride, src_stride, quant_pre_rnd, enable_clip_relu_pre, unit_flag_mode,    \
            quant_pre_mode, relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post, clip_relu_post, \
            eltwise_op, eltwise_antq_en, c0_pad_en);                                                                  \
        GlobalMockObject::verify();                                                                                   \
    }

// Overload with enable_nz2dn (no quant_pre_rnd)
#define TEST_CUBE_DATAMOVE_L0C2L1_NZ2DN(class_name, c_api_name, cce_name, dst_data_type, src_data_type, index)      \
                                                                                                                    \
    class TestCubeDatamove##class_name##_##dst_data_type##_##src_data_type##_Nz2dn_##index : public testing::Test { \
    protected:                                                                                                      \
        void SetUp() { g_coreType = C_API_AIC_TYPE; }                                                               \
        void TearDown() { g_coreType = C_API_AIV_TYPE; }                                                            \
    };                                                                                                              \
                                                                                                                    \
    TEST_F(                                                                                                         \
        TestCubeDatamove##class_name##_##dst_data_type##_##src_data_type##_Nz2dn_##index,                           \
        c_api_name##_##dst_data_type##_##src_data_type##_Nz2dn_Succ)                                                \
    {                                                                                                               \
        __cbuf__ dst_data_type* dst = reinterpret_cast<__cbuf__ dst_data_type*>(1);                                 \
        __cc__ src_data_type* src = reinterpret_cast<__cc__ src_data_type*>(2);                                     \
        uint16_t n_size = 4;                                                                                        \
        uint16_t m_size = 5;                                                                                        \
        uint32_t dst_stride = 6;                                                                                    \
        uint16_t src_stride = 7;                                                                                    \
        uint8_t enable_clip_relu_pre = 8;                                                                           \
        uint8_t unit_flag_mode = 10;                                                                                \
        uint64_t quant_pre_mode = 11;                                                                               \
        uint8_t relu_pre_mode = 12;                                                                                 \
        bool enable_channel_split = true;                                                                           \
        bool enable_nz2nd = true;                                                                                   \
        uint64_t quant_post = 13;                                                                                   \
        uint8_t relu_post = 14;                                                                                     \
        bool clip_relu_post = true;                                                                                 \
        uint8_t eltwise_op = 15;                                                                                    \
        uint8_t eltwise_antq_en = 1;                                                                                \
        bool c0_pad_en = true;                                                                                      \
        bool enable_nz2dn = true;                                                                                   \
                                                                                                                    \
        c_api_name(                                                                                                 \
            dst, src, n_size, m_size, dst_stride, src_stride, enable_clip_relu_pre, unit_flag_mode, quant_pre_mode, \
            relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post, clip_relu_post, eltwise_op,   \
            eltwise_antq_en, c0_pad_en, enable_nz2dn);                                                              \
        GlobalMockObject::verify();                                                                                 \
    }

// Overload with quant_pre_rnd + enable_nz2dn (half/float only in current impl)
#define TEST_CUBE_DATAMOVE_L0C2L1_RND_NZ2DN(class_name, c_api_name, cce_name, dst_data_type, src_data_type, index)     \
                                                                                                                       \
    class TestCubeDatamove##class_name##_##dst_data_type##_##src_data_type##_RndNz2dn_##index : public testing::Test { \
    protected:                                                                                                         \
        void SetUp() { g_coreType = C_API_AIC_TYPE; }                                                                  \
        void TearDown() { g_coreType = C_API_AIV_TYPE; }                                                               \
    };                                                                                                                 \
                                                                                                                       \
    TEST_F(                                                                                                            \
        TestCubeDatamove##class_name##_##dst_data_type##_##src_data_type##_RndNz2dn_##index,                           \
        c_api_name##_##dst_data_type##_##src_data_type##_RndNz2dn_Succ)                                                \
    {                                                                                                                  \
        __cbuf__ dst_data_type* dst = reinterpret_cast<__cbuf__ dst_data_type*>(1);                                    \
        __cc__ src_data_type* src = reinterpret_cast<__cc__ src_data_type*>(2);                                        \
        uint16_t n_size = 4;                                                                                           \
        uint16_t m_size = 5;                                                                                           \
        uint32_t dst_stride = 6;                                                                                       \
        uint16_t src_stride = 7;                                                                                       \
        bool quant_pre_rnd = true;                                                                                     \
        uint8_t enable_clip_relu_pre = 8;                                                                              \
        uint8_t unit_flag_mode = 10;                                                                                   \
        uint64_t quant_pre_mode = 11;                                                                                  \
        uint8_t relu_pre_mode = 12;                                                                                    \
        bool enable_channel_split = true;                                                                              \
        bool enable_nz2nd = true;                                                                                      \
        uint64_t quant_post = 13;                                                                                      \
        uint8_t relu_post = 14;                                                                                        \
        bool clip_relu_post = true;                                                                                    \
        uint8_t eltwise_op = 15;                                                                                       \
        uint8_t eltwise_antq_en = 1;                                                                                   \
        bool c0_pad_en = true;                                                                                         \
        bool enable_nz2dn = true;                                                                                      \
                                                                                                                       \
        c_api_name(                                                                                                    \
            dst, src, n_size, m_size, dst_stride, src_stride, quant_pre_rnd, enable_clip_relu_pre, unit_flag_mode,     \
            quant_pre_mode, relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post, clip_relu_post,  \
            eltwise_op, eltwise_antq_en, c0_pad_en, enable_nz2dn);                                                     \
        GlobalMockObject::verify();                                                                                    \
    }

// void* s4 overload with l2_cache_mode
#define TEST_CUBE_DATAMOVE_L0C2L1_S4(class_name, c_api_name, cce_name, src_data_type, index)                           \
                                                                                                                       \
    class TestCubeDatamove##class_name##_void_##src_data_type##_S4_##index : public testing::Test {                    \
    protected:                                                                                                         \
        void SetUp() { g_coreType = C_API_AIC_TYPE; }                                                                  \
        void TearDown() { g_coreType = C_API_AIV_TYPE; }                                                               \
    };                                                                                                                 \
                                                                                                                       \
    TEST_F(                                                                                                            \
        TestCubeDatamove##class_name##_void_##src_data_type##_S4_##index, c_api_name##_void_##src_data_type##_S4_Succ) \
    {                                                                                                                  \
        __cbuf__ void* dst = reinterpret_cast<__cbuf__ void*>(1);                                                      \
        __cc__ src_data_type* src = reinterpret_cast<__cc__ src_data_type*>(2);                                        \
        uint16_t n_size = 3;                                                                                           \
        uint16_t m_size = 4;                                                                                           \
        uint32_t dst_stride = 6;                                                                                       \
        uint16_t src_stride = 7;                                                                                       \
        uint8_t l2_cache_mode = 5;                                                                                     \
        uint8_t enable_clip_relu_pre = 8;                                                                              \
        uint8_t unit_flag_mode = 10;                                                                                   \
        uint64_t quant_pre_mode = 11;                                                                                  \
        uint8_t relu_pre_mode = 12;                                                                                    \
        bool enable_channel_split = true;                                                                              \
        bool enable_nz2nd = true;                                                                                      \
        uint64_t quant_post = 13;                                                                                      \
        uint8_t relu_post = 14;                                                                                        \
        bool clip_relu_post = true;                                                                                    \
        uint8_t eltwise_op = 15;                                                                                       \
        bool eltwise_antq_en = true;                                                                                   \
        bool c0_pad_en = true;                                                                                         \
        bool broadcast_en = false;                                                                                     \
        bool enable_nz2dn = false;                                                                                     \
                                                                                                                       \
        c_api_name(                                                                                                    \
            dst, src, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre, unit_flag_mode,     \
            quant_pre_mode, relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post, clip_relu_post,  \
            eltwise_op, eltwise_antq_en, c0_pad_en, broadcast_en, enable_nz2dn);                                       \
        GlobalMockObject::verify();                                                                                    \
    }

// void* s4 overload with quant_pre_rnd
#define TEST_CUBE_DATAMOVE_L0C2L1_S4_RND(class_name, c_api_name, cce_name, src_data_type, index)                      \
                                                                                                                      \
    class TestCubeDatamove##class_name##_void_##src_data_type##_S4Rnd_##index : public testing::Test {                \
    protected:                                                                                                        \
        void SetUp() { g_coreType = C_API_AIC_TYPE; }                                                                 \
        void TearDown() { g_coreType = C_API_AIV_TYPE; }                                                              \
    };                                                                                                                \
                                                                                                                      \
    TEST_F(                                                                                                           \
        TestCubeDatamove##class_name##_void_##src_data_type##_S4Rnd_##index,                                          \
        c_api_name##_void_##src_data_type##_S4Rnd_Succ)                                                               \
    {                                                                                                                 \
        __cbuf__ void* dst = reinterpret_cast<__cbuf__ void*>(1);                                                     \
        __cc__ src_data_type* src = reinterpret_cast<__cc__ src_data_type*>(2);                                       \
        uint16_t n_size = 3;                                                                                          \
        uint16_t m_size = 4;                                                                                          \
        uint32_t dst_stride = 6;                                                                                      \
        uint16_t src_stride = 7;                                                                                      \
        bool quant_pre_rnd = true;                                                                                    \
        uint8_t enable_clip_relu_pre = 8;                                                                             \
        uint8_t unit_flag_mode = 10;                                                                                  \
        uint64_t quant_pre_mode = 11;                                                                                 \
        uint8_t relu_pre_mode = 12;                                                                                   \
        bool enable_channel_split = true;                                                                             \
        bool enable_nz2nd = true;                                                                                     \
        uint64_t quant_post = 13;                                                                                     \
        uint8_t relu_post = 14;                                                                                       \
        bool clip_relu_post = true;                                                                                   \
        uint8_t eltwise_op = 15;                                                                                      \
        bool eltwise_antq_en = true;                                                                                  \
        bool c0_pad_en = true;                                                                                        \
        bool broadcast_en = false;                                                                                    \
        bool enable_nz2dn = false;                                                                                    \
                                                                                                                      \
        c_api_name(                                                                                                   \
            dst, src, n_size, m_size, dst_stride, src_stride, quant_pre_rnd, enable_clip_relu_pre, unit_flag_mode,    \
            quant_pre_mode, relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post, clip_relu_post, \
            eltwise_op, eltwise_antq_en, c0_pad_en, broadcast_en, enable_nz2dn);                                      \
        GlobalMockObject::verify();                                                                                   \
    }

#endif
