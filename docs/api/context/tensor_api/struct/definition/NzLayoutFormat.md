# NzLayoutFormat

## 功能说明

NzLayoutFormat用于定义NZ格式的布局，NZ格式是昇腾AI处理器常用的分块存储格式，外层矩阵按行存储，内层矩阵按列存储。

## 结构体定义

```cpp
template <typename T>
struct NzLayoutFormat {
    static constexpr size_t kInnerRow = 16;
    static constexpr size_t kInnerCol = 32 / sizeof(T);
};
```

## 字段说明

| 字段名 | 类型 | 描述 |
|--------|------|------|
| kInnerRow | size_t | 内层矩阵的行数，固定为16 |
| kInnerCol | size_t | 内层矩阵的列数，由数据类型决定 |

## 约束说明

- T必须是有效的数据类型，如half、float、int32_t等
- 内层矩阵的大小固定为16 * (32 / sizeof(T))

## 调用示例

```cpp
// 创建NZ格式Layout
using T = half;
size_t mLength = 128;
size_t kLength = 64;

auto layout = AscendC::MakeNzLayout<T>(mLength, kLength);
```