# Mask_interface_summary

<!-- npu="A3,910b" id3 -->
以下内容针对如下型号生效：
<!-- npu="A3" id1 -->
- Atlas A3 训练系列产品/Atlas A3 推理系列产品
<!-- end id1 -->
<!-- npu="910b" id2 -->
- Atlas A2 训练系列产品/Atlas A2 推理系列产品
<!-- end id2 -->

本节汇总了各接口中模板参数isSetMask的支持情况。

针对Tensor前n个数计算和Tensorhigh_dim_split计算两种接口，Mask设置方式分为以下情形：

- isSetMask参数情况：

    - 模板参数有isSetMask，支持[接口外设置Mask](../SIMD-API/basic_api/memory_vector_compute/SIMD_compute/mask.md#接口外设置Mask)。

    - 模板参数无isSetMask，不支持接口外设置Mask。

    - 模板参数有isSetMask，该参数不起作用。

- Mask设置方式：

    - **接口内已设置Mask**：接口内部调用[SetMaskCount](../SIMD-API/basic_api/memory_vector_compute/mask_operations/SetMaskCount.md)、[SetVectorMask](../SIMD-API/basic_api/memory_vector_compute/mask_operations/SetVectorMask.md)、[SetMaskNorm](../SIMD-API/basic_api/memory_vector_compute/mask_operations/SetMaskNorm.md)、[ResetMask](../SIMD-API/basic_api/memory_vector_compute/mask_operations/ResetMask.md)。

    - **接口内通过模板参数控制**：接口内通过isSetMask模板参数控制如何设置Mask。isSetMask为true时表示在接口内设置Mask；isSetMask为false时表示在接口外设置Mask，开发者需使用[SetVectorMask](../SIMD-API/basic_api/memory_vector_compute/mask_operations/SetVectorMask.md)接口设置Mask值。

    - **接口不支持Mask或者无需设置Mask**。

更多Mask信息可参考[基于全局mask复用的计算性能优化](../../guide/算子实践参考/SIMD算子性能优化/矢量计算/基于全局mask复用的计算性能优化.md)。

注：表中的`-`表示不存在该类型接口。

**表 1**  各接口模板参数isSetMask支持情况汇总表

| 接口类别 | 接口名称 | Tensor前n个数计算 | Tensorhigh_dim_split计算 | 
| --- | --- | --- | --- |
| basic_arithmetic | [Add](../SIMD-API/basic_api/memory_vector_compute/basic_arithmetic/Add.md)、[Exp](../SIMD-API/basic_api/memory_vector_compute/basic_arithmetic/Exp.md)、[Ln](../SIMD-API/basic_api/memory_vector_compute/basic_arithmetic/Ln.md)、[Abs](../SIMD-API/basic_api/memory_vector_compute/basic_arithmetic/Abs.md)、[Reciprocal](../SIMD-API/basic_api/memory_vector_compute/basic_arithmetic/Reciprocal.md)、[Sqrt](../SIMD-API/basic_api/memory_vector_compute/basic_arithmetic/Sqrt.md)、[Rsqrt](../SIMD-API/basic_api/memory_vector_compute/basic_arithmetic/Rsqrt.md)、[Relu](../SIMD-API/basic_api/memory_vector_compute/basic_arithmetic/Relu.md)、[Sub](../SIMD-API/basic_api/memory_vector_compute/basic_arithmetic/Sub.md)、[Mul](../SIMD-API/basic_api/memory_vector_compute/basic_arithmetic/Mul.md)、[Div](../SIMD-API/basic_api/memory_vector_compute/basic_arithmetic/Div.md)、[Max](../SIMD-API/basic_api/memory_vector_compute/basic_arithmetic/Max.md)、[Min](../SIMD-API/basic_api/memory_vector_compute/basic_arithmetic/Min.md) | &bull; 无isSetMask。<br>&bull; 接口内已设置Mask。 | &bull; 有isSetMask。<br>&bull; 接口内通过模板参数控制。 |
| basic_arithmetic | [BilinearInterpolation](../SIMD-API/basic_api/memory_vector_compute/basic_arithmetic/BilinearInterpolation_ISASI.md) | - | &bull; 无isSetMask。<br>&bull; 接口内已设置Mask。 |
| basic_arithmetic | [Adds](../SIMD-API/basic_api/memory_vector_compute/basic_arithmetic/Adds.md)、[Muls](../SIMD-API/basic_api/memory_vector_compute/basic_arithmetic/Muls.md)、[Maxs](../SIMD-API/basic_api/memory_vector_compute/basic_arithmetic/Maxs.md)、[Mins](../SIMD-API/basic_api/memory_vector_compute/basic_arithmetic/Mins.md)、[LeakyRelu](../SIMD-API/basic_api/memory_vector_compute/basic_arithmetic/LeakyRelu.md) | &bull; 有isSetMask。<br>&bull; 接口内已设置Mask。 | &bull; 有isSetMask。<br>&bull; 接口内通过模板参数控制。 |
| logical_compute | [Not](../SIMD-API/basic_api/memory_vector_compute/logical_compute/Not.md)、[And](../SIMD-API/basic_api/memory_vector_compute/logical_compute/And.md)、[Or](../SIMD-API/basic_api/memory_vector_compute/logical_compute/Or.md) | &bull; 无isSetMask。<br>&bull; 接口内已设置Mask。 | &bull; 有isSetMask。<br>&bull; 接口内通过模板参数控制。 |
| logical_compute | [ShiftLeft](../SIMD-API/basic_api/memory_vector_compute/logical_compute/ShiftLeft.md)、[ShiftRight](../SIMD-API/basic_api/memory_vector_compute/logical_compute/ShiftRight.md) | &bull; 有isSetMask。<br>&bull; 接口内通过模板参数控制。 | &bull; 有isSetMask。<br>&bull; 接口内通过模板参数控制。 |
| composite_compute | [Axpy](../SIMD-API/basic_api/memory_vector_compute/composite_compute/Axpy.md)、[MulAddDst](../SIMD-API/basic_api/memory_vector_compute/composite_compute/MulAddDst.md)、[MulCast](../SIMD-API/basic_api/memory_vector_compute/composite_compute/MulCast.md)、[AddRelu](../SIMD-API/basic_api/memory_vector_compute/composite_compute/AddRelu.md)、[AddReluCast](../SIMD-API/basic_api/memory_vector_compute/composite_compute/AddReluCast.md)、[SubRelu](../SIMD-API/basic_api/memory_vector_compute/composite_compute/SubRelu.md)、[SubReluCast](../SIMD-API/basic_api/memory_vector_compute/composite_compute/SubReluCast.md)、[FusedMulAdd](../SIMD-API/basic_api/memory_vector_compute/composite_compute/FusedMulAdd.md)、[MulAddRelu](../SIMD-API/basic_api/memory_vector_compute/composite_compute/MulAddRelu.md)、[CastDequant](../SIMD-API/basic_api/memory_vector_compute/composite_compute/CastDequant.md)、[AddDeqRelu](../SIMD-API/basic_api/memory_vector_compute/composite_compute/AddDeqRelu.md) | &bull; 无isSetMask。<br>&bull; 接口内已设置Mask。 | &bull; 有isSetMask。<br>&bull; 接口内通过模板参数控制。 |
| compare_and_select | [Compare](../SIMD-API/basic_api/memory_vector_compute/compare_and_select/Compare.md) | &bull; 无isSetMask。<br>&bull; 接口不支持Mask。 | &bull; 有isSetMask，不起作用。<br>&bull; 接口无需设置Mask。 |
| compare_and_select | [Compare（结果存入寄存器）](../SIMD-API/basic_api/memory_vector_compute/compare_and_select/Compare_store_to_register.md) | - | &bull; 有isSetMask。<br>&bull; 接口内通过模板参数控制。 |
| compare_and_select | [Compares](../SIMD-API/basic_api/memory_vector_compute/compare_and_select/Compares.md) | &bull; 无isSetMask。<br>&bull; 接口不支持Mask。 | &bull; 有isSetMask。<br>&bull; 接口无需设置Mask。 |
| compare_and_select | [Select](../SIMD-API/basic_api/memory_vector_compute/compare_and_select/Select.md) | &bull; 无isSetMask。<br>&bull; 接口内已设置Mask。 | &bull; 有isSetMask，不起作用。<br>&bull; 如需使用在接口外部设置Mask的功能，可以调用[不传入mask参数的接口](../SIMD-API/basic_api/memory_vector_compute/compare_and_select/Select.md#函数原型)来实现。 |
| compare_and_select | [GatherMask](../SIMD-API/basic_api/memory_vector_compute/compare_and_select/GatherMask.md) | - | &bull; 无isSetMask。<br>&bull; 接口内已设置Mask。 |
| type_conversion | [Cast](../SIMD-API/basic_api/memory_vector_compute/type_conversion/Cast.md) | &bull; 无isSetMask。<br>&bull; 接口内已设置Mask。 | &bull; 有isSetMask。<br>&bull; 接口内通过模板参数控制。 |
| reduction_compute | [ReduceDataBlock](../SIMD-API/basic_api/memory_vector_compute/reduction_compute/ReduceDataBlock.md)、[ReduceRepeat](../SIMD-API/basic_api/memory_vector_compute/reduction_compute/ReduceRepeat.md)、[ReducePairElem](../SIMD-API/basic_api/memory_vector_compute/reduction_compute/ReducePairElem.md) | - | &bull; 有isSetMask。<br>&bull; 接口内通过模板参数控制。 |
| reduction_compute | [ReduceSum](../SIMD-API/basic_api/memory_vector_compute/reduction_compute/ReduceSum.md) | &bull; 有isSetMask。<br>&bull; 接口内通过模板参数控制。 | &bull; 无isSetMask。<br>&bull; 接口内已设置Mask。 |
| reduction_compute | [ReduceMax](../SIMD-API/basic_api/memory_vector_compute/reduction_compute/ReduceMax.md)、[ReduceMin](../SIMD-API/basic_api/memory_vector_compute/reduction_compute/ReduceMin.md) | &bull; 无isSetMask。<br>&bull; 接口内已设置Mask。 | &bull; 无isSetMask。<br>&bull; 接口内已设置Mask。 |
| data_layout_conversion | [Transpose](../SIMD-API/basic_api/memory_vector_compute/data_layout_conversion/Transpose.md)、[TransDataTo5HD](../SIMD-API/basic_api/memory_vector_compute/data_layout_conversion/TransDataTo5HD.md) | &bull; 无isSetMask。<br>&bull; 接口不支持Mask。 | &bull; 无isSetMask。<br>&bull; 接口不支持Mask。 |
| data_padding | [Brcb](../SIMD-API/basic_api/memory_vector_compute/data_padding/Brcb.md) | - | &bull; 无isSetMask。<br>&bull; 接口不支持Mask。 |
| data_padding | [Duplicate](../SIMD-API/basic_api/memory_vector_compute/data_padding/Duplicate.md) | &bull; 无isSetMask。<br>&bull; 接口内已设置Mask。 | &bull; 有isSetMask。<br>&bull; 接口内通过模板参数控制。 |
| data_padding | [CreateVecIndex](../SIMD-API/basic_api/memory_vector_compute/data_padding/CreateVecIndex.md) | &bull; 无isSetMask。<br>&bull; 接口不支持Mask。 | &bull; 无isSetMask。<br>&bull; 接口不支持Mask。 |
| 排序组合（ISASI） | [Sort32](../SIMD-API/basic_api/memory_vector_compute/sort_and_merge_ISASI/Sort32.md)、[MrgSort](../SIMD-API/basic_api/memory_vector_compute/sort_and_merge_ISASI/MrgSort.md) | - | &bull; 无isSetMask。<br>&bull; 接口不支持Mask。 |
| scatter_gather | [Gather](../SIMD-API/basic_api/memory_vector_compute/scatter_gather/Gather.md)、[Gatherb](../SIMD-API/basic_api/memory_vector_compute/scatter_gather/Gatherb_ISASI.md) | &bull; 无isSetMask。<br>&bull; 接口不支持Mask。 | &bull; 无isSetMask。<br>&bull; 接口不支持Mask。 |
| mask_operations | [SetMaskCount](../SIMD-API/basic_api/memory_vector_compute/mask_operations/SetMaskCount.md)、[SetMaskNorm](../SIMD-API/basic_api/memory_vector_compute/mask_operations/SetMaskNorm.md)、[SetVectorMask](../SIMD-API/basic_api/memory_vector_compute/mask_operations/SetVectorMask.md)、[ResetMask](../SIMD-API/basic_api/memory_vector_compute/mask_operations/ResetMask.md) | - | - |

<!-- end id3 -->
