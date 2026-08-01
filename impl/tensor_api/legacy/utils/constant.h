/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef IMPL_TENSOR_API_LEGACY_UTILS_CONSTANT_H
#define IMPL_TENSOR_API_LEGACY_UTILS_CONSTANT_H

namespace asc {
namespace te {

namespace Inst = inst;
using KeepMark = keep_mark;
using ArchVersion = arch_version;
using GetArchVersion = get_arch_version;

namespace copy_mode {
using NORMAL = normal;
using TRANS = trans;
using TRANS_B8B4 = trans_b8b4;
} // namespace copy_mode
namespace CopyMode = copy_mode;

namespace location {
using INVALID = invalid;
using GM = gm;
using UB = ub;
using L1 = l1;
using L0A = l0a;
using L0B = l0b;
using L0ScaleA = l0scalea;
using L0ScaleB = l0scaleb;
using L0C = l0c;
using BIAS = bias;
using FIXBUF = fixbuf;
using SSBUF = ssbuf;
} // namespace location
namespace Location = location;

template <typename T>
using IsHardware = is_hardware<T>;

template <typename T>
constexpr bool IsHardwareV = is_hardware_v<T>;

template <typename A, typename ProcessedTuple>
using AllElementsSameAsA = all_elements_same_as_a<A, ProcessedTuple>;

template <typename A, typename... BList>
using CheckAllSame = check_all_same<A, BList...>;

template <typename A, typename... BList>
constexpr bool IsOneOfAttrV = is_one_of_attr_v<A, BList...>;

template <typename DataType>
inline constexpr bool IsDataType = is_data_type<DataType>;

template <typename DataType>
inline constexpr bool IsB4Type = is_b4_type<DataType>;

template <typename T = Std::ignore_t>
__aicore__ inline constexpr size_t GetC0Size()
{
    return get_c0_size<T>();
}

template <typename T>
using IsIntegralConstant = is_integral_constant<T>;

template <typename T>
constexpr bool IsIntegralConstantV = is_integral_constant_v<T>;

#if defined(__NPU_ARCH__)
using VectorTypeTransform = vector_type_transform;
#endif

} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_LEGACY_UTILS_CONSTANT_H
