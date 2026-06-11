# Mask接口汇总

<cann-filter npu-type="A3,910b">

本节汇总了各接口中模板参数isSetMask的支持情况，以下内容针对如下型号生效：

<cann-filter npu-type="A3">

- Atlas A3 训练系列产品/Atlas A3 推理系列产品

</cann-filter>
<cann-filter npu-type="910b">

- Atlas A2 训练系列产品/Atlas A2 推理系列产品

</cann-filter>

**表 1**  各接口模板参数isSetMask支持情况汇总表

| 接口类别 | 接口名称 | Tensor前n个数计算 | Tensor高维切分计算 | 
| --- | --- | --- | --- |
| 基础算术 | [Add](../SIMD-API/基础API/Memory矢量计算/基础算术/Add.md)、[Exp](../SIMD-API/基础API/Memory矢量计算/基础算术/Exp.md)、[Ln](../SIMD-API/基础API/Memory矢量计算/基础算术/Ln.md)、[Abs](../SIMD-API/基础API/Memory矢量计算/基础算术/Abs.md)、[Reciprocal](../SIMD-API/基础API/Memory矢量计算/基础算术/Reciprocal.md)、[Sqrt](../SIMD-API/基础API/Memory矢量计算/基础算术/Sqrt.md)、[Rsqrt](../SIMD-API/基础API/Memory矢量计算/基础算术/Rsqrt.md)、[Relu](../SIMD-API/基础API/Memory矢量计算/基础算术/Relu.md)、[Sub](../SIMD-API/基础API/Memory矢量计算/基础算术/Sub.md)、[Mul](../SIMD-API/基础API/Memory矢量计算/基础算术/Mul.md)、[Div](../SIMD-API/基础API/Memory矢量计算/基础算术/Div.md)、[Max](../SIMD-API/基础API/Memory矢量计算/基础算术/Max.md)、[Min](../SIMD-API/基础API/Memory矢量计算/基础算术/Min.md) | &bull; 无isSetMask，不支持外部设置Mask。<br>&bull; 接口内强集成的调用Mask接口。 | &bull; 有isSetMask，支持外部设置Mask。<br>&bull; 接口内通过模板参数控制isSetMask。 |
| 基础算术 | [BilinearInterpolation](../SIMD-API/基础API/Memory矢量计算/基础算术/BilinearInterpolation(ISASI).md) | / | &bull; 无isSetMask，不支持外部设置Mask。<br>&bull; 接口内强集成的调用Mask接口。 |
| 基础算术 | [Adds](../SIMD-API/基础API/Memory矢量计算/基础算术/Adds.md)、[Muls](../SIMD-API/基础API/Memory矢量计算/基础算术/Muls.md)、[Maxs](../SIMD-API/基础API/Memory矢量计算/基础算术/Maxs.md)、[Mins](../SIMD-API/基础API/Memory矢量计算/基础算术/Mins.md)、[LeakyRelu](../SIMD-API/基础API/Memory矢量计算/基础算术/LeakyRelu.md) | &bull; 有isSetMask，支持外部设置Mask。<br>&bull; 接口内强集成的调用Mask接口。 | &bull; 有isSetMask，支持外部设置Mask。<br>&bull; 接口内通过模板参数控制isSetMask。 |
| 逻辑计算 | [Not](../SIMD-API/基础API/Memory矢量计算/逻辑计算/Not.md)、[And](../SIMD-API/基础API/Memory矢量计算/逻辑计算/And.md)、[Or](../SIMD-API/基础API/Memory矢量计算/逻辑计算/Or.md) | &bull; 无isSetMask，不支持外部设置Mask。<br>&bull; 接口内强集成的调用Mask接口。 | &bull; 有isSetMask，支持外部设置Mask。<br>&bull; 接口内通过模板参数控制isSetMask。 |
| 逻辑计算 | [ShiftLeft](../SIMD-API/基础API/Memory矢量计算/逻辑计算/ShiftLeft.md)、[ShiftRight](../SIMD-API/基础API/Memory矢量计算/逻辑计算/ShiftRight.md) | &bull; 有isSetMask，支持外部设置Mask。<br>&bull; 接口内通过模板参数控制isSetMask。 | &bull; 有isSetMask，支持外部设置Mask。<br>&bull; 接口内通过模板参数控制isSetMask。 |
| 复合计算 | [Axpy](../SIMD-API/基础API/Memory矢量计算/复合计算/Axpy.md)、[MulAddDst](../SIMD-API/基础API/Memory矢量计算/复合计算/MulAddDst.md)、[MulCast](../SIMD-API/基础API/Memory矢量计算/复合计算/MulCast.md)、[AddRelu](../SIMD-API/基础API/Memory矢量计算/复合计算/AddRelu.md)、[AddReluCast](../SIMD-API/基础API/Memory矢量计算/复合计算/AddReluCast.md)、[SubRelu](../SIMD-API/基础API/Memory矢量计算/复合计算/SubRelu.md)、[SubReluCast](../SIMD-API/基础API/Memory矢量计算/复合计算/SubReluCast.md)、[FusedMulAdd](../SIMD-API/基础API/Memory矢量计算/复合计算/FusedMulAdd.md)、[MulAddRelu](../SIMD-API/基础API/Memory矢量计算/复合计算/MulAddRelu.md)、[CastDequant](../SIMD-API/基础API/Memory矢量计算/复合计算/CastDequant.md)、[AddDeqRelu](../SIMD-API/基础API/Memory矢量计算/复合计算/AddDeqRelu.md) | &bull; 无isSetMask，不支持外部设置Mask。<br>&bull; 接口内强集成的调用Mask接口。 | &bull; 有isSetMask，支持外部设置Mask。<br>&bull; 接口内通过模板参数控制isSetMask。 |
| 比较与选择 | [Compare](../SIMD-API/基础API/Memory矢量计算/比较与选择/Compare.md) | 无isSetMask。 | 有isSetMask，不起作用。 |
| 比较与选择 | [Compare（结果存入寄存器）](../SIMD-API/基础API/Memory矢量计算/比较与选择/Compare（结果存入寄存器）.md) | / | &bull; 有isSetMask，支持外部设置Mask。<br>&bull; 接口内通过模板参数控制isSetMask。 |
| 比较与选择 | [Compares](../SIMD-API/基础API/Memory矢量计算/比较与选择/Compares.md) | 无isSetMask，不支持外部设置Mask。 | &bull; 有isSetMask，不支持外部设置Mask。<br>&bull; 接口参数mask为保留参数，不起作用。 |
| 比较与选择 | [Select](../SIMD-API/基础API/Memory矢量计算/比较与选择/Select.md) | &bull; 无isSetMask，不支持外部设置Mask。<br>&bull; 接口内强集成的调用Mask接口。 | &bull; 有isSetMask，不起作用。<br>&bull; 如需使用在接口外部设置Mask的功能，可以调用[不传入mask参数的接口](../SIMD-API/基础API/Memory矢量计算/比较与选择/Select.md#函数原型)来实现。 |
| 比较与选择 | [GatherMask](../SIMD-API/基础API/Memory矢量计算/比较与选择/GatherMask.md) | / | 无isSetMask，不支持外部设置Mask。 |
| 类型转换 | [Cast](../SIMD-API/基础API/Memory矢量计算/类型转换/Cast.md) | &bull; 无isSetMask，不支持外部设置Mask。<br>&bull; 接口内强集成的调用Mask接口。 | &bull; 有isSetMask，支持外部设置Mask。<br>&bull; 接口内通过模板参数控制isSetMask。 |
| 归约计算 | [ReduceDataBlock](../SIMD-API/基础API/Memory矢量计算/归约计算/ReduceDataBlock.md)、[ReduceRepeat](../SIMD-API/基础API/Memory矢量计算/归约计算/ReduceRepeat.md)、[ReducePairElem](../SIMD-API/基础API/Memory矢量计算/归约计算/ReducePairElem.md) | / | &bull; 有isSetMask，支持外部设置Mask。<br>&bull; 接口内通过模板参数控制isSetMask。 |
| 归约计算 | [ReduceSum](../SIMD-API/基础API/Memory矢量计算/归约计算/ReduceSum.md) | 有isSetMask，不起作用。 | 无isSetMask，不支持外部设置Mask。 |
| 归约计算 | [ReduceMax](../SIMD-API/基础API/Memory矢量计算/归约计算/ReduceMax.md)、[ReduceMin](../SIMD-API/基础API/Memory矢量计算/归约计算/ReduceMin.md) | 无isSetMask，不支持外部设置Mask。 | 无isSetMask，不支持外部设置Mask。 |
| 数据排布转换 | [Transpose](../SIMD-API/基础API/Memory矢量计算/数据排布转换/Transpose.md)、[TransDataTo5HD](../SIMD-API/基础API/Memory矢量计算/数据排布转换/TransDataTo5HD.md) | 无isSetMask，不支持设置Mask。 | 无isSetMask，不支持设置Mask。 |
| 数据填充 | [Brcb](../SIMD-API/基础API/Memory矢量计算/数据填充/Brcb.md) | / | 无isSetMask，不支持设置Mask。 |
| 数据填充 | [Duplicate](../SIMD-API/基础API/Memory矢量计算/数据填充/Duplicate.md) | &bull; 无isSetMask，不支持外部设置Mask。<br>&bull; 接口内强集成的调用Mask接口。 | &bull; 有isSetMask，支持外部设置Mask。<br>&bull; 接口内通过模板参数控制isSetMask。 |
| 数据填充 | [CreateVecIndex](../SIMD-API/基础API/Memory矢量计算/数据填充/CreateVecIndex.md) | 无isSetMask，不支持外部设置Mask。 | 无isSetMask，不支持外部设置Mask。 |
| 排序组合（ISASI） | [Sort32](../SIMD-API/基础API/Memory矢量计算/排序组合（ISASI）/Sort32.md)、[MrgSort](../SIMD-API/基础API/Memory矢量计算/排序组合（ISASI）/MrgSort.md) | / | 无isSetMask，不支持设置Mask。 |
| 离散与聚合 | [Gather](../SIMD-API/基础API/Memory矢量计算/离散与聚合/Gather.md)、[Gatherb](../SIMD-API/基础API/Memory矢量计算/离散与聚合/Gatherb(ISASI).md) | 无isSetMask，不支持外部设置Mask。 | 无isSetMask，不支持外部设置Mask。 |
| 掩码操作 | [SetMaskCount](../SIMD-API/基础API/Memory矢量计算/掩码操作/SetMaskCount.md)、[SetMaskNorm](../SIMD-API/基础API/Memory矢量计算/掩码操作/SetMaskNorm.md)、[SetVectorMask](../SIMD-API/基础API/Memory矢量计算/掩码操作/SetVectorMask.md)、[ResetMask](../SIMD-API/基础API/Memory矢量计算/掩码操作/ResetMask.md) | / | / |

</cann-filter>
