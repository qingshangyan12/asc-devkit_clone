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
  
  本样例实现了多核Matmul功能，具体规格见下表。
  
  2、relu和随路量化功能

  本样例通过设置Copy接口的参数实现了数据随路量化激活搬运，支持在L0C到GM数据搬运过程中通过vector量化将float类型转换为int8_t类型。

- 样例规格：

  在核函数直调样例中，样例实现支持的shape为：M = 128, N = 128, K = 96。
  <table border="2" align="center">
  <caption>表1：样例规格表</caption>
  <tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="4" align="center">Matmul</td></tr>
  </tr>
  <tr><td rowspan="4" align="center">样例输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">a</td><td align="center">[M, K]</td><td align="center">half</td><td align="center">ND</td></tr>
  <tr><td align="center">b</td><td align="center">[K, N]</td><td align="center">half</td><td align="center">ND</td></tr>
  <tr><td align="center">quant vector</td><td align="center">[1, N]</td><td align="center">uint64_t</td><td align="center">ND</td></tr>
  </tr>
  </tr>
  <tr><td rowspan="1" align="center">样例输出</td><td align="center">c</td><td align="center">[M, N]</td><td align="center">int8_t</td><td align="center">ND</td></tr>
  </tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">tensor_api_matmul_custom</td></tr>
  </table>

  - Kernel实现  
    - 调用Mmad接口，将矩阵乘法场景下的half类型的输入矩阵A[128, 96]与矩阵B[96, 128]做矩阵乘法，得到float类型的结果矩阵C[128, 128]。
    - 在Copy接口设置参数，将Mmad计算出的L0C Buffer上的结果由float量化成int8_t，且完成Relu激活后搬出到Global Memory。

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