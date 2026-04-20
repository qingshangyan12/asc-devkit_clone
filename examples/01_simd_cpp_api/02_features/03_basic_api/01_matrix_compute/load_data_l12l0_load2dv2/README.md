# load_data_l12l0_load2dv2样例

## 概述

本样例介绍了输入为ND格式，B8 / B16 / B32输入数据类型（具体以int8_t / half / float为例），左、右矩阵转置、不转置组合共 7 种矩阵乘法场景中使用 `Load2DV2` 接口将矩阵数据从 L1 搬运到 L0A/L0B 的方法。
- 使用 `LoadData2DParamsV2` 参数结构体
- L0A 数据排布为 **Nz 格式**
- L0B 数据排布为 **Zn 格式**
- 支持转置功能，通过 `ifTranspose` 参数控制
- 支持 int8_t、half、float 三种数据类型

## 支持的产品
- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构

```
├── load_data_l12l0_load2dv2
│   ├── img                         // 图示
│   ├── scripts
│   │   ├── gen_data.py             // 输入数据和真值数据生成脚本
│   │   └── verify_result.py        // 验证输出数据和真值数据是否一致的验证脚本
│   ├── CMakeLists.txt              // 编译工程文件
│   ├── data_utils.h                // 数据读入写出函数
│   └── load_data_l12l0_load2dv2.asc         // Ascend C样例实现 & 调用样例
```

## 样例描述

### 样例功能

样例整体流程如下：

```
GM(ND) -> L1(Nz) -> L0A(Nz)/L0B(Zn) -> L0C(Nz) -> GM(ND)
   │         │              │              │         │
 DataCopy  Load2DV2       Mmad        Fixpipe    输出
```

**步骤详解**：

1. **GM → L1**：使用 `DataCopy` 实现 ND 到 Nz 格式转换
2. **L1 → L0A/L0B**：使用 `Load2DV2` 接口搬运，通过 `ifTranspose` 控制转置
3. **矩阵乘**：使用 `Mmad` 接口执行矩阵乘法
4. **L0C → GM**：使用 `Fixpipe` 接口搬出结果


所有场景基于相同的矩阵乘规格：[m, n, k] = [40, 50, 70]，核函数名称为"KernelLoadDataL12L0Load2Dv2"。以参数scenarioNum来代表上述7种场景，scenarioNum不同取值对应的含义如下，如下[表1](#表1)所示：<br>

<a name="表1"></a>
<table border="2">
<caption style="font-weight: normal;">
    <span style="font-weight: bold; font-size: 1.2em;">📌 表1：scenarioNum不同取值的含义</span>
  <tr>
    <td ><span style="font-weight: bold;">scenarioNum</span></td>
    <td><span style="font-weight: bold;">输入数据类型</span></td>
    <td><span style="font-weight: bold;">输出数据类型</span></td>
    <td><span style="font-weight: bold;">isAtranspose</span></td>
    <td><span style="font-weight: bold;">isBtranspose</span></td>
    <td><span style="font-weight: bold;">是否多搬多算</span></td>
    <td><span style="font-weight: bold;">L1->L0调用接口方式</span></td>
  </tr>
  <tr>
    <td><span style="font-weight: bold;">1</span></td>
    <td rowspan="2" >int8_t</td>
    <td rowspan="2" >int32_t</td>
    <td>false</td>
    <td>true</td>
    <td>否</td>
    <td>Load2DV2</td>
  </tr>
  <tr>
    <td><span style="font-weight: bold;">2</span></td>
    <td>true</td>
    <td>false</td>
    <td>是</td>
    <td>Load2DV2</td>
  </tr>
  <tr>
    <td><span style="font-weight: bold;">3</span></td>
    <td rowspan="2" >half</td>
    <td rowspan="2" >float</td>
    <td>false</td>
    <td>true</td>
    <td>否</td>
    <td>Load2DV2</td>
  </tr>
  <tr>
    <td><span style="font-weight: bold;">4</span></td>
    <td>true</td>
    <td>false</td>
    <td>否</td>
    <td>Load2DV2</td>
  </tr>
  <tr>
    <td><span style="font-weight: bold;">5</span></td>
    <td rowspan="2" >float</td>
    <td rowspan="2" >float</td>
    <td>false</td>
    <td>true</td>
    <td>否</td>
    <td>Load2DV2</td>
  </tr>
  <tr>
    <td><span style="font-weight: bold;">6</span></td>
    <td>true</td>
    <td>false</td>
    <td>否</td>
    <td>Load2DV2</td>
  </tr>
  <tr>
    <td><span style="font-weight: bold;">7</span></td>
    <td rowspan="1" >int8_t</td>
    <td rowspan="1" >int32_t</td>
    <td>true</td>
    <td>false</td>
    <td>否</td>
    <td>for循环+Load2DV2</td>
  </tr>
</table>

注：scenarioNum取值7时，L1->L0A的搬运采用for循环+Load2DV2, 其余场景均调用一次Load2dV2。


**场景1：输入int8_t数据类型，isAtranspose=False，isBtranspose=True**
- 输入A [40, 70]，int8_t类型，ND格式; B [50, 70]，int8_t类型，ND格式;
- 输出C [40, 50]，int32_t类型，ND格式；
- 实现：使用一次Load2DV2将A矩阵从L1搬运到L0A，使用一次Load2DV2将B矩阵从L1搬运到L0B;
- 说明：L1->L0A不需要转置，通过配置mStep、kStep、srcStride、srcStride等参数完成A矩阵L1 -> L0A的搬运；同理，L1->L0B不需要转置，B矩阵通过配置Step、kStep、srcStride、srcStride等参数完成L1 -> L0B的搬运及大分型排布格式的变化。

**场景2：输入int8_t数据类型，isAtranspose=True，isBtranspose=False**
- 输入A [70, 40]，int8_t类型，ND格式; B [70, 50]，int8_t类型，ND格式;
- 输出C [40, 50]，int32_t类型，ND格式;
- 实现：使用一次Load2DV2将A矩阵从L1搬运到L0A，使用一次Load2DV2将B矩阵从L1搬运到L0B;
- 说明：A矩阵调用一次Load2DV2指令，通过配置mStep、kStep、srcStride、srcStride，并配置ifTranspose为true，完成A矩阵从L1到L0A的搬运，搬运过程中会伴随转置；B矩阵调用一次Load2DV2指令，配置mStep、kStep、srcStride、srcStride，并配置ifTranspose为true，完成B矩阵从L1到L0B的搬运，搬运过程中会伴随转置。该场景m方向多搬多算脏数据分型，在L0C搬出到GM时，通过fixpipeParam.mSize = m来保证无效数据参与计算的结果不会被搬出。

**场景3：输入half数据类型，isAtranspose=False，isBtranspose=True**
- 输入A [40, 70]，half类型，ND格式; B [50, 70]，half类型，ND格式;
- 输出C [40, 50]，float类型，ND格式；
- 实现：使用一次Load2DV2将A矩阵从L1搬运到L0A，使用一次Load2DV2将B矩阵从L1搬运到L0B;
- 说明：L1->L0A不需要转置，通过配置mStep、kStep、srcStride、srcStride等参数完成A矩阵L1 -> L0A的搬运；同理，L1->L0B不需要转置，B矩阵通过配置Step、kStep、srcStride、srcStride等参数完成L1 -> L0B的搬运及大分型排布格式的变化。

**场景4：输入half数据类型，isAtranspose=True，isBtranspose=False**
- 输入A [70, 40]，half类型，ND格式; B [70, 50]，half类型，ND格式;
- 输出C [40, 50]，float类型，ND格式；
- 实现：使用一次Load2DV2将A矩阵从L1搬运到L0A，使用一次Load2DV2将B矩阵从L1搬运到L0B;
- 说明：A矩阵调用一次Load2DV2指令，通过配置mStep、kStep、srcStride、srcStride，并配置ifTranspose为true，完成A矩阵从L1到L0A的搬运，搬运过程中会伴随转置；B矩阵调用一次Load2DV2指令，配置mStep、kStep、srcStride、srcStride，并配置ifTranspose为true，完成B矩阵从L1到L0B的搬运，搬运过程中会伴随转置。

**场景5：输入float数据类型，isAtranspose=False，isBtranspose=True**
- 输入A [40, 70]，float类型，ND格式; B [50, 70]，float类型，ND格式;
- 输出C [40, 50]，float类型，ND格式；
- 实现：使用一次Load2DV2将A矩阵从L1搬运到L0A，使用一次Load2DV2将B矩阵从L1搬运到L0B;
- 说明：L1->L0A不需要转置，通过配置mStep、kStep、srcStride、srcStride等参数完成A矩阵L1 -> L0A的搬运；同理，L1->L0B不需要转置，B矩阵通过配置Step、kStep、srcStride、srcStride等参数完成L1 -> L0B的搬运及大分型排布格式的变化。

**场景6：输入float数据类型，isAtranspose=True，isBtranspose=False**
- 输入A [70, 40]，float类型，ND格式; B [70, 50]，float类型，ND格式;
- 输出C [40, 50]，float类型，ND格式；
- 实现：使用一次Load2DV2将A矩阵从L1搬运到L0A，使用一次Load2DV2将B矩阵从L1搬运到L0B;
- 说明：A矩阵调用一次Load2DV2指令，通过配置mStep、kStep、srcStride、srcStride，并配置ifTranspose为true，完成A矩阵从L1到L0A的搬运，搬运过程中会伴随转置；B矩阵调用一次Load2DV2指令，配置mStep、kStep、srcStride、srcStride，并配置ifTranspose为true，完成B矩阵从L1到L0B的搬运，搬运过程中会伴随转置。

**场景7：输入int8_t数据类型，isAtranspose=True，isBtranspose=False**
- 输入A [70, 40]，int8_t类型，ND格式; B [70, 50]，int8_t类型，ND格式;
- 输出C [40, 50]，int32_t类型，ND格式;
- 实现：使用for循环+Load2DV2将A矩阵从L1搬运到L0A，使用一次Load2DV2将B矩阵从L1搬运到L0B;
- 说明：A矩阵沿m方向for循环，调用多次Load2DV2指令，一次从L1上搬运m轴方向上的2个分型 * k轴方向上的CeilDivision(k, fractalShape[1])个分型，通配置mStep、kStep、srcStride、srcStride，并配置ifTranspose为true，完成A矩阵从L1到L0A的搬运，搬运过程中会伴随转置；B矩阵调用一次Load2DV2指令，配置mStep、kStep、srcStride、srcStride，并配置ifTranspose为true，完成B矩阵从L1到L0B的搬运，搬运过程中会伴随转置。该场景通过for循环配合dstStride参数在写入L0A时跳过转置时多读的m方向脏数据分型，使矩阵计算m方向没有多余脏数据分型参与计算。

为了方便描述，在此对后续常用概念给出定义：
 
（1）fractalShape: 小分型的shape为[16, 32 / sizeof(T)]，其中T表示输入数据类型。本样例涉及到数据类型的分型相关信息如[表2](#表2)。

（2）fractalSize: 1个小分型包含的元素个数，具体可参考[表2](#表2)。


（3）fractalNum: 当从L1 -> L0A/L0B需要转置时，Load2DV2接口内部会按照小方块矩阵转置，对于B8和B32数据类型fractalShape分别为[16,32]和[16,8]，都需要两个连续的小分型合并为一个方块然后转置，因此该参数表示一个方块包含几个小分型，具体可参考[表2](#表2)。


<a name="表2"></a>
<table border="2">
<caption style="font-weight: normal;">
    <span style="font-weight: bold; font-size: 1.2em;">📌 表2：不同数据类型分型相关信息</span></caption>
  <tr>
    <td></td>
    <td align="center"><span style="font-weight: bold;">fractalShape</span></td>
    <td align="center"><span style="font-weight: bold;">fractalSize</span></td>
    <td align="center"><span style="font-weight: bold;">fractalNum</span></td>
  </tr>
    <tr>
    <td align="center"><span style="font-weight: bold;">B8</span></td>
    <td align="center">[16, 32]</td>
    <td align="center">512B</td>
    <td align="center">2</td>
  </tr>
    <tr>
    <td align="center"><span style="font-weight: bold;">B16</span></td>
    <td align="center">[16, 16]</td>
    <td align="center">256B</td>
    <td align="center">1</td>
  </tr>
    <tr>
    <td align="center"><span style="font-weight: bold;">B32</span></td>
    <td align="center">[16, 8]</td>
    <td align="center">128B</td>
    <td align="center">2</td>
  </tr>
</table>


（4）CeilAlign：向上对齐操作，例如m=30时，CeilAlign(30, 16)=32，表示将m轴向16对齐，对齐后m轴长度为32。



      __aicore__ inline uint16_t CeilAlign(uint16_t size, uint16_t alignValue) {
          return (size + alignValue - 1) / alignValue * alignValue;
      }

（5）CeilDivision：向上取整除法，一般用于求解向上对齐后的循环次数。

（6）mAlignValue：m轴向mAlignValue对齐，例如mAlignValue=32，代表m轴对齐到32，依次类推还有nAlignValuem表示轴向nAlignValue对齐，kaAlignValue表示A矩阵k轴向kaAlignValue对齐，kbAlignValue表示B矩阵k轴向kbAlignValue对齐。

（7）mAlignL0和mAlignL1：A矩阵分别在L1和L0A上时，m轴对齐后的值。依次类推还有nAlignL0、nAlignL1、kaAlignL0、kaAlignL1、kbAlignL0、kbAlignL1。


另外，A、B矩阵在L1和L0上在row和col方向上对齐的要求也不相同，现将[表2](#表2)中scenarioNum取1-6对应的6种场景(L1上的排布格式为Nz)的对齐要求总结得到
如下[表3](#表3)、[表4](#表4)：

<a name="表3"></a>
<table border="2">
<caption style="font-weight: normal;">
    <span style="font-weight: bold; font-size: 1.2em;">📌 表3：A、B矩阵在L1上各个轴对齐要求(L1排布格式为Nz)</span></caption>
  <tr>
    <td></td>
    <td align="center"><span style="font-weight: bold;">B8 (fractalNum=2)</span></td>
    <td align="center"><span style="font-weight: bold;">B16 (fractalNum=1)</span></td>
    <td align="center"><span style="font-weight: bold;">B32 (fractalNum=2)</span></td>
  </tr>
  <tr>
    <td rowspan="2" align="center"><span style="font-weight: bold;">A矩阵不转置输入[m, k]</span></td>
    <td colspan="3" align="center">mAlignValue = fractalShape[0]</td>
  </tr>
  <tr>
    <td colspan="3" align="center" >kaAlignValue = fractalShape[1]</td>
  </tr>
  <tr>
    <td rowspan="2" align="center"><span style="font-weight: bold;">A矩阵转置输入[k, m]</span></td>
    <td colspan="2" align="center">kaAlignValue = fractalShape[0] * fractalNum</td>
    <td colspan="1" align="center">kaAlignValue = fractalShape[0]</td>
  </tr>
  <tr>
    <td colspan="2" align="center" >mAlignValue = fractalShape[1]</td>
    <td colspan="1" align="center" >mAlignValue = fractalShape[1] * fractalNum</td>
  </tr>
    <tr>
    <td rowspan="2" align="center"><span style="font-weight: bold;">B矩阵不转置输入[k, n]</span></td>
    <td colspan="2" align="center">kbAlignValue = fractalShape[0] * fractalNum</td>
    <td colspan="1" align="center">kbAlignValue = fractalShape[0]</td>
  </tr>
  <tr>
    <td colspan="2" align="center" >nAlignValue = fractalShape[1]</td>
    <td colspan="1" align="center" >nAlignValue = fractalShape[1] * fractalNum</td>
  </tr>
 <tr>
    <td rowspan="2" align="center"><span style="font-weight: bold;">B矩阵转置输入[n, k]</span></td>
    <td colspan="3" align="center">nAlignValue = fractalShape[0]</td>
  </tr>
  <tr>
    <td colspan="3" align="center" >kbAlignValue = fractalShape[1]</td>
  </tr>
</table>


<a name="表4"></a>
<table border="2">
<caption style="font-weight: normal;">
    <span style="font-weight: bold; font-size: 1.2em;">📌 表4：A、B矩阵在L0上各个轴对齐要求</span></caption>
  <tr>
    <td></td>
    <td align="center"><span style="font-weight: bold;">B8 (fractalNum=2)</span></td>
    <td align="center"><span style="font-weight: bold;">B16 (fractalNum=1)</span></td>
    <td align="center"><span style="font-weight: bold;">B32 (fractalNum=2)</span></td>
  </tr>
  <tr>
    <td rowspan="2" align="center"><span style="font-weight: bold;">A矩阵不转置输入[m, k]，L1->L0A不需要转置</span></td>
    <td colspan="3" align="center">mAlignValue = fractalShape[0]</td>
  </tr>
  <tr>
    <td colspan="3" align="center" >kaAlignValue = fractalShape[1]</td>
  </tr>
  <tr>
    <td rowspan="2" align="center"><span style="font-weight: bold;">A矩阵转置输入[k, m]，L1->L0A需要转置</span></td>
    <td colspan="2" align="center">kaAlignValue = fractalShape[1]</td>
    <td >kaAlignValue = fractalShape[1] * fractalNum</td>
  </tr>
  <tr>
    <td colspan="2" align="center" >mAlignValue = fractalShape[0] * fractalNum</td>
    <td align="center" >mAlignValue = fractalShape[0]</td>
  </tr>
    <tr>
    <td rowspan="2" align="center"><span style="font-weight: bold;">B矩阵不转置输入[k, n]，L1->L0B需要转置</span></td>
    <td colspan="2" align="center">kbAlignValue = fractalShape[1]</td>
      <td align="center">kbAlignValue = fractalShape[1] * fractalNum</td>
  </tr>
  <tr>
    <td colspan="2" align="center">nAlignValue = fractalShape[0] * fractalNum</td>
    <td align="center" >nAlignValue = fractalShape[0]</td>
  </tr>
 <tr>
    <td rowspan="2" align="center"><span style="font-weight: bold;">B矩阵转置输入[n, k]，L1->L0B不需要转置</span></td>
    <td colspan="3" align="center">nAlignValue = fractalShape[0]</td>
  </tr>
  <tr>
    <td colspan="3" align="center" >kbAlignValue = fractalShape[1]</td>
  </tr>
</table>


特别的，当scenarioNum=7时，A矩阵用for循环+Load2DV2实现L1->L0A的搬运，L0A仅写入有效数据向分型对齐后的数据，A、B矩阵在L1和L0上在高度和宽度方向上对齐的要求如[表5](#表5)、[表6](#表6)：
<a name="表5"></a>
<table border="2">
<caption style="font-weight: normal;">
    <span style="font-weight: bold; font-size: 1.2em;">📌 表5：scenarioNum=7，A、B矩阵在L1上各个轴对齐要求</span></caption>
  <tr>
    <td align="center" ></td>
    <td align="center" ><span style="font-weight: bold;">int8_t (fractalNum=2)</span></td>
  </tr>
   <tr>
    <td rowspan="2"><span style="font-weight: bold;">A矩阵转置输入[k, m]</span></td>
    <td align="center" >kaAlignValue = fractalShape[0] * fractalNum</td>
  </tr>
    <tr>
    <td align="center" >mAlignValue = fractalShape[1]</td>
  </tr>
   <tr>
    <td rowspan="2"><span style="font-weight: bold;">B矩阵不转置输入[k, n]</span></td>
    <td align="center" >kbAlignValue = fractalShape[0] * fractalNum</td>
  </tr>
    <tr>
    <td align="center" >nAlignValue = fractalShape[1]</td>
  </tr>
</table>

<a name="表6"></a>
<table border="2">
<caption style="font-weight: normal;">
    <span style="font-weight: bold; font-size: 1.2em;">📌 表6：scenarioNum=7，A、B矩阵在L0上各个轴对齐要求</span></caption>
  <tr>
    <td align="center" ></td>
    <td align="center" ><span style="font-weight: bold;">int8_t (fractalNum=2)</span></td>
  </tr>
   <tr>
    <td rowspan="2"><span style="font-weight: bold;">A矩阵转置输入[k, m]，L1->L0A需要转置</span></td>
    <td align="center" >mAlignValue = fractalShape[0]</td>
  </tr>
    <tr>
    <td align="center" >kaAlignValue = fractalShape[1]</td>
  </tr>
   <tr>
    <td rowspan="2"><span style="font-weight: bold;">B矩阵不转置输入[k, n]，L1->L0B需要转置</span></td>
    <td align="center" >kbAlignValue = fractalShape[1]</td>
  </tr>
    <tr>
    <td align="center" >nAlignValue = fractalShape[0] * fractalNum</td>
  </tr>
</table>

### 样例实现

#### Load2DV2 参数说明

`LoadData2DParamsV2` 结构体参数：

| 参数 | 含义 |
|------|------|
| mStartPosition | 源矩阵 M 轴方向的起始位置，单位为 16 个元素 |
| kStartPosition | 源矩阵 K 轴方向的起始位置，单位为 32B |
| mStep | 源矩阵 M 轴方向搬运长度，单位为 16 个元素 |
| kStep | 源矩阵 K 轴方向搬运长度，单位为 32B |
| srcStride | 源矩阵 K 方向前一个分形与后一个分形起始地址的间隔，单位为 512B |
| dstStride | 目标矩阵 K 方向前一个分形与后一个分形起始地址的间隔，单位为 512B |
| ifTranspose | 是否启用转置功能（true/false） |
| sid | 预留参数，配置为 0 |

#### L1到L0 数据搬运详解

**A矩阵 L1->L0A 不转置**

当 GM 上输入 A 矩阵为 [m, k] 时，L1->L0A 仅进行大分型排布格式变换（Nz → Nz）。

**图1: LoadData2DParamsV2 参数示意图（A矩阵不转置，以 int8_t 为例）**

```
                      L1 上 A 矩阵 (Nz 格式)
         ┌────────────────────────────────────────────────┐
         │  K轴(32B为单位) →                              │
         │                                                │
         │  ┌─────┐ ┌─────┐ ┌─────┐ ┌─────┐              │
         │  │ F0  │ │ F1  │ │ F2  │ │ F3  │  ...         │ ← mStep (M轴搬运长度)
         │  │16×32│ │16×32│ │16×32│ │16×32│              │
         │  └─────┘ └─────┘ └─────┘ └─────┘              │
         │  ┌─────┐ ┌─────┐ ┌─────┐ ┌─────┐              │
         │  │ F4  │ │ F5  │ │ F6  │ │ F7  │  ...         │
         │  │16×32│ │16×32│ │16×32│ │16×32│              │
         │  └─────┘ └─────┘ └─────┘ └─────┘              │
         │                                                │
         │  kStartPosition=0                              │
         │  mStartPosition=0                              │
         │  kStep (K轴搬运长度，单位32B)                    │
         │  srcStride (源K方向分形间隔)                     │
         └────────────────────────────────────────────────┘
                         │
                         │ LoadData(a2Local, a1Local, loadDataParams)
                         │ ifTranspose = false
                         ↓
                      L0A 上 A 矩阵 (Nz 格式)
         ┌────────────────────────────────────────────────┐
         │  K轴(32B为单位) →                              │
         │                                                │
         │  ┌─────┐ ┌─────┐ ┌─────┐ ┌─────┐              │
         │  │ F0  │ │ F1  │ │ F2  │ │ F3  │  ...         │
         │  │16×32│ │16×32│ │16×32│ │16×32│              │
         │  └─────┘ └─────┘ └─────┘ └─────┘              │
         │                                                │
         │  dstStride (目标K方向分形间隔)                   │
         │                                                │
         └────────────────────────────────────────────────┘
```

**代码示例**：

```cpp
AscendC::LoadData2DParamsV2 loadDataParams;
loadDataParams.mStartPosition = 0;
loadDataParams.kStartPosition = 0;
loadDataParams.mStep = CeilDivision(m, 16);
loadDataParams.kStep = CeilDivision(k, 32);
loadDataParams.srcStride = CeilDivision(mAlignL1, 16);
loadDataParams.dstStride = CeilDivision(mAlignL0, 16);
loadDataParams.ifTranspose = false;
loadDataParams.sid = 0;
AscendC::LoadData(a2Local, a1Local, loadDataParams);
```

**A矩阵 L1->L0A 转置**

当 GM 上输入 A 矩阵为 [k, m] 时，L1->L0A 需要转置。

**图2: LoadData2DParamsV2 参数示意图（A矩阵转置，以 half 为例）**

```
                      L1 上 A 矩阵 (Nz 格式)
                输入shape: [k, m] → 转置输入
         ┌────────────────────────────────────────────────┐
         │  M轴(16元素为单位) →                           │
         │                                                │
         │  ┌─────┐ ┌─────┐ ┌─────┐ ┌─────┐              │
         │  │ F0  │ │ F1  │ │ F2  │ │ F3  │  ...         │
         │  │16×16│ │16×16│ │16×16│ │16×16│              │
         │  └─────┘ └─────┘ └─────┘ └─────┘              │
         │                                                │
         │  mStep, kStep, srcStride                       │
         └────────────────────────────────────────────────┘
                         │
                         │ ifTranspose = true (每个分形内部转置)
                         ↓
                      L0A 上 A 矩阵 (Nz 格式)
                最终shape: [m, k] 用于矩阵乘
         ┌────────────────────────────────────────────────┐
         │  K轴(16元素为单位) →                           │
         │                                                │
         │  ┌─────┐ ┌─────┐ ┌─────┐ ┌─────┐              │
         │  │ F0' │ │ F1' │ │ F2' │ │ F3' │  ...         │
         │  │转置 │ │转置 │ │转置 │ │转置 │              │
         │  └─────┘ └─────┘ └─────┘ └─────┘              │
         │                                                │
         └────────────────────────────────────────────────┘

分形内部转置:
  L1分形[16,16]:        L0A分形[16,16]:
  a0  a1  ... a15  →    a0  a16 ... a240
  a16 a17 ... a31       a1  a17 ... a241
  ...                   ...
  a240...   a255        a15 a31 ... a255
```

**代码示例**：

```cpp
loadDataParams.mStep = CeilDivision(m, 16);  // half: 单位16元素
loadDataParams.kStep = CeilDivision(k, 16);
loadDataParams.ifTranspose = true;
```

**B矩阵 L1->L0B 不转置**

当 GM 上输入 B 矩阵为 [n, k] 时，L1->L0B 不转置，进行 Nz → Zn 格式变换。

**图3: LoadData2DParamsV2 参数示意图（B矩阵不转置）**

```
                      L1 上 B 矩阵 (Nz 格式)
                输入shape: [n, k] → 转置输入
         ┌────────────────────────────────────────────────┐
         │  K轴 →                                        │
         │                                                │
         │  按 N轴分行，每行内按K轴排列分形                  │
         │  N=0:  F0 F1 F2 F3 ...                         │
         │  N=16: F4 F5 F6 F7 ...                         │
         │                                                │
         └────────────────────────────────────────────────┘
                         │
                         │ ifTranspose = false
                         │ Nz → Zn 格式变换
                         ↓
                      L0B 上 B 矩阵 (Zn 格式)
                最终shape: [k, n] 用于矩阵乘
         ┌────────────────────────────────────────────────┐
         │  N轴 →                                        │
         │                                                │
         │  按 K轴分行，每行内按N轴排列分形                  │
         │  K=0:  F0 F4 F8  F12 ...                       │
         │  K=16: F1 F5 F9  F13 ...                       │
         │                                                │
         └────────────────────────────────────────────────┘
```

**B矩阵 L1->L0B 转置**

当 GM 上输入 B 矩阵为 [k, n] 时，L1->L0B 需要转置。

**图4: LoadData2DParamsV2 参数示意图（B矩阵转置）**

```
                      L1 上 B 矩阵 (Nz 格式)
                输入shape: [k, n] → 不转置输入
         ┌────────────────────────────────────────────────┐
         │  N轴 →                                        │
         │                                                │
         │  按 K轴分行，每行内按N轴排列分形                  │
         │                                                │
         └────────────────────────────────────────────────┘
                         │
                         │ ifTranspose = true (分形内部转置)
                         ↓
                      L0B 上 B 矩阵 (Zn 格式)
         ┌────────────────────────────────────────────────┐
         │  N轴 →                                        │
         │                                                │
         │  每个分形转置后，Zn格式排布                       │
         │                                                │
         └────────────────────────────────────────────────┘
```

#### 约束条件

**转置约束**：
- 当数据类型为 b8 (int8_t) 时，mStep 必须是 2 的倍数
- 当数据类型为 b16 (half) 时，mStep 必须是 1 的倍数
- 当数据类型为 b32 (float) 时，kStep 必须是 2 的倍数

**对齐要求**：

| 输入类型 | L1上A不转置[m,k] | L1上A转置[k,m] | L1上B不转置[k,n] | L1上B转置[n,k] |
|----------|------------------|----------------|------------------|----------------|
| int8_t | m向16对齐, k向32对齐 | k向32对齐, m向32对齐 | k向32对齐, n向32对齐 | n向16对齐, k向32对齐 |
| half | m向16对齐, k向16对齐 | k向16对齐, m向16对齐 | k向16对齐, n向16对齐 | n向16对齐, k向16对齐 |
| float | m向16对齐, k向8对齐 | k向16对齐, m向16对齐 | k向16对齐, n向16对齐 | n向16对齐, k向8对齐 |

**其他约束**：
- 本样例仅适用于 dav-3510 架构（Ascend 950PR/950DT）
- 不支持 Atlas A2/A3 系列产品

## 编译运行

### 配置环境变量

请根据当前环境上 CANN 开发套件包的安装方式，选择对应配置环境变量的命令。

```bash
# 默认路径，root用户安装CANN软件包
source /usr/local/Ascend/cann/set_env.sh

# 默认路径，非root用户安装CANN软件包
source $HOME/Ascend/cann/set_env.sh

# 指定路径install_path，安装CANN软件包
source ${install_path}/cann/set_env.sh

# 使用 ks_cann 环境（支持 dav-3510 架构）
source /mnt/workspace/gitCode/cann/ks_cann/cann/set_env.sh
```

- 样例执行
  ```bash
  SCENARIO=1
  mkdir -p build && cd build;
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=$SCENARIO ..;make -j;
  python3 ../scripts/gen_data.py -scenarioNum=$SCENARIO
  ./demo
  python3 ../scripts/verify_result.py -scenarioNum=$SCENARIO output/output.bin output/golden.bin
  ```

  使用 CPU调试 或 NPU仿真 模式时，添加 `-DCMAKE_ASC_RUN_MODE=cpu` 或 `-DCMAKE_ASC_RUN_MODE=sim` 参数即可。

  示例如下：
  ```bash
  cmake -DCMAKE_ASC_RUN_MODE=cpu -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=$SCENARIO ..;make -j;   # CPU调试模式
  cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=$SCENARIO ..;make -j;   # NPU仿真模式
  ```

  > **注意：** 切换编译模式前需清理 cmake 缓存，可在 build 目录下执行 `rm CMakeCache.txt` 后重新 cmake。

- 编译选项说明

  | 选项 | 可选值 | 说明 |
  |------|--------|------|
  | `CMAKE_ASC_RUN_MODE` | `npu`（默认）、`cpu`、`sim` | 运行模式：NPU 运行、CPU调试、NPU仿真 |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU 架构：Ascend 950PR/Ascend 950DT |
  | `SCENARIO_NUM` | `1`-`7` | 场景编号：不同数据类型和转置组合 |

- 执行结果

  执行结果如下，说明精度对比成功。
  ```bash
  test pass!
  ```

## 相关文档

- [Load2D API 文档](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/900beta2/API/ascendcopapi/atlasascendc_api_07_00169.html)
- [Mmad API 文档](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/900beta2/API/ascendcopapi/atlasascendc_api_07_0249.html)
- [Fixpipe API 文档](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/900beta2/API/ascendcopapi/atlasascendc_api_07_0251.html)