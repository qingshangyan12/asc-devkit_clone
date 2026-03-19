# MakeStride

## 产品支持情况

| 产品     | 是否支持 |
| ----------- |:----:|
| Atlas A3 训练系列产品/Atlas A3 推理系列产品 | √    |
| Atlas A2 �训练系列产品/Atlas A2 推理系列产品 | √    |

## 功能说明

构造Stride对象，用于定义张量的步长。

## 函数原型

```cpp
template <typename... Ts>
__aicore__ inline constexpr Stride<Ts...> MakeStride(const Ts&... t)
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
|--------|-----------|------|
| t | 输入 | 各维度的步长，可变参数 |

## 返回值说明

返回Stride<Ts...>对象。

## 约束说明

- 参数数量必须与对应的Shape维度数量一致
- 各参数必须为正整数
- 支持的数据类型包括：size_t、int等整数类型

## 调用示例

```cpp
// 创建一个3维张量的Stride
auto stride = AscendC::MakeStride(1, 100, 200);

// 获取各维度的步长
auto stride0 = AscendC::Std::get<0>(stride.value); // stride0 = 1
auto stride1 = AscendC::Std::get<1>(stride.value); // stride1 = 100
auto stride2 = AscendC::Std::get<2>(stride.value); // stride2 = 200
```