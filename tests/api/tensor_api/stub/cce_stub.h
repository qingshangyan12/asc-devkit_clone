/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/
#ifndef TESTS_API_TENSOR_API_STUB_CCE_STUB_H
#define TESTS_API_TENSOR_API_STUB_CCE_STUB_H

#include <cstdint>
#include "stub_fun.h"

#ifndef __biasbuf__
#define __biasbuf__
#endif

#endif


static void * img2colv2_cbuf_to_ca_origin_dst_addr;
static void * img2colv2_cbuf_to_ca_origin_src_addr;
static bool is_mock_img2colv2_cbuf_to_ca = false;
static bool img2colv2_cbuf_to_ca_call_flag = false;

#define mock_img2colv2_cbuf_to_ca(TYPE) \
inline void img2colv2_cbuf_to_ca(__ca__ TYPE* dst, __cbuf__ TYPE* src, \
    uint16_t stepK, uint16_t stepM, uint16_t posK, uint16_t posM, \
    uint8_t strideW, uint8_t strideH, uint8_t Wk, uint8_t Hk, \
    uint8_t dilationW, uint8_t dilationH, bool filterW, \
    bool filterH, bool transpose, bool fmatrixCtrl, uint16_t sizeChannel) { \
    if (is_mock_img2colv2_cbuf_to_ca) { \
        img2colv2_cbuf_to_ca_call_flag = true; \
        std::cout << "JH: mock_img2colv2_cbuf_to_ca" << std::endl; \
        EXPECT_EQ(img2colv2_cbuf_to_ca_origin_dst_addr, dst); \
        EXPECT_EQ(img2colv2_cbuf_to_ca_origin_src_addr, src); \
    } \
} \

mock_img2colv2_cbuf_to_ca(bfloat16_t);
mock_img2colv2_cbuf_to_ca(half);
mock_img2colv2_cbuf_to_ca(float);
mock_img2colv2_cbuf_to_ca(int32_t);
mock_img2colv2_cbuf_to_ca(uint32_t);
mock_img2colv2_cbuf_to_ca(int8_t);
mock_img2colv2_cbuf_to_ca(uint8_t);
mock_img2colv2_cbuf_to_ca(int16_t);
mock_img2colv2_cbuf_to_ca(uint16_t);


static void * img2colv2_cbuf_to_cb_origin_dst_addr;
static void * img2colv2_cbuf_to_cb_origin_src_addr;
static bool is_mock_img2colv2_cbuf_to_cb = false;
static bool img2colv2_cbuf_to_cb_call_flag = false;

#define mock_img2colv2_cbuf_to_cb(TYPE) \
inline void img2colv2_cbuf_to_cb(__ca__ TYPE* dst, __cbuf__ TYPE* src, \
    uint16_t stepK, uint16_t stepM, uint16_t posK, uint16_t posM, \
    uint8_t strideW, uint8_t strideH, uint8_t Wk, uint8_t Hk, \
    uint8_t dilationW, uint8_t dilationH, bool filterW, \
    bool filterH, bool transpose, bool fmatrixCtrl, uint16_t sizeChannel) { \
    if (is_mock_img2colv2_cbuf_to_cb) { \
        img2colv2_cbuf_to_cb_call_flag = true; \
        EXPECT_EQ(img2colv2_cbuf_to_cb_origin_dst_addr, dst); \
        EXPECT_EQ(img2colv2_cbuf_to_cb_origin_src_addr, src); \
    } \
} \

mock_img2colv2_cbuf_to_cb(bfloat16_t);
mock_img2colv2_cbuf_to_cb(half);
mock_img2colv2_cbuf_to_cb(float);
mock_img2colv2_cbuf_to_cb(int32_t);
mock_img2colv2_cbuf_to_cb(uint32_t);
mock_img2colv2_cbuf_to_cb(int8_t);
mock_img2colv2_cbuf_to_cb(uint8_t);
mock_img2colv2_cbuf_to_cb(int16_t);
mock_img2colv2_cbuf_to_cb(uint16_t);