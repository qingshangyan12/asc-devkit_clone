---
name: example_sim_support
description: 样例 NPU/CPU/Sim 多模式编译支持技能。用于将 Ascend C 样例的 CMakeLists.txt 改造为通过 cmake 命令行参数（-DRUN_MODE=npu/cpu/sim -DNPU_ARCH=xxx）灵活切换编译模式。包含：CMakeLists.txt 模板、README 编译说明模板、.asc 文件 cpudebug 头文件添加、改造流程指导。
---

# 样例 NPU/CPU/Sim 多模式编译支持

## 技能描述

将 Ascend C 样例的 `CMakeLists.txt` 改造为通过 cmake 命令行参数灵活选择 NPU 模式、CPU调试模式或NPU仿真模式。ASC cmake 包（`find_package(ASC REQUIRED)`）会根据 `CMAKE_ASC_RUN_MODE` 缓存变量自动处理不同模式的编译配置，无需手动添加 if/elseif 分支。同时更新 `README.md` 编译说明。

## 触发时机

- 用户要求为样例添加 NPU/CPU/Sim 多模式编译支持
- 用户要求将样例的编译选项改为 cmake 参数传入
- 用户提到"样例编译模式切换"、"NPU/CPU 双模式"、"添加 sim 模式"等需求

## 参数说明

用户通过 cmake 命令行传入短参数名，CMakeLists.txt 中通过别名映射机制将其转换为 ASC cmake 包所需的内部变量名。

| 用户参数（短名） | 内部变量 | 说明 | 默认值 |
|------|------|------|--------|
| `RUN_MODE` | `CMAKE_ASC_RUN_MODE` | 运行模式：`npu`、`cpu` 或 `sim` | `npu` |
| `NPU_ARCH` | `CMAKE_ASC_ARCHITECTURES` | NPU 硬件架构 | `dav-2201` |

### NPU_ARCH 可选值

| NPU_ARCH | 对应产品 |
|----------|---------|
| `dav-2201` | Atlas A2 训练/推理系列产品、Atlas A3 训练/推理系列产品 |
| `dav-3510` | Ascend 950PR/Ascend 950DT |

## 改造流程

### 步骤 1：分析原始 CMakeLists.txt

读取目标样例的 `CMakeLists.txt`，识别以下信息：

1. **target 名称** — `add_executable` 的第一个参数（通常是 `demo`，但也可能是 `add`、`c_api_add_example` 等）
2. **源文件列表** — `.asc` 文件名
3. **现有编译选项** — 硬编码的 `--npu-arch`、`--run-mode` 等
4. **现有链接库** — 记录原始的 `target_link_libraries`（如果有），改造时原样保留
5. **现有编译定义** — `_GLIBCXX_USE_CXX11_ABI=0`、`ASCENDC_DEBUG=1` 等

### 步骤 2：替换 CMakeLists.txt

参考 [cmake-template.md](references/cmake-template.md)，将原始 CMakeLists.txt 改造为支持多模式。

**关键改造点：**

1. 在 `cmake_minimum_required` 之后、`find_package(ASC)` **之前**，添加别名映射（将用户友好的短参数名映射到 ASC 内部变量名）：

```cmake
if(DEFINED RUN_MODE)
    set(CMAKE_ASC_RUN_MODE ${RUN_MODE} CACHE STRING "Alias of RUN_MODE")
endif()

if(DEFINED NPU_ARCH)
    set(CMAKE_ASC_ARCHITECTURES ${NPU_ARCH} CACHE STRING "Alias of NPU_ARCH")
endif()
```

2. 在 `add_executable` 之后，设置架构默认值：

```cmake
set(NPU_ARCH "dav-2201" CACHE STRING "NPU ARCH, e.g. dav-2201, dav-3510")
```

3. 在 `target_compile_options` 中使用 `NPU_ARCH` 变量：

```cmake
target_compile_options(demo PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=${NPU_ARCH}>
)
```

4. **不需要手动添加 CPU/Sim 的 if/elseif 分支** — ASC cmake 包会根据 `CMAKE_ASC_RUN_MODE` 自动处理
5. **不要删除原始文件中已有的配置**（如 `target_link_libraries` 等），原样保留

**完整改造示例（以 target 名为 `demo` 为例）：**

```cmake
# copyright header...

cmake_minimum_required(VERSION 3.16)

if(DEFINED RUN_MODE)
    set(CMAKE_ASC_RUN_MODE ${RUN_MODE} CACHE STRING "Alias of RUN_MODE")
endif()

if(DEFINED NPU_ARCH)
    set(CMAKE_ASC_ARCHITECTURES ${NPU_ARCH} CACHE STRING "Alias of NPU_ARCH")
endif()

find_package(ASC REQUIRED)

project(kernel_samples LANGUAGES ASC CXX)

add_executable(demo
    hello_world.asc
)

set(NPU_ARCH "dav-2201" CACHE STRING "NPU ARCH, e.g. dav-2201, dav-3510")

target_compile_options(demo PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=${NPU_ARCH}>
)

# 此处保留原始 CMakeLists.txt 中的其他 NPU 配置（如 target_link_libraries 等），不要删除或新增
```

**注意：**
- 将 `demo` 替换为实际的 target 名称
- 别名映射（`if(DEFINED RUN_MODE)` ...）必须放在 `find_package(ASC)` **之前**，因为 ASC cmake 包在 find_package 时读取缓存变量

### 步骤 3：修改 .asc 源文件添加 cpudebug 头文件

在 `.asc` 源文件中，找到 `#include "kernel_operator.h"` 等基础头文件之后，添加 CPU 调试模式的条件编译头文件：

```cpp
#ifdef ASCENDC_CPU_DEBUG
#include "cpu_debug_launch.h"
#endif
```

**典型插入位置：** 在所有常规 `#include` 语句之后、核函数定义之前。

**示例（改造前）：**
```cpp
#include "kernel_operator.h"
#include "acl/acl.h"

__global__ __vector__ void hello_world()
{
    // ...
}
```

**示例（改造后）：**
```cpp
#include "kernel_operator.h"
#include "acl/acl.h"

#ifdef ASCENDC_CPU_DEBUG
#include "cpu_debug_launch.h"
#endif

__global__ __vector__ void hello_world()
{
    // ...
}
```

**注意：** 如果 `.asc` 文件中已经包含此头文件，则跳过此步骤。

### 步骤 4：更新 README.md

参考 [readme-template.md](references/readme-template.md)，更新编译运行部分：

1. 给出一个编译命令示例（默认 NPU 模式），使用短参数名 `-DNPU_ARCH=dav-2201`
2. 再加一句话提示可添加 `-DRUN_MODE=cpu` 切换CPU调试模式或 `-DRUN_MODE=sim` 切换NPU仿真模式
3. 给出 CPU 和 Sim 模式的 cmake 命令示例
4. 添加缓存清理提示：`> **注意：** 切换编译模式前需清理 cmake 缓存，可在 build 目录下执行 \`rm CMakeCache.txt\` 后重新 cmake。`
5. 添加编译选项说明表格（包含 `RUN_MODE` 和 `NPU_ARCH`）
6. 保留原有的环境变量配置和执行结果说明

### 步骤 5：验证

1. 检查 `cmake -DNPU_ARCH=dav-2201 ..` 能否正常生成 Makefile
2. 检查 `cmake -DRUN_MODE=cpu -DNPU_ARCH=dav-2201 ..` 能否正常生成 Makefile
3. 检查 `cmake -DRUN_MODE=sim -DNPU_ARCH=dav-2201 ..` 能否正常生成 Makefile

## 常见场景

### 场景 A：原始样例仅有 NPU 模式（最常见）

大多数样例仅有 NPU 编译选项。改造时：
- 添加 `RUN_MODE` / `NPU_ARCH` 别名映射（放在 `find_package(ASC)` 之前）
- 将硬编码的架构值替换为 `NPU_ARCH` CACHE 变量
- 保留原始 NPU 配置不变
- 用户通过 `-DRUN_MODE=cpu` 切换CPU调试模式或 `-DRUN_MODE=sim` 切换NPU仿真模式（由 ASC cmake 包自动处理）

### 场景 B：原始样例同时有 NPU 和 CPU 配置

少数样例在同一个 CMakeLists.txt 中同时包含 NPU 和 CPU 配置（未做互斥）。改造时：
- 删除手动编写的 CPU 配置块（`if/elseif` 分支），改为依赖 ASC cmake 包自动处理
- 添加 `RUN_MODE` / `NPU_ARCH` 别名映射
- 将架构值统一替换为 `NPU_ARCH` CACHE 变量

### 场景 C：原始样例已有 NPU 和 CPU 配置，需新增 Sim 模式

改造时：
- 删除手动编写的 CPU if 块，改为依赖 ASC cmake 包自动处理
- 添加 `RUN_MODE` / `NPU_ARCH` 别名映射
- 将架构值统一替换为 `NPU_ARCH` CACHE 变量
- 更新 README.md 中的参数说明，使用 `RUN_MODE` 和 `NPU_ARCH`

## 注意事项

1. **target 名称** — 如果样例的 target 名称不是 `demo`，所有 `target_*` 调用中的名称都要对应修改
2. **保留版权头** — 不修改 CMakeLists.txt 开头的版权声明
3. **别名映射必须在 find_package 之前** — `if(DEFINED RUN_MODE)` 和 `if(DEFINED NPU_ARCH)` 必须放在 `find_package(ASC REQUIRED)` 之前
4. **保留 project 和 find_package** — 不修改 `cmake_minimum_required`、`find_package(ASC REQUIRED)`、`project` 声明
5. **CACHE 变量类型** — `NPU_ARCH` 使用 `CACHE STRING`
6. **不要添加原始文件中没有的配置** — 原始 NPU 配置原样保留
7. **不需要手动添加 CPU/Sim 编译配置** — ASC cmake 包会根据 `CMAKE_ASC_RUN_MODE` 自动处理所有模式切换

## 参考资料

- [CMakeLists.txt 完整模板](references/cmake-template.md)
- [README.md 编译说明模板](references/readme-template.md)
