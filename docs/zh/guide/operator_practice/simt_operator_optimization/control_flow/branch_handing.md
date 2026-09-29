# Warp分支发散谓词优化

【优先级】中

【描述】SIMT编程模式下，Warp是基本的调度和执行单位，同一Warp内的32个线程执行同一条指令。流控制语句（if、switch、do、for、while）会产生分支跳转。当同一Warp内不同线程的跳转目标不一致时线程执行路径产生分歧，引起分支发散（Warp Divergence）。各分支只能串行执行，执行某一分支时，未进入该分支的线程被活跃线程掩码屏蔽、空转等待；硬件将汇合点地址与各方向的活跃线程掩码压入分支栈，全部分支执行完毕后在汇合点重新汇合。
以一个简单的if语句为例：

  ```cpp
  if (cond) {
      r = a + b;    // if 体
  } else {
      r = a - b;    // else 体
  }
  ```

该语句的执行顺序如下：

1. START_DVG将当前active mask压栈；
2. branch cond，跳转到else分支，将if分支的PC和active mask压栈；
3. 执行else分支；
4. 第一次执行END_DVG，出栈并跳转到if分支的PC然后设置active mask；
5. 执行if分支；
6. 第二次执行END_DVG，出栈并设置active mask；
7. warp脱离divergence状态。

**图1** divergence与predicate优化对比图

![](../../../figures/branch_handing.png)

predicate优化是指编译器将较为简单的分支改写为直线执行：分支条件不再控制是否跳转，而是由每个线程各自计算，结果记录为一个谓词位（predicate flag）；分支体内的指令被改写为带谓词的指令，Warp内32个线程全部发射，指令流保持一条直线。此时仅谓词为真的线程写回结果，谓词为假的线程丢弃结果（不写回寄存器、不产生访存副作用）。

Warp中的32个线程使用同一个PC(程序计数器)，出现分支发散时，各分支只能串行执行，会带来跳转和分支栈开销。当Warp中出现分支链时，使用predicate优化可以消除divergence带来的分支跳转的开销。

【样例介绍】样例以[hlog10](../../../../api/SIMT-API/math_functions/half_type/half_math_functions/hlog10.md)接口为例。hlog10接口使用$\log_{10}(x) = \dfrac{\log(x)}{\log(10)}$计算输入数据的常用对数，并对6个精度有偏差的特殊值做修正，样例以8192个half元素为输入。样例中通过对比“分支命中即return”和“各分支使用临时变量存储结果，最后统一return”的性能差异，展示predicate优化的收益来源。

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

上述提前return写法的实际控制流为：

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

接口中有6层分支判断会产生divergence，带来额外的跳转与分支栈开销。

反例算子的性能数据如下（execute cycle为30次运行的中位数，采集时特殊配置为1个block、32个线程，防止warp调度影响；Task Duration与aiv_total_cycles为msOpProf采集3次的中位数，下同）：

| execute cycle | Task Duration(μs) | aiv_total_cycles |
| :---------------: | :--------------: | :--------------: |
|      218        |     2.246       |      2458.75      |

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
|      190       |     2.186       |      2400.62      |

提前return在实际场景中很难触发：SIMT代码实际以warp为单位执行，必须warp内所有线程的数据都是特殊值才能提前return，从概率上可以忽略不计，绝大部分场景下，特殊值线程需要等其他线程完成计算后再一起往下执行。通过临时变量存储中间值，逐级覆盖最后统一return这种写法，可以引导编译器进行predicate优化，消除跳转与分支栈开销，相比反例的218，execute cycle降至190，下降约12.8%。

对比反例与正例可得：predicate优化通过消除跳转与分支栈开销提高性能，但复杂逻辑编译器不会用predicate优化，因此类似异常值处理的简单逻辑建议使用predicate优化。若某分支命中率较高，predicate优化会让这部分线程执行完全部指令才返回，多执行大量无用指令，会造成性能损失。后续样例将对不同命中率的分支采用不同处理方式，展示predicate优化的适用范围。

【样例介绍】样例以[j0f](../../../../api/SIMT-API/math_functions/float_math_functions/j0f.md)接口为例。j0f接口计算输入数据第一类零阶贝塞尔函数。接口按输入数据绝对值分三段计算，并对两个特殊值进行单独处理。样例以8192个float元素为输入，按分支命中情况构造两批输入：

| 输入数据 | 数据范围 |
| :---------------: | :--------------------------------------------------- |
| input1 | 在实数域随机取值（对数均匀），并混入NaN/-Inf特殊值 |
| input2 | 在`[-1.0e13f,1.0e13f]`范围内随机取值，全部落在中值段 |

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

上述实现中，先判断特殊值并即时返回；再顺序逐段匹配，命中即返回该段结果。各分支独立返回，无统一出口。input2全部落在中值段，各线程统一命中`ax <= 1.0e13f`分支提前return，只执行命中方向的分支体，未命中的分支仍逐级产生跳转与分支栈开销，execute cycle为1111。性能数据如下：

| execute cycle | Task Duration(μs) | aiv_total_cycles |
| :---------------: | :--------------: | :--------------: |
|      1111        |     2.486       |      2802.00      |

作为对比，若将该实现反转为“分段计算逐级覆盖result、全部分支统一return”的全谓词化写法，长分支体被谓词化后所有线程无条件执行全部三段计算，实测execute cycle反而升至3490、Task Duration升至8.587μs、aiv_total_cycles升至12086.38，显著慢于基线。这说明统一return、逐级覆盖的写法只应作用于分支体短的分支。

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
| input1 | 3646 | 12.502 | 17648.88 |
| input2 | 1115 | 2.453 | 2793.00 |

两批输入上，input1覆盖实数域并混入NaN/-Inf特殊值，低/中/高值段与特殊值混杂，分段跳转分支产生divergence，未命中分段提前return的线程还需执行高值段的Payne-Hanek辐角缩减计算，execute cycle为3646；input2全部命中`8.0f =< ax <= 1.0e13f`时立即返回，不执行后续未命中的分支计算，execute cycle为1115，与反例基线的1111接近，Task Duration与aiv_total_cycles分别为2.453μs与2793.00，与反例的2.486μs与2802.00基本持平，整体开销与基线相当。这是因为命中率高的分支提前return只执行命中方向，避免执行不必要分支，命中率低的分支使用predicate优化，两种机制各取所长。

【总结】在SIMT算子中，处理分支链路时使用predicate可以消除divergence方法带来的跳转和分支栈开销，带来性能收益；但复杂逻辑编译器不会用predicate优化，类似异常值处理的简单逻辑建议使用predicate优化。当某个分支命中率较高或分支体较长时应该使用divergence：命中即提前return，只执行命中方向；命中率低的短分支用临时变量承接结果、统一出口返回，引导编译器进行predicate优化，两种机制结合可最大程度消除divergence，提高算子性能。
