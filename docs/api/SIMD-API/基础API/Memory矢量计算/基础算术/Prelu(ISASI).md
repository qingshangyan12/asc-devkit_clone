# Prelu\(ISASI\)<a name="ZH-CN_TOPIC_0000002193806085"></a>

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：不支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：不支持
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


## 功能说明<a name="section618mcpsimp"></a>

源操作数src0大于0的情况下直接将src0写入目的操作数dst，否则将源操作数src0 \* src1的结果写入dst。计算公式如下：

![](../../../../figures/zh-cn_formulaimage_0000002391634445.png)

## 函数原型<a name="section620mcpsimp"></a>

```cpp
template <typename T>
__aicore__ inline void Prelu(const LocalTensor<T>& dst, const LocalTensor<T>& src0, const LocalTensor<T>& src1, const uint32_t count)
```

## 参数说明<a name="section622mcpsimp"></a>

**表1** 模板参数说明


<a name="table4835205712588"></a>
| 参数名 | 描述 |
|---|---|
| T | 操作数数据类型。<br>Ascend 950PR/Ascend 950DT，支持的数据类型为：half、float。 |


**表2** 参数说明


<a name="zh-cn_topic_0235751031_table33761356"></a>
| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| dst | 输出 | 目的操作数。<br>类型为[LocalTensor](../../数据结构/LocalTensor和GlobalTensor定义/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。<br>LocalTensor的起始地址需要32字节对齐。 |
| src0、src1 | 输入 | 源操作数。<br>类型为[LocalTensor](../../数据结构/LocalTensor和GlobalTensor定义/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。<br>LocalTensor的起始地址需要32字节对齐。<br>两个源操作数的数据类型需要与目的操作数保持一致。 |
| count | 输入 | 参与计算的元素个数。 |


## 返回值说明<a name="section640mcpsimp"></a>

无

## 约束说明<a name="section633mcpsimp"></a>

-   操作数地址对齐要求请参见[通用地址对齐约束](../../../通用说明和约束.md#section796754519912)。

## 调用示例<a name="section642mcpsimp"></a>

更多样例可参考[LINK](更多样例-9.md)。

```cpp
AscendC::Prelu(dstLocal, src0Local, src1Local, 512);
```

结果示例如下：

```bash
输入数据src0Local：[1 -2 -3 ... -512]
输入数据src1Local：[513 -514 515 ... 1]
输出数据dstLocal：[1 1024 -1045 ... -512]
```
