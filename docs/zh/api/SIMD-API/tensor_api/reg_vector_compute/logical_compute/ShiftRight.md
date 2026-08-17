# operator>>

## 产品支持情况

- Ascend 950PR/Ascend 950DT：支持。
- 其他产品：不支持。

## 功能说明

头文件路径：`"tensor_api/experimental/reg/logical_compute.h"`。

按照寄存器或标量给出的移位量，对 `src` 逐元素右移。

## 函数原型

```cpp
template <typename T, typename U>
__simd_callee__ inline decltype(auto) operator>>(
    const reg_tensor<T>& src, const U& shift);
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `src` | 输入 | 待右移的整数寄存器。 |
| `shift` | 输入 | 逐元素移位寄存器或 `int16_t` 标量。 |

## 数据类型

寄存器移位支持以下组合：

| `src` 元素类型 | `shift` 寄存器元素类型 |
| --- | --- |
| `int8_t`、`uint8_t` | `int8_t` |
| `int16_t`、`uint16_t` | `int16_t` |
| `int32_t`、`uint32_t` | `int32_t` |

标量移位支持上述全部 `src` 类型，`shift` 必须为 `int16_t`。

## 返回值说明

返回 `reg_tensor<T>`，mask 与 `src.mask` 一致。

## 约束说明

- 调用方应保证移位量非负且处于底层指令允许的范围内。
- 有符号数和无符号数的右移行为遵循对应底层指令语义。

## 调用示例

```cpp
auto result0 = src >> shift_reg;
auto result1 = src >> static_cast<int16_t>(2);
```
