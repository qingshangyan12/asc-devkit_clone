# 毕昇编译器相关知识

本文档介绍 AscendC 算子开发中使用的毕昇编译器（Bisheng Compiler）的相关知识和约束条件。

---

## 1. 编译器概述

毕昇编译器是华为基于 LLVM/Clang 开发的昇腾AI处理器专用编译器，支持将 AscendC 源代码（`.asc` 文件）编译为昇腾NPU可执行的二进制指令。

### 编译流程

```
.asc 源码  →  毕昇前端  →  LLVM IR  →  毕昇后端  →  NPU机器码
     (AscendC/C++)     (异构编译)     (链接优化)     (AICore指令)
```

### 两种编译模式

| 模式 | 目标 | 用途 |
|------|------|------|
| **Device 模式** | NPU AICore | 生产部署，生成真实NPU指令 |
| **CPU 模式** (`ASCENDC_CPU_DEBUG=1`) | x86/ARM CPU | 本地调试，在CPU上仿真运行 |

### SIM 仿真模式

在无 NPU 硬件环境下，可通过 SIM 仿真模式运行和调试 AscendC 算子：
- 不需真实 NPU 硬件
- 可在 x86 开发机上完整运行算子逻辑
- 通过 CMake 编译选项 `-DSIM=ON` 启用

---

## 2. 编译约束

### 2.1 文件扩展名与编译模式

| 文件扩展名 | 含义 | 编译行为 |
|-----------|------|---------|
| `.asc` | AscendC 内核源文件 | 使用毕昇编译器，Device模式编译 |
| `.cpp` | C++ Host 源文件 | 使用 GNU/GCC/毕昇，Host模式编译 |
| `.h` | 头文件 | 根据包含它的源文件确定编译模式 |

### 2.2 设备代码限制

编写 `.asc` 文件（Device代码）时需注意以下约束：

```
禁止使用：
- 标准库容器 (std::vector, std::string, std::map 等)
- 动态内存分配 (new/delete, malloc/free) —— 除 UB 分配器外
- 异常处理 (try/catch/throw)
- RTTI (typeid, dynamic_cast)
- 文件 I/O
- 系统调用
- 线程创建/std::thread
- 递归调用 (编译器通常不支持设备栈深度递归)
- 虚函数 (vtable 在设备内存中不可用)
```

### 2.3 有效的 C++ 标准

- AscendC 内核代码基于 C++17 标准
- 使用 `-std=c++17` 编译选项
- 标准链接库: `-stdlib=libstdc++`

---

## 3. 内核函数要求

### 3.1 内核函数签名

SIMD 内核（向量化）：

```cpp
// 返回类型必须是 void
// 必须有 __global__ 或 __global__ __vector__ 修饰
__global__ __vector__ void kernel_name(params...) { ... }

// 或者（SIMT 模式）：
__global__ void kernel_name(params...) { ... }
```

SIMT 内核（线程化）：

```cpp
__global__ void kernel_name(params...) {
    int32_t idx = blockIdx.x * blockDim.x + threadIdx.x;
    // ...
}
```

### 3.2 内核函数约束

- 内核函数不能有返回值（必须返回 `void`）
- 内核参数必须是指针类型或POD类型（plain old data）
- 不能传递复杂对象（如类、结构体含指针成员等）
- 不允许递归调用内核
- `__aicore__` 函数只能在设备代码中调用

### 3.3 函数修饰符对照

| 修饰符 | 含义 | 可调用 |
|--------|------|--------|
| `__global__` | 内核入口（Host调用） | 仅在Host端通过`<<<>>>`调用 |
| `__aicore__` | AICore设备函数 | 仅在Device端由内核或同属性函数调用 |
| 无修饰符 | Host函数 | 仅在Host端调用 |

---

## 4. `<<<>>>` 内核调用语法

### 基本语法

```cpp
kernel_name<<<blocks, threads, dynUbSize, stream>>>(arg1, arg2, ...);
```

| 参数 | 类型 | SIMD模式 | SIMT模式 | 说明 |
|------|------|---------|---------|------|
| `blocks` | `uint32_t` | 核数 | grid维度 | SIMD: 使用的AICore数量；SIMT: grid大小 |
| `threads` | `uint32_t` / `nullptr` | `nullptr` | block维度 | SIMD: 必须为`nullptr`；SIMT: 每个block线程数 |
| `dynUbSize` | `uint32_t` | 动态UB大小 | 动态UB大小 | 内核需要的动态UB总大小（字节），`nullptr`表示0 |
| `stream` | `aclrtStream` | 流 | 流 | ACL运行时流对象 |

### 示例

```cpp
// SIMD 内核调用
uint32_t blockNum = 8;
add_kernel<<<blockNum, nullptr, nullptr, stream>>>(x, y, z, len);

// SIMT 内核调用
uint32_t gridDim = 48;
uint32_t blockDim = 256;
add_kernel<<<gridDim, blockDim, nullptr, stream>>>(x, y, z, len);
```

---

## 5. 编译宏与条件编译

### 架构宏

```cpp
__NPU_ARCH__          // NPU 架构版本，如 2201 (Atlas A2), 3510 (Ascend 950)
ASCENDC_CPU_DEBUG     // CPU 调试模式标识
```

### 条件编译示例

```cpp
#if defined(ASCENDC_CPU_DEBUG) && ASCENDC_CPU_DEBUG == 1
// CPU调试模式下的代码路径
#include <cstdio>
printf("debug: value=%f\n", val);
#else
// NPU设备模式下的代码路径
AscendC::DumpTensor(xLocal, 256);
#endif
```

---

## 6. clangd IDE 支持

### 推荐配置

安装 `clangd-15` 或更高版本：

```bash
sudo apt install -y clangd-15
```

### `.clangd` 项目级配置

```yaml
CompileFlags:
  Add:
    - "-std=c++17"
    - "-stdlib=libstdc++"
    - "-D__NPU_ARCH__=2201"
    - "-DASCENDC_CPU_DEBUG=1"

---
If:
  PathMatch: ".*\\.(asc|aicpu)$"
CompileFlags:
  CompilationDatabase: None
  Add:
    - "-x"
    - "c++"
Diagnostics:
  Suppress:
    - "attributes_not_allowed"
    - "decomp_decl_template"
    - "ignored_attributes"
    - "unknown_type_name"
    - "undeclared_var_use"
    - "invalid_token_after_toplevel_declarator"
    - "missing_type_specifier"
    - "typename_nested_not_found"
    - "redefinition"
```

### 配置说明

- `-D__NPU_ARCH__=2201`：设定目标 NPU 架构（2201=Atlas A2, 3510=Ascend 950）
- `-DASCENDC_CPU_DEBUG=1`：开启 CPU 调试模式以获取更好的 IDE 支持
- `.asc` 文件使用 `-x c++` 强制作为 C++ 解析
- `Suppress` 列表屏蔽因 AscendC 扩展语法导致的 clangd 误报

### VSCode settings.json

```json
{
  "clangd.path": "/usr/bin/clangd",
  "clangd.arguments": [
    "--background-index=0",
    "--clang-tidy=0"
  ],
  "C_Cpp.intelliSenseEngine": "disabled"
}
```

---

## 7. CMake 编译配置

### 核心 CMake 变量

```cmake
# NPU 架构
set(ASCENDC_ARCH "2201")        # 2201(v200), 3510(Ascend 950)
# 编译模式
set(CPU_DEBUG ON)               # ON=CPU仿真; OFF=设备编译
set(SIM_MODE OFF)               # ON=SIM仿真; OFF=真实硬件
# 额外编译选项
add_definitions(-D__NPU_ARCH__=${ASCENDC_ARCH})
```

### 设备代码编译

```cmake
# .asc 文件使用毕昇编译器编译为 NPU 目标
# 通常由 cmake/asc/ 下的脚本自动处理
ascendc_add_library(target_name
    SOURCES kernel.asc
    ARCH ${ASCENDC_ARCH}
)
```

---

## 8. 常见编译错误与解决

| 错误信息 | 可能原因 | 解决方法 |
|---------|---------|---------|
| `unknown type name '__ubuf__'` | 未包含正确的头文件或编译模式不对 | 检查`#include`路径，确认使用毕昇编译器 |
| `attributes_not_allowed` | clangd 对__aicore__的误报 | 在`.clangd`中添加`Suppress`规则 |
| `undeclared_var_use` | AscendC宏展开后的变量 | 在`.clangd`中屏蔽该诊断 |
| `cannot convert 'float*' to '__gm__ float*'` | 地址空间不匹配 | 使用`__gm__`修饰全局内存指针 |
| `__global__ function cannot have return value` | 内核函数有返回值 | 修改为`void`返回类型 |

---

## 9. 调试支持

### CPU 调试模式

```cpp
// 设置环境变量开启 CPU 模式
ASCENDC_CPU_DEBUG=1

// 代码中可用的调试方法
#if ASCENDC_CPU_DEBUG
std::cout << "value: " << xLocal.GetValue(0) << std::endl;
#endif
```

### DumpTensor 调试

```cpp
AscendC::DumpTensor(tensor, count);   // 打印 Tensor 内容（仅 CPU 模式）
```

### Profiling

通过 `kernel_prof_trace_intf.h` 插入性能追踪点，使用 `msprof` 工具采集和分析。
