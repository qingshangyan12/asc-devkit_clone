# block_index

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
<!-- @ref: asc-devkit/res/docs/zh/api/SIMT-API/cooperative_groups/grid_group/block_index_res.md#id1 -->

## 功能说明

头文件路径为：`"simt_api/cooperative_groups.h"`。

获取当前线程所属线程块在Grid中的三维索引。

## 函数原型

```cpp
static dim3 block_index()
```

## 参数说明

无

## 返回值说明

当前线程所属线程块在Grid中的三维索引。

## 约束说明

仅支持SIMT编程场景，不支持SIMD与SIMT混合编程场景。

## 调用示例

- SIMT编程场景：

    ```cpp
    #include "simt_api/cooperative_groups.h"

    using namespace cooperative_groups;
    __global__ void simt_kernel(dim3* result)
    {
        grid_group g = this_grid();
        dim3 block_index = g.block_index();
        if (threadIdx.x == 0) {
            result[blockIdx.x].x = block_index.x;
            result[blockIdx.x].y = block_index.y;
            result[blockIdx.x].z = block_index.z;
        }
    }
    ```

输入输出示例如下：

```text
使用启动参数: blocks = 4, threads = 1024
result: dim3(0, 0, 0), dim3(1, 0, 0), dim3(2, 0, 0), dim3(3, 0, 0)
```
