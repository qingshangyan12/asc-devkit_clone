# asc\_atomic\_xor

## 产品支持情况

| 产品 | 是否支持 |
| --- | --- |
| Ascend 950PR/Ascend 950DT | √ |
| Atlas A3 训练系列产品/Atlas A3 推理系列产品 | x |
| Atlas A2 训练系列产品/Atlas A2 推理系列产品 | x |
| Atlas 200I/500 A2 推理产品 | x |
| Atlas 推理系列产品AI Core | x |
| Atlas 推理系列产品Vector Core | x |
| Atlas 训练系列产品 | x |

## 功能说明

对Unified Buffer或Global Memory上address的数值与指定数值val进行原子异或（^）操作，即将address数值异或（^）val的结果赋值到Unified Buffer或Global Memory上。

## 函数原型

```
inline int32_t asc_atomic_xor(int32_t *address, int32_t val)
```

```
inline uint32_t asc_atomic_xor(uint32_t *address, uint32_t val)
```

```
inline int64_t asc_atomic_xor(int64_t *address, int64_t val)
```

```
inline uint64_t asc_atomic_xor(uint64_t *address, uint64_t val)
```

## 参数说明

**表 1**  参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| address | 输出 | Unified Buffer或Global Memory的地址。 |
| val | 输入 | 源操作数。 |

不同数据类型支持的内存范围说明如下：

**表 2**  不同数据类型支持的内存范围

| 参数数据类型 | 支持的内存空间 |
| --- | --- |
| int32_t、uint32_t | Unified Buffer、Global Memory |
| int64_t、uint64_t | Global Memory |

## 返回值说明

Unified Buffer或Global Memory上的初始数据。

## 约束说明

无

## 文档检查与使用建议

- 上方函数原型对应纯 SIMT 直调场景；在 SIMD 与 SIMT 混合编程场景中，需要按示例对 Global Memory 指针添加 `__gm__`，对 Unified Buffer 指针添加 `__ubuf__`。
- `address` 指向的对象会被原子更新，函数返回值为执行本次原子操作前 `address` 上的旧值。只需要最终写回结果时，可以不接收返回值。
- 32 位整型重载支持 Unified Buffer 与 Global Memory；按官方支持矩阵和本次复测，64 位整型重载仅支持 Global Memory，纯 SIMT 下 64 位 Unified Buffer 调用虽可编译，但实测未发生原子更新。该接口不支持浮点类型。

## 需要包含的头文件

使用该接口需要包含"simt\_api/device\_atomic\_functions.h"头文件。

```
#include "simt_api/device_atomic_functions.h"
```

## 实测验证

实测环境：Ascend 950PR，CANN 9.0.0，`bisheng --enable-simt --npu-arch=dav-3510`。

| 覆盖项 | 初始值 | 操作 | 期望结果 | 实测结果 |
| --- | --- | --- | --- | --- |
| `uint32_t` Global Memory | `0` | 128 个线程各执行 `asc_atomic_xor(address, 1U)` | `0` | 通过 |

本用例执行偶数次异或，最终值回到 0，用于验证原子异或在多线程竞争写同一地址时结果一致。

## 调用示例

**场景**
多个线程检查事件是否命中，命中时使用 `asc_atomic_xor` 翻转共享奇偶标志。最终值为 1 表示命中次数为奇数，0 表示命中次数为偶数。

**输入说明**

| 名称 | 说明 |
| --- |  --- |
| `hit` | 每个元素表示一个线程是否命中事件，1 为命中，0 为未命中。 |
| `parity` | Global Memory 中的奇偶标志，kernel 启动前清零。 |
| `n` | 输入元素个数。 |

**代码实现**

-   SIMT编程场景：

    ```cpp
    __global__ __launch_bounds__(256) void compute_hit_parity(uint32_t *parity,
                                                             uint32_t *hit,
                                                             uint32_t n)
    {
        uint32_t idx = blockIdx.x * blockDim.x + threadIdx.x;
        if (idx >= n) {
            return;
        }

        if (hit[idx] != 0U) {
            asc_atomic_xor(parity, 1U);
        }
    }
    ```

-   SIMD与SIMT混合编程场景：

    SIMD与SIMT混合编程场景，需要显式使用地址空间限定符表示地址空间：\_\_gm\_\_表示Global Memory内存空间，\_\_ubuf\_\_表示Unified Buffer内存空间。

    ```cpp
    __simt_vf__ __launch_bounds__(1024) inline void compute_hit_parity(__gm__ uint32_t *parity,
                                                                      __gm__ uint32_t *hit,
                                                                      uint32_t n)
    {
        uint32_t idx = blockIdx.x * blockDim.x + threadIdx.x;
        if (idx >= n) {
            return;
        }

        if (hit[idx] != 0U) {
            asc_atomic_xor(parity, 1U);
        }
    }
    ```

**结果说明**

```
hit: 1, 0, 1, 1
parity: 1 // 共有3次命中，奇数次翻转后结果为1
```
