# Warp分支发散谓词优化样例

## 概述

本样例通过数学接口`hlog10`、`j0f`展现SIMT编程模式下[Warp分支发散](../../../../../../docs/zh/guide/programming_guide/programming_model/ai_core_simt_programming/thread_architecture.md#warp执行机制)的调优方法，重点呈现了predicate（谓词）优化的性能收益与适用场景。

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
## 基础知识说明

- **Warp与分支发散（Warp Divergence）**

  在SIMT编程模式下，Warp是基本的调度和执行单位，当代码中出现分支判断时，Warp内的线程会产生分支发散（Warp divergence），各分支代码串行执行，发散点处汇合点地址与各方向的活跃线程掩码被压入分支栈（Divergence Stack），并通过活跃掩码（active mask）屏蔽未进入当前分支的线程、仅让进入该分支的线程执行。更多说明请参考[Warp执行机制](../../../../../../docs/zh/guide/programming_guide/programming_model/ai_core_simt_programming/thread_architecture.md#warp执行机制)。

  以Warp内一半线程`cond`为真、一半为假的if-else为例：

  ```cpp
  if (cond) {
      r = a + b;   
  } else {
      r = a - b;   
  }
  ```

  上述if-else在分支发散下的控制流如下：

  ![Warp分支发散的控制流](./figures/branch.png "Warp分支发散的控制流")

- **Predicate（谓词）优化**

  为避免分支切换产生的开销，编译器可通过predicate优化（又称if-conversion）将简单分支改写为直线执行：分支条件不再驱动跳转，而是被计算为每线程一个谓词位（predicate flag）；分支体内的指令被改写为带谓词的指令，Warp内32个线程**全部发射**，指令流保持一条直线，仅谓词为真的线程写回结果，谓词为假的线程丢弃结果（不写回寄存器、不产生访存副作用）。同样的if-else，predicate优化后的控制流如下：

  ![谓词优化后的控制流](./figures/predicate.png "谓词优化后的控制流")

  将两种机制下Warp的完整指令流逐环节对比：

  | 环节 | 分支发散（divergence） | 谓词执行（predication） |
  | --- | --- | --- |
  | 控制流指令 | 每个分支一条跳转指令 | 无跳转，只有一次条件计算 |
  | 分支栈 | 发散点压栈、逐方向弹栈、汇合点弹栈，嵌套分支叠加 | 完全没有分支栈操作 |
  | 分支体执行 | 各方向串行执行，每趟约一半线程空转 | 两个分支体各执行一次，每个发射槽都对一半线程有效 |
  | 汇合 | 需要硬件同步恢复掩码 | 天然顺序执行，无需汇合 |

  汇总成指令序列：分支发散下，Warp依次发出`条件计算 → 跳转 → 压栈 → if体 → 弹栈切换 → else体 → 弹栈汇合`；predicate优化后，Warp依次发出`条件计算 → if体 → else体`。由此可见，predicate优化可以避免divergence中跳转和分支栈的开销。

## hlog10样例描述

- 样例功能

  以[hlog10](../../../../api/SIMT-API/math_functions/half_type/half_math_functions/hlog10.md)分支链路为载体。使用`logf(x) / logf(10.0f)`计算输入数据的常用对数，并对6个精度有偏差的特殊值各做一步修正，样例以8192个half元素为输入。通过对比“分支命中即return”和“使用临时变量存储结果，统一return”的性能差异，展示predicate优化的收益来源。

- 样例规格：
  <table>
  <tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="4" align="center">hlog10</td></tr>
  <tr><td rowspan="3" align="center">样例输入/输出</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">input / output</td><td align="center">[8192]</td><td align="center">half</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">hlog10（两组Case共用，由编译宏SCENARIO_NUM选择Case版本）</td></tr>
  </table>

## hlog10样例实现

本组两组Case共用同一个核函数名`hlog10`，由编译宏`SCENARIO_NUM`在编译期选择Case版本。

| Case | 实现特点 | 使用的核函数 | 分支链处理方式 |
|--------|---------------------------------------------------|--------------------|---------------------|
| Case 0 | 特殊值处理放在正常值路径之前，命中即提前return | hlog10 | 基线版本divergence |
| Case 1 | 正常值路径前置，修正链统一return覆盖temp | hlog10 | predicate优化 |

各Case启动配置与输入生成方式一致（同组Case共用同一批输入的构造规则），区别仅在于kernel内分支链的组织方式。

### 性能指标说明

|             字段名             | 字段含义                                             |
|:---------------------------:|:-------------------------------------------------|
|      execute cycle          | kernel内`clock()`采样到的单线程执行周期数，反映kernel指令流的长度与分支开销。          |
|      Task Duration(μs)      | Task整体耗时，包含调度到加速器的时间、加速器上的执行时间以及响应结束时间。          |
|      aiv_total_cycles       | Task在Vector Core上执行所消耗的CPU周期（Cycle）总数。          |

execute cycle由kernel内`clock()`在block 0的线程0上采样并写回GM、host侧读出，性能数据取30次运行的中位数；Task Duration与aiv_total_cycles通过msOpProf采集（各采集3次取中位数），其中aiv_total_cycles为所有block上的平均值，采集方法见[性能调试](#性能调试)。

### Case 0: hlog10分支链divergence版本

**实现方式**：先对精度有偏差的六个特殊值做矫正，命中即返回修正结果；全部未命中则继续执行正常值路径。

**关键代码**：

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

将上述提前return写法展开为等价的显式if-else嵌套，其真实控制流为：

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

**性能数据**：

| execute cycle | Task Duration(μs) | aiv_total_cycles |
|:-----------:|:---------------:|:--------------:|
| 433 | 2.198 | 2399.50 |

**性能数据分析**：

- 正常值路径位于6级判断链的最内层，需依次通过6个条件判断才能到达，逐级叠加的跳转与分支栈开销无法避免。

### Case 1: hlog10分支链predicate优化版本

**实现方式**：先完整执行正常值路径得到temp，再对精度有偏差的六个特殊值做矫正，覆盖temp，最后统一return。

**关键代码**：

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

**性能数据**：

| execute cycle | Task Duration(μs) | aiv_total_cycles |
|:-----------:|:---------------:|:--------------:|
| 394.5 | 2.226 | 2385.75 |

**优化效果分析**：

- 统一return后，编译器将会自动进行predicate优化，消除了跳转与分支栈操作，相比Case 0的433，execute cycle降至394.5，下降约8.9%。
- 提前return在实际场景中很难触发：SIMT代码实际以warp为单位执行，必须warp内所有线程的数据都是特殊值才能提前return，从概率上可以忽略不计，绝大部分场景下，特殊值线程需要等其他线程完成计算后再一起往下执行。

对比Case0和Case1可得：Warp 内出现分支发散时，predicate优化相对divergence的收益，主要来自消除跳转与分支栈开销。但若某分支命中率较高，predicate优化会让这部分线程执行完全部指令才返回，多执行大量无用指令，会造成性能损失。后续样例将对不同命中率的分支采用不同处理方式，展示predicate优化的适用范围。

## j0f样例描述

- 样例功能

以[j0f](../../../../api/SIMT-API/math_functions/float_math_functions/j0f.md)分支链路为载体。计算输入数据第一类零阶贝塞尔函数。接口按输入数据绝对值分三段计算，并对两个特殊值进行单独处理。样例以8192个float元素为输入，正常情况下数据的取值范围为`ax <= 1.0e13f`。

- 样例规格：
  <table>
  <tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="4" align="center">j0f</td></tr>
  <tr><td rowspan="3" align="center">样例输入/输出</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">input / output</td><td align="center">[8192]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">j0f（三组Case共用，由编译宏SCENARIO_NUM选择Case版本）</td></tr>
  </table>

  case2与case3在常规数据分布下对比divergence基线与predicate优化的开销；case4按命中情况构造两批输入，以不同命中率下的性能数据界定predicate优化的适用边界：

  | 输入数据 | 数据范围 |
  | --- | --- |
  | input（normal） | `ax <= 1.0e13f` |
  | input（mixed） | `ax <= 8.0f`、`8.0f < ax <= 1.0e13f`、`ax > 1.0e13f`、NaN、-Inf按`i % 5`交替 |

## j0f样例实现

本组三组Case共用同一个核函数名`j0f`，由编译宏`SCENARIO_NUM`在编译期选择Case版本。

| Case | 实现特点 | 使用的核函数 | 分支链处理方式 |
|--------|---------------------------------------------------|--------------------|---------------------|
| Case 2 | NaN/Inf特殊值前置，所有分支命中即直接返回 | j0f | 基线版本divergence |
| Case 3 | 分段计算逐级覆盖result，特殊值处理放最后，统一return | j0f | predicate优化，统一return |
| Case 4 | 命中率高的分支命中即返回，剩余分支统一return | j0f | divergence与predicate优化结合 |

各Case启动配置与输入生成方式一致（同组Case共用同一批输入的构造规则），区别仅在于kernel内分支链的组织方式。

本组各Case的性能指标含义与采集方式同上，参见[性能指标说明](#性能指标说明)。

### Case 2: j0f分支链divergence版本

**实现方式**：先判断 NaN/Inf 特殊值并即时返回；再顺序逐段匹配，命中即返回该段结果。各分支独立返回，无统一出口。

**关键代码**：

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

**性能数据**：

| execute cycle | Task Duration(μs) | aiv_total_cycles |
|:-----------:|:---------------:|:--------------:|
| 1221.5 | 2.551 | 3015.88 |

**性能数据分析**：

- 输入数据不含特殊值，两次特殊值判断均未命中；各线程命中对应分段并返回，只执行命中方向的分段体，未命中的判断仍逐级产生跳转与分支栈开销，execute cycle为1221.5；

### Case 3: j0f分支链计算后统一return版本

**实现方式**：先计算`ax > 1.0e13f`范围的结果，再依次执行其余分支，命中则逐级覆盖result，最后统一返回。

**关键代码**：

```cpp
__aicore__ inline float j0f(float x)
{
    float ax = fabsf(x);
    float result = __internal_j0f_huge_range(ax);   
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

**性能数据**：

| execute cycle | Task Duration(μs) | aiv_total_cycles |
|:-----------:|:---------------:|:--------------:|
| 2104.5 | 2.829 | 3468.25 |

**优化效果分析**：

- 相比Case 2的1221.5，execute cycle升至2104.5，Task Duration从2.551μs增至2.829μs，aiv_total_cycles从3015.88升至3468.25。Case 3反而显著慢于基线，原因在于统一return后三段计算全部无条件执行，大量线程做了无用计算，造成性能损耗。
- Case3和Case2的对比结果说明在进行predicate优化时，需要考虑分支命中率，Case 4进一步结合命中情况，探究谓词化的适用边界。

### Case 4: j0f分支链高命中率分支提前return,低命中率分支predicate优化版本

**实现方式**：命中率高的分支命中即返回；未命中的分支先计算`ax > 1.0e13f`范围的结果，再处理特殊值覆盖result，最后统一返回。

**关键代码**：

```cpp
__aicore__ inline float j0f(float x)
{
    float ax = fabsf(x);
    if (ax <= 8.0f) {
        return __internal_j0f_less8(ax);    
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

**性能数据**：

| 数据批 | execute cycle | Task Duration(μs) | aiv_total_cycles |
|:---:|:-----------:|:---------------:|:--------------:|
| input（normal） | 1301 | 2.538 | 2972.25 |
| input（mixed） | 19260 | 13.136 | 18508.50 |

**优化效果分析**：

- input（normal）：命中`ax <= 8.0f`和` 8.0f =< ax <= 1.0e13f`时立即返回，不执行后续未命中的分支计算，execute cycle为1301，与Case 2基线1221.5接近；Task Duration与aiv_total_cycles分别为2.538μs与2972.25，略低于Case 2的2.551μs与3015.88，整体开销与基线相当；
- input（mixed）：四类数据交替，分段跳转分支产生divergence，特殊值触发Payne-Hanek辐角缩减计算，execute cycle为19260。

两批数据性能对比结果可知，输入数据落在`ax <= 1.0e13f`范围内时，execute cycle最低：命中率高的分段提前return只执行命中方向，剩余分支predicate优化，两种机制结合性能最优。

## 性能对比总结

**综合优化效果**：

- hlog10样例：从Case 0基线版本到Case 1优化版本，execute cycle从433降低到394.5，下降约8.9%；
- j0f样例：输入数据构成相同时，从Case 2基线版本到Case 3,execute cycle从1221.5升至2104.5；Case 4在input（normal）上execute cycle为1301，与Case 2接近，且明显快于Case 3。

- hlog10组（全正常值输入）：

| Case | execute cycle | Task Duration(μs) | aiv_total_cycles | 优化点 |
|--------|:-----------:|:---------------:|:--------------:|--------------------|
| Case 0 | 433 | 2.198 | 2399.50 | 基线版本，特殊值前置、命中即提前return |
| Case 1 | **394.5** | 2.226 | 2385.75 | 修正链统一return，整链谓词化消除跳转与分支栈开销 |

- j0f组case2/3（常规输入：`ax <= 8.0f`为主，约1/16落在`8.0f < ax <= 1.0e13f`）：

| Case | execute cycle | Task Duration(μs) | aiv_total_cycles | 优化点 |
|--------|:-----------:|:---------------:|:--------------:|--------------------|
| Case 2 | **1221.5** | 2.551 | 3015.88 | 基线版本，特殊值前置、全部分支命中即返回 |
| Case 3 | 2104.5 | 2.829 | 3468.25 | 全部分支统一return、逐级覆盖，长短分支全部谓词化，execute cycle高于基线 |

- j0f组case4（execute cycle，按数据批）：

| 数据批 | Case 4 | 命中率特征 |
|--------|:-----:|--------------------|
| input（normal） | **1301** | 低值段命中率高 |
| input（mixed） | 19260 | 各方向命中率接近，含特殊值 |

复杂逻辑编译器不会用predicate优化，在开发时需要根据输入数据的分布，考虑提前return的收益，引导编译器选择divergence或者predicate优化，来提高算子性能。

## 调优建议

1. **使用predicate优化分支链路**：当出现warp divergence时，可使用predicate优化消除分支跳转开销。

2. **divergence和predicate优化结合使用**：当分支链路中各分支命中率不同时，命中率高的分支保留divergence，命中率低的分支进行predicate优化，既可以避免执行不必要的指令又可以消除分支跳转的开销。

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
  | `SCENARIO_NUM` | `1` ~ `6`（默认`1`） | 编译期选择运行的用例，见下表 |

  | SCENARIO_NUM | 用例 | 说明 |
  | --- | --- | --- |
  | 1 | hlog10 | 基线版本 |
  | 2 | hlog10 | 优化版本 |
  | 3 | j0f | 基线版本 |
  | 4 | j0f | 全谓词化版本 |
  | 5 / 6 | j0f | 优化版本，输入批依次为常规批（`ax <= 8.0f`为主）/混合批 |

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
