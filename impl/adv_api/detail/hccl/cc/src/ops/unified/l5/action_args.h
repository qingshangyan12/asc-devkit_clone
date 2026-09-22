/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
/**
 * L5 公共组件 B：消息槽参数排布接口（Plan ABI）——单一事实源
 *
 * 替代 devkit 现状的三处人工对齐：
 *   ① template 侧 FillCachedArgs(submitInfo, inputAddr, outputAddr, token, offset, sliceSize)
 *      —— 变参模板，槽位全靠调用方书写顺序（ccu_temp_all_gather_mesh_1D.cc:105）
 *   ② kernel 侧 GeneArgs 返回 {inputAddr, outputAddr, token, offset, goSize[0..3]}
 *      —— 必须与 ① 顺序一致，靠人记（ccu_kernel_all_gather_mesh1d.cc:142）
 *   ③ kernel 侧 LoadArgs 的 Load() 调用顺序
 *      —— 必须与 ② 一致，靠人记（ccu_kernel_all_gather_mesh1d.cc:68）
 * 三处任一处顺序写错 = 运行时数据错乱且不报错（困难点 2）。
 *
 * 本文件用一份声明生成全部三处所需信息：槽位常量 + 装配函数 + 容量断言。
 *
 * 【边界】消息槽排布（*SlotArgs）是 BuildArgs 产出（ArgBundle）的**子集**：
 *   进槽的只有"kernel 无法自行推导的量"——input/output 基址、token、offset、sliceSize；
 *   逐 peer 的收发地址由 kernel 从通道资源（Variable/RemoteAddr）现场推导，不占槽位
 *   （devkit 实证：ccu_kernel_all_gather_mesh1d.cc InitResource 按 channel 建 Variable，
 *    LoadArgs 只 Load 5 个值）。ArgBundle 里的 peers[] 供 AICPU 后端逐线程展开使用。
 */
#ifndef REFACTOR_ACTION_ARGS_H
#define REFACTOR_ACTION_ARGS_H

#include <cstdint>

#include "alg_param.h"   // devkit u32/u64

namespace mc2_ops_hccl::refactor {

// u32/u64 复用 devkit 定义（base.h：unsigned int / unsigned long long），
// 不自定义以免与真实接口签名不匹配

// ---------------------------------------------------------------------------
// 声明宏：一个动作一份，声明"该动作的参数按什么顺序摆进消息槽"
//
//   ACTION_ARGS(CollectAll, v1,
//       (inputAddr)(outputAddr)(token)(offset)(sliceSize))
//
// 展开产出三件事：
//   1. struct CollectAllArgs      —— BuildArgs 的返回类型（字段名即语义名）
//   2. enum CollectAllSlot        —— 槽位常量 K_INPUT_ADDR/... （消灭魔数下标）
//   3. AssembleSlots(args, slots) —— 装配函数（替代手写 FillCachedArgs 调用）
//
// PoC 用显式展开代替 Boost.PP 式列表遍历（devkit 编译选项为 C++14 且 -Werror，
// 保持宏简单可审）；真实落地时用代码生成器或 PP 库展开同一份声明。
// ---------------------------------------------------------------------------

#define ACTION_ARGS_5(ActionName, Ver, F0, F1, F2, F3, F4)                    \
    struct ActionName##SlotArgs {                                                 \
        u64 F0{0};                                                            \
        u64 F1{0};                                                            \
        u64 F2{0};                                                            \
        u64 F3{0};                                                            \
        u64 F4{0};                                                            \
        static constexpr u32 VERSION = Ver;                                   \
    };                                                                        \
    struct ActionName##Slot {                                                 \
        enum : u32 {                                                          \
            K_##F0 = 0,                                                       \
            K_##F1 = 1,                                                       \
            K_##F2 = 2,                                                       \
            K_##F3 = 3,                                                       \
            K_##F4 = 4,                                                       \
            SLOT_COUNT = 5                                                    \
        };                                                                    \
    };                                                                        \
    inline void AssembleSlots(const ActionName##SlotArgs& a, u64* slots)           \
    {                                                                         \
        slots[ActionName##Slot::K_##F0] = a.F0;                                \
        slots[ActionName##Slot::K_##F1] = a.F1;                                \
        slots[ActionName##Slot::K_##F2] = a.F2;                                \
        slots[ActionName##Slot::K_##F3] = a.F3;                                \
        slots[ActionName##Slot::K_##F4] = a.F4;                                \
    }                                                                         \
    template <typename T> struct ActionArgsTraits;                             \
    template <> struct ActionArgsTraits<ActionName##SlotArgs> {                    \
        static constexpr u32 SLOT_COUNT = ActionName##Slot::SLOT_COUNT;         \
        static constexpr u32 VERSION = Ver;                                     \
        static constexpr const char* NAME = #ActionName;                        \
    };

// ---------------------------------------------------------------------------
// CollectAll 的参数排布声明（字段顺序 == devkit 现状 FillCachedArgs 调用顺序，
// 迁移期保持逐槽兼容；后续新增字段只在此处加一行，三处生成物同步更新）
// ---------------------------------------------------------------------------
ACTION_ARGS_5(CollectAll, 1, INPUT_ADDR, OUTPUT_ADDR, TOKEN, OFFSET, SLICE_SIZE)

// 语义命名的字段访问（BuildArgs 侧写 args.inputAddr 而非 args.f0）
// —— PoC 用 using 别名保持与宏展开字段名一致
struct CollectAllSlotArgsView {
    CollectAllSlotArgs& raw;
    explicit CollectAllSlotArgsView(CollectAllSlotArgs& r) : raw(r) {}
    u64& inputAddr() { return raw.INPUT_ADDR; }
    u64& outputAddr() { return raw.OUTPUT_ADDR; }
    u64& token() { return raw.TOKEN; }
    u64& offset() { return raw.OFFSET; }
    u64& sliceSize() { return raw.SLICE_SIZE; }
};

} // namespace mc2_ops_hccl::refactor

#endif
