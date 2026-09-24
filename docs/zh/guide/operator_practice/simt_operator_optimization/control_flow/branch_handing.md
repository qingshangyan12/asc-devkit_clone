# Warp分支发散谓词优化

【优先级】中

【描述】SIMT编程模式下，Warp是基本的调度和执行单位，同一Warp内的32个线程执行同一条指令。流控制语句（if、switch、do、for、while）会产生分支跳转。当同一Warp内不同线程的跳转目标不一致时线程执行路径产生分歧，引起分支发散（Warp Divergence）。各分支只能串行执行，执行某一分支时，未进入该分支的线程被活跃线程掩码屏蔽、空转等待；硬件将汇合点地址与各方向的活跃线程掩码压入分支栈，全部分支执行完毕后在汇合点重新汇合。
以Warp内一半线程`cond`为真、一半为假的if-else为例，对比predicate和divergence执行流程：

  ```cpp
  if (cond) {
      r = a + b;    // if 体
  } else {
      r = a - b;    // else 体
  }
  ```
**图1** divergence与predicate优化对比图

![](../../../figures/branch_handing.png)

predicate优化（又称if-conversion）是指编译器将较为简单的分支改写为直线执行：分支条件不再控制是否跳转，而是由每个线程各自计算，结果记录为一个谓词位（predicate flag）；分支体内的指令被改写为带谓词的指令，Warp内32个线程全部发射，指令流保持一条直线。此时仅谓词为真的线程写回结果，谓词为假的线程丢弃结果（不写回寄存器、不产生访存副作用）。

Warp中的32个线程使用同一个PC(程序计数器)，出现分支发散时，各分支只能串行执行，会带来跳转和分支栈开销。当Warp中出现分支链时，使用predicate优化可以消除divergence带来的分支跳转的开销。

【样例介绍】样例以[hlog10](../../../../api/SIMT-API/math_functions/half_type/half_math_functions/hlog10.md)分支链路为载体。使用`logf(x) / logf(10.0f)`计算输入数据的常用对数，并对6个精度有偏差的特殊值各做一步修正，输入为8192个half元素。样例中通过对比“分支命中即return”和“使用临时变量存储结果，统一return”的性能差异，展示predicate优化的收益来源。

【反例】特殊值修正链置于正常值路径之前，命中即return。

```cpp
__aicore__ inline half hlog10(half x)
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

上述实现中，每个提前return都隐含一个else分支。将其展开为等价的显式if-else嵌套，其真实控制流为：

```cpp
__aicore__ inline half hlog10(half x)
{
    if (x == static_cast<half>(0.2362060546875f)) {
        return static_cast<half>(-0.62646484375f);
    } else {
        if (x == static_cast<half>(0.2490234375f)) {
            return static_cast<half>(-0.60400390625f);
        } else {
            if (x == static_cast<half>(3.0703125f)) {
                return static_cast<half>(0.487060546875f);
            } else {
                if (x == static_cast<half>(126.0625f)) {
                    return static_cast<half>(2.099609375f);
                } else {
                    if (x == static_cast<half>(11496.0f)) {
                        return static_cast<half>(4.05859375f);
                    } else {
                        if (x == static_cast<half>(2976.0f)) {
                            return static_cast<half>(3.474609375f);
                        } else {
                            // 正常值路径位于6层嵌套的最内层
                            float x_fp32 = __half2float(x);
                            float result_fp32 = logf(x_fp32) / logf(10.0f);
                            return __float2half_rn(result_fp32);
                        }
                    }
                }
            }
        }
    }
}
```

正常值路径位于6级判断链的最内层，需依次通过6个条件判断才能到达，逐级叠加的跳转与分支栈开销无法避免。

反例算子的性能数据如下（execute cycle为30次运行的中位数，Task Duration与aiv_total_cycles为msOpProf采集3次的中位数，下同）：

| execute cycle | Task Duration(μs) | aiv_total_cycles |
| :---------------: | :--------------: | :--------------: |
|      433        |     2.198       |      2399.50      |

【正例】正常值路径前置，修正链统一return、覆盖temp。

```cpp
__aicore__ inline half hlog10(half x)
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
|      394.5       |     2.226       |      2385.75      |

从execute cycle看，谓词化后从433降至394.5，下降约8.9%。使用predicate优化后，编译器将整个修正链全部谓词化为直线执行，跳转与分支栈开销被完全消除。

对比Case0和Case1可得：Warp 内出现分支发散时，predicate优化相对divergence的收益，主要来自消除跳转与分支栈开销。但若某分支命中率较高，predicate优化会让这部分线程执行完全部指令才返回，多执行大量无用指令，会造成性能损失。后续样例将对不同命中率的分支采用不同处理方式，展示predicate优化的适用范围。

【样例介绍】样例以[j0f](../../../../api/SIMT-API/math_functions/float_math_functions/j0f.md)分支链路为载体。计算输入数据第一类零阶贝塞尔函数。接口按输入数据绝对值分三段计算，并对两个特殊值进行单独处理。样例以8192个float元素为输入，正常情况下数据的取值范围为`ax <= 1.0e13f`；混合批在此基础上加入`ax > 1.0e13f`与NaN/-Inf特殊值。

【反例】全部分支命中即return。

```cpp
__aicore__ inline float j0f(float x)
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

上述实现中，先判断特殊值并即时返回；再顺序逐段匹配，命中即返回该段结果。各分支独立返回，无统一出口。常规批不含特殊值，两次特殊值判断均未命中；各线程命中对应分段并返回，只执行命中方向的分段体，未命中的判断仍逐级产生跳转与分支栈开销，execute cycle为1221.5。性能数据如下：

| execute cycle | Task Duration(μs) | aiv_total_cycles |
| :---------------: | :--------------: | :--------------: |
|      1221.5        |     2.551       |      3015.88      |

【正例】命中率高的分支命中即提前return，其余分支统一返回。

```cpp
__aicore__ inline float j0f(float x)
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

该实现输入数据在`ax <= 8.0f`及`ax <= 1.0e13f`区间时，命中即提前return，其余分支使用临时变量存储结果，逐级覆盖并统一return。性能数据如下：

| 输入数据 | execute cycle | Task Duration(μs) | aiv_total_cycles |
| :---------------: | :--------------: | :--------------: | :--------------: |
| input（normal） | 1301 | 2.538 | 2972.25 |
| input（mixed） | 19260 | 13.136 | 18508.50 |

两批输入上，常规批数据落在`ax <= 1.0e13f`区间时execute cycle为1301，与基线Case 2的1221.5接近，整体开销与基线相当；混合批含特殊值时execute cycle为19260，远高于常规批。这是因为命中率高的分支提前return只执行命中方向，避免执行不必要分支，命中率低的分支使用predicate优化，两种机制各取所长。

【总结】在SIMT算子中，处理分支链路时使用predicate可以消除divergence方法带来的跳转和分支栈开销，带来性能收益，但当某个分支命中率高时应该使用divergence：命中即提前return，只执行命中方向；命中率低的分支用临时变量承接结果、统一出口返回，引导编译器进行predicate优化，可最大程度消除divergence，提高算子性能。
