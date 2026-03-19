# ZnLayoutFormat

## 功能说明

ZnLayoutFormat用于定义Zn格式的布局，Zn格式是NZ格式的转置版本，外层矩阵按列存储，内层矩阵按行存储。

## 结构体定义

```cpp
template <typename Z>
struct ZnLayoutFormat {
    static constexpr size_t kInnerRow = 32 / sizeof(T);
    static constexpr size_t kInnerCol = 16;
};
```

## 字段说明

| 字段名 | 类型 | 描述 |
|--------|------|------|
| kInnerRow | size_t | 内层矩阵的行数，由数据类型决定 |
| kInnerCol | size_t | 内层矩阵的列数，固定为16 |

## 约束说明

- T必须是有效的数据类型，如half、float、int32_t等
- 内层矩阵的大小固定为(32 / sizeof(T)) * 16

## 调用示例

```cpp
// 创建Zn格式Layout
using T = half;
size_t mLength = 128;
size_t kLength = 64;

auto layout = AscendC::MakeZnLayout<T>(size_t mLength, size_t kLength);
```