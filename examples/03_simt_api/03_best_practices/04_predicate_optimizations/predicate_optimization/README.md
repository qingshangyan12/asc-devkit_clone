# Warp分支发散谓词优化样例

## 概述

本样例通过数学接口`hlog10`、`j0f`的分支链路展现SIMT编程模式下[Warp分支发散](../../../../../../docs/zh/guide/programming_guide/programming_model/ai_core_simt_programming/thread_architecture.md#warp执行机制)的调优方法，重点呈现了predicate（谓词）优化的性能收益与适用场景。样例包含2个基线版本以及3个优化版本：基线版本命中分支即提前return，分支判断产生真实跳转，引发分支发散；优化版本消除分支跳转，将分支转换为谓词直线执行，并按分支体长短选择谓词化与真跳转的适用范围。

## 本样例支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
|------|-------------|
| Ascend 950PR/Ascend 950DT | >= CANN 9.2.0 |

## 目录结构介绍

```text
├── predicate_optimization
│   ├── predicate_optimization.asc  // SIMT实现&调用样例
│   ├── figures                     // README中的图片资源
│   ├── CMakeLists.txt              // cmake编译文件
│   ├── README.md                   // 样例说明文档
│   ├── README_en.md                // 英文样例说明文档
```

## 样例描述

- 样例功能

  对数学接口`hlog10`（half输入，6个精度特殊值需修正）与`j0f`（float输入，按低/中/高值段分段计算并处理NaN/Inf）的分支链路，分别以"提前return"与"统一return"等不同分支组织方式实现kernel，前者体现短修正分支链，后者体现长短混合的区间分段链。通过不同分支组织方式的对比，展示SIMT编程方式下predicate优化的性能收益与适用场景。

- **Warp与分支发散（Warp Divergence）**

  在C++代码开发中，开发者遇到特殊值时，通常将特殊值处理置于正常计算逻辑之前，并通过控制流语句划分出不同的执行分支。然而在SIMT编程模式下，Warp是基本的调度和执行单位，当代码中出现分支判断时，Warp内的线程会产生分支发散（divergence），各分支代码只能串行执行，发散点处汇合点地址与各方向的活跃线程掩码被压入分支栈（Divergence Stack），并通过活跃掩码（active mask）屏蔽未进入当前分支的线程、仅让进入该分支的线程执行。更多说明请参考[Warp执行机制](../../../../../../docs/zh/guide/programming_guide/programming_model/ai_core_simt_programming/thread_architecture.md#warp执行机制)。

  当分支条件依赖线程各自的数据时，同一Warp内各线程的跳转目标不一致，分支发散随之产生。以Warp内一半线程`cond`为真、一半为假的if-else为例：

  ```cpp
  if (cond) {
      r = a + b;    // if 体
  } else {
      r = a - b;    // else 体
  }
  ```

  上述if-else在分支发散下的控制流如下：

  ![Warp分支发散的控制流](./figures/branch.png "Warp分支发散的控制流")

  对应的指令流：

  ```text
      cmp.eq  p0, cond, 1          ; 每线程独立计算分支条件
      branch  .BB_IF, p0           ; 条件跳转：Warp内跳转目标不一致 → 分支发散
      ; ─── 分支栈 push：{汇合点 .END_DVG 地址, else 方向的活跃掩码} ───
  .BB_IF:                          ; ── 第1趟：执行 if 体 ──
      add     r4, r2, r3           ; cond=1 的16线程生效，
      ;                              cond=0 的16线程被掩码屏蔽空转
      branch  .END_DVG             ; if 体完成，跳到汇合点
      ; ─── 分支栈 pop：切换活跃掩码到 else 方向，跳回执行 else 体 ───
  .BB_ELSE:                        ; ── 第2趟：执行 else 体 ──
      sub     r4, r2, r3           ; cond=0 的16线程生效，
      ;                              cond=1 的16线程被掩码屏蔽空转
  .END_DVG:                        ; ── 汇合点 ──
      ; ─── 分支栈 pop：恢复完整掩码，Warp重新汇合 ───
      stg     r2, [r5]             ; 汇合后继续顺序执行
  ```

- **Predicate（谓词）优化**

  为避免上述开销，编译器可通过谓词优化（Predication，又称if-conversion）将较为简单的分支改写为直线执行：分支条件不再驱动跳转，而是被计算为每线程一个谓词位（predicate flag）；分支体内的指令被改写为带谓词的指令，Warp内32个线程**全部发射**，指令流保持一条直线，仅谓词为真的线程写回结果，谓词为假的线程丢弃结果（不写回寄存器、不产生访存副作用）。同样的if-else，predicate优化后的控制流如下：

  ![谓词优化后的控制流](./figures/predicate.png "谓词优化后的控制流")


  对应的指令流：

  ```text
      cmp.eq  p0, cond, 1          ; 每线程独立计算谓词，不产生跳转
      add     r4, r2, r3,  p0      ; 全部32线程发射；仅 p0=1 的16线程写回 r4
      sub     r4, r2, r3, ~p0      ; 全部32线程发射；仅 p0=0 的16线程写回 r4
      ; 无跳转指令、无分支栈操作，Warp内指令流始终一条，顺序执行
  ```

  将两种机制下Warp的完整指令流逐环节对比：

  | 环节 | 分支发散（divergence） | 谓词执行（predication） |
  | --- | --- | --- |
  | 控制流指令 | 每个分支一条跳转指令 | 无跳转，只有一次条件计算 |
  | 分支栈 | 发散点压栈、逐方向弹栈、汇合点弹栈，嵌套分支叠加 | 完全没有分支栈操作 |
  | 分支体执行 | 各方向串行执行，每趟约一半线程空转 | 两个分支体各执行一次，每个发射槽都对一半线程有效 |
  | 汇合 | 需要硬件同步恢复掩码 | 天然顺序执行，无需汇合 |

  汇总成指令序列：分支发散下，Warp依次发出`条件计算 → 跳转 → 压栈 → else体 → 弹栈切换 → if体 → 弹栈汇合`；谓词执行下，Warp依次发出`条件计算 → if体 → else体`。两个序列中分支体的计算量相同，差异全部来自发散序列中额外的跳转与分支栈操作。因此，谓词执行以"两个方向的指令都需发射"为代价，节省了跳转、分支栈与汇合的控制开销。

  是否进行谓词化，不仅与分支体长短有关，还与分支命中率（输入中实际命中该分支的比例）有关。谓词执行节省的跳转与分支栈开销是固定的，而多发射的指令数正比于分支体长度：分支体短（几条指令的赋值、返回、算术运算）时，多发射的指令很少，谓词化收益明显；分支体长（函数调用、长计算）时，多发射的执行量超过控制开销的节省，保留分支跳转、仅执行命中方向更优；而当某分支命中率高时，提前return可使大部分线程直接跳过后续全部分支的判断与执行，此时无论分支体长短，保留真跳转都更为有利。此外，包含复杂控制流或带副作用指令（写内存、原子操作、同步）的分支体不可谓词化，编译器对其保留分支跳转。

  落到同一条分支链路上，长短分支可以并存，两种机制组合使用：命中率高或分支体长的分支命中即提前return，分支体短的分支统一return、以谓词化的赋值逐条覆盖临时变量。本样例基于hlog10（短修正分支链）与j0f（长短混合的区间分段链）两组样例，展示两种机制在不同分支体长短与命中情况下的收益与适用边界。

- 样例规格：

  <table>
  <tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="4" align="center">hlog10 / j0f（数学接口分支链路）</td></tr>
  <tr><td rowspan="3" align="center">样例输入/输出</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">input / output（hlog10组）</td><td align="center">[8192]</td><td align="center">half</td><td align="center">ND</td></tr>
  <tr><td align="center">input / output（j0f组）</td><td align="center">[8192]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">hlog10_case0_kernel / hlog10_case1_kernel / j0f_case2_kernel / j0f_case3_kernel / j0f_case4_kernel</td></tr>
  </table>

由于不同收益与代价需在不同的分支命中情况下分别测量，j0f组按命中情况构造测试数据；hlog10组则直接使用特殊值与正常值并存的一批输入：

- j0f组：

  case2与case3共用一批常规输入（大量数据落在`ax <= 8.0f`低值段，约1/16为NaN/Inf特殊值，构造思路与hlog10组一致），在常规数据分布下对比真跳转基线与长短分支全部谓词化的开销；

  case4再按命中情况构造四批输入，以不同命中率下的性能数据界定predicate优化的适用边界：

  | 数据批 | 输入构成 |
  | --- | --- |
  | 低值段为主批（low） | `ax <= 8.0f`的低值段 |
  | 高值段为主批（high） | `ax > 8.0f`的高值段（含中值段） |
  | 全特殊值批（special） | NaN / +Inf交替填充 |
  | 混合批（mixed） | 低值段、高值段、NaN、-Inf按`i % 4`交替 |

## 样例实现

本样例包含5个独立的kernel，每个kernel对应特定的Case版本。

| Case | 实现特点 | 使用的核函数 | 优化特性 |
|--------|---------------------------------------------------|--------------------|---------------------|
| Case 0 | 特殊值处理放在正常值路径之前，命中即提前return | hlog10_case0_kernel | 基线版本，真跳转修正链 |
| Case 1 | 正常值路径前置，修正链统一return覆盖temp | hlog10_case1_kernel | 谓词化修正链，减少跳转与分支栈操作 |
| Case 2 | NaN/Inf特殊值前置，所有分支命中即直接返回 | j0f_case2_kernel | 基线版本，全真跳转 |
| Case 3 | 分段计算逐级覆盖result，特殊值处理放最后，统一return | j0f_case3_kernel | 全分支统一return，长短分支全部谓词化；长分支体多发射开销超过控制开销的节省 |
| Case 4 | 分支体长的分段命中即返回，剩余短修正统一return | j0f_case4_kernel | 真跳转与谓词化混合，各取所长 |

各Case启动配置与输入生成方式一致（同组Case共用同一批输入的构造规则），区别仅在于kernel内分支链的组织方式。

### 性能指标说明

|             字段名             | 字段含义                                             |
|:---------------------------:|:-------------------------------------------------|
|      execute cycle          | kernel内`clock()`采样到的单线程执行周期数，反映kernel指令流的长度与分支开销。          |
|      Task Duration(μs)      | Task整体耗时，包含调度到加速器的时间、加速器上的执行时间以及响应结束时间。          |
|      aiv_total_cycles       | Task在Vector Core上执行所消耗的CPU周期（Cycle）总数。          |

除 Task Duration 外，其余指标均为所有 block 上的平均值。execute cycle由kernel内`clock()`直接打印；Task Duration与aiv_total_cycles通过msOpProf采集，方法见[性能调试](#性能调试)。

### Case 0: hlog10基线版本（分支发散）

**实现方式**：先对精度有偏差的六个特殊值做矫正，命中即返回修正结果；全部未命中则继续执行正常值路径。

**关键代码**：

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

**性能数据**（输入中约1/16为特殊值，其余为正常值）：

| execute cycle | Task Duration(μs) | aiv_total_cycles |
|:-----------:|:---------------:|:--------------:|
| 1890 | 14.919 | 5232.88 |

**性能数据分析**：

- Warp内少数线程命中特殊值后提前返回，多数线程继续走正常路径。提前返回的线程并不能使Warp跳过后续指令，仍需在汇合点等待其他线程执行完正常路径，造成计算资源浪费；6级真跳转判断链的分支栈开销也固定存在。两者构成本样例后续各优化版本的对比基线。

### Case 1: hlog10优化版本（谓词优化）

**实现方式**：先完整执行正常值路径得到temp，再对精度有偏差的六个特殊值做矫正，覆盖temp，最后统一返回temp。各修正分支的分支体只有一条赋值语句，整条链共用统一的返回出口。

**关键代码**：

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

**性能数据**（输入与Case 0同为混合批：约1/16为特殊值，其余为正常值）：

| execute cycle | Task Duration(μs) | aiv_total_cycles |
|:-----------:|:---------------:|:--------------:|
| 1406 | 14.946 | 5194.75 |

**优化效果分析**：

- 统一return后，编译器将修正链中的分支转换为带谓词的直线执行，跳转与分支栈操作减少，相比Case 0的1890，execute cycle降至1406。
- 正常路径前置后位于分支发散区域之外，命中特殊值的线程在汇合点的等待时间缩短；特殊值占比较小时，谓词化的额外执行量很小，aiv_total_cycles从Case 0的5232.88降至5194.75，性能收益明显。

### Case 2: j0f基线版本（分支发散）

**实现方式**：先判断 NaN/Inf 特殊值并即时返回；再按低值段、中值段、高值段的顺序逐段匹配，命中即返回该段结果。各分支独立返回，无统一出口。

**关键代码**：

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

将上面的提前return写法展开为等价的显式if-else嵌套，其真实控制流为：

```cpp
__aicore__ inline float j0f_case2(float x)
{
    if (isnan(x)) {
        return x;
    } else {
        float ax = fabsf(x);
        if (isinf(ax)) {
            return 0.0f;
        } else {
            if (ax <= 8.0f) {
                return __internal_j0f_less8(ax);
            } else {
                if (ax <= 1.0e13f) {
                    return __internal_j0f_middle_range(ax);
                } else {
                    return __internal_j0f_huge_range(ax);   // 尾部return实为最内层else
                }
            }
        }
    }
}
```

提前return隐式携带else分支，跳转与分支栈开销沿4级判断链逐级叠加。

**性能数据**（输入为常规批：大量数据落在`ax <= 8.0f`低值段，约1/16为NaN/Inf特殊值）：

| execute cycle | Task Duration(μs) | aiv_total_cycles |
|:-----------:|:---------------:|:--------------:|
| 2345 | 14.567 | 5025.12 |

**性能数据分析**：

- 常规批输入多在两次跳过特殊值判断后命中低值段分支，少量NaN/Inf输入在第一个分支即命中返回；未命中的判断仍产生跳转与分支栈开销，但各线程只执行命中方向的分段体，execute cycle为2345；
- aiv_total_cycles为全部block的平均值，其中首个block包含kernel启动初期的额外开销（下同）。

### Case 3: j0f全谓词化版本（谓词优化）

**实现方式**：先按高值段算出result，再依次执行中值段、低值段与NaN/Inf特殊值处理，命中则逐级覆盖result，最后统一返回。

**关键代码**：

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
        result = x;                         // NaN -> NaN
    }
    if (isinf(ax)) {
        result = 0.0f;                      // ±Inf -> 0
    }
    return result;
}
```

**性能数据**（输入与Case 2同为常规批：低值段为主 + 约1/16特殊值）：

| execute cycle | Task Duration(μs) | aiv_total_cycles |
|:-----------:|:---------------:|:--------------:|
| 14692 | 22.726 | 19514.88 |

**优化效果分析**：

- 相比Case 2的2345，execute cycle升至14692，Task Duration从14.567μs增至22.726μs，aiv_total_cycles从5025.12升至19514.88。Case 3反而显著慢于基线，原因在于长分支体的谓词化：hlog10组（Case 1）的修正分支体只有一条赋值语句，多发射代价很小；而j0f的三个分段体各是几十条指令的多项式求值，谓词化后每个输入都无条件执行高值段计算，多数低值段输入还多执行中值段计算，多发射的执行量远超节省的跳转与分支栈开销。
- Case 2与Case 3的对比说明：谓词化节省的控制开销是固定的，而多发射代价正比于分支体长度，分支体长时代价超过收益。Case 4进一步结合命中情况，探究谓词化的适用边界。

### Case 4: j0f优化版本（真跳转与谓词优化混合）

**实现方式**：低值段与中值段覆盖范围广且分支体长，命中即返回；未命中的输入按高值段算出result，再经NaN/Inf特殊值处理覆盖result，最后统一返回。

**关键代码**：

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
        result = x;
    }
    if (isinf(ax)) {
        result = 0.0f;
    }
    return result;
}
```

**性能数据**（四批输入，考察不同命中情况下的性能表现）：

| 数据批 | execute cycle | Task Duration(μs) | aiv_total_cycles |
|:---:|:-----------:|:---------------:|:--------------:|
| 低值段为主批（low） | 1767 | 14.389 | 4860.21 |
| 高值段为主批（high） | 2342 | 14.563 | 5468.46 |
| 全特殊值批（special） | 2395 | 14.088 | 5367.21 |
| 混合批（mixed） | 2704 | 14.608 | 5756.00 |

**优化效果分析**：

- 低值段为主批：命中低值段立即返回，除两个未命中的分支判断外无多余计算，execute cycle降至四批中最低的1767；
- 高值段为主批：只执行命中分支，execute cycle为2342，都快于Case 3的全谓词化；
- 全特殊值批：NaN/Inf输入先经过两次未命中的分段判断，再执行高值段计算，最后才被特殊值修正覆盖，execute cycle为2395；
- 混合批：长分支体真跳转与短分支谓词化各取所长，对不同分布的输入都只有少量额外执行，execute cycle为2704。

四批数据命中情况不同，性能表现随之变化：长分支体命中率高时提前return跳过的计算最多、收益最大（低值段为主批）；命中率低时真跳转仍快于全谓词化（高值段为主批）；分支体短的修正谓词化后多发射代价小，又能消除跳转与分支栈开销（混合批）。综合Case 3与Case 4即得predicate优化的边界：分支体短、命中率低的分支谓词化收益明显；分支体长或命中率高的分支保留真跳转、命中即提前return。

## 性能对比总结

**综合优化效果**：

- hlog10组（短修正分支链）：从Case 0基线版本到Case 1优化版本，execute cycle从1890降低到1406，下降约25.6%；
- j0f组（长短混合分段链）：Case 3长短分支全部谓词化，常规输入上execute cycle从Case 2的2345升至14692；Case 4保留长分支真跳转、仅谓词化短修正，各批输入上均快于Case 3，总体最优。

- hlog10组（约1/16特殊值的常规输入）：

| Case | execute cycle | Task Duration(μs) | aiv_total_cycles | 优化点 |
|--------|:-----------:|:---------------:|:--------------:|--------------------|
| Case 0 | 1890 | 14.919 | 5232.88 | 基线版本，特殊值前置、命中即提前return |
| Case 1 | **1406** | 14.946 | **5194.75** | 修正链统一return，跳转由6级减至4级、末级谓词化 |

- j0f组case2/3（常规输入：低值段为主 + 约1/16特殊值）：

| Case | execute cycle | Task Duration(μs) | aiv_total_cycles | 优化点 |
|--------|:-----------:|:---------------:|:--------------:|--------------------|
| Case 2 | **2345** | **14.567** | **5025.12** | 基线版本，特殊值前置、全部分支命中即返回 |
| Case 3 | 14692 | 22.726 | 19514.88 | 全部分支统一return、逐级覆盖，长短分支全部谓词化，execute cycle高于基线 |

- j0f组case4（execute cycle，按数据批）：

| 数据批 | Case 4 | 命中率特征 |
|--------|:-----:|--------------------|
| 低值段为主批（low） | **1767** | 低值段命中率高 |
| 高值段为主批（high） | 2342 | 中值段命中率高 |
| 全特殊值批（special） | 2395 | 特殊值命中率高 |
| 混合批（mixed） | 2704 | 各方向命中率接近 |

可见，predicate优化的收益与代价由分支体长短与命中情况共同决定：分支体短、命中率低的分支谓词化收益明显；分支体长或命中率高的分支保留真跳转、命中即提前return。两种机制按此取舍组合，方能覆盖长短混合的分支链（Case 4）。

## 调优建议

1. **谓词化短分支**：分支体短时谓词化收益明显，长而浅的修正链整链谓词化收益更大；分支体长或带副作用（写内存、原子操作、同步）时保留真分支（详见[样例描述](#样例描述)中的谓词化适用条件分析）。

2. **按可if-conversion的写法组织分支**：用临时变量承接各分支结果、统一出口返回，不在分支中间提前return；每个分支体只保留覆盖临时变量这类短指令，长计算留在真分支中。

3. **长分支体或高命中率分支保留提前return**：分支命中后跳过的后续计算越多，保留真跳转、只执行命中方向越划算，可将这类分支前置。

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。

- 配置环境变量  
  请根据当前环境上CANN开发套件包的[安装方式](../../../../../../docs/zh/quick_start.md#prepare&install)，配置环境变量。

  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **说明：** `${install_path}` 为CANN包安装目录，未指定安装目录时默认安装至 `/usr/local/Ascend` 下。

- 样例执行

  在本样例目录下执行如下命令。

  ```bash
  mkdir -p build && cd build;                         # 创建并进入build目录
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=1 ..; make -j;   # 编译工程（选择用例1）
  ./predicate_optimization                            # 执行样例
  ```

  使用NPU仿真模式时，添加`-DCMAKE_ASC_RUN_MODE=sim`参数即可。

  示例如下：
  ```bash
  cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=1 ..; make -j;   # NPU仿真模式
  ```

  > **注意：** 切换用例或编译模式前需清理cmake缓存，可在build目录下执行`rm CMakeCache.txt`后重新cmake。

- 编译选项说明

  | 选项                        | 可选值        | 说明                                                |
  |---------------------------|------------|---------------------------------------------------|
  | `CMAKE_ASC_RUN_MODE` | `npu`（默认）、`sim` | 运行模式：NPU运行、NPU仿真 |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU架构：本样例仅支持 dav-3510（Ascend 950PR/Ascend 950DT） |
  | `SCENARIO_NUM` | `1` ~ `8`（默认`1`） | 编译期选择运行的用例，见下表 |

  | SCENARIO_NUM | 用例 | 说明 |
  | --- | --- | --- |
  | 1 | hlog10_case0 | 基线版本 |
  | 2 | hlog10_case1 | 优化版本 |
  | 3 | j0f_case2 | 基线版本 |
  | 4 | j0f_case3 | 全谓词化版本 |
  | 5 / 6 / 7 / 8 | j0f_case4_low / j0f_case4_high / j0f_case4_special / j0f_case4_mixed | 优化版本，输入批依次为低值段为主/高值段为主/全特殊值/混合 |

  每次编译只包含选定的用例：换用不同的SCENARIO_NUM重新编译并运行，即可逐个完成各用例、完成各组的完整对比。

## 性能调试

### msOpProf工具介绍

msOpProf工具是单算子性能分析工具。包含msopprof和msopprof simulator两种使用方式。该工具协助用户定位算子内存、算子代码以及算子指令的异常，实现全方位的算子调优。当前支持基于不同运行模式（上板或仿真）和不同文件形式（可执行文件或算子二进制.o文件）进行性能数据的采集和自动解析。

使用 `msOpProf` 工具获取单个组件上的性能数据：

```bash
msopprof ./predicate_optimization   # 分析样例的性能（用例由编译时的SCENARIO_NUM决定）
```

命令完成后，会在默认目录下生成以"OPPROF_{timestamp}_XXX"命名的文件夹,性能数据文件夹结构示例如下：

```text
├──dump                       # 原始的性能数据，用户无需关注
├──ArithmeticUtilization.csv  # cube/vector指令cycle占比
├──L2Cache.csv                # L2 Cache命中率
├──Memory.csv                 # UB，L1和主存储器读写带宽速率
├──MemoryL0.csv               # L0A，L0B，和L0C读写带宽速率
├──MemoryUB.csv               # Vector和Scalar到UB的读写带宽速率
├──OpBasicInfo.csv            # 算子基础信息
├──PipeUtilization.csv        # 采集计算单元和搬运单元耗时和占比
├──ResourceConflictRatio.csv  # UB上的 bank group、bank conflict和资源冲突率在所有指令中的占比
└──visualize_data.bin         # MindStudio Insight呈现文件
```

查看具体的性能分析结果：

```
# 如查看Task Duration 相关数据
cat ./OPPROF_*/OpBasicInfo.csv
```
