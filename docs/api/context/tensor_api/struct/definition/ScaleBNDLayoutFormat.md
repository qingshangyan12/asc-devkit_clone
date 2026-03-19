# ScaleBNDLayoutFormat

## 功能说明

ScaleBNDLayoutFormat用于定义ScaleBND格式的布局，ScaleBND格式是一种支持缩放和BND格式的布局。

## 结构体定义

```cpp
template <typename T>
struct ScaleBNDLayoutFormat {
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
- ScaleBND格式不使用分块存储

## 调用示例

```cpp
// 创建ScaleBND格式Layout
using T = half;
size_t mLength = 128;
size_t nLength = 64;

auto layout = AscendC::MakeScaleBNDLayout<T>(mLength, nLength);
```