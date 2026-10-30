# Reg Vector Computation API Precision Standard Summary<a name="reg-vector-precision-summary"></a>

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-29T10:58:22.501Z pushedAt=2026-10-29T11:18:45.463Z nl=2 -->

This section summarizes the precision standards of the Reg vector compute APIs. The content in this section applies only to the following models:

Ascend 950PR & 950DT products

**Table 1**  Reg vector compute API precision standards<a id="table-precision-standard"></a>

| API Category | API | Hardware Instruction Precision Standards | Software Simulation Precision Standard |
| --- | --- | --- | --- |
| Basic Arithmetic | Exp | 1ulp, not support denormalized numbers | 1ulp, support denormalized numbers |
| Basic Arithmetic | Sqrt | 1ulp, not support denormalized numbers | - float: 0ulp <br>- half/float: 1ulp, support denormalized numbers |
| Basic Arithmetic | Add | - half/float: 0ulp, support denormalized numbers<br>- bfloat16_t: Add(x, y) = rint_to_bf16(Add(cast_to_f32(x), cast_to_f32(y))) | 0ulp, support denormalized numbers |
| Basic Arithmetic | Sub | Sub(x, y) = Add(x, Neg(y)) | 0ulp, support denormalized numbers |
| Basic Arithmetic | Adds | - half/float: 0ulp, support denormalized numbers<br>- bfloat16_t: Add(x, y) = rint_to_bf16(Add(cast_to_f32(x), cast_to_f32(y))) | 0ulp, support denormalized numbers |
| Basic Arithmetic | Div | 1ulp, not support denormalized numbers | - complex64: 0ulp, not support denormalized numbers <br>- float: 0ulp, support denormalized numbers <br>- half/float: 1ulp, support denormalized numbers |
| Basic Arithmetic | Neg | Sign bit is inverted | - |
| Basic Arithmetic | Ln/Log | 1ulp, not support denormalized numbers | 1ulp, support denormalized numbers |
| Basic Arithmetic | Log2/Log10 | - | - 1ulp, not support denormalized numbers<br>- 1ulp, support denormalized numbers |
| Basic Arithmetic | Mul | - half/float: 0ulp, support denormalized numbers<br>- bfloat16_t: Mul(x, y) = rint_to_bf16(Mul(cast_to_f32(x), cast_to_f32(y))) | - |
| Basic Arithmetic | Muls | half/float: 0ulp, support denormalized numbers | - |
| compound instruction | AbsSub | Instructions are executed serially, AbsSub(x) = Abs(Sub(x)) | - |
| compound instruction | ExpSub | - half input, float output:<br>ExpSub(x, y) = Exp(Sub(cast_to_f32(x), cast_to_f32(y)))<br>- float input, float output:<br>ExpSub(x, y) = Exp(Sub(x, y)) | - |
| compound instruction | Axpy | Result is equivalent to: multiply-accumulate uses high-precision computation, equivalent to Fma(x, y, z) | - |
| compound instruction | MulDstAdd | Result is equivalent to: multiply-accumulate uses high-precision computation, equivalent to Fma(x, y, z) | - |
| compound instruction | MulAddDst | Result is equivalent to: multiply-accumulate uses high-precision computation, equivalent to Fma(x, y, z) | - |
| compound instruction | MulsCast | MulsCast(x, y) = cast_to_f16&lt;roundingmode&gt;(x * y) | - |
| Reduction Computation | Reduce | half/float: 0ulp, uses a pairwise accumulation strategy based on a binary tree structure | - |
| Reduction Computation | ReduceDataBlock | half/float: 0ulp, uses a pairwise accumulation strategy based on a binary tree structure | - |
| Reduction Computation | PairReduceElem | half/float: 0ulp | - |

**Table 2**  Vector low-precision instruction support<a id="table-low-precision"></a>

| Data Type | API |
| --- | --- |
| fp4x2_e2m1_t | Cast, Duplicate (software simulation) |
| fp4x2_e1m2_t | Cast, Duplicate (software simulation) |
| fp8_e5m2_t | Cast, Duplicate (software simulation) |
| fp8_e4m3fn_t | Cast, Duplicate (software simulation) |
| fp8_e8m0_t | Cast, Duplicate (software simulation) |
| hifloat8_t | Cast, Duplicate (software simulation) |
| int4x2_t | Cast |
| int8_t | Cast, Add, Sub, Max, Min, Compare, Abs, Neg, Adds, Muls, Maxs, Mins, Compares, Duplicate |
| uint8_t | Cast, Add, Sub, Max, Min, Compare, Adds, Muls, Maxs, Mins, Compares, Duplicate |
