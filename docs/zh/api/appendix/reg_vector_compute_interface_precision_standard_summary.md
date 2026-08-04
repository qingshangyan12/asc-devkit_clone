# reg_vector_compute_interface_precision_standard_summary<a name="reg-vector-precision-summary"></a>

本节汇总介绍了reg_vector_compute接口精度标准的情况，本节内容仅针对如下型号生效：

Ascend 950PR/Ascend 950DT

**表 1**  reg_vector_computeAPI精度标准<a id="table-precision-standard"></a>

| 接口类型 | 接口 | 硬件指令精度标准 | 软仿精度标准 |
| --- | --- | --- | --- |
| basic_arithmetic | Exp | 1ulp, not support denormalized numbers | 1ulp, support denormalized numbers |
| basic_arithmetic | Sqrt | 1ulp, not support denormalized numbers | &bull; float: 0ulp <br>&bull; half/float: 1ulp, support denormalized numbers |
| basic_arithmetic | Add | &bull; half/float: 0ulp, support denormalized numbers<br>&bull; bfloat16_t: Add(x, y) = rint_to_bf16(Add(cast_to_f32(x), cast_to_f32(y))) | 0ulp, support denormalized numbers |
| basic_arithmetic | Sub | Sub(x, y) = Add(x, Neg(y)) | 0ulp, support denormalized numbers |
| basic_arithmetic | Adds | &bull; half/float: 0ulp, support denormalized numbers<br>&bull; bfloat16_t: Add(x, y) = rint_to_bf16(Add(cast_to_f32(x), cast_to_f32(y))) | 0ulp, support denormalized numbers |
| basic_arithmetic | Div | 1ulp, not support denormalized numbers | &bull; complex64: 0ulp, not support denormalized numbers <br>&bull; float: 0ulp, support denormalized numbers <br>&bull; half/float: 1ulp, support denormalized numbers |
| basic_arithmetic | Neg | 符号位取反 | - |
| basic_arithmetic | Ln/Log | 1ulp, not support denormalized numbers | 1ulp, support denormalized numbers |
| basic_arithmetic | Log2/Log10 | - | &bull; 1ulp, not support denormalized numbers<br>&bull; 1ulp, support denormalized numbers |
| basic_arithmetic | Mul | &bull; half/float: 0ulp, support denormalized numbers<br>&bull; bfloat16_t: Mul(x, y) = rint_to_bf16(Mul(cast_to_f32(x), cast_to_f32(y))) | - |
| basic_arithmetic | Muls | half/float: 0ulp, support denormalized numbers | - |
| 复合指令 | AbsSub | 指令串行，AbsSub(x) = Abs(Sub(x)) | - |
| 复合指令 | ExpSub | &bull; 输入half，输出float：<br>ExpSub(x, y) = Exp(Sub(cast_to_f32(x), cast_to_f32(y)))<br>&bull; 输入float，输出float：<br>ExpSub(x, y) = Exp(Sub(x, y)) | - |
| 复合指令 | Axpy | 结果等效于：乘累加采用高精度计算, 同比Fma(x, y, z) | - |
| 复合指令 | MulDstAdd | 结果等效于：乘累加采用高精度计算，同比Fma(x, y, z) | - |
| 复合指令 | MulAddDst | 结果等效于：乘累加采用高精度计算，同比Fma(x, y, z) | - |
| 复合指令 | MulsCast | MulsCast(x, y) = cast_to_f16&lt;roundingmode&gt;(x * y) | - |
| reduction_compute | Reduce | half/float: 0ulp, 采用二叉树结构的成对累加策略 | - |
| reduction_compute | ReduceDataBlock | half/float: 0ulp, 采用二叉树结构的成对累加策略 | - |
| reduction_compute | PairReduceElem | half/float: 0ulp | - |

**表 2**  Vector低精度指令支持情况<a id="table-low-precision"></a>

| 数据类型 | 接口 |
| --- | --- |
| fp4x2_e2m1_t | Cast、Duplicate（软仿） |
| fp4x2_e1m2_t | Cast、Duplicate（软仿） |
| fp8_e5m2_t | Cast、Duplicate（软仿） |
| fp8_e4m3fn_t | Cast、Duplicate（软仿） |
| fp8_e8m0_t | Cast、Duplicate（软仿） |
| hifloat8_t | Cast、Duplicate（软仿） |
| int4x2_t | Cast |
| int8_t | Cast、Add、Sub、Max、Min、Compare、Abs、Neg、Adds、Muls、Maxs、Mins、Compares、Duplicate |
| uint8_t | Cast、Add、Sub、Max、Min、Compare、Adds、Muls、Maxs、Mins、Compares、Duplicate |