# Load2DGM2L1样例

## 概述
本样例介绍了在 half 数据类型下，以及左、右矩阵均不转置，相关指令的使用方法，其中 A 和 B 分别表示矩阵乘法的左右输入矩阵，样例重点关注LoadData2D指令GM到L1的数据通路。

首先，矩阵 A 和 B 从 GM 搬运至 L1 ，从 L1 搬运至 L0A / L0B 均调用LoadData2D指令进行搬运，随后通过 mmad 指令实现矩阵乘法计算（C = A × B），最后使用 fixpipe 指令将结果矩阵 C 从 L0C 搬运回 GM。

## 支持的产品
- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍
```
├── load2d_gm2l1
│   ├── scripts
│   │   ├── gen_data.py             // 输入数据和真值数据生成脚本
│   │   └── verify_result.py        // 验证输出数据和真值数据是否一致的验证脚本
│   ├── CMakeLists.txt              // 编译工程文件
│   ├── data_utils.h                // 数据读入写出函数
│   └── load2d_gm2l1.asc            // Ascend C算子实现 & 调用样例
```

## 算子描述
- 算子功能：  
  本样例中实现的是[m, n, k]固定为[32, 32, 32]的Matmul算子，并使用Ascend C基础Api实现。对应的数学表达式为：
  ```
  C = A * B
  ```
  实现上述计算（A矩阵、B矩阵不转置），需要L1->L0A不使能转置，L1->L0B使能转置

- 算子规格：
  <table>
  <tr><td rowspan="1" align="center">算子类型(OpType)</td><td colspan="4" align="center">LoadData</td></tr>
  </tr>
  <tr><td rowspan="3" align="center">算子输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">a</td><td align="center">[32, 32]</td><td align="center">half</td><td align="center">NZ</td></tr>
  <tr><td align="center">b</td><td align="center">[32, 32]</td><td align="center">half</td><td align="center">NZ</td></tr>
  </tr>
  </tr>
  <tr><td rowspan="1" align="center">算子输出</td><td align="center">c</td><td align="center">[32, 32]</td><td align="center">float</td><td align="center">ND</td></tr>
  </tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">load2d_gm2l1</td></tr>
  </table>
- 算子实现：  

  由于LoadData2D指令不支持ND2NZ的数据分型转换，因此使用LoadData2D指令从GM至UB搬运数据需要确保GM为NZ数据分型，这通常是上一次矩阵计算的结果（调用fixpipe指令搬回GM，不使能NZ2ND）。
  本算子的实现流程分为几个基本任务：LoadA，LoadB，SplitA, SplitATranspose，SplitB，SplitBTranspose，Compute，CopyOut。LoadA负责将GM上的NZ分型A矩阵搬运到 L1 中，LoadB负责将GM上的NZ分型B矩阵搬运到 L1 中。SplitA调用LoadData接口将数据进一步搬运L0A，SplitATranspose调用LoadData接口将数据进一步搬运L0A（使能转置），SplitB调用LoadData接口将数据进一步搬运L0B，SplitBTranspose调用LoadData接口将数据进一步搬运L0B（使能转置），Compute任务负责对数据进行矩阵乘运算，计算结果存储在 L0C 中。CopyOut任务负责将输出数据从L0C搬运至GM。

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
  mkdir -p build && cd build;      # 创建并进入build目录
  cmake ..;make -j;                # 编译工程
  python3 ../scripts/gen_data.py   # 生成测试输入数据
  ./demo                           # 执行编译生成的可执行程序，执行样例
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin   # 验证输出结果是否正确，确认算法逻辑正确
  ```
  执行结果如下，说明精度对比成功。
  ```bash
  test pass!
  ```