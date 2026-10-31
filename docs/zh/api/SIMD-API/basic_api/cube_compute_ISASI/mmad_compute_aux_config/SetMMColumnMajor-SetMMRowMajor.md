# SetMMColumnMajor/SetMMRowMajor

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR&950DT系列产品：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3系列产品：支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2系列产品：支持
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2推理产品：支持
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas推理系列产品AI Core：不支持
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas推理系列产品Vector Core：不支持
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas训练系列产品：不支持
<!-- end id7 -->
<!-- @ref: asc-devkit/res/docs/zh/api/SIMD-API/basic_api/cube_compute_ISASI/mmad_compute_aux_config/SetMMColumnMajor-SetMMRowMajor_res.md#id1 -->

## 功能说明

控制Mmad/MmadWithSparse优先通过M/N的哪个方向。

## 函数原型

```cpp
__aicore__ inline void SetMMColumnMajor() // 代表CUBE将首先通过M方向，然后通过N方向生成结果
__aicore__ inline void SetMMRowMajor()   // 代表CUBE将首先通过N方向，然后通过M方向产生结果
```

## 参数说明

无

## 返回值说明

无

## 约束说明

无

## 调用示例

```cpp
AscendC::SetMMRowMajor();
```

## 参见

| 接口 | 描述 |
| --- | --- |
| [Mmad](../mmad_compute/Mmad.md) | Mmad接口是Ascend C面向昇腾AI芯片的矩阵乘加核心计算接口，专为高性能算子开发设计，封装了昇腾NPU硬件的矩阵乘加计算能力，广泛用于神经网络层（如全连接层、卷积层）、数值计算类算子的开发。 |
| [MmadWithSparse](../mmad_compute/MmadWithSparse.md) | MmadWithSparse接口负责完成特殊稀疏矩阵乘加操作。 |
