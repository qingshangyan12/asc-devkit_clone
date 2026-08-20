# operator+

> **命名空间** `asc::te` · **头文件** `tensor_api/experimental/reg/basic_arithmetic.h`

## 1. 产品支持情况

- Ascend 950PR/Ascend 950DT：支持
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：不支持
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：不支持
- Atlas 200I/500 A2 推理产品：不支持
- Atlas 推理系列产品AI Core：不支持
- Atlas 推理系列产品Vector Core：不支持
- Atlas 训练系列产品：不支持

## 2. 功能说明

头文件路径：`"tensor_api/experimental/reg/basic_arithmetic.h"`。

`operator+` 对两个操作数执行逐元素加法。操作数支持寄存器与寄存器、寄存器与标量、
标量与寄存器三种组合。

## 3. 函数原型

```cpp
namespace asc::te {

template <typename T>
__simd_callee__ inline decltype(auto) operator+(
    const reg_tensor<T>& src0, const reg_tensor<T>& src1);

template <typename T>
__simd_callee__ inline decltype(auto) operator+(
    const reg_tensor<T>& src, const T& scalar);

template <typename T>
__simd_callee__ inline decltype(auto) operator+(
    const T& scalar, const reg_tensor<T>& src);

} // namespace asc::te
```

## 4. 参数说明

**表1 参数说明**

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `src0`、`src1` | 输入 | 双寄存器重载的两个同类型输入。执行 mask 取 `src0.mask`，`src1.mask` 不参与选择。 |
| `src` | 输入 | 标量重载的寄存器输入，同时提供执行 mask。 |
| `scalar` | 输入 | 与 `src` 元素类型相同的标量，调用时广播到各有效 lane。 |

**支持的数据类型**

寄存器元素类型支持 `int8_t`、`uint8_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float`。

## 5. 返回值说明

返回 `reg_tensor<T>`：

- `reg`：有效 lane 保存逐元素加法结果。整型结果超出 `T` 的位宽时，仅保留目标类型位宽内的结果。
- `mask`：等于第一个 `reg_tensor` 操作数的 `mask`。

## 6. 约束说明

- 本接口仅支持 `__NPU_ARCH__ == 3510`，并且只能在 AIV 的 `__simd_vf__` 函数中调用；
  底层调用 `asc_add` 或 `asc_add_scalar`。
- 作为执行 mask 的第一个寄存器操作数 `.mask` 必须在调用前初始化，通常使用
  `load(...).with_mask(m)`。`m` 可由 `update_mask<T>(remain)`、`make_mask<pattern, T>()` 或 `all_mask<T>()` 生成；
  未初始化的 mask 内容不确定。
- 单个矢量寄存器宽度为 `asc_get_vf_len()` 字节；`T` 类型对应的最大 lane 数为
  `asc_get_vf_len() / sizeof(T)`。
- `operator+` 对应的 `asc_add`/`asc_add_scalar` 使用 ZEROING 模式，无效目的 lane 置零。
- 双寄存器元素类型不一致或标量类型与寄存器元素类型不一致时，找不到匹配的
  `operator+` 重载，编译失败。
- 整型 `operator+` 的结果超出元素类型表示范围时采用非饱和截断；该形式不返回进位信息。

## 7. 调用示例

以下示例展示接口在 AIV 计算函数中的典型调用方式。示例先为加载结果绑定 mask，
再执行计算并将结果写回 UB。

完整的 Kernel、Host 调用和数据验证代码请参见
[可运行样例](../../../../../../../examples/01_simd_cpp_api/02_features/03_basic_api/02_vector_compute/basic_arithmetic_tensor_api/)。

```cpp
#include "tensor_api/tensor.h"
#include "tensor_api/arch/vector/experimental/reg_tensor.h"
#include "tensor_api/experimental/reg/basic_arithmetic.h"

__simd_vf__ inline void add_example(
    __ubuf__ uint32_t* src0, __ubuf__ uint32_t* src1, __ubuf__ uint32_t* dst)
{
    auto layout =
        asc::te::make_frame_layout<asc::te::nd_ext_layout_ptn>(asc::te::_1 {}, asc::te::_64 {});
    auto input0 = asc::te::make_tensor(asc::te::make_mem_ptr(src0), layout);
    auto input1 = asc::te::make_tensor(asc::te::make_mem_ptr(src1), layout);
    auto output = asc::te::make_tensor(asc::te::make_mem_ptr(dst), layout);
    auto mask = asc::te::all_mask<uint32_t>();
    auto src0_reg = input0.load(asc::te::make_coord(0, 0)).with_mask(mask);
    auto src1_reg = input1.load(asc::te::make_coord(0, 0)).with_mask(mask);

    auto sum = src0_reg + src1_reg;
    output.store(asc::te::make_coord(0, 0), sum);
}
```
