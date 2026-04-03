# Where样例

## 概述

本样例基于[Where](../../../../../docs/api/context/Where.md)高阶API实现根据指定的条件从两个源操作数中选择元素的功能。两个源操作数均可以是LocalTensor或标量。  

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 目录结构介绍

```plain
├── where
│   ├── scripts
│   │   └── gen_data.py         // 输入数据和真值数据生成脚本
│   ├── CMakeLists.txt          // 编译工程文件
│   ├── data_utils.h            // 数据读入写出函数
│   └── where.asc               // Ascend C样例实现 & 调用样例
```

## 样例描述

- 样例功能：  
  根据指定的条件，从两个源操作数中选择元素，生成目标操作数。两个源操作数均可以是LocalTensor或标量。

  计算公式如下：  
  $$dst_i = \begin{cases}
  src0, & if condition \\
  src1, & otherwise
  \end{cases}$$

- 样例规格：  
  <table>
  <tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="4" align="center"> where </td></tr>

  <tr><td rowspan="5" align="center">样例输入</td></tr>
  <tr><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">src0</td><td align="center">[1, 32]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td align="center">src1</td><td align="center">[1, 32]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td align="center">condition</td><td align="center">[1, 32]</td><td align="center">bool</td><td align="center">ND</td></tr>
  <tr><td rowspan="2" align="center">样例输出</td></tr>
  <tr><td align="center">dst</td><td align="center">[1, 32]</td><td align="center">float</td><td align="center">ND</td></tr>

  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">where_custom</td></tr>
  </table>

- 样例实现：  
  本样例中实现的是固定shape为输入src0[1, 32]，src1[1, 32]，condition[1, 32]，输出dst[1, 32]的where_custom样例。

  - Kernel实现

    计算逻辑包含以下步骤：
    1. 将两个源操作数和条件从Global Memory搬运到Local Memory（Unified Buffer）
    2. 使用Where高阶API根据条件从两个源操作数中选择元素，支持张量和标量混合模式
    3. 将计算结果从Local Memory搬出到Global Memory

  - 调用实现  
    使用内核调用符<<<>>>调用核函数。

## 编译运行  

在本样例根目录下执行如下步骤，编译并执行样例。

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

  ```bash
  mkdir -p build && cd build;   # 创建并进入build目录
  cmake ..;make -j;             # 编译工程
  python3 ../scripts/gen_data.py   # 生成测试输入数据
  ./demo                        # 执行编译生成的可执行程序，执行样例
  ```

  执行结果如下，说明精度对比成功。

  ```bash
  test pass!
  ```
