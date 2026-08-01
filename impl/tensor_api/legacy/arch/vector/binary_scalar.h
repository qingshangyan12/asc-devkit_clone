/** Copyright (c) 2026 Huawei Technologies Co., Ltd. */
#ifndef IMPL_TENSOR_API_LEGACY_ARCH_VECTOR_BINARY_SCALAR_H
#define IMPL_TENSOR_API_LEGACY_ARCH_VECTOR_BINARY_SCALAR_H
namespace asc {
namespace te {
namespace inst {
using AddScalar = add_scalar;
using MulScalar = mul_scalar;
using MaxScalar = max_scalar;
using MinScalar = min_scalar;
using ShiftLeftScalar = shift_left_scalar;
using ShiftRightScalar = shift_right_scalar;
using MulsEven = muls_even;
using MulsOdd = muls_odd;
} // namespace inst
} // namespace te
} // namespace asc
#endif // IMPL_TENSOR_API_LEGACY_ARCH_VECTOR_BINARY_SCALAR_H
