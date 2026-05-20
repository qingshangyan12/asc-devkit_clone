# Basic Compile 样例

## 概述

本样例基于Gather算子，演示了Host侧.cpp文件调用Device侧.asc混合文件（含<<<>>>内核调用）的基本编译流程。

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 支持的CANN软件版本

- \> CANN 9.0.0

## 目录结构介绍

```
├── basic_compile
│   ├── CMakeLists.txt      // 编译工程文件
│   ├── gather_kernel.asc   // 设备端（含Kernel定义与<<<>>>内核调用）
│   └── main.cpp            // 主机端（纯Host侧代码）
```

## 算子描述
gather算子实现了从长度为100000的一维输入向量中获取指定索引的12288个数据的功能，具体功能描述可参考xxx章节。

## 异构编译介绍

本样例采用**异构编译**方式，Host侧 `.cpp` 与Device侧 `.asc` 分开编译后再链接。AI Core SIMT的基本编译流程如下：Host代码使用Host编译器编译成Host二进制，AI Core SIMT代码编译成SIMT二进制，先将SIMT二进制链接成Fatbin文件，再与Host二进制合并生成可执行二进制。

本样例的编译命令如下：

```shell
# 分步编译
bisheng -c gather_kernel.asc -o gather_kernel.o --npu-arch=dav-3510 --enable-simt  # 编译Device端核函数
bisheng -c main.cpp -o main.o -I${ASCEND_HOME_PATH}/include                     # 编译Host端代码
bisheng gather_kernel.o main.o -o demo                                             # 链接生成可执行文件

# 或一步完成
bisheng main.cpp gather_kernel.asc -o demo --npu-arch=dav-3510 --enable-simt -I${ASCEND_HOME_PATH}/include
```

此外，也可使用CMake方式编译，详见下方[编译运行](#编译运行)章节。更多bisheng编译选项及用法说明，请参考[《AI Core 算子编译基本用法》](../../../../docs/guide/编程指南/编译与运行/算子编译/AI-Core算子编译基本用法.md)。

> **注意：** SIMT编程模式需要添加 `--enable-simt` 编译选项。

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
