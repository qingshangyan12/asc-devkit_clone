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
#include "impl/c_api/instr_impl/npu_arch_2201/sync_impl.h"
#elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
#include "impl/c_api/instr_impl/npu_arch_3510/sync_impl.h"
#endif

/**
 * @ingroup simd_c_api.sync
 * @override_id asc_sync_notify__pipe_pipe_event
 * @archs 3510 2201
 * @brief 如图1所示，与[asc_sync_wait](asc_sync_wait.md)配对使用，用于实现AI Core内部不同流水之间的同步控制。
 * @param [in] pipe 源流水类型，即“等待哪条流水的前序指令完成”。取值范围为`pipe_t`枚举：`PIPE_S`、`PIPE_V`、`PIPE_M`、`PIPE_MTE1`、`PIPE_MTE2`、`PIPE_MTE3`、`PIPE_FIX`。
 * @param [in] tpipe 目标流水类型，即“解除哪条流水的`asc_sync_wait`阻塞”。取值范围与`pipe`相同，为`pipe_t`枚举。
 * @param [in] id 同步事件ID，每对`pipe`与`tpipe`组合各自拥有8个独立的同步事件ID。取值范围为`event_t`枚举类型。
 */
__aicore__ inline void asc_sync_notify(pipe_t pipe, pipe_t tpipe, event_t id);

/**
 * @ingroup simd_c_api.sync
 * @override_id asc_sync_wait__pipe_pipe_event
 * @archs 3510 2201
 * @brief 等待特定的同步标志，阻塞当前流水线直到条件满足。
 * @param [in] pipe 源流水线类型。需传入编译期常量。
 * @param [in] tpipe 目标流水线类型。需传入编译期常量。
 * @param [in] id 同步ID。
 */
__aicore__ inline void asc_sync_wait(pipe_t pipe, pipe_t tpipe, event_t id);

/**
 * @ingroup simd_c_api.sync
 * @override_id asc_sync_pipe__pipe
 * @archs 3510 2201
 * @brief 完成同一流水线内的同步控制。
 * @param [in] pipe 阻塞的流水类别。支持的取值有`PIPE_V`、`PIPE_M`、`PIPE_MTE1`、`PIPE_MTE2`、`PIPE_MTE3`、`PIPE_FIX`、`PIPE_ALL`，不支持`PIPE_S`。如果不关注流水类别，希望阻塞所有流水，可以传入`PIPE_ALL`。各流水类别的含义请参考[流水类型说明](intra_core_sync_overview.md)。
 */
__aicore__ inline void asc_sync_pipe(pipe_t pipe);

/**
 * @ingroup simd_c_api.sync
 * @override_id asc_sync_vec__i
 * @archs 3510 2201
 * @brief **无入参函数原型`asc_sync_vec()`接口已废弃，请使用[asc_sync](asc_sync.md)执行针对所有流水线的同步操作。
 * @param [in] id 同步ID。
 */
__aicore__ inline void asc_sync_vec(int id);

/**
 * @ingroup simd_c_api.sync
 * @override_id asc_sync_mte3__i
 * @archs 3510 2201
 * @brief 针对PIPE_MTE3流水线执行同步操作。其作用是保证PIPE_MTE3流水线中前序指令（如Unified Buffer（UB）到GM的数据搬运）全部完成后，PIPE_MTE2与PIPE_V流水线的后续指令才能开始执行。
 * @param [in] id 同步ID。
 */
__aicore__ inline void asc_sync_mte3(int id);

/**
 * @ingroup simd_c_api.sync
 * @override_id asc_sync_mte2__i
 * @archs 3510 2201
 * @brief 针对PIPE_MTE2流水线执行同步操作。其作用是保证PIPE_MTE2流水线中前序指令（如GM到Unified Buffer（UB）的数据搬运）全部完成后，PIPE_MTE3与PIPE_V流水线的后续指令才能开始执行。
 * @param [in] id 同步ID。
 */
__aicore__ inline void asc_sync_mte2(int id);

/**
 * @ingroup simd_c_api.sync
 * @override_id asc_sync__z
 * @archs 3510 2201
 * @brief 用于同一核内所有流水线之间的全流水同步，功能等价于[asc_sync_pipe(PIPE_ALL)](asc_sync_pipe.md)。阻塞调用点后所有硬件流水的后序指令，直到调用点之前所有硬件流水的前序指令全部完成。
 */
__aicore__ inline void asc_sync();

/**
 * @ingroup simd_c_api.sync
 * @override_id asc_sync_block_arrive__pipe_i64
 * @archs 3510 2201
 * @brief 本接口与[asc_sync_block_wait](asc_sync_block_wait.md)配对使用，实现单AI Core内AIC与全部AIV之间的同步（[四种核间同步模式](system_sync_overview.md#fig_sync_control_mode)中的模式2）。
 * @param [in] pipe 标识在哪条流水的前序指令完成后才允许向调度模块发送通知。
 * @param [in] flag_id 核间同步的标记，用于标识同一组同步信号。取值范围为[0, 15]，每个`flag_id`各自拥有独立的4位计数器。
 */
__aicore__ inline void asc_sync_block_arrive(pipe_t pipe, int64_t flag_id);

/**
 * @ingroup simd_c_api.sync
 * @override_id asc_sync_subblock_arrive__pipe_i64
 * @archs 3510 2201
 * @brief 本接口与[asc_sync_subblock_wait](asc_sync_subblock_wait.md)配对使用，实现同一AI Core内全部AIV（[subblock](inter_core_sync_overview.md#fig_block_subblock_relationship)）之间的同步（[四种核间同步模式](system_sync_overview.md#fig_sync_control_mode)中的模式1）。
 * @param [in] pipe 标识在哪条流水的前序指令完成后才允许向调度模块发送通知。
 * @param [in] flag_id 核间同步的标记，用于标识同一组同步信号。取值范围为[0, 15]，每个`flag_id`各自拥有独立的4位计数器。
 */
__aicore__ inline void asc_sync_subblock_arrive(pipe_t pipe, int64_t flag_id);

/**
 * @ingroup simd_c_api.sync
 * @override_id asc_sync_inter_arrive__pipe_i64
 * @archs 3510 2201
 * @brief 本接口与[asc_sync_inter_wait](asc_sync_inter_wait.md)配对使用，实现不同AI Core之间全部AIC或全部AIV的同步（[四种核间同步模式](system_sync_overview.md#fig_sync_control_mode)中的模式0）。
 * @param [in] pipe 标识在哪条流水的前序指令完成后才允许向调度模块发送通知。
 * @param [in] flag_id 核间同步的标记，用于标识同一组同步信号。取值范围为[0, 15]，每个`flag_id`各自拥有独立的4位计数器。
 */
__aicore__ inline void asc_sync_inter_arrive(pipe_t pipe, int64_t flag_id);

/**
 * @ingroup simd_c_api.sync
 * @override_id asc_sync_data_barrier__memdsb
 * @archs 3510 2201
 * @brief 阻塞后续的指令执行，直到所有之前的内存访问指令（需要等待的内存位置可通过参数控制）执行结束，用于解决Scalar的内存依赖。
 * @param [in] arg 表示需要等待的内存位置，类型为mem_dsb_t，可取值为： &bull; DSB_ALL：等待所有内存访问指令。 &bull; DSB_DDR：等待GM访问指令。&bull; DSB_UB：等待UB访问指令。&bull; DSB_SEQ：预留参数，暂未启动，为后续的功能扩展做保留。
 */
__aicore__ inline void asc_sync_data_barrier(mem_dsb_t arg);

/**
 * @ingroup simd_c_api.sync
 * @override_id asc_sync_intra_arrive__pipe_u64
 * @archs 3510
 * @brief 本接口与[asc_sync_intra_wait](asc_sync_intra_wait.md)配对使用，实现单AI Core内AIC与单个AIV之间的同步（[四种核间同步模式](system_sync_overview.md#fig_sync_control_mode)中的模式4）。
 * @param [in] pipe 标识在哪条流水的前序指令完成后才允许向调度模块发送通知。
 * @param [in] sync_id 核间同步的标记，用于标识同一组同步信号。每个`sync_id`各自拥有独立的4位计数器。一个AI Core由1个AIC与2个AIV构成，AIC侧拥有32个`sync_id`（0~31），每个AIV侧各拥有16个`sync_id`（0~15）。跨核ID映射规则如下：• 第一个AIV的ID 0~15映射到AIC的ID 0~15；第二个AIV的ID 0~15映射到AIC的ID 16~31。• AIC的ID 0~15映射到第一个AIV的ID 0~15；AIC的ID 16~31映射到第二个AIV的ID 0~15。
 */
__aicore__ inline void asc_sync_intra_arrive(pipe_t pipe, uint64_t sync_id);

/**
 * @ingroup simd_c_api.sync
 * @override_id asc_sync_intra_wait__pipe_u64
 * @archs 3510
 * @brief 本接口与[asc_sync_intra_arrive](asc_sync_intra_arrive.md)配对使用，实现单AI Core内AIC与单个AIV之间的同步（[四种核间同步模式](system_sync_overview.md#fig_sync_control_mode)中的模式4）。
 * @param [in] pipe 标识阻塞哪条流水的后续指令，直到对应`sync_id`的计数器非0。
 * @param [in] sync_id 核间同步的标记，用于标识同一组同步信号。每个`sync_id`各自拥有独立的4位计数器。一个AI Core由1个AIC与2个AIV构成，AIC侧拥有32个`sync_id`（0~31），每个AIV侧各拥有16个`sync_id`（0~15）。跨核ID映射规则如下：• 第一个AIV的ID 0~15映射到AIC的ID 0~15；第二个AIV的ID 0~15映射到AIC的ID 16~31。• AIC的ID 0~15映射到第一个AIV的ID 0~15；AIC的ID 16~31映射到第二个AIV的ID 0~15。
 */
__aicore__ inline void asc_sync_intra_wait(pipe_t pipe, uint64_t sync_id);

/**
 * @ingroup simd_c_api.sync
 * @override_id asc_sync_subblock_wait__pipe_i64
 * @archs 3510 2201
 * @brief 本接口与[asc_sync_subblock_arrive](asc_sync_subblock_arrive.md)配对使用，实现同一AI Core内全部AIV（[subblock](inter_core_sync_overview.md#fig_block_subblock_relationship)）之间的同步（[四种核间同步模式](system_sync_overview.md#fig_sync_control_mode)中的模式1）。
 * @param [in] pipe 标识阻塞哪条流水上后续指令，直到对应`flag_id`的计数器非0。
 * @param [in] flag_id 核间同步的标记，用于标识同一组同步信号。取值范围为[0, 15]，每个`flag_id`各自拥有独立的4位计数器。
 */
__aicore__ inline void asc_sync_subblock_wait(pipe_t pipe, int64_t flag_id);

/**
 * @ingroup simd_c_api.sync
 * @override_id asc_sync_inter_wait__pipe_i64
 * @archs 3510 2201
 * @brief 本接口与[asc_sync_inter_arrive](asc_sync_inter_arrive.md)配对使用，实现不同AI Core之间全部AIC或全部AIV的同步（[四种核间同步模式](system_sync_overview.md#fig_sync_control_mode)中的模式0）。
 * @param [in] pipe 标识阻塞哪条流水上后续指令，直到对应`flag_id`的计数器非0。
 * @param [in] flag_id 核间同步的标记，用于标识同一组同步信号。取值范围为[0, 15]，每个`flag_id`各自拥有独立的4位计数器。
 */
__aicore__ inline void asc_sync_inter_wait(pipe_t pipe, int64_t flag_id);

/**
 * @ingroup simd_c_api.sync
 * @override_id asc_sync_block_wait__pipe_i64
 * @archs 3510 2201
 * @brief 本接口与[asc_sync_block_arrive](asc_sync_block_arrive.md)配对使用，实现单AI Core内AIC与全部AIV之间的同步（[四种核间同步模式](system_sync_overview.md#fig_sync_control_mode)中的模式2）。
 * @param [in] pipe 标识阻塞哪条流水上后续指令，直到对应`flag_id`的计数器非0。
 * @param [in] flag_id 核间同步的标记，用于标识同一组同步信号。取值范围为[0, 15]，每个`flag_id`各自拥有独立的4位计数器。
 */
__aicore__ inline void asc_sync_block_wait(pipe_t pipe, int64_t flag_id);

/**
 * @ingroup simd_c_api.sync
 * @override_id asc_lock__pipe_u8
 * @archs 3510
 * @brief 该接口用于AI Core内部异步流水线之间的同步，可按需阻塞指定流水线的执行。
 * @param [in] pipe 指定当前接口所属的流水类型。
 * @param [in] mutex_id mutex标号，取值范围为[0, 31]。
 */
__aicore__ inline void asc_lock(pipe_t pipe, uint8_t mutex_id);

__aicore__ inline void asc_lock(pipe_t pipe, uint8_t mutex_id, const asc_mutex_execute_mode mode);

/**
 * @ingroup simd_c_api.sync
 * @override_id asc_unlock__pipe_u8
 * @archs 3510
 * @brief 用于同步AI Core内部异步流水的指令，直到当前流水的前序指令执行完成后，根据mutex_id释放对应Mutex。
 * @param [in] pipe 该指令的执行流水线。
 * @param [in] mutex_id mutex标号。取值范围[0, 31]。
 */
__aicore__ inline void asc_unlock(pipe_t pipe, uint8_t mutex_id);

__aicore__ inline void asc_unlock(pipe_t pipe, uint8_t mutex_id, const asc_mutex_execute_mode mode);

[[deprecated("NOTICE: asc_sync_vec() is deprecated. "
             "Please use asc_sync() instead")]]
/**
 * @ingroup simd_c_api.sync
 * @override_id asc_sync_vec__z
 * @archs 3510 2201
 * @brief **无入参函数原型`asc_sync_vec()`接口已废弃，请使用[asc_sync](asc_sync.md)执行针对所有流水线的同步操作。
 */
__aicore__ inline void asc_sync_vec();

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_SYNC_SYNC_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_SYNC_SYNC_H
#endif
