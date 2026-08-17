# mull

## 产品支持情况

- Ascend 950PR/Ascend 950DT：支持。
- 其他产品：不支持。

## 功能说明

头文件路径：`"tensor_api/experimental/reg/basic_arithmetic.h"`。

逐元素执行 32 位整数宽乘，并分别得到乘积的低 32 位和高 32 位。

## 函数原型

```cpp
template <typename T>
__simd_callee__ inline decltype(auto) mull(
    reg_tensor<T>& high,
    const reg_tensor<T>& src0,
    const reg_tensor<T>& src1);
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `high` | 输出 | 乘积的高 32 位。 |
| `src0`、`src1` | 输入 | 两个 32 位整数寄存器。 |

## 数据类型

`T` 支持 `int32_t`、`uint32_t`。

## 返回值说明

返回乘积低 32 位对应的 `reg_tensor<T>`，并通过 `high` 返回高 32 位。两个结果的 mask 均与 `src0.mask` 一致。

## 约束说明

`src0`、`src1` 和 `high` 的元素类型必须一致。

## 调用示例

```cpp
asc::te::reg_tensor<uint32_t> high;
auto low = asc::te::mull(high, src0, src1);
```
