# MakeLayout

## 产品支持情况

| 产品     | 是否支持 |
| ----------- |:----:|
| Atlas A3 训练系列产品/Atlas A3 推理系列产品 | √    |
| Atlas A2 训练系列产品/Atlas A2 推理系列产品 | √    |

## 功能说明

构造Layout对象，用于定义张量的布局。支持两种形式：带步长的布局和默认步长的布局。

## 函数原型

```cpp
// 带步长的Layout
template <typename T, typename U>
__aicore__ inline constexpr auto MakeLayout(const T& shape, const U& stride)

// 默认步长的Layout
template <typename T>
__aicore__ inline constexpr auto MakeLayout(const T& shape)
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
|--------|-----------|------|
| shape | 输入 | 张量的形状，Shape类型 |
| stride | 输入 | 张量的步长，Stride类型（可选） |

## 返回值说明

返回Layout对象。

## 约束说明

- Shape和Stride的维度数量必须一致
- Layout支持最多4个维度的数据配置
- Shape、Stride等数值数据，仅支持size_t类型和int类型
- 当不提供stride时，会自动计算默认步长（行主序）

## 调用示例

```cpp
// 方式1：带步长的Layout
auto shape = AscendC::MakeShape(10, 20, 30);
auto stride = AscendC::MakeStride(1, 100, 200);
auto layout = AscendC::MakeLayout(shape, stride);

// 方式2：默认步长的Layout
auto shape = AscendC::MakeShape(10, 20, 30);
auto layout = AscendC::MakeLayout(shape); // 自动计算步长

// 获取Shape和Stride
auto layoutShape = layout.shape;
auto layoutStride = layout.stride;
```