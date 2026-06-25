# Reg矢量计算接口精度标准汇总<a name="reg-vector-precision-summary"></a>

<!-- npu="950" id1 -->
本节汇总介绍了Reg矢量计算接口精度标准的情况，以下内容仅针对如下型号生效：

Ascend 950PR/Ascend 950DT

**表 1**  Reg矢量计算API精度标准<a id="table-precision-standard"></a>

| 接口类型 | 接口 | 硬件指令精度标准 | 软仿精度标准 |
| --- | --- | --- | --- |
| 基础算术 | Exp | 1-ulp compare to X86, Not Support denormalized numbers | 1-ulp compare to X86, Support denormalized numbers |
| 基础算术 | Sqrt | 1-ulp compare to X86, Not Support denormalized numbers | 0-ulp compare to X86 (float), 1-ulp compare to X86, Support denormalized numbers |
| 基础算术 | Add | &bull; half/float: IEEE754, Support denormalized numbers<br>&bull; bfloat16_t: Add(x, y) = rint_to_bf16(Add(cast_to_f32(x), cast_to_f32(y))) | IEEE754, Support denormalized numbers |
| 基础算术 | Sub | Sub(x, y) = Add(x, Neg(y)) | IEEE754, Support denormalized numbers |
| 基础算术 | Adds | &bull; half/float: IEEE754, Support denormalized numbers<br>&bull; bfloat16_t: Add(x, y) = rint_to_bf16(Add(cast_to_f32(x), cast_to_f32(y))) | IEEE754, Support denormalized numbers |
| 基础算术 | Div | 1-ulp compare to X86, Not Support denormalized numbers | &bull; complex64: 0-ulp compare to X86, Not Support denoramlized numbers <br>&bull; float: 0-ulp compare to X86, Support denormalized numbers <br>&bull; half/float: 1-ulp compare to X86, Support denormalized numbers |
| 基础算术 | Neg | 符号位取反 | / |
| 基础算术 | Ln/Log | 1-ulp compare to X86, Not Support denormalized numbers | 1-ulp compare to X86, Support denormalized numbers |
| 基础算术 | Log2/Log10 | / | &bull; 1-ulp compare to X86, Not Support denormalized numbers<br>&bull; 1-ulp compare to X86, Support denormalized numbers |
| 基础算术 | Mul | &bull; half/float: IEEE754, Support denormalized numbers<br>&bull; bfloat16_t: Mul(x, y) = rint_to_bf16(Mul(cast_to_f32(x), cast_to_f32(y))) | / |
| 基础算术 | Muls | half/float: IEEE754, Support denormalized numbers | / |
| 复合指令 | AbsSub | AbsSub(x) = Abs(Sub(x)), 指令串行 | / |
| 复合指令 | ExpSub | &bull; 输入half，输出float<br>ExpSub(x, y) = Exp(Sub(cast_to_f32(x), cast_to_f32(y)))<br>&bull; 输入float，输出float<br>ExpSub(x, y) = Exp(Sub(x,y)) | / |
| 复合指令 | Axpy | 结果等效于：乘累加采用高精度计算, 同比Fma(x, y, z) | / |
| 复合指令 | MulDstAdd | 结果等效于：乘累加采用高精度计算, 同比Fma(x, y, z) | / |
| 复合指令 | MulAddDst | 结果等效于：乘累加采用高精度计算, 同比Fma(x, y, z) | / |
| 复合指令 | MulsCast | MulsCast(x, y) = cast_to_f16&lt;roundingmode&gt;(x * y) | / |
| 归约计算 | Reduce | half/float: IEEE754, 采用二叉树结构的成对累加策略 | / |
| 归约计算 | ReduceDataBlock | half/float: IEEE754, 采用二叉树结构的成对累加策略 | / |
| 归约计算 | PairReduceElem | half/float: IEEE754 | / |

**表 2**  Vector低精度指令支持情况<a id="table-low-precision"></a>

| 数据类型 | 接口 |
| --- | --- |
| fp4x2_e2m1_t | Cast、Duplicate（scalarValue需要为int8_t） |
| fp4x2_e1m2_t | Cast、Duplicate（scalarValue需要为int8_t） |
| fp8_e5m2_t | Cast、Duplicate（scalarValue需要为int8_t） |
| fp8_e4m3fn_t | Cast、Duplicate（scalarValue需要为int8_t） |
| fp8_e8m0_t | Cast、Duplicate（scalarValue需要为int8_t） |
| hifloat8_t | Cast、Duplicate（scalarValue需要为int8_t） |
| int4x2_t | Cast |
| int8_t | Cast、Add、Sub、Max、Min、Compare、Abs、Neg、Adds、Muls、Maxs、Mins、Compares、Duplicate |
| uint8_t | Cast、Add、Sub、Max、Min、Compare、Adds、Muls、Maxs、Mins、Compares、Duplicate |
<!-- end id1 -->