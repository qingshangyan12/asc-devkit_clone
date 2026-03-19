# L0CLayoutFormat

## 功能说明

L0CLayoutFormat用于定义L0C Buffer的布局格式，L0C Buffer用于存储矩阵计算的结果。

## 结构体定义

```cpp
template <typename T>
struct L0CLayoutFormat {
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
// 创建L0C格式Layout
using T = half;
size_t mLength = 128;
size_t nLength = 64;

auto layout = AscendC::MakeL0CLayout<T>(mLength, nLength);
```