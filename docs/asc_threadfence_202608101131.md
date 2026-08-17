# asc_threadfence

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

调用`asc_threadfence`函数后，调用线程在该函数调用点之前对所有共享内存和全局内存执行的写入操作，对调用点之后的所有其他线程可见。该函数在调用点插入一个全设备范围的内存栅栏，保证栅栏之前的所有内存写操作在栅栏之后的任何内存操作之前完成并可见，防止编译器或硬件对内存访问指令进行重排序优化导致的数据不一致问题。使用时在需要保证内存写入可见性的代码位置直接调用`asc_threadfence()`即可，无需传入任何参数。

计算公式如下：

$$
\text{Write}_{\text{before}} \prec \text{asc\_threadfence} \prec \text{Read/Write}_{\text{after}}
$$

其中，$\text{Write}_{\text{before}}$为调用线程在栅栏之前执行的所有共享内存和全局内存写入操作，$\prec$表示"happens-before"（先于）关系，$\text{Read/Write}_{\text{after}}$为栅栏之后的任意线程的内存读写操作。该关系保证栅栏之前的写入操作对栅栏之后的所有线程可见。

## 函数原型

```cpp
__SIMT_DEVICE_FUNCTIONS_DECL__ inline void asc_threadfence()
```

## 参数说明

无参数。

## 返回值说明

无。

## 约束说明

- 该函数仅在<term>Ascend 950PR/Ascend 950DT</term>上可用，其余产品不支持。
- 该函数为SIMT设备侧函数，在SIMT编程模型的线程上下文中调用，用于线程间的内存可见性同步。与`asc_syncthreads`（线程块内同步）不同，`asc_threadfence`提供的是内存操作顺序保证，不阻塞线程执行。
- 该函数无操作数参数，不涉及地址对齐要求和地址复用约束。
- 该函数不修改全局状态（如mask寄存器、向量寄存器），调用后无需恢复环境。
- 该函数是否同时支持Kernel直调开发方式和工程化开发方式：【待开发补齐】。

## 调用示例

【示例为基于函数原型构造】

```cpp
// SIMT编程模型下，线程0写入共享数据后调用asc_threadfence确保写入对其他线程可见
// 线程1在asc_syncthreads同步后读取数据，此时可看到线程0的写入结果

extern "C" __global__ void ThreadFenceKernel(__gm__ uint32_t* gmOutput) {
    // 线程块内共享内存（通过SIMT机制）
    __shared__ uint32_t sharedData[64];

    uint32_t tid = AscendC::Simt::GetThreadId();

    if (tid == 0) {
        // 线程0写入共享数据
        sharedData[0] = 0x1234;

        // 调用asc_threadfence，确保上述写入对设备上所有线程可见
        asc_threadfence();
    }

    // 线程块内同步，确保线程0已完成写入
    asc_syncthreads();

    if (tid == 1) {
        // 线程1读取共享数据，此时可看到线程0的写入结果
        uint32_t value = sharedData[0];

        // 将结果写回Global Memory
        gmOutput[0] = value;
    }
}
```

结果示例如下：

```text
输入数据(gmOutput): [0]
输出数据(gmOutput): [4660]
```
