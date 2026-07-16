# block_tile_memory

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

`block_tile_memory`用于为跨Warp的`thread_block_tile`提供组内同步和跨Warp通信所需的临时存储，可通过带`scratch`参数的`this_thread_block`接口传入。

## 类定义

```c++
template <unsigned int MaxBlockSize = 1024>
struct block_tile_memory
```

## 参数说明

**表1**  模板参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| MaxBlockSize | 输入 | 当前线程块中需要支持的最大线程数，默认值为1024。 |

## 约束说明

- `MaxBlockSize`必须大于0且小于等于2048。
- `MaxBlockSize`必须是Warp大小32的整数倍。
- 线程块实际启动线程数不能超过`MaxBlockSize`。

## 调用示例

- SIMT编程场景：

    ```c++
    using namespace cooperative_groups;
    __global__ void simt_kernel(...)
    {
        ...
        __ubuf__ block_tile_memory<2048> scratch;
        thread_block block = this_thread_block(scratch);
        auto tile64 = tiled_partition<64>(block);
        tile64.sync();
        ...
    }
    ```

- SIMD与SIMT混合编程场景：

    ```c++
    using namespace cooperative_groups;
    __simt_vf__ inline void simt_kernel(...)
    {
        ...
        __ubuf__ block_tile_memory<2048> scratch;
        thread_block block = this_thread_block(scratch);
        auto tile64 = tiled_partition<64>(block);
        tile64.sync();
        ...
    }
    ```
