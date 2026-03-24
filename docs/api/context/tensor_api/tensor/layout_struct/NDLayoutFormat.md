# NDLayoutFormat

## 产品支持情况

| 产品     | 是否支持 |
| ----------- |:----:|
|Ascend 950PR/Ascend 950DT|√|
|Atlas A3 训练系列产品/Atlas A3 推理系列产品|x|
|Atlas A2 训练系列产品/Atlas A2 推理系列产品|x|
|Atlas 200I/500 A2 推理产品|x|
|Atlas 推理系列产品AI Core|x|
|Atlas 推理系列产品Vector Core|x|
|Atlas 训练系列产品|x|
|Atlas 200/300/500 推理产品|x|

## 功能说明

NDLayoutFormat用于定义ND格式的布局，ND格式是标准的行主序存储格式，数据按行连续存储。

## 结构体定义

```cpp
template <typename T>
struct NDLayoutFormat {
    template <size_t row, size_t column>
    using type = NDFormatLayout<T, row, column>;

    template <typename U, typename S>
    __aicore__ inline decltype(auto) operator()(U row, S column) {
        return MakeNDLayout<T, U, S>(row, column);
    }  
};
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
|--------|-----------|------|
| `T` | 输入 | 数据类型模板参数。<br>AscendC 950PR/AscendC 950DT 支持的数据类型为：int8_t、uint8_t、int16_t、uint16_t、half、bfloat16_t、int32_t、uint32_t、float、complex32、int64_t、uint64_t |
| `row` | 输入 | 矩阵的总行数 |
| `column` | 输入 | 矩阵的总列数 |

## 返回值

- 输入为编译时常量时，返回DN格式的Layout类型。
- 输入为整型变量时，返回DN格式的Layout对象。

## 约束说明

- 参数row和column需为整型。

## 调用示例

```cpp
// 创建ND格式Layout
// 编译时常量
using MyNDLayout = NDLayoutFormat<half>::type<Std::Int<32>, Std::Int<32>>;
constexpr MyNDLayout staticLayout1;
auto staticLayout2 = MyNDLayout{};

// 运行时值
NDLayoutFormat<half> ndFormat;
auto layout = ndFormat(32, 32);
```