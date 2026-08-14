# GetGeGLUMaxMinTmpSize

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
    <p id="p_atlas_200i_500"><a name="p_atlas_200i_500"></a><a name="p_atlas_200i_500"></a><span id="ph_atlas_200i_500"><a name="ph_atlas_200i_500"></a><a name="ph_atlas_200i_500"></a><term id="term_atlas_200i_500">Atlas 200I/500 A2 推理产品</term></span></p>
  </td>
  <td class="cellrowborder" align="center" valign="top" width="42%" headers="mcps1.1.3.1.2 ">
    <p id="p_support_atlas_200i_500"><a name="p_support_atlas_200i_500"></a><a name="p_support_atlas_200i_500"></a>√</p>
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

获取GeGLU算子计算所需的临时空间大小的最大值和最小值。开发者在Tiling阶段调用此接口获取临时空间范围，根据实际可用空间大小选择合适的值作为Tiling参数，传入Kernel侧GeGLU接口的sharedTmpBuffer参数。

GeGLU是采用GELU作为激活函数的GLU（Gated Linear Unit）变体，计算公式为GeGLU(x1, x2) = x1 * GELU(x2)。对于half数据类型，底层实现将half转换为float进行高精度计算，需要额外的临时空间；对于float数据类型，底层实现直接在目的操作数上原地计算，不需要额外临时空间。

计算公式如下：

$$
maxValue = calcPro \times \max(inputSize \times typeSize,\ 256)
$$

$$
minValue = calcPro \times 256
$$

其中，$inputSize$为输入Tensor的元素个数，等于`srcShape.GetShapeSize()`；$typeSize$为输入数据类型的大小（字节），取值2（half）或4（float）；$calcPro$为临时空间计算因子，$typeSize = 4$（float）时$calcPro = 0$，$typeSize = 2$（half）时$calcPro = 4$；256为单次矢量计算Repeat的字节大小；$maxValue$为最大临时空间大小（字节），通过输出参数返回；$minValue$为最小临时空间大小（字节），通过输出参数返回。

## 函数原型

```cpp
void GetGeGLUMaxMinTmpSize(
    const AscendC::TensorShape& srcShape,
    const uint32_t typeSize,
    const bool isReuseSource,
    uint32_t& maxValue,
    uint32_t& minValue
)
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
  <td class="cellrowborder" valign="top"><p>srcShape</p></td>
  <td class="cellrowborder" valign="top"><p>输入</p></td>
  <td class="cellrowborder" valign="top"><p>输入Tensor的shape信息。类型为<code>AscendC::TensorShape</code>，即<code>ge::Shape</code>的类型别名。</p><p>约束：<code>srcShape.GetShapeSize()</code>必须大于0。</p></td>
</tr>
<tr>
  <td class="cellrowborder" valign="top"><p>typeSize</p></td>
  <td class="cellrowborder" valign="top"><p>输入</p></td>
  <td class="cellrowborder" valign="top"><p>输入数据类型的大小。单位：字节。</p><p>取值范围：{2, 4}，其中2对应half，4对应float。</p></td>
</tr>
<tr>
  <td class="cellrowborder" valign="top"><p>isReuseSource</p></td>
  <td class="cellrowborder" valign="top"><p>输入</p></td>
  <td class="cellrowborder" valign="top"><p>是否复用源操作数的输入空间。</p><p>取值范围：{true, false}。</p><p>该参数预留，暂未启用，为后续的功能扩展做保留，传入默认值false即可。传入true不会报错但不产生任何效果，仅打印WARNING日志。</p></td>
</tr>
<tr>
  <td class="cellrowborder" valign="top"><p>maxValue</p></td>
  <td class="cellrowborder" valign="top"><p>输出</p></td>
  <td class="cellrowborder" valign="top"><p>计算所需的最大临时空间大小。单位：字节。开发者可在此值与minValue之间选择合适的临时空间大小作为Tiling参数。函数内部直接赋值覆盖，调用前无需初始化。</p></td>
</tr>
<tr>
  <td class="cellrowborder" valign="top"><p>minValue</p></td>
  <td class="cellrowborder" valign="top"><p>输出</p></td>
  <td class="cellrowborder" valign="top"><p>计算所需的最小临时空间大小。单位：字节。此值为保证GeGLU接口正常工作的最小临时空间需求。函数内部直接赋值覆盖，调用前无需初始化。</p></td>
</tr>
</tbody>
</table>

## 返回值说明

无

## 约束说明

- 对于half数据类型（typeSize=2），maxValue = 4 × max(inputSize × 2, 256)，minValue = 1024。开发者需在minValue到maxValue之间选择合适的临时空间大小，传入Kernel侧GeGLU接口的sharedTmpBuffer参数。临时空间大小不能小于minValue，否则GeGLU计算可能产生精度问题或计算错误。
- 对于float数据类型（typeSize=4），maxValue = 0，minValue = 0，表示不需要额外临时空间。Kernel侧GeGLU接口可直接使用不传sharedTmpBuffer的重载，或传入空临时空间。
- 本接口为Host侧Tiling函数，在TilingFunc中调用，不涉及Kernel侧执行单元约束。
- 本接口为Host侧Tiling函数，不涉及寄存器状态修改或全局状态变更，调用后无需恢复环境。

## 调用示例

```cpp
AscendC::LocalTensor<T> dstLocal = outQueue.AllocTensor<T>();
        AscendC::LocalTensor<T> src0Local = inQueue0.DeQue<T>();
        AscendC::LocalTensor<T> src1Local = inQueue1.DeQue<T>();
        AscendC::LocalTensor<uint8_t> tmpLocal;
        if (tmpBufSize > 0) {
            tmpLocal = bufQueue.Get<uint8_t>();
        }
        // 调用GeGLU高阶API，对src0和src1执行GeGLU计算：dst = src0 * GELU(src1)
        if ((tmpBufSize > 0) && (calCount > 0)) {
            AscendC::GeGLU<T, false>(dstLocal, src0Local, src1Local, tmpLocal, calCount);
        } else if (tmpBufSize > 0) {
            AscendC::GeGLU<T, false>(dstLocal, src0Local, src1Local, tmpLocal);
        } else if (calCount > 0) {
            AscendC::GeGLU<T, false>(dstLocal, src0Local, src1Local, calCount);
        } else {
            AscendC::GeGLU<T, false>(dstLocal, src0Local, src1Local);
        }
        outQueue.EnQue<T>(dstLocal);
        inQueue0.FreeTensor(src0Local);
        inQueue1.FreeTensor(src1Local);

```

结果示例如下：

```text
输入数据：【待开发补齐】
输出数据：【待开发补齐】
```
