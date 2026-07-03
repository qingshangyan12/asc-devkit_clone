# CMake 函数参考

> 类型：参考

**适用读者**：需要深入了解算子工程编译机制的高级开发者

## 1 概述

使用 msOpGen 工具创建算子工程时，相关编译脚本被固化在本地。为便于开发者直接使用算子工程对外提供的 cmake 函数接口而无需频繁重建工程，CANN 软件包中将算子工程的 cmake 脚本打包，开发者可通过 `find_package` 查找对应的 cmake modules 包来使用这些函数。

本文说明算子工程编译过程中使用的 8 个 CMake 函数，按用途分为 **Package 类**、**Library 类**、**其他** 三类。

## 2 函数一览

| 分类 | 函数 | 功能 |
|------|------|------|
| Package | `npu_op_package` | 创建一个 package |
| Package | `npu_op_package_add` | 将目标或文件添加到 package 中 |
| Library | `npu_op_library` | 创建 Host 侧库 |
| Library | `npu_op_kernel_library` | 创建 Kernel 侧库 |
| Library | `npu_op_kernel_options` | 添加 Kernel 目标编译选项 |
| Library | `npu_op_kernel_sources` | 描述 Kernel 目标的源码信息 |
| Library | `npu_op_device_tiling_library` | 创建 Device 侧 Tiling 库 |
| 其他 | `npu_op_code_gen` | 执行代码生成过程，生成 aclnn 单算子调用代码和入图所需的原型定义代码 |

---

## 3 Package 类函数

### 3.1 npu_op_package

创建一个 package。

```cmake
npu_op_package(<package_name> TYPE <type> [CONFIG] [ENABLE_SOURCE_PKG <value>] [ENABLE_BINARY_PACKAGE <value>] [INSTALL_PATH <path>])
```

**参数说明：**

- `<package_name>`（必选）：package 的名称。
- `TYPE <type>`（必选）：package 的类型，取值为 `RUN`、`SHARED`、`STATIC`。分别对应算子 run 包形式、算子动态库形式与算子静态库形式。
- `[CONFIG]`（可选）：用于配置 package 的内容和安装位置。
  - `[ENABLE_SOURCE_PKG <value>]`（可选）：是否将源码打包到 package 中，默认为 True。
  - `[ENABLE_BINARY_PACKAGE <value>]`（可选）：是否将二进制文件打包到 package 中，默认为 True。
  - `[INSTALL_PATH <path>]`（可选）：指定包的安装路径，默认为 `CMAKE_BINARY_DIR`。
  - `[ENABLE_CPACK <value>]`（可选）：是否打包，默认为 True。对于用户需要定制打包的场景，可以设置为 False，配置为 False 的情况下直接在 `<path>` 路径下生成编译产物，不会打包为 run 包。

**示例：**

```cmake
set(package_name ${vendor_name})
npu_op_package(${package_name}                     # package name
    TYPE RUN                                       # 指定编译产物形态，[RUN|STATIC|SHARED]
    CONFIG
        ENABLE_SOURCE_PACKAGE True                 # 是否将源码打包到run包中
        ENABLE_BINARY_PACKAGE True                 # 是否编译Kernel二进制
        INSTALL_PATH ${CMAKE_BINARY_DIR}/          # package的安装位置
)
```

### 3.2 npu_op_package_add

将目标或文件添加到 package 中。

```cmake
# 添加目标
npu_op_package_add(<package_name> LIBRARY <target_name1> [<target_name2>...] )
# 添加文件，仅给run包模式使用
npu_op_package_add(<package_name> FILES <file_name1> [<file_name2>...] [TYPE <target_type>] [PACKAGE_PATH <pkg_path>])
```

**参数说明：**

- `<package_name>`（必选）：package 的名称。
- `LIBRARY`（必选）：指定需要添加到 package 中的目标名称。
  - `<target_name1> [<target_name2>...]`（必选）：目标名称列表。
- `FILES`（必选）：指定需要添加到 package 中的文件名称。
  - `<file_name1> [<file_name2>...]`（必选）：文件名称列表。
- `[TYPE <target_type>]`（可选）：指定文件类型，将文件安装到对应的目录中，取值为 `ACLNN`、`GRAPH`。配置为 `ACLNN`，会将文件打包至 run 包目录下 aclnn 单算子调用头文件所在目录；配置为 `GRAPH`，会将文件打包至 run 包目录下入图原型定义头文件目录下。
- `[PACKAGE_PATH <pkg_path>]`（可选）：指定文件在包中的相对路径位置。`TYPE` 和 `PACKAGE_PATH` 参数互斥，即只能选择其中一个进行配置。

**示例：**

```cmake
# 添加Host侧相关library
npu_op_package_add(${package_name}
    LIBRARY
        cust_optiling
        cust_opapi
        cust_op_proto
)

# 添加Kernel侧相关library
npu_op_package_add(${package_name}
    LIBRARY ascendc_kernels
)
```

---

## 4 Library 类函数

### 4.1 npu_op_library

创建 Host 侧库。

```cmake
npu_op_library(<library_name> TYPE <library_type> <files>)
```

**参数说明：**

- `<library_name>`（必选）：Host 侧库的名称。
- `TYPE <library_type>`（必选）：Host 库的类型，可选值为 `TILING`、`ACLNN`、`GRAPH`、`TF_PLUGIN`。
  - `TILING`：Tiling 相关库。
  - `ACLNN`：aclnn 单算子调用库。
  - `GRAPH`：算子入图所需的算子原型库。
  - `TF_PLUGIN`：TensorFlow 框架适配相关库。
- `<files>`（必选）：设置参与编译的源文件。

**示例：**

```cmake
# 单算子调用场景，编译aclnn单算子调用库
file(GLOB autogen_aclnn_src ${ASCEND_AUTOGEN_PATH}/aclnn_*.cpp)
set_source_files_properties(${autogen_aclnn_src} PROPERTIES GENERATED TRUE)
npu_op_library(cust_opapi ACLNN
    ${autogen_aclnn_src}
)

# 算子入图场景，编译算子入图所需的算子原型库
file(GLOB proto_src ${ASCEND_AUTOGEN_PATH}/op_proto.cc)
set_source_files_properties(${proto_src} PROPERTIES GENERATED TRUE)
npu_op_library(cust_op_proto GRAPH
    ${ops_srcs}
    ${proto_src}
)

# 编译Tiling相关库
file(GLOB fallback_src ${ASCEND_AUTOGEN_PATH}/fallback_*.cpp)
set_source_files_properties(${fallback_src} PROPERTIES GENERATED TRUE)
npu_op_library(cust_optiling TILING
    ${ops_srcs}
    ${fallback_src}
)
```

### 4.2 npu_op_kernel_library

创建 Kernel 侧库。

```cmake
npu_op_kernel_library(<target_name> SRC_BASE <path> TILING_LIBRARY <tiling_target>)
```

**参数说明：**

- `<target_name>`（必选）：目标的名称。
- `SRC_BASE <path>`（必选）：指定 Kernel 源码的 base 目录，要求配置绝对路径。例如示例中的 `op_kernel` 目录的绝对路径。
- `TILING_LIBRARY <tiling_target>`（必选）：指定依赖的 Tiling 目标。

**示例：**

```cmake
npu_op_kernel_library(ascendc_kernels
    SRC_BASE ${CMAKE_SOURCE_DIR}/op_kernel/
    TILING_LIBRARY cust_optiling
)
```

### 4.3 npu_op_kernel_options

添加 Kernel 目标编译选项。

```cmake
npu_op_kernel_options(<target_name> <op_type> [COMPUTE_UNIT <soc_version>] OPTIONS …)
```

**参数说明：**

- `<target_name>`（必选）：目标的名称。
- `<op_type>`（必选）：定义配置生效的范围，取值为 `ALL`、`OP_TYPE`。`ALL` 表示对所有算子生效，`OP_TYPE` 表示对特定算子生效。
- `[COMPUTE_UNIT <soc_version>]`（可选）：用于设置算子在具体 AI 处理器型号上的编译选项，不填写该选项时默认对所有型号生效。

  > **说明**
  > - 针对如下产品：在安装 AI 处理器的服务器执行 `npu-smi info` 命令进行查询，获取 Name 信息。实际配置值为 AscendName，例如 Name 取值为 xxxyy，实际配置值为 Ascendxxxyy。
  >   - Atlas A2 训练系列产品 / Atlas A2 推理系列产品
  >   - Atlas 200I/500 A2 推理产品
  >   - Atlas 推理系列产品
  >   - Atlas 训练系列产品
  > - 针对 Atlas A3 训练系列产品 / Atlas A3 推理系列产品，在安装 AI 处理器的服务器执行 `npu-smi info -t board -i id -c chip_id` 命令进行查询，获取 Chip Name 和 NPU Name 信息，实际配置值为 Chip Name_NPU Name。例如 Chip Name 取值为 Ascendxxx，NPU Name 取值为 1234，实际配置值为 Ascendxxx_1234。其中：id 为设备 id（通过 `npu-smi info -l` 查出的 NPU ID 即为设备 id）；chip_id 为芯片 id（通过 `npu-smi info -m` 查出的 Chip ID 即为芯片 id）。
  > - 针对 Ascend 950PR / Ascend 950DT，在安装 AI 处理器的服务器执行 `npu-smi info -t board -i id` 命令进行查询，获取 Chip Name 和 NPU Name 信息，实际配置值为 Chip Name_NPU Name。例如 Chip Name 取值为 Ascendxxx，NPU Name 取值为 1234，实际配置值为 Ascendxxx_1234。其中 id 为设备 id（通过 `npu-smi info -l` 查出的 NPU ID 即为设备 id）。

- `OPTIONS …`（必选）：传递给编译器的编译选项。

**示例：**

```cmake
npu_op_kernel_options(ascendc_kernels ALL OPTIONS --save-temp-files -g)   # 为算子添加编译选项
```

### 4.4 npu_op_kernel_sources

描述 Kernel 目标的源码信息，包括设置算子的 Kernel 实现文件和源码路径等。

```cmake
npu_op_kernel_sources(<target_name> [OP_TYPE <op_type>] [KERNEL_DIR <path>] [COMPUTE_UNIT <soc_version>] [KERNEL_FILE <file>])
```

**参数说明：**

- `<target_name>`（必选）：目标的名称。
- `[OP_TYPE <op_type>]`（可选）：算子类型，必须与 `KERNEL_FILE` 同时存在。
- `[KERNEL_DIR <path>]`（可选）：指定 Kernel 源码相对于 `SRC_BASE` 的相对路径。若算子的源码文件没有平铺在 `SRC_BASE` 目录（通过 `npu_op_kernel_library` 设置）下，可以通过 `KERNEL_DIR` 指定特定目录。
- `[COMPUTE_UNIT <soc_version>]`（可选）：设置 `KERNEL_FILE` 在 `<soc_version>` 型号生效。默认 `KERNEL_FILE` 对所有型号生效。
- `[KERNEL_FILE <file>]`（可选）：指定算子入口的 Kernel 实现文件名。若算子的 Kernel 实现 cpp 文件需要自定义命名，需同时指定 `OP_TYPE`（算子类型）和 `KERNEL_FILE`（Kernel 实现 cpp 文件名），以配置两者之间的对应关系。不配置时，Kernel 实现 cpp 文件名和 OpType 之间需满足转换规则（`./算子原型定义.md`）。

**示例：**

```cmake
npu_op_kernel_sources(ascendc_kernels
    OP_TYPE AddCustom
    KERNEL_DIR ./Add
    COMPUTE_UNIT Ascendxxxyy
    KERNEL_FILE add_custom.cpp
)
```

### 4.5 npu_op_device_tiling_library

创建 Device 侧 Tiling 库。使用该选项时，package 的类型仅支持配置为 RUN（run 包模式）。

```cmake
npu_op_device_tiling_library(<target_name> <type> <files>)
```

**参数说明：**

- `<target_name>`（必选）：目标的名称。
- `<type>`（必选）：指定 Tiling 产物的类型。支持取值为 `SHARED`、`STATIC`。
- `<files>`（必选）：指定 Tiling 源码文件。

**示例：**

```cmake
npu_op_device_tiling_library(device_tiling SHARED
    device_tiling_impl.cpp
)
```

---

## 5 其他

### 5.1 npu_op_code_gen

执行代码生成过程，生成 aclnn 单算子调用代码和入图所需的原型定义代码。

```cmake
npu_op_code_gen(SRC <src_files> OUT_DIR <output_dir> PACKAGE <pkg_name> [COMPILE_OPTIONS ...] [JOIN_OP_DEF ...])
```

**参数说明：**

- `SRC <src_files>`（必选）：参与代码生成的源文件范围。
- `OUT_DIR <output_dir>`（必选）：生成代码的输出路径。
- `PACKAGE <pkg_name>`（必选）：指定生成代码的 package 名称。
- `[COMPILE_OPTIONS ...]`（可选）：自定义编译过程中的编译选项。
- `[JOIN_OP_DEF ...]`（可选）：默认为 False。设置为 False，编译 CMakePreset.json 中设置的 AI 处理器型号对应的算子；设置成 True，则编译 CMakePreset.json 和算子原型注册中共有的 AI 处理器型号对应的算子。

**示例：**

```cmake
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} ops_srcs)
npu_op_code_gen(
    SRC ${ops_srcs}
    PACKAGE ${package_name}
    COMPILE_OPTIONS -g
    OUT_DIR ${ASCEND_AUTOGEN_PATH}
    JOIN_OP_DEF False
)
```

---

## 6 相关文档

- [算子工程编译拓展](../算子工程编译拓展.md) — 完整的 CMakeLists.txt 编写方法与编译命令说明
- [算子原型定义](./算子原型定义.md) — Kernel 实现文件名与 OpType 的转换规则
- [如何使用 workspace](../../附录/常用操作/如何使用workspace.md) — Tiling 函数中 workspace 的设置方法
