# SIMD与SIMT混合编程实现快速除法算子样例

## 概述

本样例基于SIMD与SIMT混合编程方式实现快速除法算子，使用魔数乘法和移位操作替代传统除法运算，展示混合编程模式下使用快速除法算法提升除法运算效率的性能优化方式。

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 支持的CANN软件版本
- \>= CANN 9.0.0

## 目录结构

```
├── integer_fast_div
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

- 算子规格：  
  <table>
  <tr><td rowspan="1" align="center">算子类型(OpType)</td><td colspan="4" align="center">integer_fast_div</td></tr>
  <tr><td rowspan="3" align="center">算子输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">input</td><td align="center">[N]</td><td align="center">uint32_t</td><td align="center">ND</td></tr>
  <tr><td align="center">divisor</td><td align="center">标量</td><td align="center">uint32_t</td><td align="center">-</td></tr>
  <tr><td rowspan="1" align="center">算子输出</td><td align="center">output</td><td align="center">[N]</td><td align="center">uint32_t</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">integer_fast_div</td></tr>
  </table>

- 算子实现：  
  线程块数及每块线程数根据数据量和可用AIV核数动态分配，单线程处理1个元素。对于uint32类型的快速除法，计算逻辑为：
  $$
  \text{high} = \left\lfloor \frac{\text{input}[i] \times \text{magic}}{2^{32}} \right\rfloor
  $$
  $$
  \text{output}[i] = \left\lfloor \frac{\text{input}[i] + \text{high}}{2^{\text{shift}}} \right\rfloor
  $$
  
  calc_magic_shift根据除数计算对应的魔数magic和移位量shift，计算方式如下：
  ```cpp
  int64_t pos = 64 - clz(divisor);           // 除数的最高有效位位置
  int64_t cnt1 = bcnt1(divisor);             // 除数的二进制表示中1的个数
  uint32_t shift = (cnt1 == 1) ? (pos - 1) : pos;
  uint32_t magic = (1l << 32) * ((1l << shift) - divisor) / divisor + 1;
  ```
  SIMT核函数从Global Memory读取input，每个线程通过魔数乘法和移位操作完成一个元素的除法，结果写回Global Memory。
  
  代码示例：
  ```
  uint32_t idx = blockIdx.x * blockDim.x + threadIdx.x;
  ...
  uint32_t q = __umulhi(value, magic);
  uint32_t result = (value + q) >> shift;
  output[idx] = result;
  ```
  
  integer_normal_div.asc中直接使用除法运算符作为性能对比基准。

- 性能收益：  
  使用NPU硬件时钟周期（clock cycle）统计每个线程执行除法操作的耗时，clock cycle为NPU核心的最小时间单位，数值越小表示执行越快。以下为不同数据量下（除数为7），快速除法与普通除法的平均clock cycles对比：

  | 数据量（N） | 快速除法（cycles） | 普通除法（cycles） | 加速比 |
  |:-----------:|:-----------------:|:-----------------:|:------:|
  | 8,192       | 4.00              | 18.00             | 4.50x  |
  | 32,768      | 13.31             | 27.99             | 2.10x  |
  | 131,072     | 23.23             | 63.50             | 2.73x  |
  | 524,288     | 23.28             | 70.73             | 3.04x  |
  | 1,048,576   | 23.25             | 70.03             | 3.01x  |

## 编译运行

在本样例根目录下执行如下步骤，编译并执行算子。

- 配置环境变量   
  请根据当前环境上CANN开发套件包的安装方式，选择对应配置环境变量的命令。
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
  ```bash
  mkdir -p build && cd build;   # 创建并进入build目录
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..; make -j;   # 编译工程
  ./integer_div                         # 执行样例，可通过 -n <总元素数> -d <除数> 指定参数，如 ./integer_div -n 32768 -d 7
  ```

  编译选项说明

  | 选项 | 可选值 | 说明 |
  |------|--------|------|
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU 架构：本样例仅支持 dav-3510（Ascend 950PR/Ascend 950DT） |

  执行结果如下，说明精度对比成功。
  ```
  [Success] Case accuracy is verification passed.
  ```
