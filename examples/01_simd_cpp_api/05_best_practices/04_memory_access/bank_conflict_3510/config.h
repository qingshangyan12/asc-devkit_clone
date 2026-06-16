#ifndef BANK_CONFLICT_CONFIG_H
#define BANK_CONFLICT_CONFIG_H

#include "kernel_operator.h"

#if SCENARIO_NUM == 1
// 二读一写，消除读写冲突：src0=bank0, src1=bank0, dst=bank1
constexpr uint32_t TOTAL_LENGTH = 4096;
constexpr uint32_t xLocalAddr = 0x0000;
constexpr uint32_t yLocalAddr = 0x4000;
constexpr uint32_t zLocalAddr = 0x8100;
using T1 = float;
using T2 = float;
using T3 = float;
#elif SCENARIO_NUM == 2
// 二读一写，src0与dst地址重叠读写冲突：src0=dst=bank0, src1=bank1
constexpr uint32_t TOTAL_LENGTH = 4096;
constexpr uint32_t xLocalAddr = 0x0000;
constexpr uint32_t yLocalAddr = 0x4100;
constexpr uint32_t zLocalAddr = 0x0000;
using T1 = float;
using T2 = float;
using T3 = float;
#elif SCENARIO_NUM == 3
// 二读一写，消除读读冲突：src0=bank0, src1=bank1, dst=bank1
constexpr uint32_t TOTAL_LENGTH = 4096;
constexpr uint32_t xLocalAddr = 0x0000;
constexpr uint32_t yLocalAddr = 0x4100;
constexpr uint32_t zLocalAddr = 0x8900;
using T1 = float;
using T2 = float;
using T3 = float;
#elif SCENARIO_NUM == 4
// 二读一写，双搬入 DINTLV_B32 交织读取
constexpr uint32_t TOTAL_LENGTH = 4096;
constexpr uint32_t xLocalAddr = 0x0000;
constexpr uint32_t yLocalAddr = 0x4000;
constexpr uint32_t zLocalAddr = 0x8000;
using T1 = float;
using T2 = float;
using T3 = float;
#elif SCENARIO_NUM == 5
// 二读一写，全冲突(读读+读写)：src0/src1/dst 同 bank=0，无循环展开
constexpr uint32_t TOTAL_LENGTH = 4096;
constexpr uint32_t xLocalAddr = 0x0000;
constexpr uint32_t yLocalAddr = 0x4000;
constexpr uint32_t zLocalAddr = 0x8000;
using T1 = float;
using T2 = float;
using T3 = float;
#elif SCENARIO_NUM == 6
// 二读一写，全冲突 + 循环展开：同 Case 4 地址，循环展开优化
constexpr uint32_t TOTAL_LENGTH = 4096;
constexpr uint32_t xLocalAddr = 0x0000;
constexpr uint32_t yLocalAddr = 0x4000;
constexpr uint32_t zLocalAddr = 0x8000;
using T1 = float;
using T2 = float;
using T3 = float;
#elif SCENARIO_NUM == 7
// 离散访存基准：idx 全 0，同地址合并为单次读取
constexpr uint32_t TOTAL_LENGTH = 256;
constexpr uint32_t xLocalAddr = 0x0000;
constexpr uint32_t yLocalAddr = 0x0400;
constexpr uint32_t zLocalAddr = 0x0400;
using T1 = uint32_t;
using T2 = float;
using T3 = float;
#elif SCENARIO_NUM == 8
// 离散访存：idx [0..7] 重复，同地址合并读取
constexpr uint32_t TOTAL_LENGTH = 256;
constexpr uint32_t xLocalAddr = 0x0000;
constexpr uint32_t yLocalAddr = 0x0400;
constexpr uint32_t zLocalAddr = 0x0400;
using T1 = uint32_t;
using T2 = float;
using T3 = float;
#elif SCENARIO_NUM == 9
// 离散访存：idx 连续 [0..255]，跨 bank 分散
constexpr uint32_t TOTAL_LENGTH = 256;
constexpr uint32_t xLocalAddr = 0x0000;
constexpr uint32_t yLocalAddr = 0x0400;
constexpr uint32_t zLocalAddr = 0x0400;
using T1 = uint32_t;
using T2 = float;
using T3 = float;
#elif SCENARIO_NUM == 10
// 离散访存 subbank 冲突：idx [0,128] 交替
constexpr uint32_t TOTAL_LENGTH = 256;
constexpr uint32_t xLocalAddr = 0x0000;
constexpr uint32_t yLocalAddr = 0x0400;
constexpr uint32_t zLocalAddr = 0x0400;
using T1 = uint32_t;
using T2 = float;
using T3 = float;
#elif SCENARIO_NUM == 11
// 一读一写，DATA_BLOCK_COPY stride=1 横着搬运，连续 8 个 DataBlock
constexpr uint32_t TOTAL_LENGTH = 512;
constexpr uint32_t xLocalAddr = 0x0000;
constexpr uint32_t yLocalAddr = 0x0000;
constexpr uint32_t zLocalAddr = 0x4100;
using T1 = float;
using T2 = float;
using T3 = float;
#elif SCENARIO_NUM == 12
// 一读一写，DATA_BLOCK_COPY stride=8 竖着搬运，非连续 8 个 DataBlock
constexpr uint32_t TOTAL_LENGTH = 512;
constexpr uint32_t xLocalAddr = 0x0000;
constexpr uint32_t yLocalAddr = 0x0000;
constexpr uint32_t zLocalAddr = 0x4100;
using T1 = float;
using T2 = float;
using T3 = float;
#elif SCENARIO_NUM == 13
// 一读一写，DATA_BLOCK_COPY srcBlockStride=0，同 DataBlock 重复读取
constexpr uint32_t TOTAL_LENGTH = 512;
constexpr uint32_t xLocalAddr = 0x0000;
constexpr uint32_t yLocalAddr = 0x0000;
constexpr uint32_t zLocalAddr = 0x4100;
using T1 = float;
using T2 = float;
using T3 = float;
#elif SCENARIO_NUM == 14
// 一读一写，DATA_BLOCK_COPY srcRepeatStride=0，每次迭代从同一首地址读取
constexpr uint32_t TOTAL_LENGTH = 512;
constexpr uint32_t xLocalAddr = 0x0000;
constexpr uint32_t yLocalAddr = 0x0000;
constexpr uint32_t zLocalAddr = 0x4100;
using T1 = float;
using T2 = float;
using T3 = float;
#endif

#endif // BANK_CONFLICT_CONFIG_H
