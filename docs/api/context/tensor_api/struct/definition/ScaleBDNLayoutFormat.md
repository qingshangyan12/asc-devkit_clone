# ScaleBDNLayoutFormat

## 功能说明

ScaleBDNLayoutFormat用于定义ScaleBDN格式的布局，ScaleBDN格式是一种支持缩放和BDN格式的布局。

## 结构体定义

```cpp
template <typename T>
struct ScaleBDNLayoutFormat {
    static constexpr size_t kInnerRow = 1;
    static constexpr size_t kInnerCol = 1;
};
```

## 字段说明

| 字段名 | 类型 | 描述 |
|--------|------|------|
| kInnerRow | size_t | 内层矩阵的行数，固定为1 |
| kInnerCol | size_t | 内层矩阵的列数，固定为1 |

## 约束束说明

- T必须是有效的数据类型，如half、float、int32_t等
- ScaleBDN格式不使用分块存储

## 调用示例

```cpp
// 创建ScaleBDN格式Layout
using T = half;
size_t mLength = 128;
size_t nLength = 64;

auto layout = AscendC::MakeScaleBDNLayout<T>(mLength, nLength);
```