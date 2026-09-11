/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

/*!
 * \file kernel_operator_sys_var_intf.h
 * \brief
 */

#if defined(__NPU_COMPILER_INTERNAL_PURE_SIMT__)
#error "kernel_operator_sys_var_intf.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_SYS_VAR_INTF_H__
#endif

#ifndef ASCENDC_MODULE_OPERATOR_SYS_VAR_INTERFACE_H
#define ASCENDC_MODULE_OPERATOR_SYS_VAR_INTERFACE_H

#include "../../impl/basic_api/kernel_macros.h"
#include "../../impl/basic_api/utils/kernel_utils_mode.h"
#include "../../impl/basic_api/utils/kernel_utils_constants.h"

#if defined(ASCENDC_CPU_DEBUG) && ASCENDC_CPU_DEBUG == 1
#include <cstdint>
#include "stub_def.h"
#endif

namespace AscendC {

/**
 * @ingroup simd_api.basic_api.tool_interface.system_resources_and_variables
 * @override_id GetBlockNum__z
 * @archs 3510 2201 3002 2002 1001 x90 9030
 * @brief 获取当前任务配置的逻辑AI Core的数量，用于代码内部的多核逻辑控制等。
 */
__aicore__ inline int64_t GetBlockNum();

/**
 * @ingroup simd_api.basic_api.tool_interface.system_resources_and_variables
 * @override_id GetBlockIdx__z
 * @archs 3510 2201 3002 2002 1001 x90 9030
 * @brief 获取当前逻辑AI Core的索引，用于代码内部的多核逻辑控制及多核偏移量计算等。
 */
__aicore__ inline int64_t GetBlockIdx();

/**
 * @ingroup simd_api.basic_api.tool_interface.system_resources_and_variables
 * @override_id GetSubBlockIdx__z
 * @archs 3510 2201 x90 9030
 * @brief 在[分离模式架构](../../../../../guide/programming_guide/advanced_programming/hardware_implementation/basic_architecture.md)下，获取逻辑AI Core上Cube Core（AIC）或者Vector Core（AIV）的逻辑索引。
 */
__aicore__ inline int64_t GetSubBlockIdx();

/**
 * @ingroup simd_api.basic_api.tool_interface.system_resources_and_variables
 * @override_id GetTaskRatio__z
 * @archs 3510 2201 2002 x90 9030
 * @brief [分离模式](../../../../../guide/programming_guide/advanced_programming/hardware_implementation/basic_architecture.md)下，获取任务启动的Cube Core（AIC）或者Vector Core（AIV）的数量与逻辑AI Core数量的比例。
 */
__aicore__ inline int64_t GetTaskRatio();

/**
 * @ingroup simd_api.basic_api.tool_interface.system_resources_and_variables
 * @override_id GetDataBlockSizeInBytes__z
 * @archs 3510 2201 2002 1001 x90 9030
 * @brief 获取当前芯片版本一个DataBlock的大小，单位为Byte。
 */
__aicore__ inline constexpr int16_t GetDataBlockSizeInBytes() { return ONE_BLK_SIZE; }

/**
 * @ingroup simd_api.basic_api.tool_interface.system_resources_and_variables
 * @override_id GetArchVersion__Ru32
 * @archs 3510 2201 3002 2002 1001
 * @brief 获取当前AI处理器架构版本号。
 * @param [out] coreVersion AI处理器架构版本。
 */
__aicore__ inline void GetArchVersion(uint32_t& coreVersion);

/**
 * @ingroup simd_api.basic_api.tool_interface.system_resources_and_variables
 * @override_id GetSubBlockNum__z
 * @archs 3510 2201 x90 9030
 * @brief 在[分离模式架构](../../../../../guide/programming_guide/advanced_programming/hardware_implementation/basic_architecture.md)下，获取当前配置中，一个逻辑AI Core上Cube Core（AIC）或者Vector Core（AIV）的数量。
 */
__aicore__ inline int64_t GetSubBlockNum();

/**
 * @ingroup simd_api.basic_api.tool_interface.system_resources_and_variables
 * @override_id GetProgramCounter__z
 * @archs 3510 2201 2002
 * @brief 获取程序计数器的值，以追踪当前程序执行的位置。
 */
__aicore__ inline int64_t GetProgramCounter();

/**
 * @ingroup simd_api.basic_api.debug_interface.exception_detection
 * @override_id Trap__z
 * @archs 3510 2201 2002 x90 9030
 * @brief 在核函数（Kernel）侧调用，NPU模式下会中断AI Core的运行，CPU模式下等同于assert。可用于核函数（Kernel）侧异常场景的调试。
 */
__aicore__ inline void Trap();

/**
 * @ingroup simd_api.basic_api.tool_interface.system_resources_and_variables
 * @override_id GetSystemCycle__z
 * @archs 3510 2201
 * @brief 获取系统计数器当前的cycle值。通过计算代码段前后cycle值的差值，可得到代码段消耗的系统cycle数，再通过换算即可获得代码段的执行时间。
 */
__aicore__ inline int64_t GetSystemCycle();

template <SpecialPurposeReg spr>
/**
 * @ingroup simd_api.basic_api.special_register_access
 * @override_id GetSpr__z
 * @archs 3510
 * @brief 获取指定特殊寄存器的值。当前支持[表SpecialPurposeReg模板参数说明](#table37531617424)。
 */
__aicore__ inline int64_t GetSpr();

template <SpecialPurposeReg spr>
/**
 * @ingroup simd_api.basic_api.special_register_access
 * @override_id ClearSpr__z
 * @archs 3510
 * @brief 对指定的特殊寄存器进行清零。当前支持[表SpecialPurposeReg模板参数说明](GetSpr.md#table37531617424)。
 */
__aicore__ inline void ClearSpr();

/**
 * @ingroup simd_api.basic_api.tool_interface.system_resources_and_variables
 * @override_id GetUBSizeInBytes__z
 * @archs 3510
 * @brief 获取Unified Buffer（UB）空间的大小，单位为Byte。开发者根据UB的大小来计算循环次数等参数值。
 */
__aicore__ inline constexpr uint32_t GetUBSizeInBytes()
{
#if defined(__NPU_ARCH__) &&                                                                                 \
    ((__NPU_ARCH__ == 5102) || (__NPU_ARCH__ == 3003) || (__NPU_ARCH__ == 3113) || (__NPU_ARCH__ == 3510) || \
     (__NPU_ARCH__ == 5101) || (__NPU_ARCH__ == 5161) || (__NPU_ARCH__ == 5165) || (__NPU_ARCH__ == 5163))
    return TOTAL_UB_SIZE;
#else
    return 0;
#endif
}

/**
 * @ingroup simd_api.basic_api.tool_interface.system_resources_and_variables
 * @override_id GetVecLen__z
 * @archs 3510
 * @brief 获取[RegTensor](../../reg_vector_compute/register_data_types/RegTensor.md)位宽VL（Vector Length）的大小。
 */
__aicore__ inline constexpr uint32_t GetVecLen()
{
#if defined(__NPU_ARCH__) &&                                                                                 \
    ((__NPU_ARCH__ == 5102) || (__NPU_ARCH__ == 3003) || (__NPU_ARCH__ == 3113) || (__NPU_ARCH__ == 3510) || \
     (__NPU_ARCH__ == 5101) || (__NPU_ARCH__ == 5161) || (__NPU_ARCH__ == 5165) || (__NPU_ARCH__ == 5163))
    return VECTOR_REG_WIDTH;
#else
    return 0;
#endif
}

/**
 * @ingroup simd_api.basic_api.tool_interface.system_resources_and_variables
 * @override_id GetRuntimeUBSize__z
 * @archs 3510
 * @brief 获取运行时Unified Buffer（UB）空间的大小，单位为Byte。开发者根据UB的大小来计算循环次数等参数值。
 */
__aicore__ inline uint32_t GetRuntimeUBSize()
{
#if defined(__NPU_ARCH__) &&                                                                                 \
    ((__NPU_ARCH__ == 5102) || (__NPU_ARCH__ == 3003) || (__NPU_ARCH__ == 3113) || (__NPU_ARCH__ == 3510) || \
     (__NPU_ARCH__ == 5101) || (__NPU_ARCH__ == 5161) || (__NPU_ARCH__ == 5165) || (__NPU_ARCH__ == 5163))
#if defined(ASCENDC_CPU_DEBUG) && ASCENDC_CPU_DEBUG == 1
    return TOTAL_UB_SIZE;
#else
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
#if defined(SPLIT_CORE_VEC)
#if defined(__ASC_DISABLE_VF_STACK_RESERVED__)
    constexpr uint32_t RESERVED_UB_SIZE = 2 * 1024;
#else
    constexpr uint32_t RESERVED_UB_SIZE = 8 * 1024;
#endif
    return get_shmem_sz() - RESERVED_UB_SIZE;
#else
    return TOTAL_UB_SIZE; // cube core not support get_shmem_sz
#endif
#elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 5102)
    constexpr uint32_t RESERVED_UB_SIZE = 8 * 1024;
    return get_shmem_sz() - RESERVED_UB_SIZE; // m510 aicore reserve 8KB
#else
    return TOTAL_UB_SIZE;
#endif
#endif
#else
    return 0;
#endif
}

/**
 * @ingroup simd_api.basic_api.tool_interface.system_resources_and_variables
 * @override_id GetSsbufBaseAddr__z
 * @archs 3510
 * @brief 该接口用于获取SSBuffer的基地址。
 */
__aicore__ inline __ssbuf__ void* GetSsbufBaseAddr()
{
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
#if ASCENDC_CPU_DEBUG
    return reinterpret_cast<__ssbuf__ void*>(ConstDefiner::Instance().cpuSSbuf);
#else
    return (__ssbuf__ void*)0;
#endif
#else
    return (__ssbuf__ void*)0;
#endif
}
} // namespace AscendC

#include "../../impl/basic_api/kernel_operator_sys_var_intf_impl.h"
#endif // ASCENDC_MODULE_OPERATOR_SYS_VAR_INTERFACE_H

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_SYS_VAR_INTF_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_SYS_VAR_INTF_H__
#endif
