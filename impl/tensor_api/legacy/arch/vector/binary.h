/** Copyright (c) 2026 Huawei Technologies Co., Ltd. */
#ifndef IMPL_TENSOR_API_LEGACY_ARCH_VECTOR_BINARY_H
#define IMPL_TENSOR_API_LEGACY_ARCH_VECTOR_BINARY_H
namespace asc {
namespace te {
namespace inst {
using Add = add;
using Madd = madd;
using Sub = sub;
using And = bitwise_and;
using Select = select;
using AbsSub = abs_sub;
using Min = min;
using Max = max;
using Or = bitwise_or;
using Mul = mul;
using ExpSubEven = exp_sub_even;
using ExpSubOdd = exp_sub_odd;
using ShiftLeft = shift_left;
using ShiftRight = shift_right;
using Div = div;
using Xor = bitwise_xor;
using Prelu = prelu;
} // namespace inst
} // namespace te
} // namespace asc
#endif // IMPL_TENSOR_API_LEGACY_ARCH_VECTOR_BINARY_H
