# Dynamic Library Compile 样例

## 概述

本样例基于Gather算子，演示了将Device侧.asc混合文件编译为**动态共享库（.so）**，再由Host侧.cpp链接调用的编译流程。

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 支持的CANN软件版本

- \> CANN 9.0.0

## 目录结构介绍

```
├── dynamic_library_compile
│   ├── CMakeLists.txt      // 编译工程文件
│   ├── gather_kernel.asc   // 设备端（含Kernel定义与<<<>>>内核调用）
│   └── main.cpp            // 主机端（纯Host侧代码）
```

## 算子描述
gather算子实现了从长度为100000的一维输入向量中获取指定索引的12288个数据的功能，具体功能描述可参考xxx章节。

## 动态库编译介绍

通过动态库编译，Device侧 `.asc` 混合文件被编译为独立的 **动态共享库（.so）**，Host侧 `.cpp` 通过链接该 `.so` 完成调用。动态库编译的基本命令格式如下：

```shell
bisheng -shared <source_file>.asc -o <output_file>.so -fPIC --npu-arch=dav-<npu_architecture> --enable-simt
```

本样例的bisheng编译命令如下

```shell
# 1. 将.asc编译为动态共享库
bisheng -shared gather_kernel.asc -o libgather_kernel.so -fPIC --npu-arch=dav-3510 --enable-simt

# 2. 链接主程序
bisheng main.cpp -o demo -L./ -I${ASCEND_HOME_PATH}/include -lgather_kernel -lascendcl -lstdc++

# 3. 设置动态库搜索路径
export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH
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
