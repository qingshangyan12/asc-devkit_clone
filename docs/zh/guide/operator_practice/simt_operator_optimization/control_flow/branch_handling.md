# Warp分支发散谓词优化

【优先级】中

【描述】SIMT编程模式下，Warp是基本的调度和执行单位，同一Warp内的32个线程执行同一条指令。流控制语句（if、switch、do、for、while）会产生分支跳转。当同一Warp内不同线程的跳转目标不一致时线程执行路径产生分歧，引起分支发散（Warp Divergence）。各分支只能串行执行，执行某一分支时，未进入该分支的线程被活跃线程掩码屏蔽、空转等待；硬件将汇合点地址与各方向的活跃线程掩码压入分支栈，全部分支执行完毕后在汇合点重新汇合。



**图1** divergence与predicate优化对比图

![](../../../figures/Branch_handling.png)

predicate优化（又称if-conversion）是指编译器将较为简单的分支改写为直线执行：分支条件不再控制是否跳转，而是由每个线程各自计算，结果记录为一个谓词位（predicate flag）；分支体内的指令被改写为带谓词的指令，Warp内32个线程全部发射，指令流保持一条直线。此时仅谓词为真的线程写回结果，谓词为假的线程丢弃结果（不写回寄存器、不产生访存副作用）。

Warp中的32个线程使用同一个PC(程序计数器)，出现分支发散时，各分支只能串行执行，会带来跳转和分支栈开销。当Warp中出现分支链时，使用predicate优化可以消除divergence带来的分支跳转的开销。

【样例介绍】样例以[hlog10](../../../../api/SIMT-API/math_functions/half_type/half_math_functions/hlog10.md)分支链路为载体。使用`logf(x) / logf(10.0f)`计算输入数据的常用对数，并对6个精度有偏差的特殊值各做一步修正，样例以8192个half元素为输入，约1/16为特殊值，其余为正常值。样例中通过对比“分支命中即return”和“使用临时变量存储结果，统一return”的性能差异，展示predicate优化的收益来源。

【反例】特殊值修正链置于正常值路径之前，命中即return。

```cpp
__aicore__ inline half hlog10_case0(half x)
{
    if (x == static_cast<half>(0.2362060546875f)) {
        return static_cast<half>(-0.62646484375f);
    } else if (x == static_cast<half>(0.2490234375f)) {
        return static_cast<half>(-0.60400390625f);
    } else if (x == static_cast<half>(3.0703125f)) {
        return static_cast<half>(0.487060546875f);
    } else if (x == static_cast<half>(126.0625f)) {
        return static_cast<half>(2.099609375f);
    } else if (x == static_cast<half>(11496.0f)) {
        return static_cast<half>(4.05859375f);
    } else if (x == static_cast<half>(2976.0f)) {
        return static_cast<half>(3.474609375f);
    }
    float x_fp32 = __half2float(x);
    float result_fp32 = logf(x_fp32) / logf(10.0f); // log10(x) = log(x) / log(10)
    return __float2half_rn(result_fp32);
}
```

上述实现中，每个提前return都隐含一个else分支，正常值路径位于嵌套最内层，需依次通过6个条件判断才能到达，会产生跳转和分支栈开销。此外，Warp 内少数线程命中特殊值提前返回后，并不能使 Warp 跳过后续指令，仍须在汇合点等待其余线程执行完正常路径。

反例算子的性能数据如下：

| execute cycle | Task Duration(μs) | aiv_total_cycles |
| :---------------: | :--------------: | :--------------: |
|      1890       |     14.919       |      5232.88      |

【正例】正常值路径前置，修正链统一return、覆盖temp。

```cpp
__aicore__ inline half hlog10_case1(half x)
{
    float x_fp32 = __half2float(x);
    float result_fp32 = logf(x_fp32) / logf(10.0f); // log10(x) = log(x) / log(10)
    half temp = __float2half_rn(result_fp32);
    if (x == static_cast<half>(0.2362060546875f)) {
        temp = static_cast<half>(-0.62646484375f);
    } else if (x == static_cast<half>(0.2490234375f)) {
        temp = static_cast<half>(-0.60400390625f);
    } else if (x == static_cast<half>(3.0703125f)) {
        temp = static_cast<half>(0.487060546875f);
    } else if (x == static_cast<half>(126.0625f)) {
        temp = static_cast<half>(2.099609375f);
    } else if (x == static_cast<half>(11496.0f)) {
        temp = static_cast<half>(4.05859375f);
    } else if (x == static_cast<half>(2976.0f)) {
        temp = static_cast<half>(3.474609375f);
    }
    return temp;
}
```

上述实现中，正常值路径位于分支发散区域之外，特殊值修正链使用临时变量存储结果，最后统一return。正例算子的性能数据如下（输入与反例相同）：

| execute cycle | Task Duration(μs) | aiv_total_cycles |
| :---------------: | :--------------: | :--------------: |
|      1406       |     14.946       |      5194.75      |

从execute cycle看，谓词化后从1890降至1406，下降约25.6%，aiv_total_cycles从5232.88降至5194.75。使用predicate优化后，编译器将整个修正链全部谓词化为直线执行，跳转与分支栈开销被完全消除。

由Case0和Case1对比可知，当warp内分支发散时，使用predicate优化相较于divergence性能来源主要是消除跳转和分支栈的开销，但是如果某一分支的命中率较高时，若使用predicate优化，这部分线程会执行完所有指令再返回，此时这部分线程会多执行很多不必要的指令，反而带来性能损耗。接下来的样例以不同方法处理不同命中率的分支，展示predicate优化的适用范围。

【样例介绍】样例以[j0f](../../../../api/SIMT-API/math_functions/float_math_functions/j0f.md)分支链路为载体。计算输入数据第一类零阶贝塞尔函数。接口按输入绝对值分三段计算，不能对两个特殊值进行单独处理。样例以8192个float元素为输入，正常情况下大量数据的取值范围为`ax <= 8.0f`,特殊值比例较小。

【反例】全部分支统一return、逐级覆盖result。

```cpp
__aicore__ inline float j0f_case2(float x)
{
    if (isnan(x)) {
        return x;
    }
    float ax = fabsf(x);
    if (isinf(ax)) {
        return 0.0f;
    }
    if (ax <= 8.0f) {
        return __internal_j0f_less8(ax);
    }
    if (ax <= 1.0e13f) {
        return __internal_j0f_middle_range(ax);
    }
    return __internal_j0f_huge_range(ax);
}
```

上述实现中，提前return隐式携带else分支，跳转与分支栈开销沿4级判断链逐级叠加，但每个线程只执行命中方向的分段体。取值范围为`ax > 1.0e13f`的数据处理位于嵌套最内层。因此起初考虑使用临时变量存储结果，逐级覆盖计算结果，最后统一return。`ax <= 8.0f`区间命中率非常高，执，这部分线程依旧需要执行后续指令，性能不升反降，于是考虑命中率高的分支提前返回，其余分支使用predicate优化。
反例算子的性能数据如下（常规批输入）：

| execute cycle | Task Duration(μs) | aiv_total_cycles |
| :---------------: | :--------------: | :--------------: |
|      14692      |     22.726       |     19514.88      |

【正例】命中率高的分支命中即提前return，其余分支统一返回。

```cpp
__aicore__ inline float j0f_case4(float x)
{
    float ax = fabsf(x);
    if (ax <= 8.0f) {
        return __internal_j0f_less8(ax);    // 分支体长：命中即返回，只执行命中方向
    }
    if (ax <= 1.0e13f) {
        return __internal_j0f_middle_range(ax);
    }
    float result = __internal_j0f_huge_range(ax);
    if (isnan(x)) {
        result = x;                         // 分支体短：谓词化，统一return
    }
    if (isinf(ax)) {
        result = 0.0f;
    }
    return result;
}
```

该实现分别以input（low）（`ax <= 8.0f`）、input（high）（`ax > 8.0f`）、input（special）（NaN/+Inf）、input（mixed）（低值段、高值段、NaN、-Inf按`i % 4`交替）四批输入测试，性能数据如下：

| 输入数据 | execute cycle | Task Duration(μs) | aiv_total_cycles |
| :---------------: | :--------------: | :--------------: | :--------------: |
| input（low） | 1767 | 14.389 | 4860.21 |
| input（high） | 2342 | 14.563 | 5468.46 |
| input（special） | 2395 | 14.088 | 5367.21 |
| input（mixed） | 2704 | 14.608 | 5756.00 |

四批输入上execute cycle均远低于全谓词化版本的14692（常规批），分支体长的分段命中即返回、分支体短的修正谓词化，两种机制各取所长。

【总结】在SIMT算子中，应按分支特征组织分支链：分支体短的判断链用临时变量承接各分支结果、统一出口返回，引导编译器if-conversion；分支体长的分支保留跳转、命中即提前return，只执行命中方向；长短混合的分支链上两种机制组合使用。带副作用指令（写内存、原子操作、同步）的分支体不可谓词化，保持真实分支。
