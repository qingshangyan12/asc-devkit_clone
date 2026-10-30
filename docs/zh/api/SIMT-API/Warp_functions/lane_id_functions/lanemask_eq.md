# lanemask\_eq

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR&950DT系列产品：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3系列产品：不支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2系列产品：不支持
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2推理产品：不支持
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas推理系列产品AI Core：不支持
- Atlas推理系列产品Vector Core：不支持
<!-- end id5 -->
<!-- npu="910" id6 -->
- Atlas训练系列产品：不支持
<!-- end id6 -->
<!-- @ref: asc-devkit/res/docs/zh/api/SIMT-API/Warp_functions/lane_id_functions/lanemask_eq_res.md#id1 -->

## 功能说明

头文件路径为：`"simt_api/device_functions.h"`。

获取当前线程的一个32位掩码，在当前线程所属的Warp中，只有当前线程所在的Lane位被置为1，其余位为0。

如Lane ID为0的线程，调用本接口获取到32位掩码：0000 0000 0000 0000 0000 0000 0000 0001。

如Lane ID为31的线程，调用本接口获取到32位掩码：1000 0000 0000 0000 0000 0000 0000 0000。

**图1**  lanemask\_eq示意图

![](../../../figures/lanemask_eq.png "lanemask_eq图示")

## 函数原型

```cpp
int32_t lanemask_eq()
```

## 参数说明

无

## 返回值说明

返回一个32位整数的位掩码。

## 约束说明

无

## 调用示例

下面示例使用`lanemask_eq()`获取当前Lane对应的位掩码，并据此判断当前线程位于Warp的上半区还是下半区，再通过`asc_shfl()`分别广播上下半Warp中指定Lane的数据。示例中使用[asc_shfl](../Warp_shfl_functions/asc_shfl.md)，需另外包含`simt_api/device_warp_functions.h`头文件，其中`srcLane`的取值范围为[0, 15]。

-   SIMT编程场景：

    ```cpp
    #include "simt_api/device_functions.h"
    #include "simt_api/device_warp_functions.h"

    __global__ __launch_bounds__(1024) void kernel_lanemask_eq(int32_t* dst, int32_t* src, int32_t srcLane)
    {
        int idx = threadIdx.x + blockIdx.x * blockDim.x;
        int32_t self_mask = lanemask_eq();
        int32_t group_leader_lane = ((self_mask & 0x0000FFFFU) != 0) ? srcLane : (srcLane + 16);
        int32_t value = src[idx];
        value = asc_shfl(value, group_leader_lane, 32);
        dst[idx] = value;
    }
    ```

-   SIMD与SIMT混合编程场景：

    ```cpp
    #include "simt_api/device_functions.h"
    #include "simt_api/device_warp_functions.h"

    __simt_vf__ __launch_bounds__(1024) void kernel_lanemask_eq(__gm__ int32_t* dst, __gm__ int32_t* src, int32_t srcLane)
    {
        int idx = threadIdx.x + blockIdx.x * blockDim.x;
        int32_t self_mask = lanemask_eq();
        int32_t group_leader_lane = ((self_mask & 0x0000FFFFU) != 0) ? srcLane : (srcLane + 16);
        int32_t value = src[idx];
        value = asc_shfl(value, group_leader_lane, 32);
        dst[idx] = value;
    }
    ```

输入输出示例如下：

```text
src: 12, 45, 8, 39, 27, 55, 3, 68, 16, 72, 24, 6, 81, 33, 48, 19, 58, 2, 75, 31, 9, 64, 37, 51, 14, 88, 22, 5, 70, 42, 29, 66
srcLane: 3
dst: 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31
```
