# asc\_threadfence

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：不支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：不支持
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2 推理产品：不支持
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas 推理系列产品AI Core：不支持
- Atlas 推理系列产品Vector Core：不支持
<!-- end id5 -->
<!-- npu="910" id6 -->
- Atlas 训练系列产品：不支持
<!-- end id6 -->

## 功能说明

在SIMT编程范式中，来自不同线程对同一份内存的读写操作可能造成**数据竞争**（Data Race），这是由于NPU架构的特性，线程的执行顺序和内存访问顺序可能不一致，从而导致其他线程看到的数据更新顺序与实际写入顺序不同。

内存栅栏类接口可以解决这类多线程同步读写共享内存的问题，其中asc\_threadfence接口用于保证所有线程对同一份全局、共享内存的访问过程中，**写入操作的时序性和可见性**。

**关键特征**：

-   该接口**不会阻塞线程**，仅保证内存操作的可见性顺序。
-   作用范围是全局的：调用后，确保调用线程在asc\_threadfence\(\)之前的所有全局内存和共享内存写操作对其他线程可见。

本接口一般适用的场景包括：

-   生产者-消费者同步场景：确保数据写入在标志位设置之前完成。
-   原子计数器场景：在原子计数器更新前确保相关数据已写入。
-   指针更新场景：链表、树等数据结构操作时确保数据一致性
-   多线程共享数据：确保共享数据的更新顺序对其他线程可见。

下图展示了没有内存栅栏时可能出现的内存可见性问题：

![](../../../figures/threadface_内存可见性问题.png)

通过使用asc\_threadfence\(\)接口，可解决上述内存可见性问题：

![](../../../figures/threadface_接口作用.png)

## 函数原型

```cpp
inline void asc_threadfence()
```

## 参数说明

无

## 返回值说明

无

## 约束说明

无

## 需要包含的头文件

使用该接口需要包含"simt\_api/device\_sync\_functions.h"头文件。

```cpp
#include "simt_api/device_sync_functions.h"
```

## 调用示例

完整样例请参考[MemoryFence样例](../../../../../../examples/03_simt_api/02_features/01_api_features/01_sync_instruction/memory_fence/README.md)。

以下示例中，消费者线程如果读取到`y[0]`为20，则后续读取`x[0]`时不会得到旧值1，因此不会出现`result[0]`为1且`result[1]`为20的结果。第一次写入必须在第二次写入之前可见。

-   SIMT编程场景：

    ```
    __global__ __launch_bounds__(1024) void KernelThreadFence(int32_t* x, int32_t* y, int32_t* result)
    {
        if (blockIdx.x == 0 && threadIdx.x == 0) {
            x[0] = 10;
            asc_threadfence(); // 保证x[0]的写入先于y[0]的写入对全局可见
            y[0] = 20;
        }

        if (blockIdx.x == 1 && threadIdx.x == 0) {
            int32_t b = y[0];
            asc_threadfence(); // 保证y[0]的读取先于x[0]的读取完成
            int32_t a = x[0];
            result[0] = a;
            result[1] = b;
        }
    }
    ```

-   SIMD与SIMT混合编程场景：

    ```
    __simt_vf__ __launch_bounds__(1024) inline void KernelThreadFence(
        __gm__ int32_t* x, __gm__ int32_t* y, __gm__ int32_t* result)
    {
        if (blockIdx.x == 0 && threadIdx.x == 0) {
            x[0] = 10;
            asc_threadfence(); // 保证x[0]的写入先于y[0]的写入对全局可见
            y[0] = 20;
        }

        if (blockIdx.x == 1 && threadIdx.x == 0) {
            int32_t b = y[0];
            asc_threadfence(); // 保证y[0]的读取先于x[0]的读取完成
            int32_t a = x[0];
            result[0] = a;
            result[1] = b;
        }
    }
    ```