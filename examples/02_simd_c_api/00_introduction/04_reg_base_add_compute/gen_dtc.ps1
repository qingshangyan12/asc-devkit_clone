# Auto-generate all zeroed .asc files - reg_load and reg_store
$baseDir = "C:\JC\asc-devkit\examples\02_simd_c_api\00_introduction\04_reg_base_add_compute"

$license = @'
/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
'@

$includes = @'
#include <cstdint>
#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include "acl/acl.h"
#include "c_api/asc_simd.h"
'@

function MaskFor($type) {
    switch ($type) {
        'uint8_t'    { return 'b8' }
        'int8_t'     { return 'b8' }
        'hifloat8_t' { return 'b8' }
        'half'       { return 'b16' }
        'uint16_t'   { return 'b16' }
        'int16_t'    { return 'b16' }
        'bfloat16_t' { return 'b16' }
        'float'      { return 'b32' }
        'uint32_t'   { return 'b32' }
        'int32_t'    { return 'b32' }
        'int64_t'    { return 'b32' }
        default      { return 'b32' }
    }
}

function PrintFor($type, $var) {
    switch ($type) {
        {$_ -in 'uint8_t','int8_t'}             { return "(int)$var" }
        {$_ -in 'uint16_t','int16_t'}           { return "(int)$var" }
        {$_ -in 'uint32_t','int32_t'}           { return $var }
        {$_ -in 'int64_t'}                      { return "(long long)$var" }
        {$_ -in 'half','bfloat16_t','hifloat8_t','float'} { return "(float)$var" }
        default                                 { return $var }
    }
}

function WriteFile($path, $content) {
    $dir = Split-Path $path -Parent
    if (-not (Test-Path $dir)) { New-Item -ItemType Directory -Force -Path $dir | Out-Null }
    $trimmed = $content.Trim()
    [System.IO.File]::WriteAllText($path, $trimmed, [System.Text.Encoding]::UTF8)
    Write-Host "OK: $path"
}

# ============================================================
# Fix data_type_convert - regenerate with proper $ expansion
# ============================================================
$dtc = "$baseDir\data_type_convert"

function GenConvert($name, $srcType, $dstType, $total, $blocks, $srcExpr, $goldenExpr, $convFunc) {
    $mask = MaskFor $srcType
    $outP = PrintFor $dstType 'tensor[i]'
    $goldP = PrintFor $dstType 'golden[i]'
    $funcVf = $convFunc + '_vf'
    $funcCustom = $convFunc + '_custom'
    $funcKernel = 'kernel_' + $convFunc

    $content = @"
$license

$includes

constexpr uint32_t TOTAL_LENGTH = $total;
constexpr uint32_t NUM_BLOCKS = $blocks;
typedef $srcType SRC_TYPE;
typedef $dstType DST_TYPE;

__simd_vf__ inline void $funcVf(__ubuf__ DST_TYPE* dst_local, __ubuf__ SRC_TYPE* src_local, uint32_t data_len)
{
    uint16_t one_rep_size = asc_get_vf_len() / sizeof(SRC_TYPE);
    uint16_t repeat_time = (data_len + one_rep_size - 1) / one_rep_size;
    uint32_t remaining = data_len;
    vector_bool vmask;
    vector_SRC_TYPE reg_src;
    vector_DST_TYPE reg_dst;
    for (uint16_t i = 0; i < repeat_time; ++i) {
        uint32_t cur_count = (remaining > one_rep_size) ? one_rep_size : remaining;
        uint32_t mask_cnt = cur_count;
        vmask = asc_update_mask_$mask(mask_cnt);
        asc_load(reg_src, src_local + i * one_rep_size);
        $convFunc(reg_dst, reg_src, vmask);
        asc_store(dst_local + i * one_rep_size, reg_dst, cur_count);
        remaining = (remaining > one_rep_size) ? (remaining - one_rep_size) : 0;
    }
}

__vector__ __global__ void $funcCustom(__gm__ SRC_TYPE* src, __gm__ DST_TYPE* dst)
{
    asc_init();
    constexpr uint32_t block_length = TOTAL_LENGTH / NUM_BLOCKS;
    constexpr uint8_t mutex_id = 1;
    __gm__ SRC_TYPE* src_gm = src + asc_get_block_idx() * block_length;
    __gm__ DST_TYPE* dst_gm = dst + asc_get_block_idx() * block_length;
    __ubuf__ SRC_TYPE src_local[block_length];
    __ubuf__ DST_TYPE dst_local[block_length];
    uint32_t cache_mode0 = 0; uint32_t cache_mode2 = 0;
    uint32_t src_burst = block_length * sizeof(SRC_TYPE);
    uint32_t dst_burst = block_length * sizeof(DST_TYPE);
    uint64_t src_stride = 0; uint32_t dst_stride = 0;
    asc_lock(PIPE_MTE2, mutex_id);
    asc_copy_gm2ub_align((__ubuf__ SRC_TYPE*)src_local, src_gm, BLK_NUM, src_burst, 0, 0, true, cache_mode0, src_stride, dst_stride);
    asc_unlock(PIPE_MTE2, mutex_id);
    asc_lock(PIPE_V, mutex_id);
    $funcVf((__ubuf__ DST_TYPE*)dst_local, src_local, block_length);
    asc_unlock(PIPE_V, mutex_id);
    asc_lock(PIPE_MTE3, mutex_id);
    asc_copy_ub2gm_align(dst_gm, (__ubuf__ DST_TYPE*)dst_local, BLK_NUM, dst_burst, cache_mode2, src_stride, dst_stride);
    asc_unlock(PIPE_MTE3, mutex_id);
}

std::vector<DST_TYPE> $funcKernel(std::vector<SRC_TYPE>& src_data)
{
    constexpr uint32_t numBlocks = NUM_BLOCKS;
    uint32_t totalLength = TOTAL_LENGTH;
    size_t srcByteSize = totalLength * sizeof(SRC_TYPE);
    size_t dstByteSize = totalLength * sizeof(DST_TYPE);
    int32_t deviceId = 0;
    aclrtStream stream = nullptr;
    uint8_t* srcHost = reinterpret_cast<uint8_t*>(src_data.data());
    uint8_t* dstHost = nullptr;
    SRC_TYPE* srcDevice = nullptr;
    DST_TYPE* dstDevice = nullptr;
    aclInit(nullptr);
    aclrtSetDevice(deviceId);
    aclrtCreateStream(&stream);
    aclrtMallocHost((void**)(&dstHost), dstByteSize);
    aclrtMalloc((void**)&srcDevice, srcByteSize, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc((void**)&dstDevice, dstByteSize, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy((uint8_t*)srcDevice, srcByteSize, srcHost, srcByteSize, ACL_MEMCPY_HOST_TO_DEVICE);
    $funcCustom<<<numBlocks, 0, stream>>>(srcDevice, dstDevice);
    aclrtSynchronizeStream(stream);
    aclrtMemcpy(dstHost, dstByteSize, (uint8_t*)dstDevice, dstByteSize, ACL_MEMCPY_DEVICE_TO_HOST);
    std::vector<DST_TYPE> dst((DST_TYPE*)dstHost, (DST_TYPE*)(dstHost + dstByteSize));
    aclrtFree(srcDevice);
    aclrtFree(dstDevice);
    aclrtFreeHost(dstHost);
    aclrtDestroyStream(stream);
    aclrtResetDevice(deviceId);
    aclFinalize();
    return dst;
}

uint32_t VerifyResult(std::vector<DST_TYPE>& output, std::vector<DST_TYPE>& golden)
{
    auto printTensor = [](std::vector<DST_TYPE>& tensor, const char* name) {
        constexpr size_t maxPrintSize = 20;
        std::cout << name << ": ";
        for (size_t i = 0; i < std::min(tensor.size(), maxPrintSize); ++i) {
            std::cout << $outP << " ";
        }
        if (tensor.size() > maxPrintSize) {
            std::cout << "...";
        }
        std::cout << std::endl;
    };
    printTensor(output, "Output");
    printTensor(golden, "Golden");
    if (std::equal(golden.begin(), golden.end(), output.begin())) {
        std::cout << "[Success] Case accuracy is verification passed." << std::endl;
        return 0;
    } else {
        std::cout << "[Failed] Case accuracy is verification failed!" << std::endl;
        return 1;
    }
    return 0;
}

int32_t main(int32_t argc, char* argv[])
{
    constexpr uint32_t totalLength = TOTAL_LENGTH;
    std::vector<SRC_TYPE> src(totalLength);
    for (uint32_t i = 0; i < totalLength; ++i) {
        src[i] = $srcExpr;
    }
    std::vector<DST_TYPE> output = $funcKernel(src);
    std::vector<DST_TYPE> golden(totalLength);
    for (uint32_t i = 0; i < totalLength; ++i) {
        SRC_TYPE s = src[i];
        golden[i] = $goldenExpr;
    }
    return VerifyResult(output, golden);
}
"@
    WriteFile "$dtc\$name\$name.asc" $content
}

# Regenerate all data_type_convert files
GenConvert 'asc_uint82uint16'     'uint8_t'   'uint16_t'     '8*2048'  8  '(i%200)'                               '(uint16_t)s'                        'asc_uint82uint16'
GenConvert 'asc_uint82half'       'uint8_t'   'half'         '8*2048'  8  '(i%200)'                               '(half)s'                            'asc_uint82half'
GenConvert 'asc_int82int16'       'int8_t'    'int16_t'      '8*2048'  8  '(i%127)-63'                            '(int16_t)s'                         'asc_int82int16'
GenConvert 'asc_int82half'        'int8_t'    'half'         '8*2048'  8  '(i%127)-63'                            '(half)s'                            'asc_int82half'
GenConvert 'asc_half2float'       'half'      'float'        '8*2048'  8  '(half)(i*0.1f)'                        '(float)s'                           'asc_half2float'
GenConvert 'asc_half2bfloat16'    'half'      'bfloat16_t'   '8*2048'  8  '(half)(i*0.1f)'                        '(bfloat16_t)s'                      'asc_half2bfloat16'
GenConvert 'asc_half2int8'        'half'      'int8_t'       '8*2048'  8  '(half)((i%40)-20)'                     '(int8_t)s'                          'asc_half2int8'
GenConvert 'asc_half2uint8'       'half'      'uint8_t'      '8*2048'  8  '(half)(i%200)'                         '(uint8_t)s'                         'asc_half2uint8'
GenConvert 'asc_half2int16'       'half'      'int16_t'      '8*2048'  8  '(half)((i%100)-50)'                    '(int16_t)s'                         'asc_half2int16'
GenConvert 'asc_half2int32'       'half'      'int32_t'      '8*2048'  8  '(half)((i%100)-50)'                    '(int32_t)s'                         'asc_half2int32'
GenConvert 'asc_half2hif8'        'half'      'hifloat8_t'   '8*2048'  8  '(half)(i*0.1f)'                        '(hifloat8_t)s'                      'asc_half2hif8'
GenConvert 'asc_float2half'       'float'     'half'         '8*2048'  8  'i*0.1f'                                '(half)(float)s'                     'asc_float2half'
GenConvert 'asc_float2bfloat16'   'float'     'bfloat16_t'   '8*2048'  8  'i*0.1f'                                '(bfloat16_t)(float)s'               'asc_float2bfloat16'
GenConvert 'asc_float2int16'      'float'     'int16_t'      '8*2048'  8  '((i%100)-50)*1.0f'                     '(int16_t)s'                         'asc_float2int16'
GenConvert 'asc_float2int32'      'float'     'int32_t'      '8*2048'  8  '((i%200)-100)*1.0f'                    '(int32_t)s'                         'asc_float2int32'
GenConvert 'asc_float2int64'      'float'     'int64_t'      '8*2048'  8  '((i%200)-100)*1.0f'                    '(int64_t)s'                         'asc_float2int64'
GenConvert 'asc_int162float'      'int16_t'   'float'        '8*2048'  8  '(i%100)-50'                            '(float)s'                           'asc_int162float'
GenConvert 'asc_int162half'       'int16_t'   'half'         '8*2048'  8  '(i%100)-50'                            '(half)s'                            'asc_int162half'
GenConvert 'asc_int162int32'      'int16_t'   'int32_t'      '8*2048'  8  '(i%100)-50'                            '(int32_t)s'                         'asc_int162int32'
GenConvert 'asc_int162uint32'     'int16_t'   'uint32_t'     '8*2048'  8  'abs((int)(i%200)-100)'                 '(uint32_t)s'                        'asc_int162uint32'
GenConvert 'asc_int162uint8'      'int16_t'   'uint8_t'      '8*2048'  8  'abs((int)(i%200)-100)'                 '(uint8_t)s'                         'asc_int162uint8'
GenConvert 'asc_uint162uint32'    'uint16_t'  'uint32_t'     '8*2048'  8  '(i%500)'                               '(uint32_t)s'                        'asc_uint162uint32'
GenConvert 'asc_uint162uint8'     'uint16_t'  'uint8_t'      '8*2048'  8  '(i%200)'                               '(uint8_t)s'                         'asc_uint162uint8'
GenConvert 'asc_int322float'      'int32_t'   'float'        '8*2048'  8  '((int)(i%200)-100)'                    '(float)s'                           'asc_int322float'
GenConvert 'asc_int322int16'      'int32_t'   'int16_t'      '8*2048'  8  '((int)(i%200)-100)'                    '(int16_t)s'                         'asc_int322int16'
GenConvert 'asc_int322int64'      'int32_t'   'int64_t'      '8*2048'  8  '((int)(i%200)-100)'                    '(int64_t)s'                         'asc_int322int64'
GenConvert 'asc_int322uint16'     'int32_t'   'uint16_t'     '8*2048'  8  'abs((int)(i%200)-100)'                 '(uint16_t)s'                        'asc_int322uint16'
GenConvert 'asc_uint322uint16'    'uint32_t'  'uint16_t'     '8*2048'  8  '(i%500)'                               '(uint16_t)s'                        'asc_uint322uint16'
GenConvert 'asc_uint322int16'     'uint32_t'  'int16_t'      '8*2048'  8  '(i%500)'                               '(int16_t)s'                         'asc_uint322int16'
GenConvert 'asc_bfloat162float'   'bfloat16_t' 'float'       '8*2048'  8  '(bfloat16_t)(i*0.1f)'                  '(float)s'                           'asc_bfloat162float'
GenConvert 'asc_bfloat162half'    'bfloat16_t' 'half'        '8*2048'  8  '(bfloat16_t)(i*0.1f)'                  '(half)s'                            'asc_bfloat162half'
GenConvert 'asc_bfloat162int32'   'bfloat16_t' 'int32_t'     '8*2048'  8  '(bfloat16_t)((i%100)-50)'              '(int32_t)s'                         'asc_bfloat162int32'
GenConvert 'asc_int642int32'      'int64_t'   'int32_t'      '8*2048'  8  '(int64_t)((i%200)-100)'                '(int32_t)s'                         'asc_int642int32'
GenConvert 'asc_hif82half'        'hifloat8_t' 'half'        '8*2048'  8  '(hifloat8_t)(i%10)'                   '(half)s'                            'asc_hif82half'
GenConvert 'asc_truncate'         'int32_t'   'int16_t'      '8*2048'  8  '((int)(i%200)-100)'                    '(int16_t)s'                         'asc_truncate'
GenConvert 'asc_int642float'      'int64_t'   'float'        '8*2048'  8  '(int64_t)((i%200)-100)'                '(float)s'                           'asc_int642float'

Write-Host "=== DATA TYPE CONVERT DONE (36 files) ==="