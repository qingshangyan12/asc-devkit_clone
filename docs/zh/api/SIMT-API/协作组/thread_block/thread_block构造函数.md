# thread_block构造函数

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

`thread_block`不提供默认的构造函数，用户使用`this_thread_block`函数获取当前线程所属的`thread_block`对象。

如果后续需要创建跨warp的thread_block_tile（size > 32），创建`thread_block`对象时需传入GM或UB上的[block_tile_memory](../tiled_partition.md#block_tile_memory)对象，用于跨warp的thread_block_tile的内部功能实现。

## 函数原型

```c++
thread_block this_thread_block()
```

```c++
template <unsigned int MaxBlockSize>
thread_block this_thread_block(block_tile_memory<MaxBlockSize>& scratch)
```

## 参数说明

**表1**  参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| scratch | 输入 | Global Memory或Unified Buffer上的[block_tile_memory](../tiled_partition.md#block_tile_memory)对象，用于为`thread_block`携带跨Warp临时存储。 |

## 返回值说明

返回当前线程所属的`thread_block`对象。

## 约束说明

- 线程块实际启动线程数不能超过`MaxBlockSize`。
- `scratch`对应的[block_tile_memory](../tiled_partition.md#block_tile_memory)对象必须位于Global Memory或Unified Buffer，不能是在栈空间中创建的对象。

## 调用示例

参考[调用示例](sync.md#调用示例)
