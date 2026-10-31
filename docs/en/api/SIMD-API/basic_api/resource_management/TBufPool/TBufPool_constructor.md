# TBufPool Constructor<a name="ZH-CN_TOPIC_0000002019602020"></a>

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-09-23T17:36:53.035Z pushedAt=2026-10-26T07:41:44.666Z nl=2 -->

## Applicable Products

<!-- npu="950" id1 -->
- Ascend 950PR&950DT: supported
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3: supported
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2: supported
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2 inference products: not supported
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas inference products AI Core: supported
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas inference products Vector Core: not supported
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas training products: supported
<!-- end id7 -->
<!-- @ref: asc-devkit/res/docs/zh/api/SIMD-API/basic_api/resource_management/TBufPool/TBufPool_constructor_res.md#id1 -->

## Description<a name="zh-cn_topic_0000001935531088_zh-cn_topic_0000001339105488_section36583473819"></a>

The header file path is `"basic_api/kernel_tpipe.h"`.

Initializes the data members when creating a **TBufPool** object.

## Prototype<a name="zh-cn_topic_0000001935531088_zh-cn_topic_0000001339105488_section13230182415108"></a>

```cpp
template <TPosition pos, uint32_t bufIDSize = defaultBufIDSize>
__aicore__ inline TBufPool();
```

## Parameters<a name="zh-cn_topic_0000001935531088_zh-cn_topic_0000001339105488_section75395119104"></a>

**Table 1** Template parameter description

<a name="table1550165916920"></a>
<table><thead align="left"><tr id="row115015591391"><th class="cellrowborder" valign="top" width="14.099999999999998%" id="mcps1.2.3.1.1"><p id="p12501159099"><a name="p12501159099"></a><a name="p12501159099"></a>Parameter</p></th>
<th class="cellrowborder" valign="top" width="85.9%" id="mcps1.2.3.1.2"><p id="p85019592918"><a name="p85019592918"></a><a name="p85019592918"></a>Description</p></th>
</tr>
</thead>
<tbody><tr id="row1550117591914"><td class="cellrowborder" valign="top" width="14.099999999999998%" headers="mcps1.2.3.1.1 "><p id="p185019592913"><a name="p185019592913"></a><a name="p185019592913"></a>pos</p></td>
<td class="cellrowborder" valign="top" width="85.9%" headers="mcps1.2.3.1.2 "><p id="p35011591693"><a name="p35011591693"></a><a name="p35011591693"></a>Logical position of **TBufPool**, which can be <span>**VECIN**, **VECOUT**, </span>**VECCALC**, **A1**<span>, </span>**B1**, or **C1**.<span>For details about **TPosition**, see </span><a href="../../aux_data_structures/TPosition.md">TPosition</a>.</p></td>
</tr>
<tr id="row12501859799"><td class="cellrowborder" valign="top" width="14.099999999999998%" headers="mcps1.2.3.1.1 "><p id="p1650113599915"><a name="p1650113599915"></a><a name="p1650113599915"></a>bufIDSize</p></td>
<td class="cellrowborder" valign="top" width="85.9%" headers="mcps1.2.3.1.2 "><p id="p1450175912920"><a name="p1450175912920"></a><a name="p1450175912920"></a>Number of buffers that can be allocated by **TBufPool**, defaulting to 4 and not exceeding 16. For resource allocation in non-shared mode, when applying for another **TBufPool** on this **TBufPool**, the requested **bufIDSize** cannot exceed the number of remaining available buffers of the original **TBufPool**. For resource allocation in shared mode, when applying for another **TBufPool** on this **TBufPool**, the requested **bufIDSize** cannot exceed the number of buffers configured for the original **TBufPool**. For details about non-shared mode and shared mode, see <a href="./InitBufPool.md#description">InitBufPool Description</a>.</p></td>
</tr>
</tbody>
</table>

## Constraints<a name="zh-cn_topic_0000001935531088_zh-cn_topic_0000001339105488_section19165124931511"></a>

None
