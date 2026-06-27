# Interleave<a name="ZH-CN_TOPIC_0000002166544241"></a>

## 产品支持情况<a name="section1550532418810"></a>

| 产品 | 是否支持 |
| --- | --- |
| Ascend 950PR/Ascend 950DT | √ |
| Atlas A3 训练系列产品/Atlas A3 推理系列产品 | x |
| Atlas A2 训练系列产品/Atlas A2 推理系列产品 | x |
| Atlas 200I/500 A2 推理产品 | x |
| Atlas 推理系列产品AI Core | x |
| Atlas 推理系列产品Vector Core | x |
| Atlas 训练系列产品 | x |

## 功能说明<a name="section618mcpsimp"></a>

头文件路径为：`"basic_api/kernel_operator_vec_duplicate_intf.h"`。

给定源操作数src0和src1，将src0和src1中的元素交织存入结果操作数dst0和dst1中。交织排列方式如下图所示，其中每个方格代表一个元素。

![](../../../../figures/interleave1.png)

## 函数原型<a name="section620mcpsimp"></a>

```
template <typename T>
__aicore__ inline void Interleave(const LocalTensor<T>& dst0, const LocalTensor<T>& dst1, const LocalTensor<T>& src0, const LocalTensor<T>& src1, const int32_t count)
```

## 参数说明<a name="section176711403104"></a>

**表1**  模板参数说明

| 参数名 | 描述 |
| --- | --- |
| T | 操作数数据类型。 |

**表2**  参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| dst0/dst1 | 输出 | 目的操作数。类型为[LocalTensor](../../数据结构/LocalTensor和GlobalTensor定义/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。LocalTensor的起始地址需要32字节对齐。 |
| src0/src1 | 输入 | 源操作数。类型为[LocalTensor](../../数据结构/LocalTensor和GlobalTensor定义/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。LocalTensor的起始地址需要32字节对齐。源操作数的数据类型需要与目的操作数保持一致。 |
| count | 输入 | 输入/输出数据元素个数，dst0/dst1/src0/src1长度大小均为count。count必须为偶数。 |

## 数据类型

<!-- npu="950" id10 -->
- 针对Ascend 950PR/Ascend 950DT，T支持的数据类型为：uint8_t、int8_t、uint16_t、int16_t、half、bfloat16_t、uint32_t、int32_t、float、uint64_t、int64_t。源操作数的数据类型需要与目的操作数保持一致。
<!-- end id10 -->

## 返回值说明<a name="section14483414194"></a>

无

## 约束说明<a name="section633mcpsimp"></a>

-   **不支持源操作数与目的操作数地址重叠。**

## 调用示例<a name="section176061616102911"></a>

本样例中只展示Compute流程中的部分代码。

```
AscendC::Interleave(dst0Local, dst1Local, src0Local, src1Local, 512);
```

结果示例如下：

```
输入数据src0Local：[1 2 3 ... 512]
输入数据src1Local：[513 514 515 ... 1024]
输出数据dst0Local：[1 513 2 ... 768]
输出数据dst1Local：[257 769 258 ... 1024]
```
