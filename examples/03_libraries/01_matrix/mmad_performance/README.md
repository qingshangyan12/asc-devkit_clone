# Mmad_Performance样例
## 概述
本样例介绍基于基础API实现矩阵乘Mmad_Performance样例，主要实现三种场景的性能测试：
  mmad_fixpipe_after_all_mmad 是在所有迭代的mmad后，再进行fixpipe; 
  mmad_fixpipe_per_mmad 是在每次迭代的mmad之后，都进行fixpipe;
  mmad_uniflag 是通过uniflag控制mmad和fixpipe对于LOC的读写逻辑。

## 支持的产品
- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍
```
├── mmad_performance
│   ├── cases                       // 输入case的构造csv
│   ├── cmake                       // 编译工具链
│   ├── mmad_fixpipe_after_all_mmad // Ascend C算子实现 & 调用样例
│   ├── mmad_fixpipe_per_mmad
│   ├── mmad_uniflag
│   ├── scripts
│   │   ├── run_test.py             // 批跑脚本
│   ├── CMakeLists.txt              // 编译工程文件
│   └── batch_run.sh                // 批跑入口
```

## 算子描述
- 算子功能：  
  本样例中实现的是[m, n, k]的Matmul算子，并使用Ascend C基础Api实现。对应的数学表达式为：
  ```
  C = A * B
  ```
- 算子规格：
  <table>
  <tr><td rowspan="1" align="center">算子类型(OpType)</td><td colspan="4" align="center">Mmad</td></tr>
  </tr>
  <tr><td rowspan="3" align="center">算子输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">x</td><td align="center">M * K</td><td align="center">half</td><td align="center">ND</td></tr>
  <tr><td align="center">y</td><td align="center">K * N</td><td align="center">half</td><td align="center">ND</td></tr>
  </tr>
  </tr>
  <tr><td rowspan="1" align="center">算子输出</td><td align="center">z</td><td align="center">M * N</td><td align="center">float</td><td align="center">ND</td></tr>
  </tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">mmad_custom</td></tr>
  </table>
- 算子实现：  
  Mmad算子的数学表达式为：
  ```
  C = A * B
  ```
  计算逻辑是：Ascend C提供的矩阵乘计算接口的操作元素都为LocalTensor，输入数据需要先搬运进片上存储并进行分形转换，然后使用计算接口完成两个输入参数矩阵乘运算，得到最终结果，再搬出到外部存储上。  

  Mmad算子的实现流程分为基本任务：CopyIn，SplitA，SplitB，Compute，CopyOut。CopyIn任务负责将Global Memory上的输入inputGM搬运到Local Memory A1/B1中，搬运过程中进行ND至NZ分形转换。SplitA/SplitB分别将数据进一步搬运至接口所要求Local Memory A2/B2，Compute任务负责对数据进行矩阵乘运算，计算结果存储在Local Memory CO1中。CopyOut任务负责将输出数据从CO1搬运至Global Memory上的输出outputGm中，同时完成NZ到ND的分形转换。
  - 调用实现  
    使用内核调用符<<<>>>调用核函数。

## 编译运行
在本样例根目录下执行如下步骤，编译并执行算子。
- 配置环境变量  
  请根据当前环境上CANN开发套件包的[安装方式](../../../../docs/quick_start.md#prepare&install)，选择对应配置环境变量的命令。
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
  bash batch_run.sh
  ```
  执行结果如下，说明性能测试成功。
  ```bash
  ✅ 用例 [] 执行成功
  ```

## 性能结果
| DataType | M | N | K | Duration(ns) |
| --- | --- | --- | --- | --- |
| int8_t | 8 | 8 | 8 | 35.136 |
| int8_t | 16 | 16 | 16 | 35.136 |
| int8_t | 24 | 24 | 24 | 36.217 |
| int8_t | 32 | 32 | 32 | 36.217 |
| int8_t | 48 | 48 | 48 | 43.784 |
| int8_t | 64 | 64 | 64 | 51.353 |
| int8_t | 80 | 80 | 80 | 67.568 |
| int8_t | 96 | 96 | 96 | 92.433 |
| int8_t | 112 | 112 | 112 | 140.001 |
| int8_t | 128 | 128 | 128 | 172.433 |
| int8_t | 144 | 144 | 144 | 252.973 |
| int8_t | 160 | 160 | 160 | 294.595 |
| int8_t | 176 | 176 | 176 | 426.487 |
| half | 8 | 8 | 8 | 35.136 |
| half | 16 | 16 | 16 | 35.136 |
| half | 24 | 24 | 24 | 38.379 |
| half | 32 | 32 | 32 | 38.379 |
| half | 48 | 48 | 48 | 48.649 |
| half | 64 | 64 | 64 | 67.568 |
| half | 80 | 80 | 80 | 101.622 |
| half | 96 | 96 | 96 | 150.812 |
| half | 112 | 112 | 112 | 219.460 |
| half | 128 | 128 | 128 | 310.810 |
| half | 144 | 144 | 144 | 428.108 |
| half | 160 | 160 | 160 | 574.595 |
| half | 176 | 176 | 176 | 748.649 |
| float | 8 | 8 | 8 | 36.217 |
| float | 16 | 16 | 16 | 38.380 |
| float | 24 | 24 | 24 | 47.028 |
| float | 32 | 32 | 32 | 51.353 |
| float | 48 | 48 | 48 | 92.434 |
| float | 64 | 64 | 64 | 172.433 |
| float | 80 | 80 | 80 | 294.595 |
| float | 96 | 96 | 96 | 501.083 |
| float | 112 | 112 | 112 | 775.676 |
| float | 128 | 128 | 128 | 1141.083 |

