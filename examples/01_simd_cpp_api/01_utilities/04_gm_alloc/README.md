# GmAlloc/GmFree 样例

## 概述

本样例基于 tikicpulib CPU 调试模式，演示 `GmAlloc` 和 `GmFree` 接口的使用方法，在 CPU 调试模式下创建共享内存用于多核数据共享。

## 功能说明

### GmAlloc

```cpp
void* GmAlloc(size_t size);
```

- **功能**: 在 `/tmp` 目录下创建共享文件，返回映射指针
- **参数**: `size` - 要分配的共享内存大小（字节）
- **返回**: 共享内存地址指针，失败返回 nullptr
- **用途**: CPU 调试模式下，多个核共享同一块内存数据

### GmFree

```cpp
void GmFree(void* ptr);
```

- **功能**: 释放 GmAlloc 创建的共享内存
- **参数**: `ptr` - GmAlloc 返回的共享内存地址

### SetKernelMode

```cpp
void SetKernelMode(KernelMode mode);
```

- **功能**: 设置内核模式（AIV/AIC/MIX），矢量算子需设置 AIV_MODE
- **参数**: `KernelMode::AIV_MODE` / `KernelMode::AIC_MODE` / `KernelMode::MIX_MODE`

### ICPU_RUN_KF

```cpp
ICPU_RUN_KF(kernel_func, numBlocks, args...);
```

- **功能**: CPU 调试模式下启动核函数，fork 多进程模拟多核并行

## 重要限制

⚠️ **仅 CPU 调试模式可用**

| 模式 | 支持情况 |
|------|---------|
| CPU 调试 (tikicpulib) | ✅ 可用 |
| NPU 运行 | ❌ 不可用 |
| NPU 仿真 | ❌ 不可用 |

⚠️ **tikicpulib 模式下只能使用纯 C/C++ 代码，不能使用 Ascend C API**（如 TPipe、TQue、DataCopy、GlobalTensor 等）

## 样例逻辑

```
┌─────────────────────────────────────────────────────────────┐
│                     共享内存 (GmAlloc)                       │
│  ┌─────────┬─────────┬─────────┬─────────┬─────────┬───────┐│
│  │ Block 0 │ Block 1 │ Block 2 │ Block 3 │ Block 4 │ ...   ││
│  │ 2048元素│ 2048元素│ 2048元素│ 2048元素│ 2048元素│       ││
│  └─────────┴─────────┴─────────┴─────────┴─────────┴───────┘│
│      ↑         ↑         ↑         ↑         ↑              │
│     Core0    Core1     Core2     Core3     Core4   ...       │
│     访问     访问      访问      访问      访问               │
└─────────────────────────────────────────────────────────────┘
```

1. `GmAlloc` 创建共享内存（在 /tmp 下创建共享文件）
2. 初始化共享内存数据
3. `SetKernelMode(AIV_MODE)` 设置矢量模式
4. `ICPU_RUN_KF` 启动多核函数，每个核访问自己的区域
5. 验证各核计算结果
6. `GmFree` 释放共享内存

## 编译运行

```bash
mkdir -p build && cd build;
cp ../CMakeLists_cpu.txt CMakeLists.txt;
cmake .;
make -j;
./demo
```

## 预期输出

```
===== tikicpulib 纯 C++ 核函数示例 =====

配置：
  - 总核数: 8
  - 每核数据量: 2048 elements

使用 GmAlloc 分配共享内存...
[SUCCESS] 共享内存已分配

设置内核模式为 AIV_MODE...
启动核函数 (ICPU_RUN_KF)...
  模拟 8 个核并行执行
[SUCCESS][CORE_0][pid ...] exit success!
...
[SUCCESS][CORE_7][pid ...] exit success!

验证结果...
[SUCCESS] 所有数据验证正确!

使用 GmFree 释放共享内存...
[SUCCESS] 共享内存已释放

===== 测试完成 =====
```

## NPU 模式替代方案

在 NPU 运行模式下，使用 ACL Runtime API：

```cpp
aclrtMalloc((void **)&deviceMem, size, ACL_MEM_MALLOC_HUGE_FIRST);
kernel<<<numBlocks, nullptr, stream>>>(deviceMem);
aclrtFree(deviceMem);
```

## 相关文件

| 文件 | 说明 |
|------|------|
| `gm_alloc_cpu_test.cpp` | tikicpulib 主样例代码 |
| `CMakeLists_cpu.txt` | 编译配置 |
| `README.md` | 本文档 |