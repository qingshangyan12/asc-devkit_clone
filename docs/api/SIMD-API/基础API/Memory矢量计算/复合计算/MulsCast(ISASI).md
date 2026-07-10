# MulsCast\(ISASI\)<a name="ZH-CN_TOPIC_0000002158325172"></a>

## 产品支持情况<a name="section1550532418810"></a>

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

将矢量源操作数前count个数据与标量相乘再按照CAST\_ROUND模式转换成half类型， 并将计算结果写入dst，此接口支持标量在前和标量在后两种场景。计算公式如下。

![](../../../../figures/zh-cn_formulaimage_0000002339181076.png)

其中标量输入也支持配置LocalTensor单点元素，计算公式如下，idx表示LocalTensor单点元素的位置系数。

![](../../../../figures/zh-cn_formulaimage_0000002339021404.png)

## 函数原型<a name="section620mcpsimp"></a>

```
template <typename T0 = BinaryDefaultType, typename T1 = BinaryDefaultType, const BinaryConfig &config = DEFAULT_BINARY_CONFIG, typename T2, typename T3, typename T4>
__aicore__ inline void MulsCast(const T2 &dst, const T3 &src0, const T4 &src1, const uint32_t count)
```

## 参数说明<a name="section622mcpsimp"></a>

**表1**  模板参数说明

<a name="table4835205712588"></a>
- T0：不支持
- T1：不支持
- config：不支持
- T2：不支持
- T3：不支持
- T4：不支持

**表2**  参数说明

<a name="zh-cn_topic_0235751031_table33761356"></a>
- dst：不支持
- src0/src1：不支持
- count：不支持

**表3**  精度转换规则

<a name="table235404962912"></a>
- float：不支持

## 返回值说明<a name="section640mcpsimp"></a>

无

## 约束说明<a name="section633mcpsimp"></a>

-   左操作数及右操作数中，必须有一个为矢量；当前不支持左右操作数同时为标量。
-   本接口传入LocalTensor单点数据作为标量时，idx参数需要传入编译期已知的常量，传入变量时需要声明为constexpr。

## 调用示例<a name="section642mcpsimp"></a>

```
// 标量在后示例
AscendC::MulsCast(dstLocal, src0Local, src1Local[0], 512);

// 标量在前示例
static constexpr AscendC::BinaryConfig config = { 0 };
AscendC::MulsCast<BinaryDefaultType, BinaryDefaultType, config>(dstLocal, src0Local[0], src1Local, 512);
```

结果示例如下：

```
输入数据src0Local：[6 5 11 ... ]
输入数据src1Local：2
输出数据dstLocal：[12 10 22 ... ]
```
