# sync

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

同步当前线程所属的`thread_block_tile`组内的所有线程。

## 函数原型

```c++
void sync() const
```

## 参数说明

无

## 返回值说明

无

## 约束说明

- 必须保证`thread_block_tile`组内所有线程都能执行到同一个`sync()`调用，否则会导致卡死。
- 当`thread_block_tile`的模板参数`Size`大于32且小于2048时，即`Size`为64、128、256、512、1024的跨Warp协作组场景，`sync()`同步当前`thread_block_tile`组内所有线程；当`Size`等于2048时，`sync()`同步整个线程块内所有线程。

## 调用示例

- SIMT编程场景：

    ```c++
    using namespace cooperative_groups;
    __global__ void simt_kernel(...)
    {
        ...
        thread_block block = this_thread_block();
        auto tile4 = tiled_partition<4>(block);
        tile4.sync();
        ...
    }
    ```

- SIMD与SIMT混合编程场景：

    ```c++
    using namespace cooperative_groups;
    __simt_vf__ inline void simt_kernel(...)
    {
        ...
        thread_block block = this_thread_block();
        auto tile4 = tiled_partition<4>(block);
        tile4.sync();
        ...
    }
    ```
