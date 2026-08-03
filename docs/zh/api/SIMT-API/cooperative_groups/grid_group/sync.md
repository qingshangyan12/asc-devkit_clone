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

同步当前Grid内所有SIMT线程。所有线程块中的所有线程都执行到该同步点位置后，Grid内线程才能继续执行。

## 函数原型

```cpp
void sync() const
```

## 参数说明

无

## 返回值说明

无

## 约束说明

- 仅支持SIMT编程场景，不支持SIMD与SIMT混合编程场景。
- 必须保证Grid内所有线程都能执行到同一个`sync()`调用，否则是未定义行为。
- `sync()`可用于跨线程块同步。同步后如果读取其他线程块写入的Global Memory数据，需要遵循Global Memory的Cache一致性要求刷新Cache，可使用[asc_dcci_single](../../memory_access_functions/asc_dcci_single.md)或[asc_dcci_entire](../../memory_access_functions/asc_dcci_entire.md)接口刷新Cache。

## 调用示例

- SIMT编程场景：

    ```cpp
    using namespace cooperative_groups;
    __global__ void simt_kernel(uint64_t* output)
    {
        grid_group g = this_grid();
        unsigned int block = static_cast<unsigned int>(g.block_rank());

        if (threadIdx.x == 0) {
            output[block] = block + 1;
        }

        g.sync(); // 同步Grid内所有线程

        if (threadIdx.x == 0) {
            uint64_t sum = 0;
            asc_dcci_entrie(output); // 刷新Cache
            for (unsigned int i = 0; i < g.num_blocks(); ++i) {
                sum += output[i];
            }
            output[g.num_blocks() + block] = sum;
        }
    }
    ```
