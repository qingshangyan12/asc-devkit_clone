# Generate reg_load files (24 files)
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

function WriteFile($path, $content) {
    $trimmed = $content.Trim()
    [System.IO.File]::WriteAllText($path, $trimmed, [System.Text.Encoding]::UTF8)
    Write-Host "OK: $path"
}

$rl = "$baseDir\reg_load"

# ==============================
# Simple load functions (int8_t, b8 mask, asc_neg)
# ==============================
function GenLoadSimple($name, $loadLine, $decls, $total, $blocks, $srcExpr, $goldenExpr, $type, $mask) {
    $funcVf = $name + '_vf'
    $funcCustom = $name + '_custom'
    $funcKernel = 'kernel_' + $name
    $pT = if ($type -in 'uint8_t','int8_t') { "(int)tensor[i]" } else { "(float)tensor[i]" }

    $content = @"
$license

$includes

constexpr uint32_t TOTAL_LENGTH = $total;
constexpr uint32_t NUM_BLOCKS = $blocks;
typedef $type TYPE;

__simd_vf__ inline void $funcVf(__ubuf__ TYPE* dst_local, __ubuf__ TYPE* src_local, uint32_t data_len)
{
    uint16_t one_rep_size = asc_get_vf_len() / sizeof(TYPE);
    uint16_t repeat_time = (data_len + one_rep_size - 1) / one_rep_size;
    uint32_t remaining = data_len;
    vector_bool vmask;
    $decls
    for (uint16_t i = 0; i < repeat_time; ++i) {
        uint32_t cur_count = (remaining > one_rep_size) ? one_rep_size : remaining;
        uint32_t mask_cnt = cur_count;
        vmask = asc_update_mask_$mask(mask_cnt);
        $loadLine
        asc_neg(reg_dst, reg_src0, vmask);
        asc_store(dst_local + i * one_rep_size, reg_dst, cur_count);
        remaining = (remaining > one_rep_size) ? (remaining - one_rep_size) : 0;
    }
}

__vector__ __global__ void $funcCustom(__gm__ TYPE* src, __gm__ TYPE* dst)
{
    asc_init();
    constexpr uint32_t block_length = TOTAL_LENGTH / NUM_BLOCKS;
    constexpr uint8_t mutex_id = 1;
    __gm__ TYPE* src_gm = src + asc_get_block_idx() * block_length;
    __gm__ TYPE* dst_gm = dst + asc_get_block_idx() * block_length;
    __ubuf__ TYPE src_local[block_length];
    __ubuf__ TYPE dst_local[block_length];
    uint32_t cache_mode0 = 0; uint32_t cache_mode2 = 0;
    uint32_t burst_length = block_length * sizeof(TYPE);
    uint64_t src_stride = 0; uint32_t dst_stride = 0;
    asc_lock(PIPE_MTE2, mutex_id);
    asc_copy_gm2ub_align((__ubuf__ TYPE*)src_local, src_gm, BLK_NUM, burst_length, 0, 0, true, cache_mode0, src_stride, dst_stride);
    asc_unlock(PIPE_MTE2, mutex_id);
    asc_lock(PIPE_V, mutex_id);
    $funcVf((__ubuf__ TYPE*)dst_local, src_local, block_length);
    asc_unlock(PIPE_V, mutex_id);
    asc_lock(PIPE_MTE3, mutex_id);
    asc_copy_ub2gm_align(dst_gm, (__ubuf__ TYPE*)dst_local, BLK_NUM, burst_length, cache_mode2, src_stride, dst_stride);
    asc_unlock(PIPE_MTE3, mutex_id);
}

std::vector<TYPE> $funcKernel(std::vector<TYPE>& src_data)
{
    constexpr uint32_t numBlocks = NUM_BLOCKS;
    uint32_t totalLength = TOTAL_LENGTH;
    size_t totalByteSize = totalLength * sizeof(TYPE);
    int32_t deviceId = 0;
    aclrtStream stream = nullptr;
    uint8_t* srcHost = reinterpret_cast<uint8_t*>(src_data.data());
    uint8_t* dstHost = nullptr;
    TYPE* srcDevice = nullptr;
    TYPE* dstDevice = nullptr;
    aclInit(nullptr);
    aclrtSetDevice(deviceId);
    aclrtCreateStream(&stream);
    aclrtMallocHost((void**)(&dstHost), totalByteSize);
    aclrtMalloc((void**)&srcDevice, totalByteSize, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc((void**)&dstDevice, totalByteSize, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy((uint8_t*)srcDevice, totalByteSize, srcHost, totalByteSize, ACL_MEMCPY_HOST_TO_DEVICE);
    $funcCustom<<<numBlocks, 0, stream>>>(srcDevice, dstDevice);
    aclrtSynchronizeStream(stream);
    aclrtMemcpy(dstHost, totalByteSize, (uint8_t*)dstDevice, totalByteSize, ACL_MEMCPY_DEVICE_TO_HOST);
    std::vector<TYPE> dst((TYPE*)dstHost, (TYPE*)(dstHost + totalByteSize));
    aclrtFree(srcDevice);
    aclrtFree(dstDevice);
    aclrtFreeHost(dstHost);
    aclrtDestroyStream(stream);
    aclrtResetDevice(deviceId);
    aclFinalize();
    return dst;
}

uint32_t VerifyResult(std::vector<TYPE>& output, std::vector<TYPE>& golden)
{
    auto printTensor = [](std::vector<TYPE>& tensor, const char* name) {
        constexpr size_t maxPrintSize = 20;
        std::cout << name << ": ";
        for (size_t i = 0; i < std::min(tensor.size(), maxPrintSize); ++i) {
            std::cout << $pT << " ";
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
    std::vector<TYPE> src(totalLength);
    for (uint32_t i = 0; i < totalLength; ++i) {
        src[i] = $srcExpr;
    }
    std::vector<TYPE> output = $funcKernel(src);
    std::vector<TYPE> golden(totalLength);
    for (uint32_t i = 0; i < totalLength; ++i) {
        TYPE s = src[i];
        golden[i] = $goldenExpr;
    }
    return VerifyResult(output, golden);
}
"@
    WriteFile "$rl\$name\$name.asc" $content
}

# Simple load: asc_load
GenLoadSimple 'asc_load' `
    'asc_load(reg_src0, src_local + i * one_rep_size);' `
    'vector_TYPE reg_src0; vector_TYPE reg_dst;' `
    '8*2048' 8 '(i%127)-63' '-(int8_t)((int)s)' 'int8_t' 'b8'

# asc_loadalign
GenLoadSimple 'asc_loadalign' `
    'asc_loadalign(reg_src0, src_local + i * one_rep_size, sizeof(TYPE));' `
    'vector_TYPE reg_src0; vector_TYPE reg_dst;' `
    '8*2048' 8 '(i%127)-63' '-(int8_t)((int)s)' 'int8_t' 'b8'

# asc_loadunalign
GenLoadSimple 'asc_loadunalign' `
    'asc_loadunalign(reg_src0, src_local + i * one_rep_size);' `
    'vector_TYPE reg_src0; vector_TYPE reg_dst;' `
    '8*2048' 8 '(i%127)-63' '-(int8_t)((int)s)' 'int8_t' 'b8'

# asc_loadunalign_pre
GenLoadSimple 'asc_loadunalign_pre' `
    'asc_loadunalign_pre(reg_src0, src_local + i * one_rep_size);' `
    'vector_TYPE reg_src0; vector_TYPE reg_dst;' `
    '8*2048' 8 '(i%127)-63' '-(int8_t)((int)s)' 'int8_t' 'b8'

# asc_loadunalign_postupdate
GenLoadSimple 'asc_loadunalign_postupdate' `
    'asc_loadunalign_postupdate(reg_src0, cur_ptr, one_rep_size);' `
    '__ubuf__ TYPE* cur_ptr = src_local; vector_TYPE reg_src0; vector_TYPE reg_dst;' `
    '8*2048' 8 '(i%127)-63' '-(int8_t)((int)s)' 'int8_t' 'b8'

# asc_loadalign_postupdate
GenLoadSimple 'asc_loadalign_postupdate' `
    'asc_loadalign_postupdate(reg_src0, cur_ptr, sizeof(TYPE), one_rep_size);' `
    '__ubuf__ TYPE* cur_ptr = src_local; vector_TYPE reg_src0; vector_TYPE reg_dst;' `
    '8*2048' 8 '(i%127)-63' '-(int8_t)((int)s)' 'int8_t' 'b8'

# asc_copy
GenLoadSimple 'asc_copy' `
    'asc_copy(reg_dst, reg_src0);' `
    'vector_TYPE reg_src0; vector_TYPE reg_dst;' `
    '8*2048' 8 '(i%127)-63' '(TYPE)s' 'int8_t' 'b8'

# asc_gather
GenLoadSimple 'asc_gather' `
    'asc_gather(reg_src0, src_local + i * one_rep_size, 2);' `
    'vector_TYPE reg_src0; vector_TYPE reg_dst;' `
    '8*2048' 8 '(i%127)-63' '-(int8_t)((int)s)' 'int8_t' 'b8'

# asc_gather_datablock
GenLoadSimple 'asc_gather_datablock' `
    'asc_gather_datablock(reg_src0, src_local + i * one_rep_size, sizeof(TYPE), 2);' `
    'vector_TYPE reg_src0; vector_TYPE reg_dst;' `
    '8*2048' 8 '(i%127)-63' '-(int8_t)((int)s)' 'int8_t' 'b8'

# ==============================
# Broadcast load functions
# ==============================

# asc_loadalign_brc_elem
GenLoadSimple 'asc_loadalign_brc_elem' `
    'asc_loadalign_brc_elem(reg_src0, src_local + i, sizeof(TYPE), one_rep_size);' `
    'vector_TYPE reg_src0; vector_TYPE reg_dst;' `
    '8*2048' 8 '(i%127)-63' '-(int8_t)((int)s)' 'int8_t' 'b8'

# asc_loadalign_brc_elem_postupdate
GenLoadSimple 'asc_loadalign_brc_elem_postupdate' `
    'asc_loadalign_brc_elem_postupdate(reg_src0, cur_ptr, sizeof(TYPE), one_rep_size);' `
    '__ubuf__ TYPE* cur_ptr = src_local; vector_TYPE reg_src0; vector_TYPE reg_dst;' `
    '8*2048' 8 '(i%127)-63' '-(int8_t)((int)s)' 'int8_t' 'b8'

# asc_loadalign_brc_datablock
GenLoadSimple 'asc_loadalign_brc_datablock' `
    'asc_loadalign_brc_datablock(reg_src0, src_local + i * one_rep_size, sizeof(TYPE), one_rep_size / 2);' `
    'vector_TYPE reg_src0; vector_TYPE reg_dst;' `
    '8*2048' 8 '(i%127)-63' '-(int8_t)((int)s)' 'int8_t' 'b8'

# asc_loadalign_brc_datablock_postupdate
GenLoadSimple 'asc_loadalign_brc_datablock_postupdate' `
    'asc_loadalign_brc_datablock_postupdate(reg_src0, cur_ptr, sizeof(TYPE), one_rep_size / 2);' `
    '__ubuf__ TYPE* cur_ptr = src_local; vector_TYPE reg_src0; vector_TYPE reg_dst;' `
    '8*2048' 8 '(i%127)-63' '-(int8_t)((int)s)' 'int8_t' 'b8'

# asc_loadalign_brc_elem2datablock
GenLoadSimple 'asc_loadalign_brc_elem2datablock' `
    'asc_loadalign_brc_elem2datablock(reg_src0, src_local + i, sizeof(TYPE), one_rep_size);' `
    'vector_TYPE reg_src0; vector_TYPE reg_dst;' `
    '8*2048' 8 '(i%127)-63' '-(int8_t)((int)s)' 'int8_t' 'b8'

# asc_loadalign_brc_elem2datablock_postupdate
GenLoadSimple 'asc_loadalign_brc_elem2datablock_postupdate' `
    'asc_loadalign_brc_elem2datablock_postupdate(reg_src0, cur_ptr, sizeof(TYPE), one_rep_size);' `
    '__ubuf__ TYPE* cur_ptr = src_local; vector_TYPE reg_src0; vector_TYPE reg_dst;' `
    '8*2048' 8 '(i%127)-63' '-(int8_t)((int)s)' 'int8_t' 'b8'

# ==============================
# Deinterleave
# ==============================

# asc_loadalign_deintlv - produces two vectors
GenLoadSimple 'asc_loadalign_deintlv' `
    'asc_loadalign_deintlv(reg_even, reg_odd, src_local + i * one_rep_size, sizeof(TYPE)); asc_add(reg_dst, reg_even, reg_odd, vmask);' `
    'vector_TYPE reg_even; vector_TYPE reg_odd; vector_TYPE reg_dst;' `
    '8*2048' 8 '(i%127)-63' '(TYPE)((int)s + (int)s)' 'int8_t' 'b8'

# asc_loadalign_deintlv_postupdate
GenLoadSimple 'asc_loadalign_deintlv_postupdate' `
    'asc_loadalign_deintlv_postupdate(reg_even, reg_odd, cur_ptr, sizeof(TYPE)); asc_add(reg_dst, reg_even, reg_odd, vmask);' `
    '__ubuf__ TYPE* cur_ptr = src_local; vector_TYPE reg_even; vector_TYPE reg_odd; vector_TYPE reg_dst;' `
    '8*2048' 8 '(i%127)-63' '(TYPE)((int)s + (int)s)' 'int8_t' 'b8'

# ==============================
# Downsample
# ==============================

# asc_loadalign_downsample
GenLoadSimple 'asc_loadalign_downsample' `
    'asc_loadalign_downsample(reg_src0, src_local + i * one_rep_size, sizeof(TYPE), 2);' `
    'vector_TYPE reg_src0; vector_TYPE reg_dst;' `
    '8*2048' 8 '(i%127)-63' '-(int8_t)((int)s)' 'int8_t' 'b8'

# asc_loadalign_downsample_postupdate
GenLoadSimple 'asc_loadalign_downsample_postupdate' `
    'asc_loadalign_downsample_postupdate(reg_src0, cur_ptr, sizeof(TYPE), 2);' `
    '__ubuf__ TYPE* cur_ptr = src_local; vector_TYPE reg_src0; vector_TYPE reg_dst;' `
    '8*2048' 8 '(i%127)-63' '-(int8_t)((int)s)' 'int8_t' 'b8'

# ==============================
# Upsample
# ==============================

# asc_loadalign_upsample
GenLoadSimple 'asc_loadalign_upsample' `
    'asc_loadalign_upsample(reg_src0, src_local + i * one_rep_size, sizeof(TYPE), 2);' `
    'vector_TYPE reg_src0; vector_TYPE reg_dst;' `
    '8*2048' 8 '(i%127)-63' '-(int8_t)((int)s)' 'int8_t' 'b8'

# asc_loadalign_upsample_postupdate
GenLoadSimple 'asc_loadalign_upsample_postupdate' `
    'asc_loadalign_upsample_postupdate(reg_src0, cur_ptr, sizeof(TYPE), 2);' `
    '__ubuf__ TYPE* cur_ptr = src_local; vector_TYPE reg_src0; vector_TYPE reg_dst;' `
    '8*2048' 8 '(i%127)-63' '-(int8_t)((int)s)' 'int8_t' 'b8'

# ==============================
# Unpack
# ==============================

# asc_loadalign_unpack
GenLoadSimple 'asc_loadalign_unpack' `
    'asc_loadalign_unpack(reg_src0, src_local + i * one_rep_size, sizeof(TYPE));' `
    'vector_TYPE reg_src0; vector_TYPE reg_dst;' `
    '8*2048' 8 '(i%127)-63' '-(int8_t)((int)s)' 'int8_t' 'b8'

# asc_loadalign_unpack_postupdate
GenLoadSimple 'asc_loadalign_unpack_postupdate' `
    'asc_loadalign_unpack_postupdate(reg_src0, cur_ptr, sizeof(TYPE));' `
    '__ubuf__ TYPE* cur_ptr = src_local; vector_TYPE reg_src0; vector_TYPE reg_dst;' `
    '8*2048' 8 '(i%127)-63' '-(int8_t)((int)s)' 'int8_t' 'b8'

# asc_loadalign_unpack4
GenLoadSimple 'asc_loadalign_unpack4' `
    'asc_loadalign_unpack4(reg_src0, src_local + i * one_rep_size, sizeof(TYPE));' `
    'vector_TYPE reg_src0; vector_TYPE reg_dst;' `
    '8*2048' 8 '(i%127)-63' '-(int8_t)((int)s)' 'int8_t' 'b8'

# asc_loadalign_unpack4_postupdate
GenLoadSimple 'asc_loadalign_unpack4_postupdate' `
    'asc_loadalign_unpack4_postupdate(reg_src0, cur_ptr, sizeof(TYPE));' `
    '__ubuf__ TYPE* cur_ptr = src_local; vector_TYPE reg_src0; vector_TYPE reg_dst;' `
    '8*2048' 8 '(i%127)-63' '-(int8_t)((int)s)' 'int8_t' 'b8'

Write-Host "=== REG LOAD DONE (24 files) ==="
