# C++ 代码与 AscendC 语言对应关系

本文档提供标准 C++ 代码与 AscendC API 的对应关系，帮助开发者将标准 C++ 算法逻辑转换为 AscendC 算子实现。

---

## 1. 内存与数据容器

### 标准 C++

```cpp
#include <vector>
std::vector<float> x(N);
float* x = new float[N];
```

### AscendC 语言扩展层 C API（指针风格）

```cpp
// 全局内存 (GM) 声明——由 Host 端分配
// 内核参数：__gm__ float* x

// 统一缓冲区 (UB) 声明——片上内存
__ubuf__ float xLocal[256];

// UB 动态内存分配
uint8_t ub_buf[256 * sizeof(float)];
__ubuf__ float* xLocal = (__ubuf__ float*)ub_buf;
```

### AscendC 基础 API（C++ Tensor 风格）

```cpp
// GM Tensor——由 GlobalTensor 封装指针
AscendC::GlobalTensor<float> xGm;
xGm.SetGlobalBuffer((__gm__ float*)x, dataSize);

// UB Tensor——通过 LocalMemoryAllocator 分配
AscendC::LocalMemAllocator<AscendC::Hardware::UB> ubAlloc;
AscendC::LocalTensor<float> xLocal = ubAlloc.Alloc<float, 256>();
```

### AscendC Tpipe/Tque 框架 API

```cpp
// 声明队列缓冲区和管道
AscendC::TQue<AscendC::QuePosition::VECIN, 1> inQueueX;
AscendC::TPipe pipe;
pipe.InitBuffer(inQueueX, 2, 256 * sizeof(float));

// 从队列获取 Tensor
AscendC::LocalTensor<float> xLocal = inQueueX.AllocTensor<float>();
```

---

## 2. 向量算术运算

### 逐元素加法

| 标准C++ | AscendC基础API | AscendC C API |
|---------|---------------|---------------|
| `for(i=0;i<n;i++) dst[i]=a[i]+b[i]` | `AscendC::Add(dst, a, b, n)` | `asc_add_sync(dst, a, b, n)` |

### 二元运算对照表

| C++ 逐元素操作 | AscendC 基础API | AscendC C API (sync) | AscendC Reg API |
|--------------|----------------|---------------------|-----------------|
| `dst[i] = a[i] + b[i]` | `AscendC::Add(dst, a, b, n)` | `asc_add_sync(dst, a, b, n)` | `Reg::Add(dst, a, b, mask)` |
| `dst[i] = a[i] - b[i]` | `AscendC::Sub(dst, a, b, n)` | `asc_sub_sync(dst, a, b, n)` | `Reg::Sub(dst, a, b, mask)` |
| `dst[i] = a[i] * b[i]` | `AscendC::Mul(dst, a, b, n)` | `asc_mul_sync(dst, a, b, n)` | `Reg::Mul(dst, a, b, mask)` |
| `dst[i] = a[i] / b[i]` | `AscendC::Div(dst, a, b, n)` | `asc_div_sync(dst, a, b, n)` | `Reg::Div(dst, a, b, mask)` |
| `dst[i] = max(a[i], b[i])` | `AscendC::Max(dst, a, b, n)` | `asc_max_sync(dst, a, b, n)` | `Reg::Max(dst, a, b, mask)` |
| `dst[i] = min(a[i], b[i])` | `AscendC::Min(dst, a, b, n)` | `asc_min_sync(dst, a, b, n)` | `Reg::Min(dst, a, b, mask)` |

### 标量运算对照表

| C++ | AscendC 基础API | AscendC C API | AscendC Reg API |
|-----|----------------|---------------|-----------------|
| `dst[i] = a[i] + s` | `AscendC::Adds(dst, a, s, n)` | `asc_adds_sync(dst, a, s, n)` | `Reg::Adds(dst, src, s, mask)` |
| `dst[i] = a[i] * s` | `AscendC::Muls(dst, a, s, n)` | `asc_muls_sync(dst, a, s, n)` | `Reg::Muls(dst, src, s, mask)` |
| `dst[i] = max(a[i], s)` | `AscendC::ClampMin(dst, a, s, n)` | — | `Reg::Maxs(dst, src, s, mask)` |
| `dst[i] = min(a[i], s)` | `AscendC::ClampMax(dst, a, s, n)` | — | `Reg::Mins(dst, src, s, mask)` |

---

## 3. 一元运算对照表

| C++ | AscendC 基础API | AscendC C API | 说明 |
|-----|----------------|---------------|------|
| `dst[i] = abs(a[i])` | `AscendC::Abs(dst, a, n)` | `asc_abs_sync(dst, a, n)` | 取绝对值 |
| `dst[i] = -a[i]` | `AscendC::Neg(dst, a, n)` | `asc_neg_sync(dst, a, n)` | 取负 |
| `dst[i] = exp(a[i])` | `AscendC::Exp(dst, a, n)` | `asc_exp_sync(dst, a, n)` | 指数运算 |
| `dst[i] = log(a[i])` | `AscendC::Log(dst, a, n)` | `asc_log_sync(dst, a, n)` | 自然对数 |
| `dst[i] = sqrt(a[i])` | `AscendC::Sqrt(dst, a, n)` | `asc_sqrt_sync(dst, a, n)` | 平方根 |
| `dst[i] = sin(a[i])` | `AscendC::Sin(dst, a, n)` | `asc_sin_sync(dst, a, n)` | 正弦 |
| `dst[i] = cos(a[i])` | `AscendC::Cos(dst, a, n)` | `asc_cos_sync(dst, a, n)` | 余弦 |

---

## 4. 融合运算对照表

| C++ | AscendC 基础API | 说明 |
|-----|----------------|------|
| `dst[i] = s*a[i] + dst[i]` | `AscendC::Axpy(dst, a, s, n)` | 标量乘加 |
| `dst[i] = a[i]*b[i] + c[i]` | `AscendC::Fma(dst, a, b, c, n)` | 融合乘加 |
| `dst[i] = cond[i] ? a[i] : b[i]` | `AscendC::Where(dst, a, b, cond, n)` | 条件选择 |

---

## 5. 比较运算对照表

| C++ | AscendC 基础API |
|-----|----------------|
| `dst[i] = (a[i] == b[i])` | `AscendC::Compare<T, EQ>(dst, a, b, n)` |
| `dst[i] = (a[i] != b[i])` | `AscendC::Compare<T, NE>(dst, a, b, n)` |
| `dst[i] = (a[i] < b[i])` | `AscendC::Compare<T, LT>(dst, a, b, n)` |
| `dst[i] = (a[i] > b[i])` | `AscendC::Compare<T, GT>(dst, a, b, n)` |
| `dst[i] = (a[i] <= b[i])` | `AscendC::Compare<T, LE>(dst, a, b, n)` |
| `dst[i] = (a[i] >= b[i])` | `AscendC::Compare<T, GE>(dst, a, b, n)` |

---

## 6. 逻辑运算对照表

| C++ | AscendC 基础API |
|-----|----------------|
| `dst[i] = (a[i] && b[i])` | `AscendC::LogicalAnd(dst, a, b, n)` |
| `dst[i] = (a[i] \|\| b[i])` | `AscendC::LogicalOr(dst, a, b, n)` |
| `dst[i] = !a[i]` | `AscendC::LogicalNot(dst, a, n)` |

---

## 7. 位运算对照表

| C++ | AscendC 基础API |
|-----|----------------|
| `dst[i] = a[i] & b[i]` | `AscendC::BitwiseAnd(dst, a, b, n)` |
| `dst[i] = a[i] \| b[i]` | `AscendC::BitwiseOr(dst, a, b, n)` |
| `dst[i] = a[i] ^ b[i]` | `AscendC::BitwiseXor(dst, a, b, n)` |
| `dst[i] = ~a[i]` | `AscendC::BitwiseNot(dst, a, n)` |

---

## 8. 类型转换

| C++ | AscendC 基础API | AscendC C API |
|-----|----------------|---------------|
| `dst[i] = (T)(a[i])` | `AscendC::Cast<T>(dst, a, n)` | `asc_cast_sync(dst, a, n)` |
| `dst[i] = ceil(a[i])` | `AscendC::Truncate<T, CAST_CEIL>(dst, a, n)` | — |
| `dst[i] = floor(a[i])` | `AscendC::Truncate<T, CAST_FLOOR>(dst, a, n)` | — |
| `dst[i] = round(a[i])` | `AscendC::Truncate<T, CAST_ROUND>(dst, a, n)` | — |

---

## 9. 数据搬运

### 标准 C++ 内存拷贝

```cpp
memcpy(dst, src, size);
```

### AscendC 数据搬运函数

| 操作 | AscendC 基础API | AscendC C API |
|------|----------------|---------------|
| GM→UB 拷贝 | `AscendC::DataCopy(ub, gm[offset], count)` | `asc_copy_gm2ub_sync(ub, gm, size)` |
| UB→GM 拷贝 | `AscendC::DataCopy(gm[offset], ub, count)` | `asc_copy_ub2gm_sync(gm, ub, size)` |
| UB→UB 拷贝 | `AscendC::DataCopy(dstUb, srcUb, count)` | — |

### 数据搬运注意

- GM→UB 和 UB→GM 在 C API 中以 `_sync` 后缀函数执行，会在数据搬运完成后自动同步
- 基础API 使用 `AscendC::DataCopy` 后需手动调用 `AscendC::SetFlag`/`AscendC::WaitFlag` 管理同步
- 框架API (Tpipe) 则通过队列和管道自动管理搬运同步

---

## 10. 规约运算

| C++ 循环规约 | AscendC 基础API |
|-------------|----------------|
| `for(i=0;i<n;i++) r+=a[i]` | `AscendC::ReduceSum<T>(dst, src, n)` |
| `for(i=0;i<n;i++) r=max(r,a[i])` | `AscendC::ReduceMax<T>(dst, src, n)` |
| `for(i=0;i<n;i++) r=min(r,a[i])` | `AscendC::ReduceMin<T>(dst, src, n)` |

---

## 11. 同步控制

### 标准 C++（单线程，无需同步）

无对应概念。

### AscendC 同步机制

| 同步需求 | 基础API | C API | 框架API |
|---------|---------|-------|---------|
| 等待数据搬运完成 | `AscendC::WaitFlag(pipe, event)` | `_sync` 后缀函数隐式 | 框架自动处理 |
| 等待向量计算完成 | `AscendC::PipeBarrier<PIPE_V>()` | 流水线屏障 | 框架自动处理 |
| 块间同步（跨核） | `AscendC::BlockBarrier()` | `asc_block_barrier()` | 手动调用 |

---

## 12. 多核并行化

### 标准 C++（OpenMP 风格）

```cpp
#pragma omp parallel for
for (int i = 0; i < N; i++) { ... }
```

### AscendC 多核模式

```cpp
// 获取当前核ID和总核数
uint32_t blockIdx = AscendC::GetBlockIdx();
uint32_t blockNum = AscendC::GetBlockNum();

// 每个核处理自己分片
uint32_t blockLen = totalLen / blockNum;
xGm.SetGlobalBuffer((__gm__ float*)x + blockIdx * blockLen, blockLen);
```

---

## 13. 函数属性对照

| C++ | AscendC | 说明 |
|-----|---------|------|
| 普通函数 | `__aicore__` 函数 | 在AICore上执行的函数 |
| 并行入口 | `__global__ __vector__` / `__global__` | 内核入口函数 |
| 全局内存指针 | `__gm__` 修饰符 | 标识全局内存地址空间 |
| 片上UB数组 | `__ubuf__` 修饰符 | 标识统一缓冲区地址空间 |

---

## 14. 快捷参考卡片

### 从 C++ for 循环到 AscendC 基础API 的转换

```cpp
// C++
for (int i = 0; i < N; i++) { dst[i] = OPERATION; }

// AscendC 基础API
AscendC::API_NAME(dstTensor, src0Tensor, src1Tensor, N);
```

### 从标量操作到向量操作

AscendC 的 API 天然是**向量化**的——每次调用处理一组连续数据（通常由硬件并行执行），无需手动编写循环。这是 AscendC 与传统 C++ 最大的编程模型差异。

### 常用数据类型

```cpp
// Tensor 数据类型 (基础API)
AscendC::LocalTensor<T>      // UB 上的 Tensor
AscendC::GlobalTensor<T>     // GM 上的 Tensor

// C API 数据类型
__ubuf__ float arr[256];     // UB 上的数组
__gm__ float* gPtr;          // GM 上的指针
```

### 常用头文件

```cpp
#include "kernel_operator.h"              // 基础API（C++ Tensor）
#include "c_api/asc_simd.h"               // 语言扩展层 C API
#include "kernel_tpipe.h"                 // Tpipe/Tque 框架API
#include "kernel_operator_vec_binary_intf.h"  // 向量二元运算
#include "kernel_operator_vec_unary_intf.h"   // 向量一元运算
#include "kernel_operator_data_copy_intf.h"   // 数据搬运
#include "kernel_operator_vec_reduce_intf.h"  // 规约运算
```
