# asc_syncthreads

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

asc_syncthreads是SIMT（Single Instruction, Multiple Threads）编程模型中的线程块内同步接口。该接口在当前线程块内设置屏障，调用该接口的线程将在此处等待，直到线程块内的所有线程都到达该屏障点后，所有线程才能继续执行后续指令。使用后，线程块内所有线程在屏障点之前的内存写入操作对线程块内所有线程可见。在SIMT Kernel函数中，当需要确保线程块内所有线程完成某阶段计算后再进入下一阶段时，在阶段边界处调用asc_syncthreads()即可。

计算公式如下：

$$
\text{Barrier}(\text{Block}_{\text{current}})
$$

其中，$\text{Block}_{\text{current}}$为当前线程所属的线程块，$\text{Barrier}$表示屏障同步操作——线程块内所有线程到达该同步点后方可继续执行。

## 函数原型

```cpp
__SIMT_DEVICE_FUNCTIONS_DECL__ inline void asc_syncthreads()
```

## 参数说明

无参数。

## 返回值说明

无。

## 约束说明

- 该接口仅在<term>Ascend 950PR/Ascend 950DT</term>上可用，其他产品不支持。
- 该接口为SIMT编程模型专用接口，需在SIMT Kernel函数中调用。
- 该接口同步范围为当前线程块内的所有线程，不跨线程块同步。
- 一个线程块最大支持2048个线程，调用asc_syncthreads时线程块内所有线程均需到达同步点。
- 函数声明包含`__SIMT_DEVICE_FUNCTIONS_DECL__`限定符，表明该接口为设备侧SIMT调用接口。

## 调用示例

【示例为基于函数原型构造】

```cpp
#include "simt_api/device_sync_functions.h"

// SIMT Kernel函数示例
__aicore__ void MySimtKernel(__gm__ uint8_t* gmInput, __gm__ uint8_t* gmOutput)
{
    // 阶段1：各线程独立完成数据加载和计算
    // ... 线程块内各线程执行各自的计算任务 ...

    // 线程块内同步：确保所有线程完成阶段1的计算
    asc_syncthreads();

    // 阶段2：同步后所有线程可见阶段1的写入结果
    // ... 线程块内各线程继续执行后续计算 ...
}
```

结果示例如下：

```text
输入数据: 线程块内各线程加载各自数据
输出数据: 阶段1完成后经asc_syncthreads同步，阶段2可见阶段1的写入结果
```
