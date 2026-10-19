/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#if defined(__NPU_COMPILER_INTERNAL_PURE_SIMT__)
#error "sync.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_SYNC_SYNC_H
#endif

#ifndef INCLUDE_C_API_SYNC_SYNC_H
#define INCLUDE_C_API_SYNC_SYNC_H
#include "c_api/defs/defs.h"

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201)
#include "impl/c_api/memory_base_impl/sync_intf_impl.h"
#elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510 || __NPU_ARCH__ == 9201)
#include "impl/c_api/reg_base_impl/sync_intf_impl.h"
#endif

/**
 * @ingroup simd_c_api.sync.intra_core_sync
 * @brief Sets a synchronization event after preceding source-pipeline accesses complete without blocking subsequent source instructions.
 *
 * @override_id asc_sync_notify__pipe_pipe_event
 * @archs 2201 3510
 * @param [in] pipe 源流水类型，即“等待哪条流水的前序指令完成”。 参数的类型是`pipe_t`枚举，各个枚举取值的含义请参考[硬件流水类型](intra_core_sync_overview.md#硬件流水类型)。
 * @param [in] tpipe 目标流水类型，即“解除哪条流水的`asc_sync_wait`阻塞”。取值范围与`pipe`相同，为`pipe_t`枚举。
 * @param [in] id 同步事件ID，每对`pipe`与`tpipe`组合各自拥有8个独立的同步事件ID。取值范围为`event_t`枚举类型。
 */
__aicore__ inline void asc_sync_notify(pipe_t pipe, pipe_t tpipe, event_t id);

/**
 * @ingroup simd_c_api.sync.intra_core_sync
 * @brief Waits for a matching synchronization event and clears it before allowing subsequent destination-pipeline instructions.
 *
 * @override_id asc_sync_wait__pipe_pipe_event
 * @archs 2201 3510
 * @param [in] pipe 源流水类型，即“等待哪条流水的前序指令完成”。 参数的类型是`pipe_t`枚举，各个枚举取值的含义请参考[硬件流水类型](intra_core_sync_overview.md#硬件流水类型)。
 * @param [in] tpipe 目标流水类型，即“解除哪条流水的`asc_sync_wait`阻塞”。取值范围与`pipe`相同，为`pipe_t`枚举。
 * @param [in] id 同步事件ID，每对`pipe`与`tpipe`组合各自拥有8个独立的同步事件ID。取值范围为`event_t`枚举类型。
 */
__aicore__ inline void asc_sync_wait(pipe_t pipe, pipe_t tpipe, event_t id);

/**
 * @ingroup simd_c_api.sync.intra_core_sync
 * @brief Synchronizes within a single pipe: PIPE_ALL waits for all pipes and equals asc_sync, while any other pipe value blocks only that pipe's later instructions.
 *
 * @override_id asc_sync_pipe__pipe
 * @archs 2201 3510
 * @param [in] pipe 阻塞的流水类别。支持的取值有`PIPE_V`、`PIPE_M`、`PIPE_MTE1`、`PIPE_MTE2`、`PIPE_MTE3`、`PIPE_FIX`、`PIPE_ALL`，不支持`PIPE_S`。如果不关注流水类别，希望阻塞所有流水，可以传入`PIPE_ALL`。 参数的类型是`pipe_t`枚举，各个枚举取值的含义请参考[硬件流水类型](intra_core_sync_overview.md#硬件流水类型)。
 */
__aicore__ inline void asc_sync_pipe(pipe_t pipe);

/**
 * @ingroup simd_c_api.sync.intra_core_sync
 * @brief Synchronizes PIPE_V: after all prior PIPE_V instructions (such as vector compute) finish, later PIPE_MTE2, PIPE_MTE3 and PIPE_S instructions may start; id in [0,7] selects the sync event.
 *
 * @override_id asc_sync_vec__i
 * @archs 2201 3510
 * @param [in] id 核内多流水同步的同步ID，取值范围为[0, 7]，其含义与[asc_sync_notify](asc_sync_notify.md)中`id`参数的`event_t`枚举值相同，各枚举取值请参考[event\_t枚举定义](asc_sync_notify.md#event_t_enum)。
 */
__aicore__ inline void asc_sync_vec(int id);

/**
 * @ingroup simd_c_api.sync.intra_core_sync
 * @brief Ease-of-use PIPE_MTE3 synchronization (AIV only): makes subsequent PIPE_MTE2 and PIPE_V instructions start only after all prior MTE3 instructions such as UB-to-GM copies finish.
 *
 * @override_id asc_sync_mte3__i
 * @archs 2201 3510
 * @param [in] id 核内多流水同步的同步ID，取值范围为[0, 7]，其含义与[asc_sync_notify](asc_sync_notify.md)中`id`参数的`event_t`枚举值相同，各枚举取值请参考[event\_t枚举定义](asc_sync_notify.md#event_t_enum)。
 */
__aicore__ inline void asc_sync_mte3(int id);

/**
 * @ingroup simd_c_api.sync.intra_core_sync
 * @brief Ease-of-use PIPE_MTE2 synchronization (AIV only): makes subsequent PIPE_MTE3 and PIPE_V instructions start only after all prior MTE2 instructions such as GM-to-UB copies finish.
 *
 * @override_id asc_sync_mte2__i
 * @archs 2201 3510
 * @param [in] id 核内多流水同步的同步ID，取值范围为[0, 7]，其含义与[asc_sync_notify](asc_sync_notify.md)中`id`参数的`event_t`枚举值相同，各枚举取值请参考[event\_t枚举定义](asc_sync_notify.md#event_t_enum)。
 */
__aicore__ inline void asc_sync_mte2(int id);

/**
 * @ingroup simd_c_api.sync.intra_core_sync
 * @brief Synchronizes all hardware pipelines inside one core, equivalent to asc_sync_pipe(PIPE_ALL); it blocks later instructions until all prior instructions on every pipeline complete.
 *
 * @override_id asc_sync__z
 * @archs 2201 3510
 */
__aicore__ inline void asc_sync();

/**
 * @ingroup simd_c_api.sync.inter_core_sync
 * @brief Pairs with asc_sync_block_wait to sync the AIC with all AIVs in one AI Core: after prior instructions on pipe finish it notifies the scheduler and does not block later instructions.
 *
 * @override_id asc_sync_block_arrive__pipe_i64
 * @archs 2201 3510
 * @param [in] pipe 标识在哪条流水的前序指令完成后才允许向调度模块发送通知。 参数的类型是`pipe_t`枚举，各个枚举取值的含义请参考[硬件流水类型](../intra_core_sync/intra_core_sync_overview.md#硬件流水类型)。 不同NPU架构中AIC和AIV支持的`pipe`取值存在差异，具体请参考[约束说明](#supported_pipe_combinations)。
 * @param [in] flag_id 核间同步的标记，用于标识同一组同步信号。取值范围为[0, 15]，每个`flag_id`各自拥有独立的4位计数器。
 */
__aicore__ inline void asc_sync_block_arrive(pipe_t pipe, int64_t flag_id);

/**
 * @ingroup simd_c_api.sync.inter_core_sync
 * @brief Pairs with asc_sync_subblock_wait to sync all AIVs (subblocks) within one AI Core: after prior instructions on pipe finish it notifies the scheduler without blocking later instructions.
 *
 * @override_id asc_sync_subblock_arrive__pipe_i64
 * @archs 2201 3510
 * @param [in] pipe 标识在哪条流水的前序指令完成后才允许向调度模块发送通知。 参数的类型是`pipe_t`枚举，各个枚举取值的含义请参考[硬件流水类型](../intra_core_sync/intra_core_sync_overview.md#硬件流水类型)。本接口仅支持AIV调用，AIV支持的`pipe`取值请参考[约束说明](#约束说明)。
 * @param [in] flag_id 核间同步的标记，用于标识同一组同步信号。取值范围为[0, 15]，每个`flag_id`各自拥有独立的4位计数器。
 */
__aicore__ inline void asc_sync_subblock_arrive(pipe_t pipe, int64_t flag_id);

/**
 * @ingroup simd_c_api.sync.inter_core_sync
 * @brief Pairs with asc_sync_inter_wait to sync all AICs or all AIVs across AI Cores: after prior instructions on pipe finish it notifies the scheduler without blocking later instructions.
 *
 * @override_id asc_sync_inter_arrive__pipe_i64
 * @archs 2201 3510
 * @param [in] pipe 标识在哪条流水的前序指令完成后才允许向调度模块发送通知。 参数的类型是`pipe_t`枚举，各个枚举取值的含义请参考[硬件流水类型](../intra_core_sync/intra_core_sync_overview.md#硬件流水类型)。不同NPU架构中AIC和AIV支持的`pipe`取值存在差异，具体请参考[约束说明](#supported_pipe_combinations)。
 * @param [in] flag_id 核间同步的标记，用于标识同一组同步信号。取值范围为[0, 15]，每个`flag_id`各自拥有独立的4位计数器。
 */
__aicore__ inline void asc_sync_inter_arrive(pipe_t pipe, int64_t flag_id);

/**
 * @ingroup simd_c_api.sync.intra_core_sync
 * @brief Blocks subsequent instructions until all previously issued but unfinished memory accesses complete; mem_dsb_t selects the waited scope such as DSB_ALL, DSB_DDR or DSB_UB.
 *
 * @override_id asc_sync_data_barrier__memdsb
 * @archs 2201 3510
 * @param [in] arg 表示需要等待的内存位置，类型为mem_dsb_t，可取值为：  &bull; DSB_ALL：等待所有内存访问指令。  &bull; DSB_DDR：等待GM访问指令。 &bull; DSB_UB：等待UB访问指令。 &bull; DSB_SEQ：预留参数，暂未启动，为后续的功能扩展做保留。
 */
__aicore__ inline void asc_sync_data_barrier(mem_dsb_t arg);

/**
 * @ingroup simd_c_api.sync.inter_core_sync
 * @brief Pairs with asc_sync_intra_wait to sync the AIC with a single AIV in one AI Core: after prior instructions on pipe finish it notifies the scheduler without blocking later instructions.
 *
 * @override_id asc_sync_intra_arrive__pipe_u64
 * @archs 3510
 * @param [in] pipe 标识在哪条流水的前序指令完成后才允许向调度模块发送通知。 参数的类型是`pipe_t`枚举，各个枚举取值的含义请参考[硬件流水类型](../intra_core_sync/intra_core_sync_overview.md#硬件流水类型)。 AIC和AIV支持的`pipe`取值请参考[约束说明](#supported_pipe_combinations)。
 * @param [in] sync_id 核间同步的标记，用于标识同一组同步信号。每个`sync_id`各自拥有独立的4位计数器。一个AI Core由1个AIC与2个AIV构成，AIC侧拥有32个`sync_id`（0~31），每个AIV侧各拥有16个`sync_id`（0~15）。 同步时`asc_sync_intra_arrive`与`asc_sync_intra_wait`的`sync_id`对应关系请参考[约束说明](#约束说明)。
 */
__aicore__ inline void asc_sync_intra_arrive(pipe_t pipe, uint64_t sync_id);

/**
 * @ingroup simd_c_api.sync.inter_core_sync
 * @brief Pairs with asc_sync_intra_arrive to sync the AIC with a single AIV in one AI Core: it blocks later instructions on pipe until the matching sync_id counter is non-zero.
 *
 * @override_id asc_sync_intra_wait__pipe_u64
 * @archs 3510
 * @param [in] pipe 标识阻塞哪条流水的后续指令，直到对应`sync_id`的计数器非0。 参数的类型是`pipe_t`枚举，各个枚举取值的含义请参考[硬件流水类型](../intra_core_sync/intra_core_sync_overview.md#硬件流水类型)。 AIC和AIV支持的`pipe`取值请参考[约束说明](#supported_pipe_combinations)。
 * @param [in] sync_id 核间同步的标记，用于标识同一组同步信号。每个`sync_id`各自拥有独立的4位计数器。一个AI Core由1个AIC与2个AIV构成，AIC侧拥有32个`sync_id`（0~31），每个AIV侧各拥有16个`sync_id`（0~15）。 同步时`asc_sync_intra_arrive`与`asc_sync_intra_wait`的`sync_id`对应关系请参考[约束说明](#约束说明)。
 */
__aicore__ inline void asc_sync_intra_wait(pipe_t pipe, uint64_t sync_id);

/**
 * @ingroup simd_c_api.sync.inter_core_sync
 * @brief Pairs with asc_sync_subblock_arrive to sync all AIVs (subblocks) within one AI Core: it blocks later instructions on pipe until the matching flag_id counter is non-zero.
 *
 * @override_id asc_sync_subblock_wait__pipe_i64
 * @archs 2201 3510
 * @param [in] pipe 标识阻塞哪条流水上后续指令，直到对应`flag_id`的计数器非0。 参数的类型是`pipe_t`枚举，各个枚举取值的含义请参考[硬件流水类型](../intra_core_sync/intra_core_sync_overview.md#硬件流水类型)。 本接口对参数`pipe`是否生效与NPU架构有关，具体请参考[约束说明](#约束说明)。
 * @param [in] flag_id 核间同步的标记，用于标识同一组同步信号。取值范围为[0, 15]，每个`flag_id`各自拥有独立的4位计数器。
 */
__aicore__ inline void asc_sync_subblock_wait(pipe_t pipe, int64_t flag_id);

/**
 * @ingroup simd_c_api.sync.inter_core_sync
 * @brief Pairs with asc_sync_inter_arrive to sync all AICs or all AIVs across AI Cores: it blocks later instructions on pipe until the matching flag_id counter is non-zero.
 *
 * @override_id asc_sync_inter_wait__pipe_i64
 * @archs 2201 3510
 * @param [in] pipe 标识阻塞哪条流水上后续指令，直到对应`flag_id`的计数器非0。 参数的类型是`pipe_t`枚举，各个枚举取值的含义请参考[硬件流水类型](../intra_core_sync/intra_core_sync_overview.md#硬件流水类型)。 本接口对参数`pipe`是否生效与NPU架构有关，具体请参考[约束说明](#约束说明)。
 * @param [in] flag_id 核间同步的标记，用于标识同一组同步信号。取值范围为[0, 15]，每个`flag_id`各自拥有独立的4位计数器。
 */
__aicore__ inline void asc_sync_inter_wait(pipe_t pipe, int64_t flag_id);

/**
 * @ingroup simd_c_api.sync.inter_core_sync
 * @brief Pairs with asc_sync_block_arrive to sync the AIC with all AIVs in one AI Core: it blocks later instructions on pipe until the matching flag_id counter is non-zero.
 *
 * @override_id asc_sync_block_wait__pipe_i64
 * @archs 2201 3510
 * @param [in] pipe 标识阻塞哪条流水上后续指令，直到对应`flag_id`的计数器非0。 参数的类型是`pipe_t`枚举，各个枚举取值的含义请参考[硬件流水类型](../intra_core_sync/intra_core_sync_overview.md#硬件流水类型)。 本接口对参数`pipe`是否生效与NPU架构有关，具体请参考[约束说明](#约束说明)。
 * @param [in] flag_id 核间同步的标记，用于标识同一组同步信号。取值范围为[0, 15]，每个`flag_id`各自拥有独立的4位计数器。
 */
__aicore__ inline void asc_sync_block_wait(pipe_t pipe, int64_t flag_id);

/**
 * @ingroup simd_c_api.sync.intra_core_sync
 * @brief Acquires the Mutex for mutex_id; if already locked, blocks subsequent instructions on the pipe until all Mutexes with the same mutex_id are released by asc_unlock.
 *
 * @override_id asc_lock__pipe_u8
 * @archs 3510
 * @param [in] pipe 标识阻塞哪条流水上后续指令，直到所有流水中具有相同`mutex_id`的Mutex都已经被[asc_unlock](asc_unlock.md)释放。 参数的类型是`pipe_t`枚举，各个枚举取值的含义请参考[硬件流水类型](intra_core_sync_overview.md#硬件流水类型)。
 * @param [in] mutex_id mutex标号，取值范围为[0, 31]。
 */
__aicore__ inline void asc_lock(pipe_t pipe, uint8_t mutex_id);

/**
 * @ingroup simd_c_api.sync.intra_core_sync
 * @brief Acquires the Mutex for mutex_id with an explicit asc_mutex_execute_mode: ASC_LOCK_BLOCK blocks the pipe until matching unlocks complete, ASC_LOCK_NON_BLOCK does not block it.
 *
 * @override_id asc_lock__pipe_u8_mutexmode
 * @archs 3510
 * @param [in] pipe 标识阻塞哪条流水上后续指令，直到所有流水中具有相同`mutex_id`的Mutex都已经被[asc_unlock](asc_unlock.md)释放。 参数的类型是`pipe_t`枚举，各个枚举取值的含义请参考[硬件流水类型](intra_core_sync_overview.md#硬件流水类型)。
 * @param [in] mutex_id mutex标号，取值范围为[0, 31]。
 * @param [in] mode 可选参数，用于指定是否阻塞流水，默认值为`ASC_LOCK_BLOCK`。 &bull; `ASC_LOCK_BLOCK`：阻塞`pipe`对应流水的执行，直到代码中位于当前`asc_lock`之前且`mutex_id`相同的所有`asc_unlock`调用均已执行完成。 &bull; `ASC_LOCK_NON_BLOCK`：不阻塞`pipe`对应流水的执行。
 */
__aicore__ inline void asc_lock(pipe_t pipe, uint8_t mutex_id, const asc_mutex_execute_mode mode);

/**
 * @ingroup simd_c_api.sync.intra_core_sync
 * @brief Releases the Mutex for mutex_id once prior instructions on the pipe have completed; must be paired with asc_lock using the same pipe, mutex_id and mode.
 *
 * @override_id asc_unlock__pipe_u8
 * @archs 3510
 * @param [in] pipe 指令的执行流水线。 参数的类型是`pipe_t`枚举，各个枚举取值的含义请参考[硬件流水类型](intra_core_sync_overview.md#硬件流水类型)。
 * @param [in] mutex_id mutex标号。取值范围[0, 31]。
 */
__aicore__ inline void asc_unlock(pipe_t pipe, uint8_t mutex_id);

/**
 * @ingroup simd_c_api.sync.intra_core_sync
 * @brief Releases the Mutex for mutex_id with an explicit asc_mutex_execute_mode; ASC_LOCK_BLOCK waits only for prior pipe instructions, ASC_LOCK_NON_BLOCK also waits for matching unlocks.
 *
 * @override_id asc_unlock__pipe_u8_mutexmode
 * @archs 3510
 * @param [in] pipe 指令的执行流水线。 参数的类型是`pipe_t`枚举，各个枚举取值的含义请参考[硬件流水类型](intra_core_sync_overview.md#硬件流水类型)。
 * @param [in] mutex_id mutex标号。取值范围[0, 31]。
 * @param [in] mode 可选参数，用于指定是否阻塞流水，默认值为`ASC_LOCK_BLOCK`。 &bull; `ASC_LOCK_BLOCK`：该指令等待`pipe`所对应的流水线中所有前置指令完成后执行。 &bull; `ASC_LOCK_NON_BLOCK`：该指令等待`pipe`所对应的流水线中所有前置指令完成且相同`mutex_id`的所有`asc_unlock`指令执行完成后执行。
 */
__aicore__ inline void asc_unlock(pipe_t pipe, uint8_t mutex_id, const asc_mutex_execute_mode mode);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_sync)
/**
 * @ingroup simd_c_api.sync.intra_core_sync
 * @brief Deprecated no-argument variant that synchronizes PIPE_V so later PIPE_MTE2, PIPE_MTE3 and PIPE_S instructions wait for prior vector compute; use asc_sync instead.
 *
 * @override_id asc_sync_vec__z
 * @archs 2201 3510
 */
__aicore__ inline void asc_sync_vec();

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_SYNC_SYNC_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_SYNC_SYNC_H
#endif
