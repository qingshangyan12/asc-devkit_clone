# SIMT接口精度标准汇总表

## bf16数据类型计算API精度标准

### 数学库函数

| 接口 | 硬件指令精度标准 | 软仿精度标准 |
| --- | --- | --- |
| hexp | - | 1-ulp compare to X86，Not Support denormalized numbers |
| htanh | - | 1-ulp compare to X86，Not Support denormalized numbers |
| hexp2 | - | 1-ulp compare to X86，Not Support denormalized numbers |
| hexp10 | - | 1-ulp compare to X86，Not Support denormalized numbers |
| hlog | - | 1-ulp compare to X86，Not Support denormalized numbers |
| hlog2 | - | 1-ulp compare to X86，Not Support denormalized numbers |
| hlog10 | - | 1-ulp compare to X86，Not Support denormalized numbers |
| hcos | - | - |
| hsin | - | - |
| hsqrt | - | 1-ulp compare to X86，Not Support denormalized numbers |
| hrsqrt | - | 1-ulp compare to X86，Not Support denormalized numbers |
| hrcp | 使用操作符“/”实现倒数计算 | - |
| hrint | 调用\_\_rintf(bfloat16)完成计算 | - |
| hfloor | 调用\_\_floorf(bfloat16)完成计算 | - |
| hceil | 调用\_\_ceilf(bfloat16)完成计算 | - |
| htrunc | 判断元素是否大于0，如果大于0调用\_\_floorf(bfloat16)完成计算，否则调用\_\_ceilf(bfloat16)完成计算 | - |
| h2tanh | - | 1-ulp compare to X86，Not Support denormalized numbers |
| h2exp | - | 1-ulp compare to X86，Not Support denormalized numbers |
| h2exp2 | - | 1-ulp compare to X86，Not Support denormalized numbers |
| h2exp10 | - | 1-ulp compare to X86，Not Support denormalized numbers |
| h2log | - | 1-ulp compare to X86，Not Support denormalized numbers |
| h2log2 | - | 1-ulp compare to X86，Not Support denormalized numbers |
| h2log10 | - | 1-ulp compare to X86，Not Support denormalized numbers |
| h2cos | - | - |
| h2sin | - | - |
| h2sqrt | - | 1-ulp compare to X86，Not Support denormalized numbers |
| h2rsqrt | - | 1-ulp compare to X86，Not Support denormalized numbers |
| h2rcp | 各个元素使用操作符“/”实现倒数计算 | - |
| h2rint | 各个元素调用\_\_rintf(bfloat16)完成计算 | - |
| h2floor | 各个元素调用\_\_floorf(bfloat16)完成计算 | - |
| h2ceil | 各个元素调用\_\_ceilf(bfloat16)完成计算 | - |
| h2trunc | 判断各个元素是否大于0，如果大于0调用\_\_floorf(bfloat16)完成计算，否则调用\_\_ceilf(bfloat16)完成计算 | - |

### 算术函数

| 接口 | 硬件指令精度标准 | 软仿精度标准 |
| --- | --- | --- |
| \_\_habs | - | IEEE754, Support denormalized numbers |
| \_\_hfma | - | - |
| \_\_hadd | IEEE754, Support denormalized numbers | - |
| \_\_hsub | 直接使用算术运算符“-”实现功能 | - |
| \_\_hmul | IEEE754, Support denormalized numbers | - |
| \_\_hdiv | 1-ulp compare to X86，Not Support denormalized numbers | - |
| \_\_hneg | 路寄存器取反，再配合ADD指令实现的。 | - |
| \_\_hfma_relu | 调用\_\_hmax_nan(bfloat16)和\_\_fma(bfloat16)实现 | - |
| \_\_haddx2 | IEEE754, Support denormalized numbers | - |
| \_\_hsubx2 | 直接使用算术运算符“-”实现功能 | - |
| \_\_hmulx2 | IEEE754, Support denormalized numbers | - |
| \_\_hdivx2 | 1-ulp compare to X86，Not Support denormalized numbers | - |
| \_\_habsx2 | - | - |
| \_\_hfmax2 | - | - |
| \_\_hnegx2 | 路寄存器取反，再配合ADD指令实现的 | - |
| \_\_hfmax2_relu | 调用\_\_hmaxx2(bfloat16)和\_\_hfmax2(bfloat16)实现 | - |
| \_\_hcmadd | - | - |

## fp16数据类型计算API精度标准

### 数学库函数

| 接口 | 硬件指令精度标准 | 软仿精度标准 |
| --- | --- | --- |
| htah | \_\_cvt_float(half)、tanhf(float)、\_\_cvt(float)指令串行 | - |
| hexp | 1-ulp compare to X86 | - |
| hexp2 | - | 1-ulp compare to X86，Not Support denormalized numbers |
| hexp10 | - | 1-ulp compare to X86，Not Support denormalized numbers |
| hlog | 1-ulp compare to X86，Not Support denormalized numbers | - |
| hlog2 | - | 1-ulp compare to X86，Not Support denormalized numbers |
| hlog10 | - | 1-ulp compare to X86，Not Support denormalized numbers |
| hcos | - | - |
| hsin | - | - |
| hsqrt | 1-ulp compare to X86，Not Support denormalized numbers | - |
| hrsqrt | - | 1-ulp compare to X86，Not Support denormalized numbers |
| hrcp | 使用操作符“/”实现倒数计算 | - |
| hrint | 调用\_\_rintf(half )完成计算 | - |
| hfloor | 调用\_\_floorf(half )完成计算 | - |
| hceil | 调用\_\_ceilf(half )完成计算 | - |
| htrunc | 判断元素是否大于0，如果大于0调用\_\_floorf(half )完成计算，否则调用\_\_ceilf(half )完成计算 | - |
| h2tanh | - | 1-ulp compare to X86，Not Support denormalized numbers |
| h2exp | 1-ulp compare to X86，Not Support denormalized numbers | - |
| h2exp2 | - | 1-ulp compare to X86，Not Support denormalized numbers |
| h2exp10 | - | 1-ulp compare to X86，Not Support denormalized numbers |
| h2log | 1-ulp compare to X86，Not Support denormalized numbers | - |
| h2log2 | - | 1-ulp compare to X86，Not Support denormalized numbers |
| h2log10 | - | 1-ulp compare to X86，Not Support denormalized numbers |
| h2cos | - | - |
| h2sin | - | - |
| h2sqrt | 1-ulp compare to X86，Not Support denormalized numbers | - |
| h2rsqrt | - | 1-ulp compare to X86，Not Support denormalized numbers |
| h2rcp | 各个元素使用操作符“/”实现倒数计算 | - |
| h2rint | 各个元素调用\_\_rintf(half )完成计算 | - |
| h2floor | 各个元素调用\_\_floorf(half )完成计算 | - |
| h2ceil | 各个元素调用\_\_ceilf(half )完成计算 | - |
| h2trunc | 各个元素判断元素是否大于0，如果大于0调用\_\_floorf(half )完成计算，否则调用\_\_ceilf(half )完成计算 | - |

### 算术函数

| 接口 | 硬件指令精度标准 | 软仿精度标准 |
| --- | --- | --- |
| \_\_habs | - | - |
| \_\_hfma | - | - |
| \_\_hadd | IEEE754, Support denormalized numbers | - |
| \_\_hsub | 直接使用算术运算符“-”实现功能 | - |
| \_\_hmul | IEEE754, Support denormalized numbers | - |
| \_\_hdiv | 1-ulp compare to X86，Not Support denormalized numbers | - |
| \_\_hneg | 路寄存器取反，再配合ADD指令实现的 | - |
| \_\_hfma_relu | 调用\_\_hmax_nan(half)和\_\_fma(half)实现 | - |
| \_\_haddx2 | IEEE754, Support denormalized numbers | - |
| \_\_hsubx2 | 直接使用算术运算符“-”实现功能 | - |
| \_\_hmulx2 | IEEE754, Support denormalized numbers | - |
| \_\_hdivx2 | 1-ulp compare to X86，Not Support denormalized numbers | - |
| \_\_habsx2 | - | - |
| \_\_hfmax2 | - | - |
| \_\_hnegx2 | 路寄存器取反，再配合ADD指令实现的 | - |
| \_\_hfmax2_relu | 调用\_\_hmaxx2(half)和\_\_hfmax2half)实现 | - |
| \_\_hcmadd | - | - |

## fp32数据类型计算API精度标准

| 接口 | 硬件指令精度标准 | 软仿精度标准 |
| --- | --- | --- |
| expf | 1-ulp compare to X86，Not Support denormalized numbers | - |
| tanf | - | - |
| tanhf | - | 1-ulp compare to X86，Not Support denormalized numbers |
| tanpif | - | 1-ulp compare to X86，Not Support denormalized numbers |
| atanf | - | 1-ulp compare to X86，Not Support denormalized numbers |
| atan2f | - | 1-ulp compare to X86，Not Support denormalized numbers |
| atanhf | - | 1-ulp compare to X86，Not Support denormalized numbers |
| exp2f | 调用接口powf实现 | - |
| exp10f | 调用接口powf实现 | - |
| expm1f | - | 1-ulp compare to X86，Not Support denormalized numbers |
| logf | - | 1-ulp compare to X86，Not Support denormalized numbers |
| log2f | - | 1-ulp compare to X86，Not Support denormalized numbers |
| log10f | - | 1-ulp compare to X86，Not Support denormalized numbers |
| log1pf | - | 1-ulp compare to X86，Not Support denormalized numbers |
| logbf | - | - |
| ilogbf | 调用logbf(float)实现 | - |
| cosf | - | 1-ulp compare to X86，Not Support denormalized numbers |
| coshf | 调用fabsf(float)和expf(float)实现 | - |
| cospif | 调用cosf(float)实现 | - |
| acosf | 调用asinf(float)实现 | - |
| acoshf | - | 1-ulp compare to X86，Not Support denormalized numbers |
| sinf | 调用+、-、\*、fabsf(float)、fmaf(float)实现 | - |
| sinhf | 调用+、\*、fabsf(float)、expf(float)实现 | - |
| sinpif | 调用sinf(float)实现 | - |
| asinf | 调用+、-、\*、fabsf(float)、sqrtf(float)实现 | - |
| asinhf | 调用+、-、\*、fabsf(float)、logf(float)、sqrtf(float)实现 | - |
| sincosf | 调用+、-、\*、fabsf(float)、fmaf(float)实现 | - |
| sincospif | 调用+、-、\*、fabsf(float)、fmaf(float)实现 | - |
| frexpf | 调用+、-、\*、/、fabsf(float)实现 | - |
| ldexpf | 调用+、-、\*、/、fabsf(float)实现 | - |
| sqrtf | 1-ulp compare to X86，Not Support denormalized numbers | - |
| rsqrtf | - | 1-ulp compare to X86，Not Support denormalized numbers |
| hypotf | 调用\*、/、fabsf(float)、\_\_fmaxf(float)、\_\_fminf(float)、sqrtf(float)实现 | - |
| rhypotf | 调用hypotf(float)实现 | - |
| powf | - | - |
| norm3df | 调用fabsf(float)、fmaf(float)、sqrtf(float)、\_\_fmaxf(float)实现 | - |
| rnorm3df | 调用norm3df(float)实现 | - |
| norm4df | 调用fabsf(float)、fmaf(float)、sqrtf(float)、\_\_fmaxf(float)实现 | - |
| rnorm4df | 调用norm4df(float)实现 | - |
| normf | 调用+、-、\*、/、fabsf(float)、fmaf(float)、sqrtf(float)、\_\_fmaxf(float)实现 | - |
| rnormf | 调用normf(float)实现 | - |
| cbrtf | - | 1-ulp compare to X86，Not Support denormalized numbers |
| rcbrtf | - | 1-ulp compare to X86，Not Support denormalized numbers |
| erff | 调用-、\*、fabsf(float)、fmaf(float)、\_\_powf(float, float)实现 | - |
| erfcf | 调用+、-、\*、/、fabsf(float)、fmaf(float)、\_\_powf(float, float)实现 | - |
| erfinvf | 调用-、\*、/、fmaf(float)、log2f(float)、sqrtf(float)实现 | - |
| erfcinvf | 调用-、\*、/、fmaf(float)、log2f(float)、sqrtf(float)实现 | - |
| erfcxf | 调用+、-、\*、/、fabsf(float)、fmaf(float)、\_\_fmaxf(float)、\_\_fminf(float)、\_\_powf(float, float)实现 | - |
| tgammaf | 调用+、-、\*、/、fabsf(float)、fmaf(float)实现 | - |
| lgammaf | 调用+、-、\*、/、fabsf(float)、fmaf(float)实现 | - |
| cyl_bessel_i0f | 调用+、-、\*、/、fabsf(float)、fmaf(float)、\_\_expf(float)、sqrtf(float)实现 | - |
| cyl_bessel_i1f | 调用+、-、\*、/、fabsf(float)、fmaf(float)、\_\_expf(float)、sqrtf(float)实现 | - |
| normcdff | 调用+、-、\*、/、fabsf(float)、fmaf(float)实现 | - |
| normcdfinvf | 调用+、-、\*、/、log2f(float)、fmaf(float)实现 | - |
| j0f | 调用+、-、\*、/、fmaf(float)、sqrtf(float)实现 | - |
| j1f | 调用+、-、\*、/、fmaf(float)、sqrtf(float)实现 | - |
| jnf | 调用+、-、\*、/、fmaf(float)、sqrtf(float)实现 | - |
| y0f | 调用+、-、\*、/、fabsf(float)、fmaf(float)、sqrtf(float)实现 | - |
| y1f | 调用+、-、\*、/、fabsf(float)、fmaf(float)、sqrtf(float)实现 | - |
| ynf | 调用+、-、\*、/、fabsf(float)、fmaf(float)、sqrtf(float)实现 | - |
| fabsf | - | - |
| fmaf | - | - |
| fmaxf | - | - |
| fminf | - | - |
| fdimf | - | - |
| remquof | - | 1-ulp compare to X86，Not Support denormalized numbers |
| fmodf | 调用-、\*、fabsf(float)实现 | - |
| remainderf | 调用remquof(float)实现 | - |
| copysignf | 调用fabsf(float)实现 | - |
| nearbyintf | 调用\_\_rintf(float)实现 | - |
| nextafterf | - | - |
| scalbnf | - | - |
| scalblnf | 调用scalbnf(float，int32_t)实现 | - |
| modff | - | - |
| fdividef | 1-ulp compare to X86，Not Support denormalized numbers | - |
| signbit | 调用signbitf(float)实现 | - |
| \_\_saturatef | 调用fmaxf(float)和fminf(float)实现 | - |
| \_\_fdividef | - | - |
| rintf | 直接调用\_\_rintf(float)实现 | - |
| lrintf | 调用指令rintf(float)和\_\_cvt_int64_t(float)实现 | - |
| llrintf | 调用指令rintf(float)和\_\_cvt_int64_t(float)实现 | - |
| roundf | 直接调用\_\_roundf(float)实现 | - |
| lroundf | 调用指令roundf(float)和\_\_cvt_int64_t(float)实现 | - |
| llroundf | 调用指令roundf(float)和\_\_cvt_int64_t(float)实现 | - |
| floorf | 直接调用\_\_floorf(float)实现 | - |
| ceilf | 直接调用\_\_ceilf(float)实现 | - |
| truncf | 判断元素是否大于0，如果大于0调用\_\_floorf(float)完成计算，否则调用\_\_ceilf(float)完成计算 | - |
| isfinite | 直接调用\_\_isfinite(float)实现 | - |
| isnan | 直接调用\_\_isnan(float)实现 | - |
| isinf | 直接调用\_\_isinf(float)实现 | - |
