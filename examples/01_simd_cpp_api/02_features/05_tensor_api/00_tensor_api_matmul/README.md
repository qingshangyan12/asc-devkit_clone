# Matmul样例

## 概述

本样例在矩阵乘法场景下，基于Tensor_API接口实现Mmad多核场景样例，并使能Relu和随路量化将矩阵乘结果由float类型数据量化为int8_t类型。

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 目录结构介绍

```
├── 00_tensor_api_matmul
│   └── scripts
│       ├── gen_data.py         // 输入数据和真值数据生成脚本文件
│       └── verify_result.py    // 真值对比文件
│   ├── CMakeLists.txt          // 编译工程文件
│   ├── data_utils.h            // 数据读入写出函数
│   └── tensor_api_matmul.asc   // Ascend C样例实现 & 调用样例
```

## 样例描述

- 样例功能：

  本样例实现了Matmul、relu和随路量化功能。
  1、Matmul功能
  本样例中实现的是[M, K, N]固定为[128, 128, 96]的Matmul功能。
  Matmul样例的数学表达式为：
  $$
  C = A * B
  $$
  其中A的形状为[128, 96]，B的形状为[96, 128]，C的形状为[128, 128]。
  2、relu和随路量化功能
  本样例通过Copy接口实现了数据随路量化激活搬运，支持在L0C到GM数据搬运过程中通过vector量化将float类型转换为int8_t类型，同时支持Relu激活，其中vector量化数据形状为[1, 96]。
  用户在使用过程中，Copy接口可以通过是否输入量化参数控制是否随路量化，以及FixpipeTrait中的enableRelu参数控制是否随路执行relu激活。
- 样例规格：

  在核函数直调样例中，样例实现支持的shape为：M = 128, N = 128, K = 96。
  <table border="2" align="center">
  <caption>表1：样例规格表</caption>
  <tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="4" align="center">Matmul</td></tr>
  </tr>
  <tr><td rowspan="3" align="center">样例输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">a</td><td align="center">[M, K]</td><td align="center">half</td><td align="center">ND</td></tr>
  <tr><td align="center">b</td><td align="center">[K, N]</td><td align="center">half</td><td align="center">ND</td></tr>
  </tr>
  </tr>
  <tr><td rowspan="1" align="center">样例输出</td><td align="center">c</td><td align="center">[M, N]</td><td align="center">int8_t</td><td align="center">ND</td></tr>
  </tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">tensor_api_matmul_custom</td></tr>
  </table>
- 样例实现：
  本样例中以32核为例，实现的是[M, K, N]固定为[128, 128, 96]的多核Matmul样例，并使能随路量化和relu，tiling参数如下：
  
| 参数          | 值     |
| ------------- | ----- |
| `m`           | 128   |
| `k`           | 96    |
| `n`           | 128   |
| `singleCoreM` | 16    |
| `singleCoreK` | 96    |
| `singleCoreN` | 32    |
| `baseM`       | 16    |
| `baseK`       | 16    |
| `baseN`       | 32    |
| `stepM`       | 1     |
| `stepKa`      | 6     |
| `stepKb`      | 6     |
| `stepN`       | 1     |
  
  Matmul的计算公式为：C = A * B。  
  A、B为源操作数，A为左矩阵，形状为[M, K]；B为右矩阵，形状为[K, N]。  
  C为目的操作数，存放矩阵乘结果的矩阵，形状为[M, N]。
  vector量化矩阵形状为[1, N]。

  - Kernel实现  
    - 计算逻辑：C = A * B
      - A、B为源操作数，A为左矩阵，形状为[M, K]；B为右矩阵，形状为[K, N]。  
      - C为目的操作数，存放矩阵乘结果的矩阵，形状为[M, N]。
    - 具体步骤：
      - 调用Mmad接口，将矩阵乘法场景下的half类型的输入矩阵A[128, 96]与矩阵B[96, 128]做矩阵乘法，得到float类型的结果矩阵C[128, 128]。
      - 自定义FixpipeTrait参数用于Copy随路激活，设置enableRelu=true，并在Copy接口传入vector量化矩阵（本样例量化矩阵形状为[1, 96]），将Mmad计算出的L0C Buffer上的结果由float量化成int8_t，并完成Relu激活后搬出到Global Memory。

  - Tiling实现  
    初始化Tiling数据，从设备全局内存中读取关键参数。   
    流程如下：   
    - 设置输入矩阵A、B和输出矩阵C的全局Tensor，并指定维度。  
    - 初始化Matmul对象，设置接口配置计算参数并执行计算。 

  - 调用实现  
    使用内核调用符<<<>>>调用核函数。

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。
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
  mkdir -p build && cd build;   # 创建并进入build目录
  cmake ..;make -j;             # 编译工程
  python3 ../scripts/gen_data.py   # 生成测试输入数据
  ./demo                        # 执行编译生成的可执行程序，执行样例
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin   # 验证输出结果是否正确，确认算法逻辑正确
  ```
  执行结果如下，说明精度对比成功。
  ```bash
  test pass!
  ```