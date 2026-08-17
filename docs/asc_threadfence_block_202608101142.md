# asc_threadfence_block

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
    <p id="p_support_atlas_a3"><a name="p_support_atlas_a3"></a><a name="p_support_atlas_a3"></a>×</p>
  </td>
</tr>
<tr id="row_atlas_a2">
  <td class="cellrowborder" valign="top" width="57.99999999999999%" headers="mcps1.1.3.1.1 ">
    <p id="p_atlas_a2"><a name="p_atlas_a2"></a><a name="p_atlas_a2"></a><span id="ph_atlas_a2"><a name="ph_atlas_a2"></a><a name="ph_atlas_a2"></a><term id="term_atlas_a2_training">Atlas A2 训练系列产品</term>/<term id="term_atlas_a2_inference">Atlas A2 推理系列产品</term></span></p>
  </td>
  <td class="cellrowborder" align="center" valign="top" width="42%" headers="mcps1.1.3.1.2 ">
    <p id="p_support_atlas_a2"><a name="p_support_atlas_a2"></a><a name="p_support_atlas_a2"></a>×</p>
  </td>
</tr>
<tr id="row_atlas_200i_500">
  <td class="cellrowborder" valign="top" width="57.99999999999999%" headers="mcps1.1.3.1.1 ">
    <p id="p_atlas_200i_500"><a name="p_atlas_200i_500"></a><a name="p_atlas_200i_500"></a><span id="ph_atlas_200i_500"><a name="ph_atlas_200i_500"></a><a name="ph_atlas_200i_500"></a><term id="term_atlas_200i_500">Atlas 200I/500 A2 推理产品</term></span></p>
  </td>
  <td class="cellrowborder" align="center" valign="top" width="42%" headers="mcps1.1.3.1.2 ">
    <p id="p_support_atlas_200i_500"><a name="p_support_atlas_200i_500"></a><a name="p_support_atlas_200i_500"></a>×</p>
  </td>
</tr>
<tr id="row_atlas_inference">
  <td class="cellrowborder" valign="top" width="57.99999999999999%" headers="mcps1.1.3.1.1 ">
    <p id="p_atlas_inference"><a name="p_atlas_inference"></a><a name="p_atlas_inference"></a><span id="ph_atlas_inference"><a name="ph_atlas_inference"></a><a name="ph_atlas_inference"></a><term id="term_atlas_inference">Atlas 推理系列产品</term></span></p>
  </td>
  <td class="cellrowborder" align="center" valign="top" width="42%" headers="mcps1.1.3.1.2 ">
    <p id="p_support_atlas_inference"><a name="p_support_atlas_inference"></a><a name="p_support_atlas_inference"></a>×</p>
  </td>
</tr>
<tr id="row_atlas_training">
  <td class="cellrowborder" valign="top" width="57.99999999999999%" headers="mcps1.1.3.1.1 ">
    <p id="p_atlas_training"><a name="p_atlas_training"></a><a name="p_atlas_training"></a><span id="ph_atlas_training"><a name="ph_atlas_training"></a><a name="ph_atlas_training"></a><term id="term_atlas_training">Atlas 训练系列产品</term></span></p>
  </td>
  <td class="cellrowborder" align="center" valign="top" width="42%" headers="mcps1.1.3.1.2 ">
    <p id="p_support_atlas_training"><a name="p_support_atlas_training"></a><a name="p_support_atlas_training"></a>×</p>
  </td>
</tr>
</tbody>
</table>

## 功能说明

在当前线程块（Thread Block）范围内执行内存栅栏操作，确保调用线程在调用此函数之前的所有内存写入操作对同一线程块内的其他线程可见。调用后，调用线程在栅栏之前执行的所有store操作将被刷新，使得同一线程块内的其他线程能够观察到这些写入的最新值。该栅栏作用范围限于当前线程块，不保证跨线程块的内存写入可见性。在需要保证线程块内内存可见性的位置直接调用 `asc_threadfence_block()` 即可，无需传入任何参数。

计算公式如下：
$$
\forall\, w \in W_{\text{before}},\, \forall\, t \in T_{\text{block}} \setminus \{t_{\text{caller}}\}: \text{Visible}(w, t)
$$

其中，$W_{\text{before}}$为调用线程在调用`asc_threadfence_block()`之前执行的所有内存写入操作集合，$T_{\text{block}}$为与调用线程处于同一线程块内的所有线程集合，$t_{\text{caller}}$为调用`asc_threadfence_block()`的线程，$\text{Visible}(w, t)$表示写入操作$w$的结果对线程$t$可见。

## 函数原型

```cpp
__SIMT_DEVICE_FUNCTIONS_DECL__ inline void asc_threadfence_block()
```

## 参数说明

无参数。

## 返回值说明

无。

## 约束说明

- 该函数仅在<term>Ascend 950PR/Ascend 950DT</term>上可用，其他产品不支持。
- `asc_threadfence_block`的内存栅栏作用范围限于当前线程块（Thread Block）内的线程，不保证跨线程块的内存写入可见性。如需全局内存栅栏，应使用`asc_threadfence`。
- 该函数为设备侧SIMT函数，仅在Kernel函数内的SIMT编程上下文中调用。

## 调用示例

【示例为基于函数原型构造】

```cpp
#include "simt_api/device_sync_functions.h"

__aicore__ extern "C" void KernelExample(__gm__ int32_t* gmInput, __gm__ int32_t* gmOutput)
{
    // 线程将计算结果写入Global Memory
    gmOutput[0] = gmInput[0] + 1;

    // 执行线程块内内存栅栏，确保上述写入对同一线程块内的其他线程可见
    asc_threadfence_block();

    // 后续同线程块内的其他线程可安全读取 gmOutput[0] 的最新值
}
```

结果示例如下：

```text
输入数据(gmInput[0]): [10]
输出数据(gmOutput[0]): [11]
调用asc_threadfence_block后，线程块内其他线程可观察到gmOutput[0]的写入值。
```
