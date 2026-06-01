# gridDim配置优化

【优先级】高

【描述】SIMT编程模式下，通过`<<<...>>>`配置`gridDim`决定本次任务启动多少个block，这些block最终被调度到硬件的Vector Core（物理核）上执行。由于硬件限制，一个物理核同一时刻只能驻留并执行一个block，每个block的启动和调度都会引入固定开销。当`gridDim`远超物理核数时，需要启动调度的block大幅增加，调度的固定开销累积明显；`gridDim`不足时，物理核闲置、并行核数不足、单核工作量增加，会显著影响耗时。此外数据规模越小、每核工作量越少，调度固定开销占比越高，最优`gridDim`会小于物理核数。因此，gridDim应结合物理核数与数据规模设置。

以Ascend 950PR/Ascend 950DT为例，实测环境运行时物理核（Vector Core）数量为64。`gridDim`与物理核数的关系会影响调度开销：当`gridDim ≤ 64`时，所有block可以一次性占用全部物理核并行执行，需要启动调度的block较少、固定开销有限；当`gridDim > 64`时，需要启动和调度的block数量继续增加，多block启动和调度的固定开销开始明显累积，调度开销随gridDim增大而上升。

物理核数可以在运行期通过[aclrtGetDeviceInfo](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/910beta1/API/runtimeapi/aclcppdevg_03_1867.html)查询。
下面以Gather为例，对比大shape（`1024*2048=2097152`个元素）下不同gridDim配置的性能差异。

【反例】将gridDim与元素总数强绑定，配置远超物理核数的`gridDim`，导致需要启动和调度的block大幅增加。

```cpp
// 基础写法：每个线程只处理1个元素，无for循环
// 要求 gridDim.x * blockDim.x == 元素总数(本样例 1024 * 2048 == 2097152)
__global__ __launch_bounds__(INDEX_WIDTH) void gather_naive_kernel(
    float* input, uint32_t* index, float* output,
    uint32_t input_total_length, uint32_t index_total_length)
{
    uint32_t global_idx = blockIdx.x * blockDim.x + threadIdx.x;

    uint32_t gather_idx = index[global_idx];
    if (gather_idx >= input_total_length) {
        gather_idx = 0;
    }
    output[global_idx] = input[gather_idx];
}

// gridDim.x * blockDim.x = 1024 * 2048 = 2097152，恰好等于元素总数
gather_naive_kernel<<<1024, INDEX_WIDTH, 0, stream>>>(
    input_device, index_device, output_device,
    input_total_length, index_total_length);
```

上述实现让每个线程用全局线程号`blockIdx.x * blockDim.x + threadIdx.x`定位唯一一个元素，因此gridDim被"线程号=元素号"这条隐式约束限制为`元素总数 / 单核线程数 = 2097152 / 2048 = 1024`，核数被数据规模决定。`gridDim=1024`远超物理核数64，需要启动和调度的block数量是物理核数的16倍，多block启动和调度的固定开销明显累积。该实现的性能数据如下：

| gridDim | Task Duration(us) |
| :-------: | :---------------: |
|   1024    |      78.289       |

【正例】解耦`gridDim`与元素总数，把gridDim设为物理核数，改用全局grid-stride循环遍历整个数组。

```cpp
// 优化写法：全局 grid-stride loop，所有线程以总线程数为步长跨步遍历整个数组
__global__ __launch_bounds__(MAX_THREAD_NUM) void gather_strided_kernel(
    float* input, uint32_t* index, float* output,
    uint32_t input_total_length, uint32_t index_total_length)
{
    uint32_t global_idx = blockIdx.x * blockDim.x + threadIdx.x;
    uint32_t stride = gridDim.x * blockDim.x;   // 乘法，非除法

    for (uint32_t i = global_idx; i < index_total_length; i += stride) {
        uint32_t gather_idx = index[i];
        if (gather_idx >= input_total_length) {
            gather_idx = 0;
        }
        output[i] = input[gather_idx];
    }
}

// gridDim固定为最大物理核数64，每block仍取硬件上限2048线程
gather_strided_kernel<<<64, MAX_THREAD_NUM, 0, stream>>>(
    input_device, index_device, output_device,
    input_total_length, index_total_length);
```

上述实现的gather语义与反例完全相同，唯一变化的变量是把`gridDim`从1024降低到物理核数64。`gridDim`降低后，总线程数`gridDim.x * blockDim.x = 64 * 2048 = 131072`小于元素总数，每个线程从自己的全局编号`global_idx`起步、以总线程数为步长跨步遍历整个数组（每个线程循环`2097152 / 131072 = 16`次）。该实现在大shape下的性能数据如下：

| gridDim.x | Task Duration(us) |
| :-------: | :---------------: |
|    64     |      58.583       |

根据Task Duration数据，`gridDim`从1024降低到64后，执行耗时从78.289us下降到58.583us，下降约25.2%，整体性能提升约1.34倍。gather读写总量和每block线程数都没有变化，提升完全来自唯一变化的变量——`gridDim`，`gridDim=1024`需要启动和调度的block是物理核数的16倍、固定开销累积，而`gridDim=64`一次性占用全部物理核、需要启动调度的block较少。

需要注意的是，核数并非越少越好。若把核数继续降低到32（仅用一半物理核），Task Duration会升到110.540us，比64核慢约88.7%，且慢于1024核配置的78.289us。三种核数配置的对比如下：

| 写法 | gridDim | Task Duration(us) | 说明 |
| :--: | :-------: | :---------------: | ---- |
| 单线程多数据 | 32 | 110.540 | `gridDim`偏小，并行核数不足 |
| **单线程多数据** | **64** | **58.583** | **`gridDim`匹配物理核数，最优** |
| 单线程单数据 | 1024 | 78.289 | `gridDim`过大，多block调度开销高 |

<img src="../../../figures/griddim_config_large_shape.png" alt="大shape(2097152) Task Duration 随核数变化" width="480" />

上述大shape实验结论是"`gridDim`=物理核数最优"。但这条经验不能直接沿用到小shape：当数据量很小、每核工作量很少时，启动更多核带来的调度固定开销会超过有效计算的收益。下面以小shape（`8*2048=16384`个元素）为例，对比同一stride写法下不同gridDim配置的性能差异。

【反例】小shape下沿用"使用全部物理核"的经验，把gridDim直接设为最大物理核数64。

```cpp
// kernel与大shape正例完全相同(gather_strided_kernel)，仅将shape调整为小shape 16384
// 直接沿用"使用全部物理核"的经验，gridDim=64
gather_strided_kernel<<<64, MAX_THREAD_NUM, 0, stream>>>(
    input_device, index_device, output_device,
    input_total_length, index_total_length);
```

上述实现对小shape也使用64个物理核，每核只处理`16384 / 64 = 256`个元素，此时计算本身耗时很短，64核启动带来的调度固定开销在总耗时中占比升高。该实现在小shape下的性能数据如下：

| gridDim | Elements Per Block | Threads Per Block | Task Duration(us) |
| :-------: | :----------------: | :---------------: | :---------------: |
|    64     |        256         |        256        |       4.133       |

【正例】小shape下不直接沿用"使用全部物理核"的经验，而是设置多档核数实测，取Task Duration最低点。

```cpp
// kernel不变，仅扫描不同gridDim，用msprof 工具采集Task Duration确定最低点
// 小shape(16384)实测：4 / 8 / 16 / 32 / 64核，最低点在32核
gather_strided_kernel<<<32, MAX_THREAD_NUM, 0, stream>>>(
    input_device, index_device, output_device,
    input_total_length, index_total_length);
```

上述实现的kernel不变，仅在4到64之间扫描gridDim并逐档实测（每block线程数按每核实际元素数自适应：每核元素数≥2048时取2048，小于2048时降到实际值）。小shape下Task Duration随核数变化呈"先降后升"的趋势，最低点对应32核（物理核数的一半），而非使用全部物理核的64核：

| gridDim | Elements Per Block | Threads Per Block | Task Duration(us) | 说明 |
| :-------: | :----------------: | :---------------: | :---------------: | ---- |
|     4     |        4096        |       2048        |      10.831       | 核数不足，单核工作量重 |
|     8     |        2048        |       2048        |       6.569       | 增加核数仍有明显收益 |
|    16     |        1024        |       1024        |       4.500       | 核数偏少，单核工作量偏重 |
|  **32**   |       **512**      |      **512**      |     **3.696**     | **最优核数，Task Duration最低** |
|    64     |        256         |        256        |       4.133       | 使用全部物理核，调度固定开销超过收益 |

<img src="../../../figures/griddim_config_small_shape.png" alt="小shape(16384) Task Duration 随核数变化" width="480" />

根据Task Duration数据，4 → 8 → 16 → 32核时耗时持续下降（10.831 → 6.569 → 4.500 → 3.696us），说明小核数下单核计算量过大、增加核数仍有收益；但32 → 64核时耗时从3.696us升到4.133us，变慢约11.8%——多核场景下每个核的任务量很小、计算时间很短，启动更多核的调度开销会显著影响整体性能。因此小shape的最优核数为32，比大shape使用64核的结论要小。与大shape对照可见，"最优核数"随shape缩小而变化，数据量越小最优核数越偏离物理核上限，必须按实际shape实测，不能直接沿用大shape的经验值。

【总结】SIMT算子的gridDim配置应结合物理核数与数据规模来设置。一般原则是：

- **大shape优先匹配物理核数**：`gridDim`尽量等于物理核数，既避免`gridDim`不足导致单核工作量过重，也避免`gridDim`过多带来更多block启动和调度开销。
- **小shape需实测核数**：当数据量较小、每核工作量很小时，启动更多核带来的调度固定开销会超过有效计算收益，最优核数不一定是最大物理核数，应设置不同核数用msprof 工具采集Task Duration确定最低点。数据量越大，最优核数越靠近物理核数。
