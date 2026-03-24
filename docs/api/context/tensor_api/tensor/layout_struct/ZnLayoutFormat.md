# ZnLayoutFormat

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

ZnLayoutFormat用于定义Zn格式的布局，Zn格式是NZ格式的转置版本，外层矩阵按列优先存储，内层矩阵按行优先存储。

## 结构体定义

```cpp
template <typename T>
struct ZnLayoutFormat {
    template <size_t row, size_t column>
    using type = ZNFormatLayout<T, row, column>;

    template <typename U, typename S>
    __aicore__ inline decltype(auto) operator()(U row, S column) {
        return MakeZnLayout<T, U, S>(row, column);
    }  
};
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
|--------|-----------|------|
| `T` | 输入 | 数据类型模板参数。<br>AscendC 950PR/AscendC 950DT 支持的数据类型为：fp4x2_e2m1_t、fp4x2_e1m2_t、int8_t、uint8_t、int16_t、uint16_t、half、bfloat16_t、int32_t、uint32_t、float、complex32、int64_t、uint64_t |
| `row` | 输入 | 矩阵的总行数 |
| `column` | 输入 | 矩阵的总列数 |

## 返回值

- 输入为编译时常量时，返回Zn格式的Layout类型。
- 输入为整型变量时，返回Zn格式的Layout对象。
- 对于T数据类型为fp4x2_e2m1_t、fp4x2_e1m2_t时，NzLayout的小分形行数为64，列数为16。
- **自动对齐**: 如果输入的行数不是 64 的倍数，列数不是16的倍数，返回的Layout会向上取整。例如：
  - 对于T为half类型，输入 `row = 17, column = 33` 会自动对齐为 `row = 64, column = 48`
- 对于T数据类型为：int8_t、uint8_t、int16_t、uint16_t、half、bfloat16_t、int32_t、uint32_t、float、complex32、int64_t、uint64_t时，NzLayout的小分形行数为32B/sizeof(T) ，列数为16。
- **自动对齐**: 如果输入的行数不是 32B/sizeof(T) 的倍数，列数不是16的倍数，返回的Layout会向上取整。例如：
  - 对于T为half类型，输入 `row = 17, column = 33` 会自动对齐为 `row = 32, column = 48`

## 约束说明

- 参数row和column需为整型。

## 调用示例

```cpp
// 创建Zn格式Layout
// 编译时常量
using MyZnLayout = ZnLayoutFormat<half>::type<Std::Int<32>, Std::Int<32>>;
constexpr MyZnLayout staticLayout1;
auto staticLayout2 = MyZnLayout{};

// 运行时值
ZnLayoutFormat<half> znFormat;
auto layout = znFormat(32, 32);
```