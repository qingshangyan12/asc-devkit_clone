/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software; you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

/**
 * tikicpulib 示例 - 纯 C/C++ 核函数
 * 
 * tikicpulib 模式下：
 * - 只能使用纯 C/C++ 代码（不能使用 GlobalTensor, TPipe, DataCopy 等 Ascend C API）
 * - GmAlloc 分配共享内存用于多核数据共享
 * - ICPU_RUN_KF 宏启动核函数模拟多核并行
 */

#include "tikicpulib.h"
#include <iostream>
#include <cstring>
#include <sys/stat.h>

using namespace AscendC;

constexpr uint32_t numBlocks = 8;
constexpr uint32_t blockLength = 2048;

__aicore__ void add_custom(GM_ADDR x, GM_ADDR y, GM_ADDR z)
{
    uint32_t blockIdx = block_idx;
    uint32_t offset = blockIdx * blockLength;
    
    uint16_t* xData = reinterpret_cast<uint16_t*>(x) + offset;
    uint16_t* yData = reinterpret_cast<uint16_t*>(y) + offset;
    uint16_t* zData = reinterpret_cast<uint16_t*>(z) + offset;
    
    for (uint32_t i = 0; i < blockLength; i++) {
        zData[i] = xData[i] + yData[i];
    }
}

void ReadFile(const char* filePath, size_t fileSize, uint8_t* buffer, size_t bufferSize)
{
    FILE* fp = fopen(filePath, "rb");
    if (fp == nullptr) {
        std::cerr << "无法打开文件: " << filePath << std::endl;
        return;
    }
    fread(buffer, 1, fileSize, fp);
    fclose(fp);
}

void WriteFile(const char* filePath, uint8_t* buffer, size_t bufferSize)
{
    FILE* fp = fopen(filePath, "wb");
    if (fp == nullptr) {
        std::cerr << "无法创建文件: " << filePath << std::endl;
        return;
    }
    fwrite(buffer, 1, bufferSize, fp);
    fclose(fp);
}

int32_t main(int32_t argc, char* argv[])
{
    std::cout << "===== tikicpulib 纯 C++ 核函数示例 =====" << std::endl;
    std::cout << "\n说明：" << std::endl;
    std::cout << "  - tikicpulib 模式下不能使用 Ascend C API" << std::endl;
    std::cout << "  - 只能用纯 C/C++ 代码实现核函数逻辑" << std::endl;
    std::cout << "  - GmAlloc 在 /tmp 创建共享文件实现多核数据共享" << std::endl;
    
    size_t inputByteSize = numBlocks * blockLength * sizeof(uint16_t);
    size_t outputByteSize = numBlocks * blockLength * sizeof(uint16_t);
    
    std::cout << "\n配置：" << std::endl;
    std::cout << "  - 总核数: " << numBlocks << std::endl;
    std::cout << "  - 每核数据量: " << blockLength << " elements" << std::endl;
    std::cout << "  - 输入大小: " << inputByteSize << " bytes" << std::endl;
    
    mkdir("./input", 0755);
    mkdir("./output", 0755);
    
    std::cout << "\n使用 GmAlloc 分配共享内存..." << std::endl;
    uint8_t* x = (uint8_t*)GmAlloc(inputByteSize);
    uint8_t* y = (uint8_t*)GmAlloc(inputByteSize);
    uint8_t* z = (uint8_t*)GmAlloc(outputByteSize);
    
    if (x == nullptr || y == nullptr || z == nullptr) {
        std::cout << "[ERROR] GmAlloc 失败!" << std::endl;
        return -1;
    }
    
    std::cout << "[SUCCESS] 共享内存已分配" << std::endl;
    std::cout << "  x: " << static_cast<void*>(x) << std::endl;
    std::cout << "  y: " << static_cast<void*>(y) << std::endl;
    std::cout << "  z: " << static_cast<void*>(z) << std::endl;
    
    std::cout << "\n初始化测试数据..." << std::endl;
    uint16_t* xData = reinterpret_cast<uint16_t*>(x);
    uint16_t* yData = reinterpret_cast<uint16_t*>(y);
    
    for (size_t i = 0; i < numBlocks * blockLength; i++) {
        xData[i] = static_cast<uint16_t>(i % 1000);
        yData[i] = static_cast<uint16_t>((i % 1000) + 1);
    }
    
    WriteFile("./input/input_x.bin", x, inputByteSize);
    WriteFile("./input/input_y.bin", y, inputByteSize);
    
    std::cout << "\n设置内核模式为 AIV_MODE..." << std::endl;
    SetKernelMode(KernelMode::AIV_MODE);
    
    std::cout << "\n启动核函数 (ICPU_RUN_KF)..." << std::endl;
    std::cout << "  模拟 " << numBlocks << " 个核并行执行" << std::endl;
    std::cout << "  框架将 fork 多进程，每个进程执行 add_custom" << std::endl;
    
    ICPU_RUN_KF(add_custom, numBlocks, x, y, z);
    
    std::cout << "\n[SUCCESS] 核函数执行完成" << std::endl;
    
    WriteFile("./output/output_z.bin", z, outputByteSize);
    std::cout << "\n输出已写入: ./output/output_z.bin" << std::endl;
    
    std::cout << "\n验证结果..." << std::endl;
    uint16_t* zData = reinterpret_cast<uint16_t*>(z);
    bool allCorrect = true;
    
    for (size_t i = 0; i < numBlocks * blockLength; i++) {
        uint16_t expected = static_cast<uint16_t>((i % 1000) + (i % 1000) + 1);
        if (zData[i] != expected) {
            std::cout << "[ERROR] Index " << i << ": expected " << expected 
                      << ", got " << zData[i] << std::endl;
            allCorrect = false;
            break;
        }
    }
    
    if (allCorrect) {
        std::cout << "[SUCCESS] 所有数据验证正确!" << std::endl;
    }
    
    std::cout << "\n部分结果：" << std::endl;
    for (uint32_t block = 0; block < 4; block++) {
        std::cout << "  Block " << block << ": ";
        for (uint32_t i = 0; i < 10; i++) {
            std::cout << zData[block * blockLength + i] << " ";
        }
        std::cout << std::endl;
    }
    
    std::cout << "\n使用 GmFree 释放共享内存..." << std::endl;
    GmFree((void*)x);
    GmFree((void*)y);
    GmFree((void*)z);
    std::cout << "[SUCCESS] 共享内存已释放" << std::endl;
    
    std::cout << "\n===== 测试完成 =====" << std::endl;
    return allCorrect ? 0 : -1;
}