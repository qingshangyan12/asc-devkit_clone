# 毕昇编译器相关知识

本文档介绍 AscendC 算子开发中使用的毕昇编译器（Bisheng Compiler）的相关知识和约束条件。

---

## 1. 编译器概述

毕昇编译器是华为基于 LLVM/Clang 开发的昇腾AI处理器专用编译器，支持将 AscendC 代码编译为昇腾NPU可执行的二进制指令。

### 1.1 两种编译模式

| 模式 | 目标 | 用途 |
|------|------|------|
| **Device 模式** | NPU AICore | 生产部署，生成真实NPU指令 |
| **CPU 模式** (`-DCMAKE_ASC_RUN_MODE=cpu`) | x86/ARM CPU | 本地调试，在CPU上仿真运行 |

### 1.2 SIM 仿真模式

在无 NPU 硬件环境下，可通过 SIM 仿真模式运行和调试 AscendC 算子：
- 不需真实 NPU 硬件
- 可在 x86 开发机上完整运行算子逻辑
- 通过 CMake 编译选项 `-DCMAKE_ASC_RUN_MODE=sim` 启用

---

## 2. 内核函数要求

### 2.1 内核函数签名

SIMD 内核（向量化）：

```cpp
// 返回类型必须是 void
// 必须有 __aicore__ 或 __simd_vf__ __simd_callee__ 修饰
__aicore__ __inline__ void api_name(params...) { ... }

__simd_vf__ __inline__ void api_name(params...) { ... }

__simd_callee__ __inline__ void api_name(params...) { ... }

```

### 2.2 内核函数约束（重要！！！）

- `__aicore__`内核函数可以有返回值
- `__aicore__`入口函数中应进行编译期数据类型校验
- `__aicore__`可以调用其他`__aicore__`, `__simd_vf__`, `__simd_callee__`函数
- `__aicore__`函数中入参LocalTensor的地址需满足32B对齐要求
- `__simd_vf__`和`__simd_callee__`函数中的RegTensor固定为256B
- `__simd_vf__`函数和`__simd_callee__`函数内MaskReg寄存器数量不能超过8个
- `__simd_vf__`函数和`__simd_callee__`函数中的MaskReg固定为32B，其中每一个bit位指示RegTensor中一个Byte是否参与计算
- `__simd_vf__`函数和`__simd_callee__`函数中，在处理条件分支时，应尽量通过布尔运算简化所需MaskReg的数量
- `__simd_vf__`函数和`__simd_callee__`函数中应尽量避免标量运算，仅允许少量标量运算
- `__simd_vf__` 和 `__simd_callee__`函数不能有返回值（必须返回 `void`）
- `__simd_vf__`函数和`__simd_callee__`函数标识该函数在寄存器中执行，只能使用RegTensor，不能使用LocalTensor
- `__simd_vf__`只能调用其他`__simd_callee__`函数
- `__simd_callee__`只能调用其他`__simd_callee__`函数
- `__simd_vf__` 和 `__simd_callee__` 函数内循环应在编译期展开
- `__simd_vf__`函数和`__simd_callee__`函数内循环为硬件循环，次数不能超过 65504次，嵌套最多4层
- `__simd_vf__`函数和`__simd_callee__`函数内循环不能包含变量声明和初始化，其应在循环外进行
- `__simd_vf__`函数和`__simd_callee__`函数内循环不能包含非编译期条件判断
- 当同一个`__simd_vf__`中寄存器数量过多时，可以将中间结果暂存到UB的tmp buffer上，之后再从UB读取到__simd_vf__中，进行后续计算。同时`__aicore__`中需要对应为中间结果分配内存。

---

## 3. 编译宏与条件编译

### 3.1 架构宏

```cpp
CMAKE_ASC_ARCHITECTURES          // NPU 架构版本，如 2201 (Atlas A2), 3510 (Ascend 950)
CMAKE_ASC_RUN_MODE     // 运行模式，如 cpu, sim,
```

### 3.2 条件编译示例

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

## 4. CMake 编译配置

### 1 核心 CMake 变量

  ```bash
  cmake -DCMAKE_ASC_RUN_MODE=cpu -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j; # cpu调试模式
  cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j; # NPU仿真模式
  ```