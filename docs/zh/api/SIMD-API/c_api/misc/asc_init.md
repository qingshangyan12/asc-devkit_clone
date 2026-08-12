# asc_init

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：支持
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2 推理产品：不支持
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas 推理系列产品AI Core：不支持
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas 推理系列产品Vector Core：不支持
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas 训练系列产品：不支持
<!-- end id7 -->

## 功能说明

本接口对AI Core的全局状态进行初始化，包括AIC（Cube Core）与AIV（Vector Core）的公共状态及各自特有状态。由于不同产品的实现存在差异，实际执行的初始化项也有所不同。

**各产品实际执行的初始化项如下：**

<!-- npu="950" id8 -->
**表1** 针对Ascend 950PR/Ascend 950DT本接口执行的初始化项

| 状态类别 | 初始化内容 |
| ---- | ---- |
| 公共状态 | 原子累加状态 |
| 公共状态 | Mask工作模式 |
| 公共状态 | CTRL寄存器 |
| 公共状态 | 原子操作开启位与原子操作类型 |
| AIC | asc_copy_l12l0a（3D格式搬运）接口调用时Pad填充的数值 |
| AIV | Mask |
| AIV | loop mode的参数 |
<!-- end id8 -->

<!-- npu="A3, 910b" id9 -->
**表2** 针对Atlas A3 训练系列产品/Atlas A3 推理系列产品本接口执行的初始化项

| 状态类别 | 初始化内容 |
| ---- | ---- |
| 公共状态 | 原子累加状态 |
| 公共状态 | Mask工作模式 |
| AIV | Mask |
<!-- end id9 -->


## 函数原型

```cpp
__aicore__ inline void asc_init()
```

## 参数说明

无

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

- 在实际运行中，这些值可能被前序执行的算子修改，若不调用该接口进行初始化，非预期的值可能导致计算结果出现精度错误。

  例如前序算子使用Counter模式但未重置为Normal模式，当前算子以默认的Normal模式设置Mask时，会导致Mask设置不符合预期，进而引发精度错误。

## 调用示例

```cpp
__global__ __vector__ void add_kernel(__gm__ float* dst, __gm__ float* src0, __gm__ float* src1)
{
    asc_init();
    // 执行其它操作
}
```