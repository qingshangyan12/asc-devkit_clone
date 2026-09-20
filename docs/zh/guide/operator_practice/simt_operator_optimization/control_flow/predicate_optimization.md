# 谓词化消除分支跳转开销

【优先级】中

【描述】SIMT编程模式下，流控制语句产生的分支跳转带来固定的控制开销：每个分支对应一条跳转指令。分支发散（Warp Divergence）时，汇合点地址与各方向活跃线程掩码被压入分支栈，各分支串行执行，未进入当前方向的线程被掩码屏蔽空转。分支发散机制的说明见[Warp执行机制](../../../programming_guide/programming_model/ai_core_simt_programming/thread_architecture.md#warp执行机制)，通过调整线程与数据的映射关系减少发散的方法见[同一Warp内避免分支跳转](./avoid_warp_branch.md)。当Warp内线程的执行路径无法统一时，对于分支体较短的分支，可用谓词化消除跳转。

谓词化（Predication，又称if-conversion）是指编译器将较为简单的分支改写为直线执行：分支条件不再驱动跳转，而是被计算为每线程一个谓词位（predicate flag）；分支体内的指令被改写为带谓词的指令，Warp内32个线程全部发射，指令流保持一条直线。此时仅谓词为真的线程写回结果，谓词为假的线程丢弃结果（不写回寄存器、不产生访存副作用）。

**图1** 谓词执行的控制流示意图（与分支发散对比）

![](../../../figures/predicate_straight_line.png)

谓词执行的代价是两个方向的指令都需发射，而节省的跳转与分支栈开销是固定的，因此谓词化的收益取决于分支体长短：分支体短时收益明显，分支体长时多发射的执行量超过控制开销的节省。带副作用指令（写内存、原子操作、同步）的分支体不可谓词化。开发者无需手写谓词，通过分支组织方式即可引导编译器：用临时变量承接各分支结果、统一出口返回的写法可被if-conversion，命中即提前return的写法保留真实跳转，同一分支链上两者可组合使用。

【样例介绍】以[Warp分支发散谓词优化样例](../../../../../../examples/03_simt_api/03_best_practices/04_predicate_optimizations/predicate_optimization)为例，通过数学接口[hlog10](../../../../api/SIMT-API/math_functions/half_type/half_math_functions/hlog10.md)（half输入，6个特殊值需一步修正，短修正分支链）与[j0f](../../../../api/SIMT-API/math_functions/float_math_functions/j0f.md)（float输入，按低/中/高值段分段计算并处理NaN/Inf，长短混合分段链）展示谓词化的性能收益与适用边界。样例包含5个用例，数据规模8192个元素，启动8个线程块、每个线程块1024个线程，用例由编译选项在编译期选定。下文仅列出各用例的计算函数，`__internal_j0f_less8`/`__internal_j0f_middle_range`/`__internal_j0f_huge_range`为j0f低/中/高值段的多项式求值实现，各为几十条指令。

<!-- npu="950" id1 -->
下文性能数据均在Ascend 950PR产品上采集。execute cycle为核函数（Kernel）内`clock()`采样到的单线程执行周期数，反映核函数（Kernel）指令流的长度与分支开销；Task Duration为Task整体耗时；aiv_total_cycles为Task在Vector Core上执行的周期总数（所有线程块平均值）。本样例核函数（Kernel）规模较小，Task Duration中调度等固定开销占比较大，核函数（Kernel）内的分支开销差异主要体现在execute cycle与aiv_total_cycles上。
<!-- end id1 -->

## 短分支链统一return谓词化

【反例】hlog10特殊值修正链前置，命中即提前return。

```cpp
__aicore__ inline half hlog10_case0(half x)
{
    if (x == static_cast<half>(0.2362060546875f)) {
        return static_cast<half>(-0.62646484375f);
    } else if (x == static_cast<half>(0.2490234375f)) {
        return static_cast<half>(-0.60400390625f);
    } else if (...) {   // 其余4个特殊值的判断与修正，写法相同
    }
    float x_fp32 = __half2float(x);
    float result_fp32 = logf(x_fp32) / logf(10.0f);
    return __float2half_rn(result_fp32);
}
```

上述实现将特殊值修正置于正常值路径之前，每个提前return隐式携带else分支，6级判断链的跳转与分支栈开销固定存在。且Warp内少数线程命中特殊值提前返回后，并不能使Warp跳过后续指令，仍需在汇合点等待其余线程执行完正常路径。

反例算子的性能数据如下（输入约1/16为特殊值，其余为正常值）：

| execute cycle | Task Duration(μs) | aiv_total_cycles |
|:---:|:---:|:---:|
| 1890 | 14.919 | 5232.88 |

【正例】正常值路径前置，修正链统一return、覆盖temp。

```cpp
__aicore__ inline half hlog10_case1(half x)
{
    float x_fp32 = __half2float(x);
    float result_fp32 = logf(x_fp32) / logf(10.0f);
    half temp = __float2half_rn(result_fp32);
    if (x == static_cast<half>(0.2362060546875f)) {
        temp = static_cast<half>(-0.62646484375f);
    } else if (x == static_cast<half>(0.2490234375f)) {
        temp = static_cast<half>(-0.60400390625f);
    } else if (...) {   // 其余4个特殊值的判断与修正，写法相同
    }
    return temp;
}
```

上述实现中，正常值路径位于分支发散区域之外，各修正分支的分支体只有一条赋值语句，编译器将修正链尽可能转换为带谓词的直线执行，跳转由6级减至4级。正例算子的性能数据如下（输入与反例相同）：

| execute cycle | Task Duration(μs) | aiv_total_cycles |
|:---:|:---:|:---:|
| 1406 | 14.946 | 5194.75 |

从execute cycle看，谓词化后从1890降至1406，下降约25.6%，aiv_total_cycles从5232.88降至5194.75。特殊值占比约1/16时，谓词化对未命中线程多发射的只是一条赋值指令，代价很小，正常值路径前置还缩短了命中特殊值的线程在汇合点的等待。

## 长分支体保留真跳转

【反例】j0f全部分支统一return、逐级覆盖result，长短分支全部谓词化。

```cpp
__aicore__ inline float j0f_case3(float x)
{
    float ax = fabsf(x);
    float result = __internal_j0f_huge_range(ax);   // 无条件执行高值段计算
    if (ax <= 1.0e13f) {
        result = __internal_j0f_middle_range(ax);
    }
    if (ax <= 8.0f) {
        result = __internal_j0f_less8(ax);
    }
    if (isnan(x)) {
        result = x;
    }
    if (isinf(ax)) {
        result = 0.0f;
    }
    return result;
}
```

上述实现中，每个输入都无条件执行高值段计算，多数低值段输入还多执行中值段计算，多发射的执行量远超节省的控制开销。

反例算子的性能数据如下（常规批输入：大量数据落在低值段，约1/16为NaN/Inf特殊值）：

| execute cycle | Task Duration(μs) | aiv_total_cycles |
|:---:|:---:|:---:|
| 14692 | 22.726 | 19514.88 |

【正例】j0f分段命中即返回，各分支只执行命中方向。

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

上述实现中，跳转与分支栈开销沿4级判断链逐级叠加，但每个线程只执行命中方向的分段体。正例算子的性能数据如下（输入与反例相同）：

| execute cycle | Task Duration(μs) | aiv_total_cycles |
|:---:|:---:|:---:|
| 2345 | 14.567 | 5025.12 |

从execute cycle看，全谓词化的反例为14692，约为保留真跳转的正例（2345）的6.3倍，Task Duration从14.567μs增至22.726μs，aiv_total_cycles从5025.12升至19514.88。与hlog10组相反，j0f分段体长，谓词化反而劣化，分支体长时应保留真跳转。

## 按命中率组合真跳转与谓词化

长短分支并存的链路上，两种机制应组合使用：覆盖范围广且分支体长的分段命中即返回，剩余短修正统一return谓词化。

【正例】j0f低/中值段命中即返回，高值段计算后接NaN/Inf短修正统一return。

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

为考察不同命中情况下的表现，该用例分别以低值段为主、高值段为主、全特殊值（NaN/+Inf交替）、混合（低/高值段与NaN、-Inf按i % 4交替）四批输入测试，性能数据如下：

| 输入分布 | execute cycle | Task Duration(μs) | aiv_total_cycles |
|:---:|:---:|:---:|:---:|
| 低值段为主 | 1767 | 14.389 | 4860.21 |
| 高值段为主 | 2342 | 14.563 | 5468.46 |
| 全特殊值 | 2395 | 14.088 | 5367.21 |
| 混合 | 2704 | 14.608 | 5756.00 |

四批输入上execute cycle均远低于全谓词化版本的14692（常规批），命中情况不同则表现随之变化：

- 低值段为主批：全批输入命中低值段，命中第一段即返回，只执行命中方向的低值段计算，execute cycle降至四批中最低的1767；
- 高值段为主批：多数输入越过低值段、命中中值段，命中即返回，execute cycle为2342；
- 全特殊值批：NaN/Inf输入经两次未命中的分段判断，再执行高值段计算，最后才被特殊值修正覆盖，execute cycle为2395；
- 混合批：各方向命中率接近，长分支体真跳转与短分支谓词化各取所长，execute cycle为2704。

综合三组对比可得谓词化的适用边界：分支体短、命中率低的分支谓词化收益明显，分支体长或命中率高的分支保留真跳转、命中即提前return。

【总结】谓词化以“两个方向的指令都需发射”为代价，消除分支跳转与分支栈操作，使用时应按分支特征组织代码：

- **分支体短的判断链**（几条指令的赋值、算术修正）：用临时变量承接各分支结果、统一出口返回，引导编译器if-conversion；
- **分支体长的分支**（函数调用、长计算）：保留真跳转、命中即提前return，只执行命中方向；
- **高命中率分支**：命中后跳过的后续计算越多，保留真跳转越划算，可将这类分支前置；
- **带副作用的分支体**（写内存、原子操作、同步）：不可谓词化，保持真实分支。
