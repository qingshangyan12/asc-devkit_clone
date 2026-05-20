# 单独编译样例

## 概述

本样例基于Gather算子，演示了将Device侧代码拆分为**多个.asc文件**单独编译的编译流程。其中 `gather_kernel.asc` 调用定义在 `gather_compute.asc` 中的函数，实现跨文件的函数调用。

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 支持的CANN软件版本

- \> CANN 9.0.0

## 目录结构介绍

```
├── seperate_compile
│   ├── CMakeLists.txt       // 编译工程文件
│   ├── gather_compute.asc   // 设备端（纯计算逻辑）
│   ├── gather_kernel.asc    // 设备端（含Kernel定义、函数声明与<<<>>>内核调用）
│   └── main.cpp             // 主机端（纯Host侧代码）
```

## 算子描述

gather算子实现了从长度为100000的一维输入向量中获取指定索引的12288个数据的功能，具体功能描述可参考xxx章节。

## 单独编译介绍

bisheng默认采用**全程序编译**模式，该模式要求单个源文件X.asc中编译的设备程序没有任何未解析的外部设备函数和变量引用。当需要在多个 `.asc` 文件间进行跨文件的设备代码调用时，需使用 **`-dc` 选项**启用**单独编译**模式。

单独编译有以下注意事项：
- 在一个编译单元中定义的非常量设备变量，在其他编译单元中引用时，必须使用extern关键字声明；
- 所有常量设备变量，定义和跨单元引用时都必须使用extern关键字；
- 所有Ascend C 源文件（.asc）都必须使用`-dc`选项编译。

本样例将 Device 侧代码拆分为两个 `.asc` 文件：`gather_compute.asc` 定义函数，`gather_kernel.asc` 声明并调用该函数。由于存在跨文件的设备函数调用，需要使用 `-dc` 选项分别编译后再链接。

本样例的bisheng编译命令如下：

```shell
# 1. 分别编译各 .asc 文件为目标文件
bisheng -dc gather_compute.asc -o gather_compute.o --npu-arch=dav-3510 --enable-simt
bisheng -dc gather_kernel.asc -o gather_kernel.o --npu-arch=dav-3510 --enable-simt

# 2. 编译 Host 侧代码
bisheng -c main.cpp -o main.o -I${ASCEND_HOME_PATH}/include

# 3. 链接所有目标文件生成可执行程序
bisheng gather_compute.o gather_kernel.o main.o -o demo
```

此外，也可使用CMake方式编译，详见下方[编译运行](#编译运行)章节。更多bisheng编译选项及用法说明，请参考[《AI Core 算子编译基本用法》](../../../../docs/guide/编程指南/编译与运行/算子编译/AI-Core算子编译基本用法.md)。

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。
- 配置环境变量
  请根据当前环境上 CANN 开发套件包的[安装方式](../../../../docs/quick_start.md#prepare&install)，选择对应配置环境变量的命令。
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
  ./demo                        # 执行样例
  ```
  执行结果如下，说明精度对比成功。
  ```
  [Success] Case accuracy is verification passed.
  ```
