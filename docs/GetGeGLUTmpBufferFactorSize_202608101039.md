# GetGeGLUTmpBufferFactorSize

## 产品支持情况

<table>
<thead align="left">
<tr id="row_support">
  <th class="cellrowborder" valign="top" width="57.99999999999999%" id="mcps1.1.3.1.1">
    <p id="p_product"><a name="p_product"></a><a name="p_product"></a><span id="ph_product_title"><a name="ph_product_title"></a><a name="ph_product_title"></a>产品</span></p>
  </th>
  <th class="cellrowborder" align="center" valign="top" width="42%" id="mcps1.1.3.1.2">
    <p id="p_support_status"><a name="p_support_status"></a><a name="p_support_status"></a>是否支持</p>
  </th>
</tr>
</thead>
<tbody>
<tr id="row_ascend910">
  <td class="cellrowborder" valign="top" width="57.99999999999999%" headers="mcps1.1.3.1.1 ">
    <p id="p_ascend910"><a name="p_ascend910"></a><a name="p_ascend910"></a><span id="ph_ascend910"><a name="ph_ascend910"></a><a name="ph_ascend910"></a><term id="term_ascend_950">Ascend 950PR/Ascend 950DT</term></span></p>
  </td>
  <td class="cellrowborder" align="center" valign="top" width="42%" headers="mcps1.1.3.1.2 ">
    <p id="p_support_ascend910"><a name="p_support_ascend910"></a><a name="p_support_ascend910"></a>√</p>
  </td>
</tr>
<tr id="row_atlas_a3">
  <td class="cellrowborder" valign="top" width="57.99999999999999%" headers="mcps1.1.3.1.1 ">
    <p id="p_atlas_a3"><a name="p_atlas_a3"></a><a name="p_atlas_a3"></a><span id="ph_atlas_a3"><a name="ph_atlas_a3"></a><a name="ph_atlas_a3"></a><term id="term_atlas_a3_training">Atlas A3 训练系列产品</term>/<term id="term_atlas_a3_inference">Atlas A3 推理系列产品</term></span></p>
  </td>
  <td class="cellrowborder" align="center" valign="top" width="42%" headers="mcps1.1.3.1.2 ">
    <p id="p_support_atlas_a3"><a name="p_support_atlas_a3"></a><a name="p_support_atlas_a3"></a>√</p>
  </td>
</tr>
<tr id="row_atlas_a2">
  <td class="cellrowborder" valign="top" width="57.99999999999999%" headers="mcps1.1.3.1.1 ">
    <p id="p_atlas_a2"><a name="p_atlas_a2"></a><a name="p_atlas_a2"></a><span id="ph_atlas_a2"><a name="ph_atlas_a2"></a><a name="ph_atlas_a2"></a><term id="term_atlas_a2_training">Atlas A2 训练系列产品</term>/<term id="term_atlas_a2_inference">Atlas A2 推理系列产品</term></span></p>
  </td>
  <td class="cellrowborder" align="center" valign="top" width="42%" headers="mcps1.1.3.1.2 ">
    <p id="p_support_atlas_a2"><a name="p_support_atlas_a2"></a><a name="p_support_atlas_a2"></a>√</p>
  </td>
</tr>
<tr id="row_atlas_200i_500">
  <td class="cellrowborder" valign="top" width="57.99999999999999%" headers="mcps1.1.3.1.1 ">
    <p id="p_atlas_200_i_500"><a name="p_atlas_200_i_500"></a><a name="p_atlas_200_i_500"></a><span id="ph_atlas_200_i_500"><a name="ph_atlas_200_i_500"></a><a name="ph_atlas_200_i_500"></a><term id="term_atlas_200i_500">Atlas 200I/500 A2 推理产品</term></span></p>
  </td>
  <td class="cellrowborder" align="center" valign="top" width="42%" headers="mcps1.1.3.1.2 ">
    <p id="p_support_atlas_200_i_500"><a name="p_support_atlas_200_i_500"></a><a name="p_support_atlas_200_i_500"></a>√</p>
  </td>
</tr>
<tr id="row_atlas_inference">
  <td class="cellrowborder" valign="top" width="57.99999999999999%" headers="mcps1.1.3.1.1 ">
    <p id="p_atlas_inference"><a name="p_atlas_inference"></a><a name="p_atlas_inference"></a><span id="ph_atlas_inference"><a name="ph_atlas_inference"></a><a name="ph_atlas_inference"></a><term id="term_atlas_inference">Atlas 推理系列产品</term></span></p>
  </td>
  <td class="cellrowborder" align="center" valign="top" width="42%" headers="mcps1.1.3.1.2 ">
    <p id="p_support_atlas_inference"><a name="p_support_atlas_inference"></a><a name="p_support_atlas_inference"></a>√</p>
  </td>
</tr>
<tr id="row_atlas_training">
  <td class="cellrowborder" valign="top" width="57.99999999999999%" headers="mcps1.1.3.1.1 ">
    <p id="p_atlas_training"><a name="p_atlas_training"></a><a name="p_atlas_training"></a><span id="ph_atlas_training"><a name="ph_atlas_training"></a><a name="ph_atlas_training"></a><term id="term_atlas_training">Atlas 训练系列产品</term></span></p>
  </td>
  <td class="cellrowborder" align="center" valign="top" width="42%" headers="mcps1.1.3.1.2 ">
    <p id="p_support_atlas_training"><a name="p_support_atlas_training"></a><a name="p_support_atlas_training"></a>√</p>
  </td>
</tr>
</tbody>
</table>

## 功能说明

获取GeGLU算子临时空间计算的因子参数`maxLiveNodeCount`和`extraBuf`。GeGLU接口在计算时需要开发者预留或申请临时空间，最大临时空间与输入占用空间之间的关系如下：

$$
maxTmpBuffer = maxLiveNodeCount \times inputSize \times typeSize + extraBuf
$$

其中，`maxTmpBuffer`为GeGLU接口所需的最大临时空间大小（字节），`maxLiveNodeCount`为最大临时空间相对于输入占用空间的倍数（由本接口输出），`inputSize`为输入数据的元素个数，`typeSize`为输入数据类型的大小（字节），`extraBuf`为额外临时空间大小（字节，由本接口输出）。

开发者在Host侧Tiling函数中调用本接口，传入输入数据类型大小`typeSize`，获取`maxLiveNodeCount`和`extraBuf`后，按上述公式计算所需的最大临时空间大小，作为Tiling参数传入GeGLU Kernel接口。关联接口`GetGeGLUMaxMinTmpSize`可直接获取最大和最小临时空间值。

本接口输出的因子值取决于`typeSize`：当`typeSize`为2（half）时，`maxLiveNodeCount`为4，`extraBuf`为0；当`typeSize`为4（float）时，`maxLiveNodeCount`为0，`extraBuf`为0。

## 函数原型

```cpp
void GetGeGLUTmpBufferFactorSize(const uint32_t typeSize, uint32_t& maxLiveNodeCount, uint32_t& extraBuf)
```

## 参数说明

**表 1**  接口参数说明

<table>
<thead align="left">
<tr>
  <th class="cellrowborder" valign="top" width="18.36%"><p>参数名</p></th>
  <th class="cellrowborder" valign="top" width="10.09%"><p>输入/输出</p></th>
  <th class="cellrowborder" valign="top" width="71.55%"><p>描述</p></th>
</tr>
</thead>
<tbody>
<tr>
  <td class="cellrowborder" valign="top"><p>typeSize</p></td>
  <td class="cellrowborder" valign="top"><p>输入</p></td>
  <td class="cellrowborder" valign="top"><p>输入数据类型的大小，单位为字节。对应GeGLU接口模板参数T的sizeof值。取值必须为2（half）或4（float），非法值将触发Host侧校验报错。</p></td>
</tr>
<tr>
  <td class="cellrowborder" valign="top"><p>maxLiveNodeCount</p></td>
  <td class="cellrowborder" valign="top"><p>输出</p></td>
  <td class="cellrowborder" valign="top"><p>最大临时空间相对于输入占用空间的倍数。开发者使用此值按公式<code>maxTmpBuffer = maxLiveNodeCount * inputSize * typeSize + extraBuf</code>计算最大临时空间。当typeSize为2（half）时输出值为4，当typeSize为4（float）时输出值为0。该参数由接口内部覆盖写入，调用前无需初始化。</p></td>
</tr>
<tr>
  <td class="cellrowborder" valign="top"><p>extraBuf</p></td>
  <td class="cellrowborder" valign="top"><p>输出</p></td>
  <td class="cellrowborder" valign="top"><p>额外临时空间大小，单位为字节。开发者使用此值按公式<code>maxTmpBuffer = maxLiveNodeCount * inputSize * typeSize + extraBuf</code>计算最大临时空间。固定输出值为0。该参数由接口内部覆盖写入，调用前无需初始化。</p></td>
</tr>
</tbody>
</table>

## 返回值说明

无。

## 约束说明

- `typeSize`取值必须为2（half）或4（float），与关联的GeGLU Kernel API支持的数据类型一致。传入其他值将触发Host侧校验报错。
- 本接口返回的`maxLiveNodeCount`和`extraBuf`用于计算GeGLU Kernel接口所需的临时空间大小。计算公式为`maxTmpBuffer = maxLiveNodeCount * inputSize * typeSize + extraBuf`，其中`inputSize`为输入数据的元素个数。开发者应在Host侧Tiling函数中调用本接口获取因子后计算空间大小，并将其作为Tiling参数传入Kernel。
- `maxLiveNodeCount`和`extraBuf`均为输出参数，由接口内部覆盖写入，调用前无需清零初始化。
- 本接口在所有支持的产品上行为一致，无芯片差异。

## 调用示例

【示例为基于函数原型构造】

```cpp
#include "adv_api/activation/geglu_tiling.h"

// Host侧Tiling函数中调用GetGeGLUTmpBufferFactorSize
void TilingFunc(uint32_t inputSize)
{
    uint32_t typeSize = sizeof(half);  // 输入数据类型为half，typeSize为2
    uint32_t maxLiveNodeCount;         // 输出参数，由接口内部覆盖写入，无需初始化
    uint32_t extraBuf;                 // 输出参数，由接口内部覆盖写入，无需初始化

    // 获取临时空间因子
    AscendC::GetGeGLUTmpBufferFactorSize(typeSize, maxLiveNodeCount, extraBuf);

    // 计算最大临时空间大小
    uint32_t maxTmpBuffer = maxLiveNodeCount * inputSize * typeSize + extraBuf;
    // maxTmpBuffer = 4 * inputSize * 2 + 0 = 8 * inputSize（字节）

    // 将maxTmpBuffer作为Tiling参数传入GeGLU Kernel接口
}
```

结果示例如下：

```text
输入数据(typeSize=2, inputSize=128): half类型，128个元素
输出数据(maxLiveNodeCount=4, extraBuf=0): maxTmpBuffer = 4 * 128 * 2 + 0 = 1024字节
```
