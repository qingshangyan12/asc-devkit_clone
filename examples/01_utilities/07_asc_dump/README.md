# MmadCustomDebug样例

## 概述

本样例通过Ascend C编程语言实现了Mmad算子，同时在算子中添加Printf、DumpTensor等调测接口，使用<<<>>>内核调用符来完成算子核函数在NPU侧运行验证的基础流程，给出了对应的端到端实现。

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 目录结构介绍

```
├── 07_asc_dump
│   ├── CMakeLists.txt              // 编译工程文件
│   ├── half.hpp                    // 数据类型依赖文件
│   └── mmad_custom_debug.asc  // Ascend C算子实现 & 调用样例
```

## 算子描述

- 算子功能：

  asc_dump介绍：asc_dump是一个系列接口，包含asc_dump_gm、asc_dump_ubuf、asc_dump_cbuf和asc_dump_l1buf，使用asc_dump可以Dump指定内存的内容，同时支持打印自定义的附加信息。

  - Mmad算子介绍：

    算子使用基础API包括DataCopy、LoadData、Mmad等，实现矩阵乘功能。

    计算公式为：

    ```
    C = A * B
    ```

    - A、B为源操作数，A为左矩阵，形状为\[M, K]；B为右矩阵，形状为\[K, N]。
    - C为目的操作数，存放矩阵乘结果的矩阵，形状为\[M, N]。

- 算子规格：
  - Mmad算子：  

    在核函数直调样例中，算子实现支持的shape为：M = 16, N = 16, K = 16。
    <table>
    <tr><td rowspan="1" align="center">算子类型(OpType)</td><td colspan="4" align="center">Mmad</td></tr>
    </tr>
    <tr><td rowspan="3" align="center">算子输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
    <tr><td align="center">a</td><td align="center">M * K</td><td align="center">half</td><td align="center">ND</td></tr>
    <tr><td align="center">b</td><td align="center">K * N</td><td align="center">half</td><td align="center">ND</td></tr>
    </tr>
    </tr>
    <tr><td rowspan="1" align="center">算子输出</td><td align="center">c</td><td align="center">M * N</td><td align="center">float</td><td align="center">ND</td></tr>
    </tr>
    <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">mmad_custom</td></tr>
    </table>

- 算子实现：  
  - Mmad算子：

    本样例中实现的是[m, n, k]固定为[16, 16, 16]的Mmad算子，并使用Ascend C基础API实现，同时添加维测功能。

    - Kernel实现  
      Mmad算子的数学表达式为：
      $$
      C = A * B
      $$
      其中A的形状为[16, 16], B的形状为[16, 16], C的形状为[16, 16]。

    - 调用实现
      使用内核调用符<<<>>>调用核函数

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
  ./demo                           # 执行编译生成的可执行程序，执行样例
  ```
  执行结果如下，说明精度对比成功。
  ```bash
  [Success] Case accuracy is verification passed.
  ```