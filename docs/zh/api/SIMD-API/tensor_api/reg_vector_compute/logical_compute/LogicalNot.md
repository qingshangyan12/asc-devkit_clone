# operator!

## 产品支持情况

- Ascend 950PR/Ascend 950DT：支持。
- 其他产品：不支持。

## 功能说明

头文件路径：`"tensor_api/experimental/reg/logical_compute.h"`。

对普通元素执行逻辑非：输入为 0 时结果为 1，输入非 0 时结果为 0。

$$
dst_i = (src_i == 0) ? 1 : 0
$$

对 `reg_tensor<bool>` 执行逐 bit 布尔取反。

## 函数原型

```cpp
template <typename T>
__simd_callee__ inline decltype(auto) operator!(reg_tensor<T> src);
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `T` | 模板参数 | 输入和返回值的元素类型。 |
| `src` | 输入 | 待执行逻辑非的寄存器。 |

## 数据类型

`T` 支持 `bool`、`int8_t`、`uint8_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float`。

## 返回值说明

- 普通类型返回 `reg_tensor<T>`，结果元素为同类型的 0 或 1，mask 与 `src.mask` 一致。
- `bool` 返回 `reg_tensor<bool>`。

## 约束说明

本接口是 C++ 逻辑非语义，不等价于 Basic API 对数值数据执行的按位 `Not`。

## 调用示例

```cpp
auto is_zero = !src_reg;
auto inverted_mask = !mask_reg;
```
