# SIMD与SIMT混合编程实现快速除法算子样例

## 概述

本样例基于SIMD与SIMT混合编程方式实现快速除法算子，针对除数固定的批量整数除法场景，使用乘法和移位操作替代传统除法运算，展示混合编程模式下使用快速除法算法提升除法运算效率的性能优化方式。

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 支持的CANN软件版本
- \>= CANN 9.1.0

## 目录结构

```
├── simt_and_simd_integer_fast_div
│   ├── CMakeLists.txt              # cmake编译文件
│   ├── integer_fast_div.asc        # 快速除法算子样例
│   ├── integer_normal_div.asc      # 普通除法算子样例（对比基准）
│   └── README.md
```

## 算子描述

- 算子功能：  
  实现对一维输入张量input中每个元素除以指定除数divisor的功能。算子输出output第i个元素计算公式为：
  
  ```
  output[i] = input[i] / divisor
  ```

  其中input每个元素的取值范围为 `[0, INT32_MAX]`。

- 算子规格：  
  <table>
  <tr><td rowspan="1" align="center">算子类型(OpType)</td><td colspan="4" align="center">integer_div</td></tr>
  <tr><td rowspan="3" align="center">算子输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">input</td><td align="center">32768</td><td align="center">uint32_t</td><td align="center">ND</td></tr>
  <tr><td align="center">divisor</td><td align="center">标量</td><td align="center">uint32_t</td><td align="center">-</td></tr>
  <tr><td rowspan="1" align="center">算子输出</td><td align="center">output</td><td align="center">32768</td><td align="center">uint32_t</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">快速除法核函数名</td><td colspan="4" align="center">integer_fast_div / integer_fast_div_clock</td></tr>
  <tr><td rowspan="1" align="center">普通除法核函数名</td><td colspan="4" align="center">integer_normal_div / integer_normal_div_clock</td></tr>
  </table>

- 线程层次结构：
  * 线程块数：64
  * 单个线程块中线程数：512

- 算子实现：  
  integer_normal_div.asc中直接使用除法运算符作为性能对比基准。整数除法在NPU上是高延迟指令，相比乘法和移位操作执行开销显著更高。
  当除数固定时，可以使用乘法和移位操作完成等价的快速除法计算，避免触发高延迟的硬件除法指令。

  对于每个输入的uint32类型元素，快速除法的计算方式如下：
  $$
  \text{output}[i] = \left\lfloor \frac{\text{input}[i] + \left\lfloor \dfrac{\text{input}[i] \times \text{magic}}{2^{32}} \right\rfloor}{2^{\text{shift}}} \right\rfloor
  $$
  
  其中魔数magic和移位量shift通过`calc_magic_shift`接口，根据输入的除数divisor预先计算获取，使用Scalar计算单元中的`clz()`、`bcnt1()`标量计算接口，计算方式如下：
  ```cpp
  int64_t pos = BIT_64_LEN - CountLeadingZero(divisor);  // CountLeadingZero: count leading zeros of divisor via clz
  int64_t cnt1 = GetBitCount1(divisor);                  // GetBitCount1: count 1-bits of divisor via bcnt1
  uint32_t shift = (cnt1 == 1) ? (pos - 1) : pos;
  uint32_t magic = (1l << BIT_32_LEN) * ((1l << shift) - divisor) / divisor + 1;
  ```
  SIMT 中读取预先从Global Memory搬运到UB上的input，每个线程通过乘法和移位操作完成一个元素的除法，结果写回Global Memory。代码示例如下：
  ```cpp
  uint32_t idx = blockIdx.x * blockDim.x + threadIdx.x;
  ...
  uint64_t start_clock = clock();
  // fast division: replace division with multiply and shift
  uint32_t q = __umulhi(value, magic);
  uint32_t result = (value + q) >> shift;

  uint64_t end_clock = clock();
  output[idx] = result;
  clock_cycles[idx] = end_clock - start_clock;
  ```

  通过在计算前后调用 `clock()` 记录时间戳，能够统计每个线程的除法操作耗时。

- 性能收益：

  **单次除法操作cycle数对比**

  使用NPU硬件时钟周期（clock cycle）统计每个线程执行除法操作的耗时，每个线程通过 `clock()` 记录除法操作前后的时间差，得到一次除法操作的cycle数。平均clock cycles为所有线程统计除法操作cycle数的平均值。快速除法与普通除法的平均clock cycles对比如下：

  | 数据量（N） | 快速除法（cycles） | 普通除法（cycles） |
  |:-----------:|:-----------------:|:-----------------:|
  | 32,768      | 14.00             | 38.25             |

  N=32768 时，快速除法平均 clock cycles 约为 14.00，普通除法约为 38.25。

  **asc_vf_call整体耗时对比**

  通过在核函数内使用 `GetSystemCycle()` 对 `asc_vf_call` 前后打点，能够采集SIMT VF函数的执行时间。以下数据为分别重复运行快速除法和普通除法样例各10次后，取第0号核测量结果统计得到，数据量32768。结果对比如下：

  | 除法方式 | 各次结果（cycles） | 均值 |
  |:--------:|:-----------------|:----:|
  | 快速除法 | 880, 543, 526, 611, 527, 526, 558, 618, 570, 515 | 587.40 |
  | 普通除法 | 867, 949, 1020, 941, 601, 1317, 1148, 756, 945, 858 | 940.20 |

  快速除法均值（587.40 cycles）约为普通除法均值（940.20 cycles）的62%，平均减少约353 cycles。其中，快速除法核函数中执行 `calc_magic_shift` 预计算magic和shift，该接口耗时约19 cycles，该开销与输入数据量无关。

## 编译运行

在本样例根目录下执行如下步骤，编译并执行算子。

- 配置环境变量   
  请根据当前环境上CANN开发套件包的[安装方式](../../../../../docs/quick_start.md#prepare&install)，选择对应配置环境变量的命令。
  - 默认路径，root用户安装CANN软件包
    ```bash
    source /usr/local/Ascend/cann/set_env.sh
    ```

  - 默认路径，非root用户安装CANN软件包
    ```bash
    source $HOME/Ascend/cann/set_env.sh
    ```

  - 指定路径install_path，安装CANN软件包
    ```bash
    source ${install_path}/cann/set_env.sh
    ```
    
- 样例执行  
  快速除法执行方式：
  ```bash
  mkdir -p build && cd build;                                          # 创建并进入build目录
  cmake -DSCENARIO_NUM=0 -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..; make -j;   # 编译工程
  ./integer_div                                                               # 执行样例
  ```
  普通除法执行方式：
  ```bash
  mkdir -p build && cd build;                                          # 创建并进入build目录
  cmake -DSCENARIO_NUM=1 -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..; make -j;   # 编译工程
  ./integer_div                                                               # 执行样例
  ```

  编译选项说明

  | 选项 | 可选值 | 说明 |
  |------|--------|------|
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU 架构：本样例仅支持 dav-3510（Ascend 950PR/Ascend 950DT） |
  | `SCENARIO_NUM` | `0`（默认）、`1` | 除法实现版本：0为快速除法，1为普通除法 |

  执行结果如下，说明精度对比成功。

  SCENARIO_NUM=0（快速除法）：
  ```
  [INFO] num_blocks: 64, THREAD_COUNT: 512
  [Magic Cycles] Block 0 calc_magic_shift elapsed cycles: 19
  [VF Cycles] Block 0 simt_fast_div elapsed cycles: 611
  [Total Sum] Total cycle sum of all threads: 458752
  [Overall Average] Overall average cycles of all threads: 14
  Output: 285190763 273084670 97058763 40098825 56423234 12197552 62755693 253536868 174168601 163232660 182703215 293380004 295897023 141711313 200383876 283387511 229752108 114712801 200504374 47543249 ...
  Golden: 285190763 273084670 97058763 40098825 56423234 12197552 62755693 253536868 174168601 163232660 182703215 293380004 295897023 141711313 200383876 283387511 229752108 114712801 200504374 47543249 ...
  [Success] Case accuracy is verification passed.
  ```

  SCENARIO_NUM=1（普通除法）：
  ```
  [INFO] num_blocks: 64, THREAD_COUNT: 512
  [VF Cycles] Block 0 simt_normal_div elapsed cycles: 941
  [Total Sum] Total cycle sum of all threads: 1253376
  [Overall Average] Overall average cycles of all threads: 38.25
  Output: 285190763 273084670 97058763 40098825 56423234 12197552 62755693 253536868 174168601 163232660 182703215 293380004 295897023 141711313 200383876 283387511 229752108 114712801 200504374 47543249 ...
  Golden: 285190763 273084670 97058763 40098825 56423234 12197552 62755693 253536868 174168601 163232660 182703215 293380004 295897023 141711313 200383876 283387511 229752108 114712801 200504374 47543249 ...
  [Success] Case accuracy is verification passed.
  ```
