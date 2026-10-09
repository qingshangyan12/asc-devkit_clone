# div_precise

## 功能说明

头文件：`tensor_api/experimental/arch/vector/basic_arithmetic.h`。

对两个 `reg_tensor<float>` 按元素进行高精度除法。该接口等价于 `div<div_algorithm::precision_0ulp_ftz_true>(src0, src1)`，复用基础 API 的 3510 差值补偿路径。

## 函数原型

```cpp
template <typename T>
__simd_callee__ inline reg_tensor<T> div_precise(
    const reg_tensor<T>& src0, const reg_tensor<T>& src1);
```

仅支持 `T = float`，且仅适用于 `__NPU_ARCH__ == 3510`。使用 `src0.mask` 控制计算，忽略 `src1.mask`；返回值的 mask 等于 `src0.mask`。与 `/` 不同，该接口会执行额外的差值补偿指令。

需要保留 subnormal 结果时，应使用 `div<div_algorithm::precision_0ulp_ftz_false>`，参见 [模式除法](div_mode.md)。
