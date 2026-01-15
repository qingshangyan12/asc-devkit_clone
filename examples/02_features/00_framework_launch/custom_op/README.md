# 自定义算子工程编译、打包和部署样例
## 概述
本样例以简单的自定义算子为示例，展示了其编译、打包成自定义算子包，并部署到CANN包中的流程。

## 支持的产品
本样例支持如下产品型号：
- Ascend 950PR/Ascend 950DT
- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品
- Atlas 200I/500 A2 推理产品
- Atlas 推理系列产品

## 目录结构介绍
```
├── custom_op
│   ├── CMakeLists.txt
│   ├── op_host
│   │   ├── add_custom
│   │   │   └── add_custom_host.cpp     // host侧编译源码文件
│   │   └── CMakeLists.txt
│   └──op_kernel
│       ├── add_custom
│       │   ├── add_custom_kernel.cpp   // kernel侧编译源码文件
│       │   └── add_custom_tiling.h     // host侧编译tiling头文件
│       └── CMakeLists.txt
```

## 算子描述
Add算子实现了两个数据相加，返回相加结果的功能。对应的数学表达式为：
```
z = x + y
```
## 算子规格描述
<table>
<tr><td rowspan="1" align="center">算子类型(OpType)</td><td colspan="4" align="center">Add</td></tr>
</tr>
<tr><td rowspan="3" align="center">算子输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
<tr><td align="center">x</td><td align="center">8 * 2048</td><td align="center">float</td><td align="center">ND</td></tr>
<tr><td align="center">y</td><td align="center">8 * 2048</td><td align="center">float</td><td align="center">ND</td></tr>
</tr>
</tr>
<tr><td rowspan="1" align="center">算子输出</td><td align="center">z</td><td align="center">8 * 2048</td><td align="center">float</td><td align="center">ND</td></tr>
</tr>
<tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">add_custom</td></tr>
</table>

## 代码实现介绍
- kernel实现：</br>
  Ascend C提供的矢量计算接口`Add`的操作元素都为`LocalTensor`，输入数据需要先搬运进片上存储，然后使用计算接口完成两个输入参数相加，得到最终结果，再搬出到外部存储上。</br>
  Add算子的实现流程分为3个基本任务：`CopyIn`，`Compute`，`CopyOut`。`CopyIn`任务负责将Global Memory上的输入Tensor `xGm`和`yGm`搬运到Local Memory，分别存储在`xLocal`、`yLocal`，`Compute`任务负责对`xLocal`、`yLocal`执行加法操作，计算结果存储在`zLocal`中，`CopyOut`任务负责将输出数据从`zLocal`搬运至Global Memory上的输出Tensor zGm中。

- tiling实现：</br>
  TilingData参数设计，`AddCustomTilingData`参数本质上是和并行数据切分相关的参数，本示例算子使用了2个tiling参数：`totalLength`、`tileNum`。`totalLength`是指需要计算的数据量大小，`tileNum`是指每个核上总计算数据分块个数。比如，`totalLength`这个参数传递到kernel侧后，可以通过除以参与计算的核数，得到每个核上的计算量，这样就完成了多核数据的切分。


## 编译运行
在本样例根目录下执行如下步骤，编译并部署自定义算子包。
- 配置环境变量</br>
  请根据当前环境上CANN开发套件包的[安装方式](../../../docs/quick_start.md#prepare&install)，选择对应配置环境变量的命令。
  - 默认路径，root用户安装CANN软件包
    ```bash
    source /usr/local/Ascend/ascend-toolkit/set_env.sh
    ```

  - 默认路径，非root用户安装CANN软件包
    ```bash
    source $HOME/Ascend/ascend-toolkit/set_env.sh
    ```

  - 指定路径install_path，安装CANN软件包
    ```bash
    source ${install_path}/ascend-toolkit/set_env.sh
    ```

- 编译算子并部署
  ```bash
  mkdir -p build && cd build
  cmake .. && make -j binary package
  ./custom_opp_*.run
  ```
