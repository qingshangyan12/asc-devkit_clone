# CMake 函数参考

> 类型：参考

**适用读者**：需要深入了解算子工程编译机制的高级开发者

## 概述

本文档详细说明了算子工程编译过程中使用的 8 个 CMake 函数的参数和用法。这些函数由 CANN 软件包提供，用于配置算子编译流程、管理编译产物。

## 函数列表

| 函数名 | 类型 | 功能描述 |
|-------|------|---------|
| [npu_op_package](#npu_op_package) | Package | 创建编译产物包 |
| [npu_op_package_add](#npu_op_package_add) | Package | 向包中添加目标或文件 |
| [npu_op_library](#npu_op_library) | Library | 创建 Host 侧库 |
| [npu_op_kernel_library](#npu_op_kernel_library) | Library | 创建 Kernel 侧库 |
| [npu_op_kernel_options](#npu_op_kernel_options) | Library | 设置 Kernel 编译选项 |
| [npu_op_kernel_sources](#npu_op_kernel_sources) | Library | 指定 Kernel 源码信息 |
| [npu_op_device_tiling_library](#npu_op_device_tiling_library) | Library | 创建 Device 侧 Tiling 库 |
| [npu_op_code_gen](#npu_op_code_gen) | Other | 生成代码 |

---

## npu_op_package

### 功能

创建一个编译产物包（package），定义编译产物的形态和内容。

### 语法

```cmake
npu_op_package(<package_name> 
    TYPE <type> 
    [CONFIG] 
        [ENABLE_SOURCE_PKG <value>] 
        [ENABLE_BINARY_PACKAGE <value>] 
        [INSTALL_PATH <path>]
        [ENABLE_CPACK <value>]
)
```

### 参数说明

- **<package_name>**（必选）：package 的名称，用于标识编译产物
- **TYPE <type>**（必选）：package 的类型，取值范围：
  - `RUN`：生成可部署的 run 包，用于算子部署场景
  - `SHARED`：生成动态库，用于集成到应用
  - `STATIC`：生成静态库，用于静态链接

- **CONFIG**（可选）：配置 package 的内容和安装位置
  - **ENABLE_SOURCE_PKG <value>**（可选）：是否将源码打包到 package 中，默认为 True
  - **ENABLE_BINARY_PACKAGE <value>**（可选）：是否编译二进制文件，默认为 True。SHARED/STATIC 模式下必须为 True
  - **INSTALL_PATH <path>**（可选）：指定 package 的安装路径，默认为 CMAKE_BINARY_DIR
  - **ENABLE_CPACK <value>**（可选）：是否启用打包功能，默认为 True。设置为 False 时直接在 INSTALL_PATH 下生成编译产物

### 示例

```cmake
# RUN 模式 - 生成可部署的 run 包
npu_op_package(my_ops 
    TYPE RUN
    CONFIG
        ENABLE_SOURCE_PACKAGE True
        ENABLE_BINARY_PACKAGE True
        INSTALL_PATH ${CMAKE_BINARY_DIR}/
)

# SHARED 模式 - 生成动态库
npu_op_package(my_ops 
    TYPE SHARED
    CONFIG
        ENABLE_SOURCE_PACKAGE True
        ENABLE_BINARY_PACKAGE True
        INSTALL_PATH ${CMAKE_BINARY_DIR}/
)

# STATIC 模式 - 生成静态库
npu_op_package(my_ops 
    TYPE STATIC
    CONFIG
        ENABLE_SOURCE_PACKAGE True
        ENABLE_BINARY_PACKAGE True
        INSTALL_PATH ${CMAKE_BINARY_DIR}/
)
```

### 注意事项

- RUN 模式下，会使用 package_name 命名算子部署后的目录
- STATIC 模式下，输出产物为 lib${package_name}.a
- SHARED 模式下，输出产物为 libcust_opapi.so

---

## npu_op_package_add

### 功能

将编译目标（library）或文件添加到已创建的 package 中。

### 语法

```cmake
# 添加编译目标
npu_op_package_add(<package_name> 
    LIBRARY <target_name1> [<target_name2>...]
)

# 添加文件（仅用于 RUN 模式）
npu_op_package_add(<package_name> 
    FILES <file_name1> [<file_name2>...] 
    [TYPE <target_type>] 
    [PACKAGE_PATH <pkg_path>]
)
```

### 参数说明

- **<package_name>**（必选）：目标 package 的名称
- **LIBRARY**（与 FILES 二选一）：添加编译目标到 package
  - **<target_name1> [<target_name2>...]**（必选）：目标名称列表
- **FILES**（与 LIBRARY 二选一）：添加文件到 package（仅 RUN 模式）
  - **<file_name1> [<file_name2>...]**（必选）：文件名称列表
  - **TYPE <target_type>**（可选）：指定文件类型，取值为 ACLNN 或 GRAPH
    - `ACLNN`：将文件打包到 aclnn 单算子调用头文件目录
    - `GRAPH`：将文件打包到入图原型定义头文件目录
  - **PACKAGE_PATH <pkg_path>**（可选）：指定文件在包中的相对路径。与 TYPE 互斥，只能选择其一

### 示例

```cmake
# 添加 Host 侧库到 package
npu_op_package_add(${package_name}
    LIBRARY
        cust_optiling
        cust_opapi
        cust_op_proto
)

# 添加 Kernel 侧库到 package
npu_op_package_add(${package_name}
    LIBRARY ascendc_kernels
)

# 添加额外文件到 RUN 包（RUN 模式）
npu_op_package_add(${package_name}
    FILES config.ini
    PACKAGE_PATH ./
)
```

### 注意事项

- LIBRARY 方式适用于所有 package 类型（RUN/SHARED/STATIC）
- FILES 方式仅适用于 RUN 模式
- TYPE 和 PACKAGE_PATH 参数互斥，不能同时使用

---

## npu_op_library

### 功能

创建 Host 侧的编译库，包括 aclnn 调用库、原型库、Tiling 库等。

### 语法

```cmake
npu_op_library(<library_name> 
    TYPE <library_type> 
    <files>
)
```

### 参数说明

- **<library_name>**（必选）：Host 侧库的名称
- **TYPE <library_type>**（必选）：库的类型，取值范围：
  - `TILING`：Tiling 相关库，用于 Host 侧 Tiling 实现
  - `ACLNN`：aclnn 单算子调用库，用于单算子调用场景
  - `GRAPH`：算子入图所需的算子原型库，用于图模式
  - `TF_PLUGIN`：TensorFlow 框架适配相关库
- **<files>**（必选）：参与编译的源文件列表

### 示例

```cmake
# 编译 aclnn 单算子调用库
file(GLOB autogen_aclnn_src ${ASCEND_AUTOGEN_PATH}/aclnn_*.cpp)
set_source_files_properties(${autogen_aclnn_src} PROPERTIES GENERATED TRUE)
npu_op_library(cust_opapi ACLNN
    ${autogen_aclnn_src}
)
target_compile_options(cust_opapi PRIVATE -fvisibility=hidden)

# 编译算子原型库（用于入图）
file(GLOB proto_src ${ASCEND_AUTOGEN_PATH}/op_proto.cc)
set_source_files_properties(${proto_src} PROPERTIES GENERATED TRUE)
npu_op_library(cust_op_proto GRAPH
    ${ops_srcs}
    ${proto_src}
)
target_compile_options(cust_op_proto PRIVATE -fvisibility=hidden)

# 编译 Tiling 库
file(GLOB fallback_src ${ASCEND_AUTOGEN_PATH}/fallback_*.cpp)
set_source_files_properties(${fallback_src} PROPERTIES GENERATED TRUE)
npu_op_library(cust_optiling TILING
    ${ops_srcs}
    ${fallback_src}
)
target_compile_options(cust_optiling PRIVATE -fvisibility=hidden)
```

### 注意事项

- 自动生成的源文件需要使用 `set_source_files_properties` 设置 GENERATED 属性
- 建议使用 `target_compile_options` 添加 `-fvisibility=hidden` 以隐藏内部符号

---

## npu_op_kernel_library

### 功能

创建 Kernel 侧的编译库，用于编译算子的 Kernel 实现代码。

### 语法

```cmake
npu_op_kernel_library(<target_name> 
    SRC_BASE <path> 
    TILING_LIBRARY <tiling_target>
)
```

### 参数说明

- **<target_name>**（必选）：Kernel 库的目标名称
- **SRC_BASE <path>**（必选）：Kernel 源码的根目录，必须使用绝对路径
- **TILING_LIBRARY <tiling_target>**（必选）：依赖的 Tiling 库目标名称

### 示例

```cmake
# 编译 Kernel 库
npu_op_kernel_library(ascendc_kernels
    SRC_BASE ${CMAKE_SOURCE_DIR}/op_kernel/
    TILING_LIBRARY cust_optiling
)
```

### 注意事项

- SRC_BASE 必须配置为绝对路径，通常使用 `${CMAKE_SOURCE_DIR}/op_kernel/`
- TILING_LIBRARY 指定的 Tiling 库必须先于 Kernel 库创建

---

## npu_op_kernel_options

### 功能

为 Kernel 编译添加编译选项，支持全局选项和针对特定算子/AI 处理器型号的选项。

### 语法

```cmake
npu_op_kernel_options(<target_name> 
    <op_type> 
    [COMPUTE_UNIT <soc_version>] 
    OPTIONS ...
)
```

### 参数说明

- **<target_name>**（必选）：Kernel 库的目标名称
- **<op_type>**（必选）：配置生效的算子范围
  - `ALL`：对所有算子生效
  - `OP_TYPE`：对特定算子生效（需配合算子类型使用）
- **COMPUTE_UNIT <soc_version>**（可选）：指定 AI 处理器型号，不填写时对所有型号生效
- **OPTIONS ...**（必选）：编译选项列表，多个选项用空格分隔

### 示例

```cmake
# 为所有算子添加全局编译选项
npu_op_kernel_options(ascendc_kernels ALL OPTIONS --save-temp-files -g)

# 为特定算子在特定 AI 处理器型号上添加编译选项
npu_op_kernel_options(ascendc_kernels 
    AddCustom 
    COMPUTE_UNIT Ascend910B 
    OPTIONS -DNEW_MACRO=1
)

# 添加 TilingKey 编译选项（加速编译）
npu_op_kernel_options(ascendc_kernels ALL OPTIONS --tiling_key=1,2)
```

### 支持的编译选项

- **-Dname=value**：定义宏
- **-g**：生成调试信息
- **-O0/-O1/-O2/-O3**：优化级别
- **--tiling_key=1,2**：指定编译的 TilingKey，加速编译
- **--op_relocatable_kernel_binary=true**：生成可重链接的二进制文件
- **--cce-auto-sync=off**：关闭自动同步功能
- **--kernel-template-input="..."**：指定模板参数组合，加速编译

详细编译选项说明请参考 [算子工程编译 — 自定义编译选项](../编译与部署/基本流程.md#自定义编译选项)

### 注意事项

- 编译选项基于"算子类型 + AI 处理器型号"进行配置
- 对相同算子类型 + AI 处理器型号多次配置时，以最后一次为准
- ALL 选项和特定算子选项冲突时，以特定算子选项为准

---

## npu_op_kernel_sources

### 功能

描述 Kernel 目标的源码信息，包括算子实现文件的位置和命名。

### 语法

```cmake
npu_op_kernel_sources(<target_name> 
    [OP_TYPE <op_type>] 
    [KERNEL_DIR <path>] 
    [COMPUTE_UNIT <soc_version>] 
    [KERNEL_FILE <file>]
)
```

### 参数说明

- **<target_name>**（必选）：Kernel 库的目标名称
- **OP_TYPE <op_type>**（可选）：算子类型，必须与 KERNEL_FILE 同时使用
- **KERNEL_DIR <path>**（可选）：Kernel 源码相对于 SRC_BASE 的相对路径
- **COMPUTE_UNIT <soc_version>**（可选）：指定 KERNEL_FILE 生效的 AI 处理器型号
- **KERNEL_FILE <file>**（可选）：Kernel 实现文件名

### 示例

```cmake
# 指定算子源码目录和文件
npu_op_kernel_sources(ascendc_kernels
    OP_TYPE AddCustom
    KERNEL_DIR ./Add
    COMPUTE_UNIT Ascend910B
    KERNEL_FILE add_custom.cpp
)

# 为不同 AI 处理器型号指定不同的实现文件
npu_op_kernel_sources(ascendc_kernels
    OP_TYPE AddCustom
    KERNEL_DIR ./Add
    COMPUTE_UNIT Ascend910B
    KERNEL_FILE add_custom_910b.cpp
)

npu_op_kernel_sources(ascendc_kernels
    OP_TYPE AddCustom
    KERNEL_DIR ./Add
    COMPUTE_UNIT Ascend310P
    KERNEL_FILE add_custom_310p.cpp
)
```

### 注意事项

- OP_TYPE 和 KERNEL_FILE 必须同时使用，或同时不使用
- 不配置 KERNEL_FILE 时，文件名需与 OpType 满足转换规则（大驼峰转下划线）
- KERNEL_DIR 是相对于 SRC_BASE 的相对路径

---

## npu_op_device_tiling_library

### 功能

创建 Device 侧的 Tiling 库。该函数仅支持 RUN 模式。

### 语法

```cmake
npu_op_device_tiling_library(<target_name> 
    <type> 
    <files>
)
```

### 参数说明

- **<target_name>**（必选）：目标名称
- **<type>**（必选）：Tiling 产物的类型，取值为 SHARED 或 STATIC
- **<files>**（必选）：Tiling 源码文件列表

### 示例

```cmake
# 创建 Device 侧 Tiling 动态库
npu_op_device_tiling_library(device_tiling SHARED
    device_tiling_impl.cpp
)

# 创建 Device 侧 Tiling 静态库
npu_op_device_tiling_library(device_tiling STATIC
    device_tiling_impl.cpp
)
```

### 注意事项

- 该函数仅支持 RUN 模式，不适用于 SHARED/STATIC 模式
- Device 侧 Tiling 用于 Tiling 下沉场景

---

## npu_op_code_gen

### 功能

执行代码生成过程，自动生成 aclnn 单算子调用代码和入图所需的原型定义代码。

### 语法

```cmake
npu_op_code_gen(
    SRC <src_files> 
    OUT_DIR <output_dir> 
    PACKAGE <pkg_name> 
    [COMPILE_OPTIONS ...] 
    [JOIN_OP_DEF ...]
)
```

### 参数说明

- **SRC <src_files>**（必选）：参与代码生成的源文件范围
- **OUT_DIR <output_dir>**（必选）：生成代码的输出路径
- **PACKAGE <pkg_name>**（必选）：指定生成代码的 package 名称
- **COMPILE_OPTIONS ...**（可选）：自定义编译选项
- **JOIN_OP_DEF ...**（可选）：控制编译的 AI 处理器型号范围
  - `False`（默认）：编译 CMakePresets.json 中设置的所有 AI 处理器型号
  - `True`：仅编译 CMakePresets.json 和算子原型注册中共有的 AI 处理器型号

### 示例

```cmake
# 生成 aclnn 调用代码和原型定义代码
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} ops_srcs)
npu_op_code_gen(
    SRC ${ops_srcs}
    PACKAGE ${package_name}
    COMPILE_OPTIONS -g
    OUT_DIR ${ASCEND_AUTOGEN_PATH}
    JOIN_OP_DEF False
)
```

### 生成的文件

- **aclnn_*.cpp / aclnn_*.h**：单算子调用代码和头文件
- **op_proto.cc / op_proto.h**：算子原型定义代码和头文件
- **fallback_*.cpp**：Tiling 相关代码（如果配置了 EnableFallBack）

### 注意事项

- JOIN_OP_DEF 设置为 False 时可能编译更多 AI 处理器型号的算子
- 生成的文件需要设置为 GENERATED 属性才能参与后续编译

---

## 相关文档

- [编译组织](../编译与部署/基本流程.md#编译组织) - 完整的 CMakeLists.txt 组织方式和编译流程
- [算子工程编译](../编译与部署/基本流程.md) - 算子工程编译配置说明
- [soc_version 获取方法](soc_version获取方法.md) - AI 处理器型号获取方法
