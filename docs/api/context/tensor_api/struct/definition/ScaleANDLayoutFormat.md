# ScaleANDLayoutFormat

## 功能说明

ScaleANDLayoutFormat用于定义ScaleAND格式的布局，ScaleAND格式是一种支持缩放和ND格式的布局。

## 结构体定义

```cpp
template <typename T>
struct ScaleANDLayoutFormat {
    static constexpr size_t kInnerRow = 1;
    static constexpr size_t kInnerCol = 1;
};
```

## 字段说明

| 字段名 | 类型 | 描述 |
|--------|------|------|
| kInnerRow | size_t | 内层矩阵的行数，固定为1 |
| kInnerCol | size_t | 内层矩阵的列数，固定为1 |

## 约束说明

- T必须是有效的数据类型，如half、float、int32_t等
- ScaleAND格式不使用分块存储

## 调用示例

```cpp
// 创建ScaleAND格式Layout
using T = half;
size_t mLength = 128;
size_t nLength = 64;

auto layout = AscendC::MakeScaleANDLayout<T>(mLength, nLength);
```