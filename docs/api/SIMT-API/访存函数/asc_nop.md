# asc\_nop

## 产品支持情况

- Ascend 950PR/Ascend 950DT：支持
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：不支持
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：不支持
- Atlas 200I/500 A2 推理产品：不支持
- Atlas 推理系列产品AI Core：不支持
- Atlas 推理系列产品Vector Core：不支持
- Atlas 训练系列产品：不支持

## 功能说明

生成一条空操作指令，占用当前线程15个时钟周期，不执行任何实际计算与访存操作。

当SIMT算子连续读取相同GM地址时，会造成Bus被长期占用，此时会影响其它核向GM该地址上写入，进而引发卡死现象。在此场景下，建议在读指令后插入若干空操作指令来释放Bus。

## 函数原型

```
inline void asc_nop()
```

## 参数说明

无

## 返回值说明

无

## 约束说明

该接口仅用来规避SIMT连续读写同地址情况下的卡死问题。

## 需要包含的头文件

使用该接口需要包含"simt\_api/device\_functions.h"头文件。

```
#include "simt_api/device_functions.h"
```

## 调用示例

-   SIMT编程场景：

    ```
    __global__ __launch_bounds__(1024) void kernel_nop(float* dst, float* src)
    {

        int idx = threadIdx.x + blockIdx.x * blockDim.x;

        // 执行一些操作
        ...

        // 等待15个cycle
        asc_nop();

        // 继续后续操作
        ...
    }
    ```

-   SIMD与SIMT混合编程场景：

    ```
    __simt_vf__ __launch_bounds__(1024) inline void kernel_nop(__gm__ float* dst, __gm__ float* src)
    {
        int idx = threadIdx.x + blockIdx.x * blockDim.x;

        // 执行一些操作
        ...

        // 等待15个cycle
        asc_nop();

        // 继续后续操作
        ...
    }
    ```