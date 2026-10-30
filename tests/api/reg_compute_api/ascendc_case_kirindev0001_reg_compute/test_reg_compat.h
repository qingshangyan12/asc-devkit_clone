/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

 #ifndef TEST_REG_COMPAT_H
 #define TEST_REG_COMPAT_H

#include "stub_fun.h"
#include "basic_api/reg_compute/kernel_reg_compute_utils.h"
#include "basic_api/reg_compute/kernel_reg_compute_common_intf.h"

namespace AscendC {
namespace Reg {
/* =====================================================================================================
 * GCC编译期间，自上而下解析头文件，测试用例如果include对应的头文件，会逐行解析
 * 例如如下：
 * impl\basic_api\reg_compute\kernel_reg_compute_vec_binary_intf_impl.h
 * 解析And函数调用到了AndImpl,但是impl\basic_api\reg_compute\dav_5161对应目录下没有AndImpl任何声明定义则报错
 * 本文件提供前置声明，防止编译期报错
 *
 * 形态（基线将来不起5161支持时的处理规则）
 * 1.pill 桩采用 template <typename...> void XxxImpl(...) = delete;
 *    函数参数为字面省略号：任何真实模板一旦出现，重载决议中的省略号
 *    转换等级必然低于精确匹配。
 * 2.唯一例外MoveMaskImpl：其包装按值返回MaskReg，桩承载真是返回语义
 *    保留精确签名；基线如将来定义同签名版本将报 redefinition,
 *    删除该桩即可（唯一预期的人工维护点）
 * 3.本文件桩均不承载用例行为：用例值调用dav_5161已经支持的公开API
 *    桩仅满足解析期名字查找
======================================================================================================= */

// kernel_reg_compute_maskreg_intf_impl.h
template <typename T>
__simd_callee__ inline MaskReg MoveMaskImpl()
{
    return MaskReg{};
}

// kernel_reg_compute_vec_unary_intf_impl.h
template <typename...>
void Log2Impl(...) = delete;

template <typename...>
void Log10Impl(...) = delete;

template <typename...>
void NotImpl(...) = delete;

// kernel_reg_compute_vec_binary_intf_impl.h
template <typename...>
void AndImpl(...) = delete;

template <typename...>
void XorImpl(...) = delete;

template <typename...>
void ShiftLeftImpl(...) = delete;

template <typename...>
void ShiftRightImpl(...) = delete;

template <typename...>
void MullImpl(...) = delete;

template <typename...>
void AddCarryOutImpl(...) = delete;

template <typename...>
void SubCarryOutImpl(...) = delete;

template <typename...>
void AddCarryOutsImpl(...) = delete;

template <typename...>
void SubCarryOutsImpl(...) = delete;

// kernel_reg_compute_vec_binary_scalar_intf_impl.h
template <typename...>
void ShiftLeftsImpl(...) = delete;

template <typename...>
void ShiftRightsImpl(...) = delete;

// kernel_reg_compute_datacopy_intf_impl.h
template <typename...>
void LoadImpl(...) = delete;

template <typename...>
void StoreImpl(...) = delete;

// kernel_reg_compute_vec_fused_intf_impl.h
template <typename...>
void FusedAbsSubImpl(...) = delete;

template <typename...>
void FusedMulDstAddImpl(...) = delete;

// kernel_reg_compute_vec_reduce_intf_impl.h
template <typename...>
void ReduceSumWithDataBlockImpl(...) = delete;

template <typename...>
void ReduceMaxWithDataBlockImpl(...) = delete;

template <typename...>
void ReduceMinWithDataBlockImpl(...) = delete;

template <typename...>
void PairReduceSumImpl(...) = delete;
} // namespace Reg
} // namespace AscendC

#endif // TEST_REG_COMPAT_H
