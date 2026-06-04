#ifndef BANK_CONFLICT_CONFIG_H
#define BANK_CONFLICT_CONFIG_H

#include "kernel_operator.h"

#if CASE_NUM == 1
// 既有读写冲突又有读读冲突（基准）
constexpr uint32_t TOTAL_LENGTH = 4096;
constexpr uint32_t xLocalAddr = 0x0000;
constexpr uint32_t yLocalAddr = 0x4000;
constexpr uint32_t zLocalAddr = 0x8000;
using T1 = float;
using T2 = float;
using T3 = float;
#define USED_VF AddVF
#elif CASE_NUM == 2
// Case 1 的无循环展开版本，用于对比展开对 bank 冲突的影响
constexpr uint32_t TOTAL_LENGTH = 4096;
constexpr uint32_t xLocalAddr = 0x0000;
constexpr uint32_t yLocalAddr = 0x4000;
constexpr uint32_t zLocalAddr = 0x8000;
using T1 = float;
using T2 = float;
using T3 = float;
#define USED_VF AddSingleVF
#elif CASE_NUM == 3
// 二读一写，地址偏移消除读读冲突
constexpr uint32_t TOTAL_LENGTH = 4096;
constexpr uint32_t xLocalAddr = 0x0000;
constexpr uint32_t yLocalAddr = 0x4100;
constexpr uint32_t zLocalAddr = 0x8900;
using T1 = float;
using T2 = float;
using T3 = float;
#define USED_VF AddVF
#elif CASE_NUM == 4
// 地址重叠，读指令减半
constexpr uint32_t TOTAL_LENGTH = 4096;
constexpr uint32_t xLocalAddr = 0x0000;
constexpr uint32_t yLocalAddr = 0x0000;
constexpr uint32_t zLocalAddr = 0x4100;
using T1 = float;
using T2 = float;
using T3 = float;
#define USED_VF AddVF
#elif CASE_NUM == 5
// 双搬入 DINTLV_B32
constexpr uint32_t TOTAL_LENGTH = 4096;
constexpr uint32_t xLocalAddr = 0x0000;
constexpr uint32_t yLocalAddr = 0x4000;
constexpr uint32_t zLocalAddr = 0x8000;
using T1 = float;
using T2 = float;
using T3 = float;
#define USED_VF AddWithInstrLoadVF
#elif CASE_NUM == 6
// 一读一写，同 bank 读写冲突
constexpr uint32_t TOTAL_LENGTH = 4096;
constexpr uint32_t xLocalAddr = 0x0000;
constexpr uint32_t yLocalAddr = 0x0000;
constexpr uint32_t zLocalAddr = 0x4000;
using T1 = float;
using T2 = float;
using T3 = float;
#define USED_VF AddsVF
#elif CASE_NUM == 7
// 一读一写，消除读写冲突
constexpr uint32_t TOTAL_LENGTH = 4096;
constexpr uint32_t xLocalAddr = 0x0000;
constexpr uint32_t yLocalAddr = 0x0000;
constexpr uint32_t zLocalAddr = 0x4100;
using T1 = float;
using T2 = float;
using T3 = float;
#define USED_VF AddsVF
#elif CASE_NUM == 8
// 离散访存冲突：idx [0..7] 重复
constexpr uint32_t TOTAL_LENGTH = 256;
constexpr uint32_t xLocalAddr = 0x0000;
constexpr uint32_t yLocalAddr = 0x0400;
constexpr uint32_t zLocalAddr = 0x0400;
using T1 = uint32_t;
using T2 = float;
using T3 = float;
#define USED_VF GatherAndScatterVF
#elif CASE_NUM == 9
// 离散访存无冲突：idx 连续 [0..255]
constexpr uint32_t TOTAL_LENGTH = 256;
constexpr uint32_t xLocalAddr = 0x0000;
constexpr uint32_t yLocalAddr = 0x0400;
constexpr uint32_t zLocalAddr = 0x0400;
using T1 = uint32_t;
using T2 = float;
using T3 = float;
#define USED_VF GatherAndScatterVF
#elif CASE_NUM == 10
// 离散访存 subbank 冲突：idx [0,128] 交替
constexpr uint32_t TOTAL_LENGTH = 256;
constexpr uint32_t xLocalAddr = 0x0000;
constexpr uint32_t yLocalAddr = 0x0400;
constexpr uint32_t zLocalAddr = 0x0400;
using T1 = uint32_t;
using T2 = float;
using T3 = float;
#define USED_VF GatherAndScatterVF
#elif CASE_NUM == 11
// 离散访存无 subbank 冲突：idx 全 0，全部 coalesce
constexpr uint32_t TOTAL_LENGTH = 256;
constexpr uint32_t xLocalAddr = 0x0000;
constexpr uint32_t yLocalAddr = 0x0400;
constexpr uint32_t zLocalAddr = 0x0400;
using T1 = uint32_t;
using T2 = float;
using T3 = float;
#define USED_VF GatherAndScatterVF
#elif CASE_NUM == 12
// 一读一写，DATA_BLOCK_COPY 跨步读写，对标 Case 7 对比跨步 vs 连续的性能差异
constexpr uint32_t TOTAL_LENGTH = 512;
constexpr uint32_t xLocalAddr = 0x0000;
constexpr uint32_t yLocalAddr = 0x0000;
constexpr uint32_t zLocalAddr = 0x4100;
using T1 = float;
using T2 = float;
using T3 = float;
#define USED_VF AddsVFWithStride1
#elif CASE_NUM == 13
// 一读一写，竖着搬运 stride=8，对标 Case 12 对比竖着 vs 横着的性能差异
constexpr uint32_t TOTAL_LENGTH = 512;
constexpr uint32_t xLocalAddr = 0x0000;
constexpr uint32_t yLocalAddr = 0x0000;
constexpr uint32_t zLocalAddr = 0x4100;
using T1 = float;
using T2 = float;
using T3 = float;
#define USED_VF AddsVFWithStride2
#endif

#endif // BANK_CONFLICT_CONFIG_H
