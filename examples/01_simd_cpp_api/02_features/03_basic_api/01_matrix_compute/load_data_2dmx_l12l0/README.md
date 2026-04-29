# load_data_2dmx_l12l0样例

## 概述
本样例介绍了输入为ND格式，A/B矩阵为FP4(fp4x2_e1m2_t / fp4x2_e2m1_t)和FP8(fp8_e4m3fn_t / fp8_e5m2_t)数据类型，量化矩阵scaleA/scaleB为fp8_e8m0_t数据类型，左矩阵和左量化矩阵、右矩阵和右量化矩阵转置、不转置组合共 6 种量化矩阵乘法(简称：MX矩阵乘法)场景中，相关指令的使用方法，着重介绍`LoadData`指令及其`LoadData2DParamsV2`、`LoadData2DMxParams`结构体参数的使用方法，整体流程如下：<br>
（1）矩阵A (矩阵乘的左输入矩阵)和 矩阵B (矩阵乘的右输入矩阵)从 GM -> L1 时，如何调用 `DataCopy` 指令，并通过 `Nd2NzParams` 结构体参数控制搬运;<br>
（2）矩阵scaleA (左量化输入矩阵)和 矩阵scaleB (右量化输入矩阵)从 GM -> L1 时，如何调用 `DataCopy` 指令，并通过 `Nd2NzParams` 或 `Dn2NzParams` 结构体参数控制搬运;<br>
（3）A/B/scaleA/scaleB矩阵从L1 -> L0A/L0B/L0A_MX/L0B_MX时，如何调用 `LoadData` 指令，并通过 `LoadData2DParamsV2` 结构体参数控制A/B矩阵数据搬运、通过 `LoadData2DMxParams` 结构体参数控制scaleA/scaleB矩阵数据搬运;<br>
（4）使用`Mmad`指令实现带有量化的矩阵乘法计算(C = (scaleA ⊗ A) * (scaleB ⊗ B)，"⊗"表示广播乘法，左/右矩阵与左/右量化系数矩阵做乘积时，k方向上每32个元素共享一个量化因子);<br>
（5）使用`Fixpipe`指令将结果矩阵C从L0C -> GM。<br>
各指令参数配置及执行指令前后各个矩阵数据排布变化，均配合示意图进行了说明。


## 支持的产品
- Atlas 350 加速卡（Ascend 950PR/Ascend 950DT）

## 目录结构介绍
```
├── load_data_2dmx_l12l0
│   ├── scripts
│   │   ├── gen_data.py             // 输入数据和真值数据生成脚本
│   │   └── verify_result.py        // 验证输出数据和真值数据是否一致的验证脚本
│   ├── CMakeLists.txt              // 编译工程文件
│   ├── data_utils.h                // 数据读入写出函数
│   ├── load_data_2dmx_l12l0.asc    // Ascend C样例实现 & 调用样例
│   └── README.md                   // 样例说明文档
```

## 样例描述
一次完整的MX矩阵乘法涉及的数据搬运过程包括：GM -> L1 -> L0A/L0B/L0A_MX/L0B_MX -> L0C -> GM，其中A/B矩阵和scaleA/scaleB矩阵在不同存储单元的数据排布格式不同，如[表1](#表1)：<br>

<a name="表1"></a>
<table border="2" align="center">
<caption style="font-weight: normal;">
    <span style="font-weight: bold; font-size: 1.2em;">📌 表1：不同存储单元的数据排布格式</span></caption>
  <tr>
    <td></td>
    <td align="center"><span style="font-weight: bold;">GM</span></td>
    <td align="center"><span style="font-weight: bold;">L1</span></td>
    <td align="center"><span style="font-weight: bold;">L0A</span></td>
    <td align="center"><span style="font-weight: bold;">L0A_MX</span></td>
    <td align="center"><span style="font-weight: bold;">L0B</span></td>
    <td align="center"><span style="font-weight: bold;">L0B_MX</span></td>
    <td align="center"><span style="font-weight: bold;">L0C</span></td>
  </tr>
  <tr>
    <td align="center"><span style="font-weight: bold;">A矩阵</span></td>
    <td align="center">ND</td>
    <td align="center">Nz</td>
    <td align="center">Nz</td>
    <td align="center">-</td>
    <td align="center">-</td>
    <td align="center">-</td>
    <td align="center">-</td>
  </tr>
  <tr>
    <td align="center"><span style="font-weight: bold;">B矩阵</span></td>
    <td align="center">ND</td>
    <td align="center">Nz</td>
    <td align="center">-</td>
    <td align="center">-</td>
    <td align="center">Zn</td>
    <td align="center">-</td>
    <td align="center">-</td>
  </tr>
  <tr>
    <td align="center"><span style="font-weight: bold;">scaleA矩阵</span></td>
    <td align="center">ND</td>
    <td align="center">Zz</td>
    <td align="center">-</td>
    <td align="center">Zz</td>
    <td align="center">-</td>
    <td align="center">-</td>
    <td align="center">-</td>
  </tr>
  <tr>
    <td align="center"><span style="font-weight: bold;">scaleB矩阵</span></td>
    <td align="center">ND</td>
    <td align="center">Nn</td>
    <td align="center">-</td>
    <td align="center">-</td>
    <td align="center">-</td>
    <td align="center">Nn</td>
    <td align="center">-</td>
  </tr>
  <tr>
    <td align="center"><span style="font-weight: bold;">C矩阵</span></td>
    <td align="center">ND</td>
    <td align="center">-</td>
    <td align="center">-</td>
    <td align="center">-</td>
    <td align="center">-</td>
    <td align="center">-</td>
    <td align="center">Nz</td>
  </tr>
</table>

当输入数据类型分别取FP4/FP8时，通过布尔变量isAtranspose和isBtranspose分别控制A/scaleA、B/scaleB矩阵是否转置输入，组合得到4种基本场景。此外，当A矩阵转置输入[k, m]时，若单次调用`LoadData`搬运会导致多搬脏数据超过1个分型，需要使用for循环方式避免搬运多余的脏数据，因此在原有4种场景的基础上，针对A/B矩阵为FP4和FP8输入数据类型新增for循环场景（场景5和6），共6种场景。

下文将介绍上述6种场景下，A/B矩阵和scaleA/scaleB矩阵在完整MX矩阵乘法流程的各个阶段前后数据的排布方式、对齐要求、所调用的指令以及如何配置相应的参数，着重介绍`LoadData`指令及其`LoadData2DParamsV2`、`LoadData2DMxParams`结构体参数的使用方法。<br>
（1）从L1 -> L0通路，调用`LoadData`指令，传入2个结构体参数，A/B矩阵使用`LoadData2DParamsV2`结构体参数，scale矩阵使用`LoadData2DMxParams`结构体参数，不同场景和数据类型可使用的搬运方式如[表2](#表2);<br>
（2）以参数scenarioNum来代表上述6种场景，scenarioNum不同取值对应的含义及L1->L0过程中调用的搬运指令，如[表3](#表3)所示。<br>
所有场景基于相同的矩阵乘规格：[m, n, k] = [40, 50, 70]，核函数名称为"KernelLoadDataL12L0Load2DMX"。

<a name="表2"></a>
<table border="2" align="center">
<caption style="font-weight: normal;">
    <span style="font-weight: bold; font-size: 1.2em;">📌 表2：L1-->L0，不同场景下A/B矩阵和scale矩阵的搬运指令</span></caption>
  <tr>
    <td></td>
    <td align="center"><span style="font-weight: bold;">FP4</span></td>
    <td align="center"><span style="font-weight: bold;">FP8</span></td>
  </tr>
  <tr>
    <td align="center"><span style="font-weight: bold;">isAtranspose=false<br>A不转置输入[m, k]<br>scaleA不转置输入[m, scaleK/2, 2]<br>L1->L0A不需要转置</span></td>
    <td align="center">LoadData(LoadData2DParamsV2(不转置), LoadData2DMxParams)</td>
    <td align="center">LoadData(LoadData2DParamsV2(不转置), LoadData2DMxParams)</td>
  </tr>
    <tr>
    <td align="center"><span style="font-weight: bold;">isAtranspose=true<br>A转置输入[k, m]<br>scaleA转置输入[scaleK, m, 2]<br>L1->L0A需要转置<br>(单次调用)</span></td>
    <td align="center">LoadData(LoadData2DParamsV2(转置), LoadData2DMxParams)</td>
    <td align="center">LoadData(LoadData2DParamsV2(转置), LoadData2DMxParams)</td>
  </tr>
    <tr>
    <td align="center"><span style="font-weight: bold;">isBtranspose=false<br>B不转置输入[k, n]<br>scaleB不转置输入[scaleK, n, 2]<br>L1->L0B需要转置</span></td>
    <td align="center">LoadData(LoadData2DParamsV2(转置), LoadData2DMxParams)</td>
    <td align="center">LoadData(LoadData2DParamsV2(转置), LoadData2DMxParams)</td>
  </tr>
    <tr>
    <td align="center"><span style="font-weight: bold;">isBtranspose=true<br>B转置输入[n, k]<br>scaleB转置输入[n, scaleK/2, 2]<br>L1->L0B不需要转置</span></td>
    <td align="center">LoadData(LoadData2DParamsV2(不转置), LoadData2DMxParams)</td>
    <td align="center">LoadData(LoadData2DParamsV2(不转置), LoadData2DMxParams)</td>
  </tr>
</table>

<a name="表3"></a>
<table border="2" align="center">
<caption style="font-weight: normal;">
    <span style="font-weight: bold; font-size: 1.2em;">📌 表3：scenarioNum不同取值的含义</span></caption>
  <tr>
    <td ><span style="font-weight: bold;">scenarioNum</span></td>
    <td><span style="font-weight: bold;">A/B数据类型</span></td>
    <td><span style="font-weight: bold;">A矩阵类型</span></td>
    <td><span style="font-weight: bold;">B矩阵类型</span></td>
    <td><span style="font-weight: bold;">输出数据类型</span></td>
    <td><span style="font-weight: bold;">isAtranspose</span></td>
    <td><span style="font-weight: bold;">isBtranspose</span></td>
    <td><span style="font-weight: bold;">A矩阵搬运方式</span></td>
    <td><span style="font-weight: bold;">B矩阵搬运方式</span></td>
  </tr>
  <tr>
    <td><span style="font-weight: bold;">1</span></td>
    <td>FP4</td>
    <td>fp4x2_e1m2_t</td>
    <td>fp4x2_e2m1_t</td>
    <td>float</td>
    <td>false</td>
    <td>true</td>
    <td>LoadData(LoadData2DParamsV2(不转置), LoadData2DMxParams)</td>
    <td>LoadData(LoadData2DParamsV2(不转置), LoadData2DMxParams)</td>
  </tr>
  <tr>
    <td><span style="font-weight: bold;">2</span></td>
    <td>FP4</td>
    <td>fp4x2_e2m1_t</td>
    <td>fp4x2_e1m2_t</td>
    <td>float</td>
    <td>true</td>
    <td>false</td>
    <td>LoadData(LoadData2DParamsV2(转置), LoadData2DMxParams)</td>
    <td>LoadData(LoadData2DParamsV2(转置), LoadData2DMxParams)</td>
  </tr>
  <tr>
    <td><span style="font-weight: bold;">3</span></td>
    <td>FP8</td>
    <td>fp8_e4m3fn_t</td>
    <td>fp8_e5m2_t</td>
    <td>float</td>
    <td>false</td>
    <td>true</td>
    <td>LoadData(LoadData2DParamsV2(不转置), LoadData2DMxParams)</td>
    <td>LoadData(LoadData2DParamsV2(不转置), LoadData2DMxParams)</td>
  </tr>
  <tr>
    <td><span style="font-weight: bold;">4</span></td>
    <td>FP8</td>
    <td>fp8_e5m2_t</td>
    <td>fp8_e4m3fn_t</td>
    <td>float</td>
    <td>true</td>
    <td>false</td>
    <td>LoadData(LoadData2DParamsV2(转置), LoadData2DMxParams)</td>
    <td>LoadData(LoadData2DParamsV2(转置), LoadData2DMxParams)</td>
  </tr>
  <tr>
    <td><span style="font-weight: bold;">5</span></td>
    <td>FP4</td>
    <td>fp4x2_e2m1_t</td>
    <td>fp4x2_e1m2_t</td>
    <td>float</td>
    <td>true</td>
    <td>false</td>
    <td>for循环 + LoadData(LoadData2DParamsV2(转置), LoadData2DMxParams)</td>
    <td>LoadData(LoadData2DParamsV2(转置), LoadData2DMxParams)</td>
  </tr>
  <tr>
    <td><span style="font-weight: bold;">6</span></td>
    <td>FP8</td>
    <td>fp8_e5m2_t</td>
    <td>fp8_e4m3fn_t</td>
    <td>float</td>
    <td>true</td>
    <td>false</td>
    <td>for循环 + LoadData(LoadData2DParamsV2(转置), LoadData2DMxParams)</td>
    <td>LoadData(LoadData2DParamsV2(转置), LoadData2DMxParams)</td>
  </tr>
</table>

**场景1：输入FP4数据类型，isAtranspose=false，isBtranspose=true**
- 输入A [40, 70]，fp4x2_e1m2_t类型，ND格式；B [50, 70]，fp4x2_e2m1_t类型，ND格式；
- ScaleA [40, 4]，fp8_e8m0_t类型；ScaleB [4, 50]，fp8_e8m0_t类型；
- 输出C [40, 50]，float类型，ND格式；
- 实现：调用`LoadData`指令，传入`LoadData2DParamsV2`(ifTranspose=false)和`LoadData2DMxParams`两个结构体参数，一次完成A矩阵从L1搬运到L0A、scaleA矩阵从L1搬运到L0A_MX；B矩阵同理，一次完成B矩阵从L1搬运到L0B、scaleB矩阵从L1搬运到L0B_MX；
- 说明：A矩阵不转置输入[m, k]，L1->L0A不需要转置，`LoadData2DParamsV2`的ifTranspose设置为false；B矩阵转置输入[n, k]，L1->L0B不需要转置，`LoadData2DParamsV2`的ifTranspose设置为false；scaleA/scaleB矩阵分别通过同一次`LoadData`调用中的`LoadData2DMxParams`搬运到L0A_MX/L0B_MX。

**场景2：输入FP4数据类型，isAtranspose=true，isBtranspose=false**
- 输入A [70, 40]，fp4x2_e2m1_t类型，ND格式；B [70, 50]，fp4x2_e1m2_t类型，ND格式；
- ScaleA [4, 40, 2]，fp8_e8m0_t类型；ScaleB [4, 50, 2]，fp8_e8m0_t类型；
- 输出C [40, 50]，float类型，ND格式；
- 实现：调用`LoadData`指令，传入`LoadData2DParamsV2`(ifTranspose=true)和`LoadData2DMxParams`两个结构体参数，一次完成A矩阵从L1搬运到L0A、scaleA矩阵从L1搬运到L0A_MX；B矩阵同理，一次完成B矩阵从L1搬运到L0B、scaleB矩阵从L1搬运到L0B_MX；
- 说明：A矩阵转置输入[k, m]，L1->L0A需要转置，`LoadData2DParamsV2`的ifTranspose设置为true，发生小分型转置和大分型排布格式变化，m方向多搬运数据超过1个分型；B矩阵不转置输入[k, n]，L1->L0B需要转置，`LoadData2DParamsV2`的ifTranspose设置为true，发生小分型转置和大分型排布格式变化，n方向多搬运数据不超过1个分型；scaleA/scaleB矩阵分别通过同一次`LoadData`调用中的`LoadData2DMxParams`搬运到L0A_MX/L0B_MX。由于A转置导致m方向多搬脏数据超过1个分型，`Mmad`指令需设置mmadParams.m = CeilAlign(m, fractalShape[0] * fractalNum)来让多搬的分型参与计算，`Fixpipe`搬出时跳过无效分型参与计算的结果。

**场景3：输入FP8数据类型，isAtranspose=false，isBtranspose=true**
- 输入A [40, 70]，fp8_e4m3fn_t类型，ND格式；B [50, 70]，fp8_e5m2_t类型，ND格式；
- ScaleA [40, 4]，fp8_e8m0_t类型；ScaleB [4, 50]，fp8_e8m0_t类型；
- 输出C [40, 50]，float类型，ND格式；
- 实现：调用`LoadData`指令，传入`LoadData2DParamsV2`(ifTranspose=false)和`LoadData2DMxParams`两个结构体参数，一次完成A矩阵从L1搬运到L0A、scaleA矩阵从L1搬运到L0A_MX；B矩阵同理，一次完成B矩阵从L1搬运到L0B、scaleB矩阵从L1搬运到L0B_MX；
- 说明：与场景1类似，但数据类型为FP8。A矩阵不转置输入[m, k]，L1->L0A不需要转置，`LoadData2DParamsV2`的ifTranspose设置为false；B矩阵转置输入[n, k]，L1->L0B不需要转置，`LoadData2DParamsV2`的ifTranspose设置为false。此外，FP8数据类型下，A矩阵不转置输入[m, k]时k方向仅向32B对齐，需要将k方向尾数据刷为0防止脏数据参与计算；scaleA/scaleB矩阵分别通过同一次`LoadData`调用中的`LoadData2DMxParams`搬运到L0A_MX/L0B_MX。

**场景4：输入FP8数据类型，isAtranspose=true，isBtranspose=false**
- 输入A [70, 40]，fp8_e5m2_t类型，ND格式；B [70, 50]，fp8_e4m3fn_t类型，ND格式；
- ScaleA [4, 40, 2]，fp8_e8m0_t类型；ScaleB [4, 50, 2]，fp8_e8m0_t类型；
- 输出C [40, 50]，float类型，ND格式；
- 实现：调用`LoadData`指令，传入`LoadData2DParamsV2`(ifTranspose=true)和`LoadData2DMxParams`两个结构体参数，一次完成A矩阵从L1搬运到L0A、scaleA矩阵从L1搬运到L0A_MX；B矩阵同理，一次完成B矩阵从L1搬运到L0B、scaleB矩阵从L1搬运到L0B_MX；
- 说明：与场景2类似，但数据类型为FP8。A矩阵转置输入[k, m]，L1->L0A需要转置，`LoadData2DParamsV2`的ifTranspose设置为true，发生小分型转置和大分型排布格式变化，m方向多搬运数据超过1个分型；B矩阵不转置输入[k, n]，L1->L0B需要转置，`LoadData2DParamsV2`的ifTranspose设置为true，发生小分型转置和大分型排布格式变化。scaleA/scaleB矩阵分别通过同一次`LoadData`调用中的`LoadData2DMxParams`搬运到L0A_MX/L0B_MX。此外，A矩阵转置输入[k, m]时需要将k方向剩余脏数据刷为0；B矩阵不转置输入[k, n]时也需要将k方向剩余脏数据刷为0。`Mmad`指令同样需设置mmadParams.m = CeilAlign(m, fractalShape[0] * fractalNum)。

**场景5：输入FP4数据类型，isAtranspose=true，isBtranspose=false**
- 输入A [70, 40]，fp4x2_e2m1_t类型，ND格式；B [70, 50]，fp4x2_e1m2_t类型，ND格式；
- ScaleA [4, 40, 2]，fp8_e8m0_t类型；ScaleB [4, 50, 2]，fp8_e8m0_t类型；
- 输出C [40, 50]，float类型，ND格式；
- 实现：A矩阵使用for循环调用`LoadData`指令，传入`LoadData2DParamsV2`(ifTranspose=true)和`LoadData2DMxParams`两个结构体参数，每次循环同时搬运A矩阵到L0A、scaleA矩阵到L0A_MX；B矩阵单次调用`LoadData`，同时搬运B矩阵到L0B、scaleB矩阵到L0B_MX；
- 说明：A矩阵转置输入[k, m]，L1->L0A需要转置，使用for循环方式调用`LoadData`搬运，避免多搬脏数据，并发生小分型转置和大分型排布格式变化。每次for循环在L0A上跳过m方向尾脏数据分型，m方向多搬运数据不超过1个分型。B矩阵不转置输入[k, n]，L1->L0B需要转置，单次调用`LoadData`搬运，`LoadData2DParamsV2`的ifTranspose设置为true，发生小分型转置和大分型排布格式变化，n方向多搬运数据不超过1个分型；scaleA/scaleB矩阵分别通过同一次`LoadData`调用中的`LoadData2DMxParams`搬运到L0A_MX/L0B_MX。A矩阵转置输入[k, m]时需要将k方向剩余脏数据刷为0；B矩阵不转置输入[k, n]时也需要将k方向剩余脏数据刷为0。

**场景6：输入FP8数据类型，isAtranspose=true，isBtranspose=false**
- 输入A [70, 40]，fp8_e5m2_t类型，ND格式；B [70, 50]，fp8_e4m3fn_t类型，ND格式；
- ScaleA [4, 40, 2]，fp8_e8m0_t类型；ScaleB [4, 50, 2]，fp8_e8m0_t类型；
- 输出C [40, 50]，float类型，ND格式；
- 实现：A矩阵使用for循环调用`LoadData`指令，传入`LoadData2DParamsV2`(ifTranspose=true)和`LoadData2DMxParams`两个结构体参数，每次循环同时搬运A矩阵到L0A、scaleA矩阵到L0A_MX；B矩阵单次调用`LoadData`，同时搬运B矩阵到L0B、scaleB矩阵到L0B_MX；
- 说明：与场景5类似，但数据类型为FP8。A矩阵转置输入[k, m]，使用for循环方式调用`LoadData`搬运，避免多搬脏数据，并发生小分型转置和大分型排布格式变化。B矩阵不转置输入[k, n]，单次调用`LoadData`搬运，`LoadData2DParamsV2`的ifTranspose设置为true，并发生小分型转置和大分型排布格式变化。scaleA/scaleB矩阵分别通过同一次`LoadData`调用中的`LoadData2DMxParams`搬运到L0A_MX/L0B_MX。A矩阵和B矩阵均需要将k方向剩余脏数据刷为0。


为了方便描述，在此对后续常用概念给出定义：

（1）fractalShape: 小分型的shape。FP4数据类型下为[16, 64]，FP8数据类型下为[16, 32]。本样例涉及到数据类型的分型相关信息如[表5](#表5)。

（2）fractalSize: 1个小分型包含的元素个数，FP4为1024，FP8为512。

（3）fractalNum: 1个方块包含的小分型个数。FP4为4，FP8为2。当`LoadData`的`LoadData2DParamsV2`参数配置为转置搬运时，连续的fractalNum个小分型合并为一个方块然后转置。

（4）packedK: A/B矩阵GM上实际存储的K轴元素个数。FP4数据类型下，2个fp4元素打包为1个fp4x2元素，因此packedK = CeilDivision(k, 2)；FP8数据类型下，packedK = k。

（5）scaleK: Scale矩阵K轴对齐后的长度。scaleK = CeilDivision(k, SCALE_BASE_FACTOR) * SCALE_EVEN_NUMBER，其中SCALE_BASE_FACTOR=64，SCALE_EVEN_NUMBER=2。本样例中k=70时，scaleK = CeilDivision(70, 64) * 2 = 4。

（6）alignK: A/B矩阵K轴对齐后的长度。alignK = CeilAlign(k, SCALE_BASE_FACTOR) = CeilAlign(k, 64)。本样例中k=70时，alignK = CeilAlign(70, 64) = 128。

<a name="表5"></a>
<table border="2" align="center">
<caption style="font-weight: normal;">
    <span style="font-weight: bold; font-size: 1.2em;">📌 表5：不同数据类型分型相关信息</span></caption>
  <tr>
    <td></td>
    <td align="center"><span style="font-weight: bold;">fractalShape</span></td>
    <td align="center"><span style="font-weight: bold;">fractalSize</span></td>
    <td align="center"><span style="font-weight: bold;">fractalNum</span></td>
    <td align="center"><span style="font-weight: bold;">packedK</span></td>
  </tr>
  <tr>
    <td align="center"><span style="font-weight: bold;">FP4</span></td>
    <td align="center">[16, 64]</td>
    <td align="center">1024</td>
    <td align="center">4</td>
    <td align="center">CeilDivision(k, 2)</td>
  </tr>
    <tr>
    <td align="center"><span style="font-weight: bold;">FP8</span></td>
    <td align="center">[16, 32]</td>
    <td align="center">512</td>
    <td align="center">2</td>
    <td align="center">k</td>
  </tr>
</table>

（7）CeilAlign：向上对齐操作，例如m=40时，CeilAlign(40, 16)=48，表示将m轴向16对齐，对齐后m轴长度为48。

      __aicore__ inline uint16_t CeilAlign(uint16_t size, uint16_t alignValue) {
          return (size + alignValue - 1) / alignValue * alignValue;
      }

（8）CeilDivision：向上取整除法，一般用于求解向上对齐后的循环次数。

（9）mAlignValue：m轴向mAlignValue对齐，例如mAlignValue=16，代表m轴对齐到16。依次类推还有kAlignValue、nAlignValue。MX场景下kAlignValue = SCALE_BASE_FACTOR = 64。

（10）mAlignL1和mAlignL0：A矩阵分别在L1和L0A上时，m轴对齐后的值。依次类推还有kaAlignL1、kaAlignL0、nAlignL1、nAlignL0、kbAlignL1、kbAlignL0。

A、B矩阵在L1和L0上在各个轴方向上对齐的要求不同，将[表3](#表3)中6种场景的对齐要求总结如下[表6](#表6)、[表7](#表7)：

<a name="表6"></a>
<table border="2" align="center">
<caption style="font-weight: normal;">
    <span style="font-weight: bold; font-size: 1.2em;">📌 表6：A、B矩阵在L1上各个轴的对齐要求(L1排布格式为Nz)</span></caption>
  <tr>
    <td></td>
    <td align="center"><span style="font-weight: bold;">FP4 (fractalNum=4)</span></td>
    <td align="center"><span style="font-weight: bold;">FP8 (fractalNum=2)</span></td>
  </tr>
  <tr>
    <td rowspan="2" align="center"><span style="font-weight: bold;">A矩阵不转置输入[m, k]</span></td>
    <td colspan="2" align="center">mAlignValue = fractalShape[0]</td>
  </tr>
  <tr>
    <td colspan="2" align="center" >kAlignValue = SCALE_BASE_FACTOR(64)</td>
  </tr>
  <tr>
    <td rowspan="2" align="center"><span style="font-weight: bold;">A矩阵转置输入[k, m]</span></td>
    <td align="center">kAlignValue = SCALE_BASE_FACTOR(64)</td>
    <td align="center">kAlignValue = SCALE_BASE_FACTOR(64)</td>
  </tr>
  <tr>
    <td align="center" >mAlignValue = fractalShape[1] = 64</td>
    <td align="center" >mAlignValue = fractalShape[1] = 32</td>
  </tr>
    <tr>
    <td rowspan="2" align="center"><span style="font-weight: bold;">B矩阵不转置输入[k, n]</span></td>
    <td align="center">kAlignValue = SCALE_BASE_FACTOR(64)</td>
    <td align="center">kAlignValue = SCALE_BASE_FACTOR(64)</td>
  </tr>
  <tr>
    <td align="center" >nAlignValue = fractalShape[1] = 64</td>
    <td align="center" >nAlignValue = fractalShape[1] = 32</td>
  </tr>
 <tr>
    <td rowspan="2" align="center"><span style="font-weight: bold;">B矩阵转置输入[n, k]</span></td>
    <td colspan="2" align="center">nAlignValue = fractalShape[0]</td>
  </tr>
  <tr>
    <td colspan="2" align="center" >kAlignValue = SCALE_BASE_FACTOR(64)</td>
  </tr>
</table>

<a name="表7"></a>
<table border="2" align="center">
<caption style="font-weight: normal;">
    <span style="font-weight: bold; font-size: 1.2em;">📌 表7：A、B矩阵在L0上各个轴的对齐要求</span></caption>
  <tr>
    <td></td>
    <td align="center"><span style="font-weight: bold;">FP4 (fractalNum=4)</span></td>
    <td align="center"><span style="font-weight: bold;">FP8 (fractalNum=2)</span></td>
  </tr>
  <tr>
    <td rowspan="2" align="center"><span style="font-weight: bold;">A矩阵不转置输入[m, k]，L1->L0A不需要转置<br>(场景1/3)</span></td>
    <td colspan="2" align="center">mAlignValue = fractalShape[0]</td>
  </tr>
  <tr>
    <td colspan="2" align="center" >kAlignValue = SCALE_BASE_FACTOR(64)</td>
  </tr>
  <tr>
    <td rowspan="2" align="center"><span style="font-weight: bold;">A矩阵转置输入[k, m]，L1->L0A需要转置<br>(场景2/4，单次调用)</span></td>
    <td align="center">mAlignValue = fractalShape[0] * fractalNum = 64</td>
    <td align="center">mAlignValue = fractalShape[0] * fractalNum = 32</td>
  </tr>
  <tr>
    <td colspan="2" align="center" >kAlignValue = SCALE_BASE_FACTOR(64)</td>
  </tr>
  <tr>
    <td rowspan="2" align="center"><span style="font-weight: bold;">A矩阵转置输入[k, m]，L1->L0A需要转置<br>(场景5/6，for循环调用)</span></td>
    <td colspan="2" align="center">mAlignValue = fractalShape[0]</td>
  </tr>
  <tr>
    <td colspan="2" align="center" >kAlignValue = SCALE_BASE_FACTOR(64)</td>
  </tr>
    <tr>
    <td rowspan="2" align="center"><span style="font-weight: bold;">B矩阵不转置输入[k, n]，L1->L0B需要转置<br>(场景2/4/5/6)</span></td>
    <td align="center">nAlignValue = fractalShape[0] * fractalNum = 64</td>
    <td align="center">nAlignValue = fractalShape[0] * fractalNum = 32</td>
  </tr>
  <tr>
    <td colspan="2" align="center" >kAlignValue = SCALE_BASE_FACTOR(64)</td>
  </tr>
 <tr>
    <td rowspan="2" align="center"><span style="font-weight: bold;">B矩阵转置输入[n, k]，L1->L0B不需要转置<br>(场景1/3)</span></td>
    <td colspan="2" align="center">nAlignValue = fractalShape[0]</td>
  </tr>
  <tr>
    <td colspan="2" align="center" >kAlignValue = SCALE_BASE_FACTOR(64)</td>
  </tr>
</table>

scaleA/scaleB矩阵的对齐要求如下：

<a name="表8"></a>
<table border="2" align="center">
<caption style="font-weight: normal;">
    <span style="font-weight: bold; font-size: 1.2em;">📌 表8：scaleA/scaleB矩阵在L1/L0上各个轴的对齐要求</span></caption>
  <tr>
    <td></td>
    <td align="center"><span style="font-weight: bold;">scaleA矩阵(L1排布格式为Zz)</span></td>
    <td align="center"><span style="font-weight: bold;">scaleB矩阵(L1排布格式为Nn)</span></td>
  </tr>
  <tr>
    <td align="center"><span style="font-weight: bold;">m轴/n轴对齐</span></td>
    <td align="center">scaleMAlignL1 = CeilAlign(m, fractalShape[0])</td>
    <td align="center">scaleNAlignL1 = CeilAlign(n, fractalShape[0])</td>
  </tr>
  <tr>
    <td align="center"><span style="font-weight: bold;">K轴对齐</span></td>
    <td align="center">scaleK = CeilDivision(k, 64) * 2</td>
    <td align="center">scaleK = CeilDivision(k, 64) * 2</td>
  </tr>
</table>

### 1.整体流程图

MX矩阵乘整体流程如下所示：

```
GM(ND) --DataCopy--> L1(Nz) --LoadData(LoadData2DParamsV2, LoadData2DMxParams)--> L0A(Nz)/L0B(Zn)/L0A_MX/L0B_MX --Mmad--> L0C(Nz) --Fixpipe--> GM(ND)
```

**步骤详解**：

1. **GM -> L1**：
   - 调用 `DataCopy` 指令，传入 `Nd2NzParams` 结构体参数，实现 ND 到 Nz 格式转换（A/B矩阵）
   - 调用 `DataCopy` 指令，传入 `Nd2NzParams` 或 `Dn2NzParams` 结构体参数，并按B16数据类型实现 ND 到 Zz和Nn 格式转换（scaleA/scaleB矩阵）
   - 使用 `Fill` 对齐填充零值，防止脏数据参与计算
2. **L1 -> L0**：
   - 调用 `LoadData` 指令，传入 `LoadData2DParamsV2` 和 `LoadData2DMxParams` 两个结构体参数
   - `LoadData2DParamsV2` 控制 A/B 矩阵搬运，`LoadData2DMxParams` 控制 scale 矩阵搬运
3. **矩阵乘**：使用 `Mmad` 接口执行 MX 矩阵乘法
4. **L0C -> GM**：使用 `Fixpipe` 接口搬出结果

### 2.GM到L1 (`DataCopy`)
本小节主要介绍A/B矩阵在GM上的数据排布格式为ND，在L1上为Nz时，在GM->L1过程中调用`DataCopy`指令并配置`Nd2NzParams`结构体参数完成数据搬运及格式变换；以及scaleA/scaleB矩阵在GM上为ND格式，在L1上分别为Zz和Nn，在GM->L1过程中调用`DataCopy`指令，并配置`Nd2NzParams`或`Dn2NzParams`结构体参数完成数据搬运及格式变换。

#### 2.1. A矩阵GM->L1

##### 2.1.1. A矩阵GM上输入为[m, k]（不转置）
A矩阵GM输入不转置([m, k])时，调用`DataCopy`指令并配置`Nd2NzParams`结构体参数，将A矩阵从GM(ND)搬运到L1(Nz)。配置`Nd2NzParams`结构体时，需要注意的是，FP4数据类型下dValue取packedK = CeilDivision(k, 2)，FP8数据类型下dValue取packedK = k；dstNzC0Stride的单位为32B，该参数取值为L1上NZ矩阵的对齐后的行数。

            AscendC::Nd2NzParams nd2nzA1Params;
            nd2nzA1Params.ndNum = 1;
            nd2nzA1Params.nValue = m;
            nd2nzA1Params.dValue = packedK;
            nd2nzA1Params.srcNdMatrixStride = 0;
            nd2nzA1Params.srcDValue = packedK;
            // dstNzC0Stride取A矩阵在L1上row方向对齐后的长度
            nd2nzA1Params.dstNzC0Stride = mAlignL1;
            nd2nzA1Params.dstNzNStride = 1;
            nd2nzA1Params.dstNzMatrixStride = 0;
            AscendC::DataCopy(a1Local, aGM, nd2nzA1Params);

##### 2.1.2. A矩阵GM上输入为[k, m]（转置）
A矩阵GM输入转置([k, m])时，调用`DataCopy`指令并配置`Nd2NzParams`结构体参数，将A矩阵从GM(ND)搬运到L1(Nz)。配置`Nd2NzParams`结构体时，源操作数的shape为[k, m]，dstNzC0Stride的单位为32B，该参数取值为L1上NZ矩阵的对齐后的行数（即k方向对齐后的长度alignK）。

            AscendC::Nd2NzParams nd2nzA1Params;
            nd2nzA1Params.ndNum = 1;
            nd2nzA1Params.nValue = packedK;
            nd2nzA1Params.dValue = m;
            nd2nzA1Params.srcNdMatrixStride = 0;
            nd2nzA1Params.srcDValue = m;
            nd2nzA1Params.dstNzC0Stride = alignK;
            nd2nzA1Params.dstNzNStride = 1;
            nd2nzA1Params.dstNzMatrixStride = 0;
            AscendC::DataCopy(a1Local, aGM, nd2nzA1Params);

#### 2.2. B矩阵GM->L1

##### 2.2.1. B矩阵GM上输入为[k, n]（不转置）
B矩阵GM输入不转置([k, n])时，调用`DataCopy`指令并配置`Nd2NzParams`结构体参数，将B矩阵从GM(ND)搬运到L1(Nz)。配置`Nd2NzParams`结构体时，源操作数的shape为[k, n]，dstNzC0Stride取值为L1上NZ矩阵的对齐后的行数（即k方向对齐后的长度alignK）。

            AscendC::Nd2NzParams nd2nzB1Params;
            nd2nzB1Params.ndNum = 1;
            nd2nzB1Params.nValue = packedK;
            nd2nzB1Params.dValue = n;
            nd2nzB1Params.srcNdMatrixStride = 0;
            nd2nzB1Params.srcDValue = n;
            nd2nzB1Params.dstNzC0Stride = alignK;
            nd2nzB1Params.dstNzNStride = 1;
            nd2nzB1Params.dstNzMatrixStride = 0;
            AscendC::DataCopy(b1Local, bGM, nd2nzB1Params);

##### 2.2.2. B矩阵GM上输入为[n, k]（转置）
B矩阵GM输入转置([n, k])时，调用`DataCopy`指令并配置`Nd2NzParams`结构体参数，将B矩阵从GM(ND)搬运到L1(Nz)。配置`Nd2NzParams`结构体时，源操作数的shape为[n, k]，dstNzC0Stride取值为L1上NZ矩阵的对齐后的行数（即n方向对齐后的长度nAlignL1）。

            AscendC::Nd2NzParams nd2nzB1Params;
            nd2nzB1Params.ndNum = 1;
            nd2nzB1Params.nValue = n;
            nd2nzB1Params.dValue = packedK;
            nd2nzB1Params.srcNdMatrixStride = 0;
            nd2nzB1Params.srcDValue = packedK;
            nd2nzB1Params.dstNzC0Stride = nAlignL1;
            nd2nzB1Params.dstNzNStride = 1;
            nd2nzB1Params.dstNzMatrixStride = 0;
            AscendC::DataCopy(b1Local, bGM, nd2nzB1Params);

#### 2.3. ScaleA矩阵GM->L1
ScaleA矩阵为fp8_e8m0_t数据类型，按fp8_e8m0_t真实数据类型排布时，scaleA在L1上为Zz格式。由于硬件约束，scale矩阵需要K方向2byte连续，`DataCopy`时需将fp8_e8m0_t按B16(half)视图进行搬运（每2个fp8_e8m0_t元素对应1个half元素），此时L1上表现为B16数据类型的Nz排布。搬运方式取决于isAtranspose的取值：

**（1）isAtranspose=false时，使用`Dn2NzParams`结构体参数（B16视图）**

当A矩阵不转置输入[m, k]时，ScaleA矩阵GM上的shape为[m, scaleK]，调用`DataCopy`指令并配置`Dn2NzParams`结构体参数，按B16视图搬运：

            // 将fp8_e8m0_t的GlobalTensor转为half视图
            AscendC::GlobalTensor<half> scaleAGMB16;
            scaleAGMB16.SetGlobalBuffer((__gm__ half *)(scaleAGM.GetPhyAddr()), m * scaleK / 2);
            auto scaleA1LocalB16 = scaleA1Local.ReinterpretCast<half>();

            AscendC::Dn2NzParams dn2nzParams;
            dn2nzParams.dnNum = 1;
            dn2nzParams.dValue = m;
            dn2nzParams.nValue = scaleK / 2;
            dn2nzParams.srcDnMatrixStride = 0;
            dn2nzParams.srcDValue = scaleK / 2;
            dn2nzParams.dstNzC0Stride = scaleK / 2;
            dn2nzParams.dstNzNStride = 1;
            dn2nzParams.dstNzMatrixStride = 0;
            AscendC::DataCopy(scaleA1LocalB16, scaleAGMB16, dn2nzParams);

**（2）isAtranspose=true时，使用`Nd2NzParams`结构体参数（B16视图）**

当A矩阵转置输入[k, m]时，ScaleA矩阵GM上的shape为[scaleK, m, 2]，调用`DataCopy`指令并配置`Nd2NzParams`结构体参数，按B16视图搬运：

            AscendC::GlobalTensor<half> scaleAGMB16;
            scaleAGMB16.SetGlobalBuffer((__gm__ half *)(scaleAGM.GetPhyAddr()), m * scaleK / 2);
            auto scaleA1LocalB16 = scaleA1Local.ReinterpretCast<half>();

            AscendC::Nd2NzParams nd2nzParams;
            nd2nzParams.ndNum = 1;
            nd2nzParams.nValue = scaleK / 2;
            nd2nzParams.dValue = m;
            nd2nzParams.srcDValue = m;
            nd2nzParams.dstNzC0Stride = scaleK / 2;
            nd2nzParams.dstNzNStride = 1;
            nd2nzParams.dstNzMatrixStride = 0;
            AscendC::DataCopy(scaleA1LocalB16, scaleAGMB16, nd2nzParams);

#### 2.4. ScaleB矩阵GM->L1
ScaleB矩阵的搬运方式与ScaleA类似。按fp8_e8m0_t真实数据类型排布时，scaleB在L1上为Nn格式。由于硬件约束，scale矩阵需要K方向2byte连续，同样需按B16视图做`DataCopy`搬运，此时L1上表现为B16数据类型的Nz排布。搬运方式取决于isBtranspose的取值：

**（1）isBtranspose=true时，使用`Dn2NzParams`结构体参数（B16视图）**

当B矩阵转置输入[n, k]时，ScaleB矩阵GM上的shape为[n, scaleK]，调用`DataCopy`指令并配置`Dn2NzParams`结构体参数，按B16视图搬运：

            AscendC::GlobalTensor<half> scaleBGMB16;
            scaleBGMB16.SetGlobalBuffer((__gm__ half *)(scaleBGM.GetPhyAddr()), n * scaleK / 2);
            auto scaleB1LocalB16 = scaleB1Local.ReinterpretCast<half>();

            AscendC::Dn2NzParams dn2nzParams;
            dn2nzParams.dnNum = 1;
            dn2nzParams.dValue = n;
            dn2nzParams.nValue = scaleK / 2;
            dn2nzParams.srcDnMatrixStride = 0;
            dn2nzParams.srcDValue = scaleK / 2;
            dn2nzParams.dstNzC0Stride = scaleK / 2;
            dn2nzParams.dstNzNStride = 1;
            dn2nzParams.dstNzMatrixStride = 0;
            AscendC::DataCopy(scaleB1LocalB16, scaleBGMB16, dn2nzParams);

**（2）isBtranspose=false时，使用`Nd2NzParams`结构体参数（B16视图）**

当B矩阵不转置输入[k, n]时，ScaleB矩阵GM上的shape为[scaleK, n, 2]，调用`DataCopy`指令并配置`Nd2NzParams`结构体参数，按B16视图搬运：

            AscendC::GlobalTensor<half> scaleBGMB16;
            scaleBGMB16.SetGlobalBuffer((__gm__ half *)(scaleBGM.GetPhyAddr()), n * scaleK / 2);
            auto scaleB1LocalB16 = scaleB1Local.ReinterpretCast<half>();

            AscendC::Nd2NzParams nd2nzParams;
            nd2nzParams.ndNum = 1;
            nd2nzParams.nValue = scaleK / 2;
            nd2nzParams.dValue = n;
            nd2nzParams.srcDValue = n;
            nd2nzParams.dstNzC0Stride = scaleK / 2;
            nd2nzParams.dstNzNStride = 1;
            nd2nzParams.dstNzMatrixStride = 0;
            AscendC::DataCopy(scaleB1LocalB16, scaleBGMB16, nd2nzParams);

#### 2.5. `Fill`操作
由于MX矩阵乘法要求K轴向64对齐（SCALE_BASE_FACTOR），而`DataCopy`指令在FP8数据类型下仅将K轴向32B对齐，当实际K轴长度不满对齐要求时，需要将剩余K方向数据刷为0，防止脏数据参与计算。具体场景如下：

**（1）A矩阵不转置输入[m, k]，FP8数据类型**

FP8数据类型下，`DataCopy` k轴仅向32B对齐，而MX要求k轴向64对齐。此时需要调用`PadOneBlockInnerAxis`将k方向尾部1个block的数据刷为0：

            if constexpr (AscendC::IsSameType<TA, fp8_e4m3fn_t>::value || AscendC::IsSameType<TA, fp8_e5m2_t>::value) {
                PadOneBlockInnerAxis(a1Local, m, packedK);
            }

**（2）A矩阵转置输入[k, m]**

A矩阵转置输入[k, m]时，K轴作为外轴，需要将k方向超出原始长度的脏数据刷为0：

            PadZerosForOuterAxis(a1Local, k, m, alignK);

**（3）B矩阵不转置输入[k, n]**

B矩阵不转置输入[k, n]时，K轴作为外轴，需要将k方向超出原始长度的脏数据刷为0：

            PadZerosForOuterAxis(b1Local, k, n, alignK);

**（4）B矩阵转置输入[n, k]，FP8数据类型**

FP8数据类型下，`DataCopy` k轴仅向32B对齐，需要将k方向尾部1个block的数据刷为0：

            if constexpr (AscendC::IsSameType<TB, fp8_e4m3fn_t>::value || AscendC::IsSameType<TB, fp8_e5m2_t>::value) {
                PadOneBlockInnerAxis(b1Local, n, packedK);
            }

### 3.L1到L0 (`LoadData`)
本小节介绍A/B矩阵从L1搬运到L0A/L0B、scaleA/scaleB矩阵从L1搬运到L0A_MX/L0B_MX时，如何调用`LoadData`指令，并通过`LoadData2DParamsV2`和`LoadData2DMxParams`两个结构体参数完成数据搬运及格式变换。

#### `LoadData2DParamsV2`结构体参数说明
`LoadData2DParamsV2`结构体参数用于控制A/B矩阵数据从L1搬运到L0A/L0B，包含：

- **sid**: 源矩阵标识，默认为0
- **mStartPosition**: 源矩阵M维度起始位置，单位为1个分形
- **kStartPosition**: 源矩阵K维度起始位置，单位为1个分形
- **mStep**: 源矩阵M维度搬运长度，单位为1个分形
- **kStep**: 源矩阵K维度搬运长度，单位为1个分形
- **srcStride**: 源矩阵M方向相邻分形起始地址间隔，单位为1个分形
- **dstStride**: 目标矩阵M方向相邻分形起始地址间隔，单位为1个分形
- **ifTranspose**: 是否启用转置功能，对每个分形矩阵进行转置，默认为false

#### `LoadData2DMxParams`结构体参数说明
`LoadData2DMxParams`结构体参数用于控制scale矩阵数据从L1搬运到L0A_MX/L0B_MX，包含：

- **xStartPosition**: 源矩阵X轴方向（M维度）起始位置，单位为1个分形（16元素）
- **yStartPosition**: 源矩阵Y轴方向（K维度）起始位置，单位为32B
- **xStep**: 源矩阵X轴方向搬运长度，单位为1个分形
- **yStep**: 源矩阵Y轴方向搬运长度，单位为32B
- **srcStride**: 源矩阵X方向相邻分形起始地址间隔，单位为32B
- **dstStride**: 目标矩阵X方向相邻分形起始地址间隔，单位为32B

调用一次`LoadData`指令并传入`LoadData2DParamsV2`、`LoadData2DMxParams`两个结构体参数，可同时完成A/B矩阵到L0A/L0B、对应scale矩阵到L0A_MX/L0B_MX的搬运：

            AscendC::LoadData(a2Local, a1Local, scaleA1Local, loadDataParams, loadMxDataParams);

#### 3.1. A矩阵 L1->L0A，scaleA矩阵 L1->L0A_MX

##### 3.1.1. A矩阵不转置输入[m, k]（场景1/3）

A矩阵不转置输入[m, k]时，L1->L0A不需要转置，单次调用`LoadData`并传入`LoadData2DParamsV2`和`LoadData2DMxParams`两个结构体参数，同时完成A矩阵到L0A、scaleA矩阵到L0A_MX的搬运：

            AscendC::LoadData2DParamsV2 loadDataParams;
            loadDataParams.sid = 0;
            loadDataParams.mStartPosition = 0;
            loadDataParams.kStartPosition = 0;
            loadDataParams.mStep = CeilDivision(mAlignL1, fractalShape[0]);
            loadDataParams.kStep = CeilDivision(kaAlignL1, fractalShape[1]);
            loadDataParams.srcStride = CeilDivision(mAlignL1, fractalShape[0]);
            loadDataParams.dstStride = CeilDivision(mAlignL0, fractalShape[0]);
            loadDataParams.ifTranspose = false;

            AscendC::LoadData2DMxParams loadMxDataParams;
            loadMxDataParams.xStartPosition = 0;
            loadMxDataParams.yStartPosition = 0;
            loadMxDataParams.xStep = CeilDivision(scaleMAlignL1, fractalShape[0]);
            loadMxDataParams.yStep = CeilDivision(packedK, SCALE_CEIL_NUMBER);
            loadMxDataParams.srcStride = scaleK;
            loadMxDataParams.dstStride = CeilDivision(packedK, SCALE_CEIL_NUMBER);

            AscendC::LoadData(a2Local, a1Local, scaleA1Local, loadDataParams, loadMxDataParams);

说明：`LoadData2DParamsV2`控制A矩阵从L1上的Nz排布搬运到L0A上的Nz排布，mStep表示M方向搬运的分形数量，kStep表示K方向搬运的分形数量。srcStride表示L1上M方向相邻分形间隔，dstStride表示L0A上M方向相邻分形间隔。`LoadData2DMxParams`控制scaleA矩阵从L1搬运到L0A_MX，xStep对应M方向分形数量，yStep对应K方向搬运的32B块数量。

##### 3.1.2. A矩阵转置输入[k, m]，单次调用（场景2/4）

A矩阵转置输入[k, m]时，L1->L0A需要转置，单次调用`LoadData`并传入`LoadData2DParamsV2`和`LoadData2DMxParams`两个结构体参数，同时完成A矩阵到L0A、scaleA矩阵到L0A_MX的搬运，发生小分型转置和大分型排布格式变化，但m方向多搬运数据超过1个分型：

            AscendC::LoadData2DParamsV2 loadDataParams;
            loadDataParams.sid = 0;
            loadDataParams.mStartPosition = 0;
            loadDataParams.kStartPosition = 0;
            loadDataParams.mStep = CeilDivision(kaAlignL1, fractalShape[0]);
            loadDataParams.kStep = CeilDivision(mAlignL1, fractalShape[1]);
            loadDataParams.srcStride = CeilDivision(kaAlignL1, fractalShape[0]);
            loadDataParams.dstStride = CeilDivision(mAlignL0, fractalShape[0]);
            loadDataParams.ifTranspose = true;

            AscendC::LoadData2DMxParams loadMxDataParams;
            loadMxDataParams.xStartPosition = 0;
            loadMxDataParams.yStartPosition = 0;
            loadMxDataParams.xStep = CeilDivision(scaleMAlignL1, fractalShape[0]);
            loadMxDataParams.yStep = CeilDivision(packedK, SCALE_CEIL_NUMBER);
            loadMxDataParams.srcStride = scaleK;
            loadMxDataParams.dstStride = CeilDivision(packedK, SCALE_CEIL_NUMBER);

            AscendC::LoadData(a2Local, a1Local, scaleA1Local, loadDataParams, loadMxDataParams);

说明：A矩阵转置输入[k, m]时，`LoadData2DParamsV2`的ifTranspose设置为true，表示从L1 -> L0A每个小分型进行转置。由于转置搬运时m方向多搬运数据超过1个分型，后续`Mmad`指令需要特殊处理（详见第4节）。注意mStep和kStep的含义变化：A矩阵转置输入[k, m]时，L1上A矩阵shape为[kaAlignL1, mAlignL1]，此时mStep对应K方向的分形数量，kStep对应M方向的分形数量。

##### 3.1.3. A矩阵转置输入[k, m]，for循环调用（场景5/6）

当A矩阵转置输入[k, m]时，若单次调用`LoadData`会导致m方向多搬脏数据超过1个分型，可使用for循环方式避免搬运多余的脏数据。每次for循环在L0A上跳过m方向尾脏数据分型，m方向多搬运数据不超过1个分型。

            uint16_t mStepAlign = isFP4 ? FP4_M_STEP_ALIGN : FP8_M_STEP_ALIGN;
            AscendC::LoadData2DParamsV2 loadDataParams;
            loadDataParams.sid = 0;
            loadDataParams.kStartPosition = 0;
            loadDataParams.mStep = mStepAlign;
            loadDataParams.kStep = CeilDivision(mAlignL0, fractalShape[1]);
            loadDataParams.srcStride = CeilDivision(kaAlignL1, fractalShape[0]);
            loadDataParams.dstStride = CeilDivision(mAlignL0, fractalShape[0]);
            loadDataParams.ifTranspose = true;

            AscendC::LoadData2DMxParams loadMxDataParams;
            loadMxDataParams.yStartPosition = 0;
            loadMxDataParams.xStep = mStepAlign;
            loadMxDataParams.yStep = CeilDivision(packedK, SCALE_CEIL_NUMBER);
            loadMxDataParams.srcStride = scaleK;
            loadMxDataParams.dstStride = CeilDivision(packedK, SCALE_CEIL_NUMBER);

            uint32_t dstOffset = 0;
            uint16_t L0ALoopNum = CeilDivision(kaAlignL0, fractalShape[0] * fractalNum);
            for (uint16_t loopIdx = 0; loopIdx < L0ALoopNum; ++loopIdx) {
                loadDataParams.mStartPosition = mStepAlign * loopIdx;
                loadMxDataParams.xStartPosition = mStepAlign * loopIdx;
                AscendC::LoadData(a2Local[dstOffset], a1Local, scaleA1Local, loadDataParams, loadMxDataParams);
                dstOffset += CeilAlign(mAlignL0, fractalShape[0]) * fractalShape[1];
            }

说明：FP4数据类型下mStepAlign=4，FP8数据类型下mStepAlign=2。for循环次数L0ALoopNum = CeilDivision(kaAlignL0, fractalShape[0] * fractalNum)，每次循环搬运mStepAlign个分形的M方向数据。mStartPosition和xStartPosition随loopIdx递增，表示每次循环的起始位置偏移。dstOffset记录L0A上每次循环的目的地址偏移。

#### 3.2. B矩阵 L1->L0B，scaleB矩阵 L1->L0B_MX

B矩阵的搬运方式与A矩阵类似，但B矩阵在L0B上的排布格式为Zn。单次调用`LoadData`并传入`LoadData2DParamsV2`和`LoadData2DMxParams`两个结构体参数，同时完成B矩阵到L0B、scaleB矩阵到L0B_MX的搬运：

            AscendC::LoadData2DParamsV2 loadDataParams;
            loadDataParams.sid = 0;
            loadDataParams.mStartPosition = 0;
            loadDataParams.kStartPosition = 0;

            AscendC::LoadData2DMxParams loadMxDataParams;
            loadMxDataParams.xStartPosition = 0;
            loadMxDataParams.yStartPosition = 0;

            if constexpr (isBtranspose) {
                // B矩阵转置输入[n, k]，L1->L0B不需要转置
                loadDataParams.mStep = CeilDivision(nAlignL1, fractalShape[0]);
                loadDataParams.kStep = CeilDivision(kbAlignL1, fractalShape[1]);
                loadDataParams.srcStride = CeilDivision(nAlignL1, fractalShape[0]);
                loadDataParams.dstStride = CeilDivision(nAlignL0, fractalShape[0]);
                loadDataParams.ifTranspose = false;
            } else {
                // B矩阵不转置输入[k, n]，L1->L0B需要转置，发生小分型转置和大分型排布格式变化
                loadDataParams.mStep = CeilDivision(kbAlignL1, fractalShape[0]);
                loadDataParams.kStep = CeilDivision(nAlignL1, fractalShape[1]);
                loadDataParams.srcStride = CeilDivision(kbAlignL1, fractalShape[0]);
                loadDataParams.dstStride = CeilDivision(nAlignL0, fractalShape[0]);
                loadDataParams.ifTranspose = true;
            }
            loadMxDataParams.xStep = CeilDivision(scaleNAlignL1, fractalShape[0]);
            loadMxDataParams.yStep = CeilDivision(packedK, SCALE_CEIL_NUMBER);
            loadMxDataParams.srcStride = scaleK;
            loadMxDataParams.dstStride = CeilDivision(packedK, SCALE_CEIL_NUMBER);

            AscendC::LoadData(b2Local, b1Local, scaleB1Local, loadDataParams, loadMxDataParams);

说明：B矩阵转置输入[n, k]时，`LoadData2DParamsV2`的ifTranspose=false；B矩阵不转置输入[k, n]时，`LoadData2DParamsV2`的ifTranspose=true，发生小分型转置和大分型排布格式变化。B矩阵不转置输入[k, n]时，n方向多搬运数据不超过1个分型。`LoadData2DMxParams`参数配置与A矩阵类似，xStep对应scaleB矩阵M方向（对于L0B_MX为K方向）的分形数量。

### 4.矩阵乘 (`Mmad`)
下面将介绍如何配置`Mmad`指令的MmadParams结构体的成员。

MX矩阵乘法公式为 C = (scaleA ⊗ A) * (scaleB ⊗ B)，`Mmad`指令会自动完成左/右矩阵与对应scale矩阵的广播乘法，K方向上每32个元素共享一个量化因子。

需要注意的是，与load_data_l12l0样例类似，当L0A/L0B上对A矩阵和B矩阵在各个轴的实际对齐要求与`Mmad`指令默认的对齐要求不一致时，就可能导致连续读入分型时，错误读入完全由无效数据填充的分型而忽略了包含有效数据的分型。

对于场景2和4（A矩阵转置输入[k, m]，单次调用`LoadData`），由于A矩阵转置搬运过程中m方向多搬脏数据超过1个分型，如果仍然设置mmadParams.m = m，cube单元会多读入无效数据的分型同时有效数据的分型也未被读入。此时可以通过设置mmadParams.m = CeilAlign(m, fractalShape[0] * fractalNum)，让此分型参与计算，搬出时跳过无效分型参与计算的结果即可。

            AscendC::MmadParams mmadParams;
            if constexpr (scenarioNum == 2 || scenarioNum == 4) {
                // mmad默认m轴向16对齐，但由于A转置过程m轴向fractalShape[0]*fractalNum对齐，
                // 填充了全部由无效数据组成的分型，设置m向fractalShape[0]*fractalNum对齐，
                // 让此分型参与计算，搬出时跳过无效分型参与计算的结果即可
                mmadParams.m = CeilAlign(m, fractalShape[0] * fractalNum);
            } else {
                mmadParams.m = m;
            }
            mmadParams.n = n;
            mmadParams.k = alignK;
            mmadParams.cmatrixInitVal = true;
            AscendC::Mmad(c1Local, a2Local, b2Local, mmadParams);

说明：mmadParams.k取alignK = CeilAlign(k, 64) = 128，而非原始的k=70，这是因为MX矩阵乘法要求K轴向64对齐。mmadParams.cmatrixInitVal = true表示初始化C矩阵。

对于场景5和6（A矩阵转置输入[k, m]，for循环调用`LoadData`），由于使用for循环方式避免了多搬脏数据超过1个分型的情况，m方向多搬运数据不超过1个分型，`Mmad`计算量为CeilAlign(m, fractalShape[0]) * CeilAlign(n, fractalShape[0] * fractalNum)，此时mmadParams.m = m即可。

### 5.L0C到GM (`Fixpipe`)
下面将介绍如何配置`Fixpipe`指令的FixpipeParamsArch3510结构体的成员。FixpipeParamsArch3510是dav-3510架构专用的Fixpipe参数结构体，CO2Layout设置为ROW_MAJOR表示输出为行主序ND格式。

            AscendC::FixpipeParamsArch3510<AscendC::CO2Layout::ROW_MAJOR> fixpipeParams;
            fixpipeParams.nSize = n;
            fixpipeParams.mSize = m;
            if constexpr (scenarioNum == 2 || scenarioNum == 4){
                // 场景2/4：A矩阵转置输入[k, m]单次调用，m方向多搬脏数据超过1个分型
                fixpipeParams.srcStride = CeilAlign(m, fractalShape[0] * fractalNum);
            } else {
                fixpipeParams.srcStride = CeilAlign(m, fractalShape[0]);
            }
            fixpipeParams.dstStride = n;
            AscendC::Fixpipe<U, U, AscendC::CFG_ROW_MAJOR>(cGM, c1Local, fixpipeParams);

说明：fixpipeParams.srcStride的单位是元素，其含义是源Nz矩阵中相邻Z排布的起始地址偏移。对于场景2/4，srcStride需要与`Mmad`的m对齐值一致，取CeilAlign(m, fractalShape[0] * fractalNum)；其余场景取CeilAlign(m, fractalShape[0])。fixpipeParams.dstStride = n表示目标ND矩阵中相邻行的起始地址偏移。fixpipeParams.nSize = n和fixpipeParams.mSize = m保证只搬出有效数据，跳过无效数据参与计算的结果。

## 编译运行
在本样例根目录下执行如下步骤，编译并执行样例。
- 配置环境变量
  请根据当前环境上CANN开发套件包的[安装方式](../../../../../../docs/quick_start.md#prepare&install)，选择对应配置环境变量的命令。
  - 默认路径，root用户安装CANN软件包
    ```bash
    source /usr/local/Ascend/cann/set_env.sh
    ```

  - 默认路径，非root用户安装CANN软件包
    ```bash
    source $HOME/Ascend/cann/set_env.sh
    ```

  - 指定路径install_path，安装CANN软件包
    ```bash
    source ${install_path}/cann/set_env.sh
    ```

  - 使用 ks_cann 环境（支持 dav-3510 架构）
    ```bash
    source /mnt/workspace/gitCode/cann/ks_cann/cann/set_env.sh
    ```

- 样例执行
  ```bash
  SCENARIO=1
  mkdir -p build && cd build;      # 创建并进入build目录
  cmake .. -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=$SCENARIO;make -j;    # 编译工程，默认npu模式
  python3 ../scripts/gen_data.py -scenarioNum=$SCENARIO   # 生成测试输入数据
  ./demo                           # 执行编译生成的可执行程序，执行样例
  python3 ../scripts/verify_result.py -scenarioNum=$SCENARIO output/output.bin output/golden.bin   # 验证输出结果是否正确，确认算法逻辑正确
  ```

  使用 CPU调试 或 NPU仿真 模式时，添加 `-DCMAKE_ASC_RUN_MODE=cpu` 或 `-DCMAKE_ASC_RUN_MODE=sim` 参数即可。

  示例如：
  ```bash
  cmake .. -DCMAKE_ASC_RUN_MODE=cpu -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=$SCENARIO;make -j;   # CPU调试模式
  cmake .. -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=$SCENARIO;make -j;   # NPU仿真模式
  ```

  > **注意：** 切换编译模式前需清理 cmake 缓存，可在 build 目录下执行 `rm CMakeCache.txt` 后重新 cmake。

- 编译选项说明

  | 参数 | 说明 | 可选值 | 默认值 |
  |------|------|--------|--------|
  | CMAKE_ASC_RUN_MODE | 运行模式 | npu, cpu, sim | npu |
  | CMAKE_ASC_ARCHITECTURES | NPU硬件架构 | dav-3510 | dav-3510 |
  | SCENARIO_NUM | 场景编号 | 1-6 | 1 |

- 执行结果

  执行结果如下，说明精度对比成功。
  ```bash
  test pass!
  ```
