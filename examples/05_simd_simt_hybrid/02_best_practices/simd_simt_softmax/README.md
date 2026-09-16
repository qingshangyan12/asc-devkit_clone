# SIMD与SIMT混合编程Softmax性能调优样例

## 概述

本样例以softmax为例，介绍SIMD与SIMT混合编程场景下行内归约算子的调优方法，依次使用三种优化手段：SIMT VF直接读GM建立基线、MTE搬入UB中转并配合双缓冲使搬运与计算流水重叠、将整行中间值驻留寄存器，消除行内计算的重复读取与重复计算。

## 本样例支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
|------|-------------|
| Ascend 950PR/Ascend 950DT | >= CANN 9.2.0 |

## 目录结构介绍

```text
├── simd_simt_softmax
│   ├── CMakeLists.txt                  // 编译工程文件
│   ├── data_utils.h                    // Host侧bin文件读写工具
│   ├── figures
│   │   ├── case0.png                  // Case 0 一个Warp处理一行示意图
│   │   ├── case1.png                  // Case 1 数据流向与流水组织示意图
│   │   ├── case2.png                  // Case 2 bucket分桶与UB布局示意图
│   │   └── perf_trend.png             // Case 1与Case 2不同cols下的Task Duration趋势
│   ├── scripts
│   │   ├── gen_data.py                 // 输入数据和真值数据生成脚本
│   │   └── verify_result.py            // 真值对比脚本
│   ├── softmax_gm.h                    // Case 0：SIMT直接读GM的基线实现
│   ├── softmax_ub.h                    // Case 1：UB中转与MTE双缓冲实现
│   ├── softmax_bucket.h                // Case 2：编译期bucket主路径
│   ├── softmax_host.asc                // Host侧运行入口
│   ├── README.md                       // 样例说明文档
│   └── README_en.md                    // 英文样例说明文档
```

## 样例描述

- 计算公式：

  $$
  y_{i,j} = \frac{\exp(x_{i,j} - \max_{k}{x_{i,k}})}{\sum_{k}\exp(x_{i,k} - \max_{k}{x_{i,k}})}
  $$

  其中，$x_{i,j}$ 为输入第 $i$ 行第 $j$ 列的元素，$y_{i,j}$ 为对应的输出；$\max_{k}{x_{i,k}}$ 表示取第 $i$ 行所有列元素的最大值（$k$ 遍历该行全部列），$\sum_{k}$ 表示对第 $i$ 行所有列求和。

- 计算规则：

  softmax按行独立计算，行与行之间互不依赖。对每一行：先求行内最大值，再将行内每个元素减去该最大值后做指数运算（避免溢出），并对指数结果按行求和，最后用每个指数值除以该行和，得到归一化输出。

- 样例规格：

  <table>
  <tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="4" align="center">Softmax</td></tr>
  <tr><td rowspan="2" align="center">样例输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">input</td><td align="center">[8192, 197]</td><td align="center">float16</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">样例输出</td><td align="center">output</td><td align="center">[8192, 197]</td><td align="center">float16</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">softmax_gm_kernel / softmax_ub_kernel / softmax_bucket_kernel</td></tr>
  </table>

  > **输入约束：** 样例默认shape为[8192, 197]；支持通过运行参数指定自定义shape，cols取值范围为1~512。

- 线程组织：

  实现中一个Warp处理一行，Warp内32个线程分别负责同一行中不同列的数据，行内最大值与求和通过Warp归约原语完成。以默认规格 [8192, 197] 为例，一行197列由32个线程分多轮处理：每个线程从与自身线程号相同的列开始，以32列为步进依次处理后续列；197 = 6×32 + 5，前6轮32个线程全部参与计算，第7轮仅前5个线程存在待处理数据。

## 样例实现

### Case实现说明

本样例通过3个Case构成一条优化路径，各Case的核函数、线程块数量、每个线程块的线程数和主要变化如下表所示。

| Case | 核函数                              | 线程块数量         | 每个线程块的线程数                        | 本步引入的变化                                                       |
| ---- | ----------------------------------- | ------------------ | ----------------------------------------- | -------------------------------------------------------------------- |
| 0    | softmax_gm_kernel   | 根据数据量动态计算 | 32 × warps_per_block，随rows自适应，最多2048 | SIMT VF直接读GM，一个Warp处理一行，Warp归约完成行内max和sum          |
| 1    | softmax_ub_kernel | 根据数据量动态计算 | 32 × rows_per_tile，随rows和UB容量自适应，最多2048 | 输入经MTE搬入UB中转，双份tile缓冲区轮换，搬运与计算重叠              |
| 2    | softmax_bucket_kernel | 根据数据量动态计算 | 512/1024/2048（按shape动态选择） | 编译期bucket使中间值以定长数组驻留寄存器；UB按bucket宽度padding |

其中线程块数量在Host侧根据数据量动态计算：先通过 `aclrtGetDeviceInfo(ACL_DEV_ATTR_VECTOR_CORE_NUM)` 查询硬件AIV核数，再取AIV核数与rows的较小值作为线程块数量。一个Warp处理一行：Case 0和Case 1中每个Thread Block实际启动的Warp数随rows自适应，rows不足AIV核数时每个Thread Block只处理一行；Case 2中每个Thread Block按固定行数处理数据，rows较小时部分Thread Block无行可处理、直接退出。三个Case通过编译选项 `SCENARIO_NUM` 选择，0、1、2分别对应Case 0、Case 1和Case 2，默认为2。

### 性能指标说明

| 指标                 | 说明                                                                        |
| -------------------- | --------------------------------------------------------------------------- |
| Task Duration(μs)   | Task整体耗时，包含调度到加速器的时间、加速器上的执行时间以及响应结束时间     |
| aiv_time(μs)        | Task在AI Vector Core上的理论执行时间，单位为μs                              |
| aiv_total_cycles    | Task被分配到每个AI Vector Core计算单元上后的执行cycle总数                   |
| aiv_vec_time(μs)    | vec类型指令（向量类运算指令）耗时，SIMT VF调用中的线程计算也体现在该类统计中 |
| aiv_vec_ratio       | vec类型指令的cycle数在total cycle数中的占用比                               |
| aiv_scalar_time(μs) | scalar类型指令（标量类运算指令）耗时，单位为μs                              |
| aiv_scalar_ratio    | scalar类型指令的cycle数在total cycle数中的占用比                            |
| aiv_mte2_time(μs)   | MTE2搬入指令耗时，主要对应GM到UB的搬运                                      |
| aiv_mte3_time(μs)   | MTE3搬出指令耗时，主要对应UB到GM的搬运                                      |

除Task Duration外，其余指标均为所有Thread Block上的平均值。

### Case 0：SIMT直接读GM基线

**优化目标**：以最直接的实现建立功能正确性和性能基线：Warp直接从GM读取输入，完成行内max、exp/sum和归一化，不引入任何中间缓冲。

**核心实现**：行内softmax分三轮完成，依次求行最大值、指数和与归一化输出，三轮均直接读GM。线程组织采用“一个Warp处理一行”：一个Thread Block内的多个Warp各自处理不同的行，行方向按grid-stride循环覆盖全部行。一个Warp处理一行时线程与行内数据的对应关系如下图所示。

<img src="./figures/case0.png">

```cpp
for (uint32_t row = blockIdx.x * warps_per_block + warp_in_block;  // 一个Warp处理一行
     row < rows;
     row += row_stride) {  // grid-stride循环覆盖全部行
    const uint32_t row_offset = row * cols;

    // 第一轮：从GM读取整行求最大值，asc_reduce_max在Warp内32个线程间归约。
    float max_value = -ASCRT_INF_F;
    for (uint32_t col = thread_in_warp; col < cols; col += WARP_SIZE) {
        const float value = static_cast<float>(input[row_offset + col]);
        max_value = max_value > value ? max_value : value;
    }
    max_value = asc_reduce_max(max_value);

    // 第二轮：重新读取GM，计算__expf(x - max)并用asc_reduce_add在Warp内归约求和。
    float sum_value = 0.0F;
    for (uint32_t col = thread_in_warp; col < cols; col += WARP_SIZE) {
        const float value = static_cast<float>(input[row_offset + col]);
        sum_value += __expf(value - max_value);
    }
    sum_value = asc_reduce_add(sum_value);

    // 第三轮：再次读取GM完成归一化写回。三轮循环读取整行三次、计算exp两次。
    for (uint32_t col = thread_in_warp; col < cols; col += WARP_SIZE) {
        const float value = static_cast<float>(input[row_offset + col]);
        output[row_offset + col] = static_cast<half>(__expf(value - max_value) / sum_value);
    }
}
```

**性能数据**：

| Task Duration(μs) | aiv_time(μs) | aiv_total_cycles | aiv_vec_time(μs) | aiv_vec_ratio | aiv_scalar_time(μs) | aiv_scalar_ratio | aiv_mte2_time(μs) | aiv_mte2_ratio | aiv_mte3_time(μs) | aiv_mte3_ratio |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| 13.039 | 11.513 | 18997.2 | 11.048 | 0.960 | 0.461 | 0.040 | 0.005 | 0.000 | 0.002 | 0.000 |

**分析**：

Case 0的 `aiv_vec_ratio` 达到0.960，耗时集中在SIMT线程的GM读写和行内计算上。从实现看，同一行输入在三轮循环中被重复读取三次，`exp` 被计算两次，且GM访问与计算串行在同一段SIMT线程内，无法与搬运流水重叠。下一步将数据搬运与计算拆开，由MTE把输入搬入UB中转。

---

### Case 1：UB中转与MTE双缓冲

**优化目标**：将Case 0的GM直读改为MTE搬入UB中转：SIMT VF只在UB上完成一行softmax；tile之间使用双缓冲，使搬运与计算重叠。

**核心实现**：

- 每个Thread Block按tile处理行数据。tile内的行数决定一次VF调用启动的Warp数量（一个Warp处理一行），该行数取以下三者中的最小值：本Thread Block分到的行数、UB容量内四份tile缓冲区可容纳的行数、单次VF调用线程数上限对应的64行。
- kernel将动态UB划分为双份输入tile缓冲区与双份输出tile缓冲区，两组缓冲区随tile交替使用，各自的生命周期由独立的事件管理。
- MTE2使用 `asc_copy_gm2ub_align` 二维搬运，按真实cols把GM每行搬入UB；MTE3按同样方式把结果搬回GM。双份tile缓冲区轮换，使搬运与计算重叠。
- SIMT VF仍按一个Warp一行的方式在UB上完成max、exp/sum和归一化三轮循环。

Case 1的数据流如下图所示。

<img src="./figures/case1.png">

```cpp
for (uint32_t tile_idx = 0; tile_idx < tile_count; ++tile_idx) {
    const uint32_t slot = tile_idx & 1;
    const event_t event_id = slot == 0 ? EVENT_ID0 : EVENT_ID1;
    const uint32_t row_base = first_row_base + tile_idx * tile_stride;
    const uint32_t rows_remaining = rows - row_base;
    const uint32_t rows_in_tile = rows_remaining < rows_per_tile ? rows_remaining : rows_per_tile;
    __ubuf__ half* input_tile_ub = slot == 0 ? input_tile_ub0 : input_tile_ub1;
    __ubuf__ half* output_tile_ub = slot == 0 ? output_tile_ub0 : output_tile_ub1;

    asc_sync_wait(PIPE_MTE2, PIPE_V, event_id);
    if (tile_idx + 1 < tile_count) {
        // 当前tile计算前，先把下一个tile搬入另一组输入缓冲区。
        const uint32_t next_slot = slot ^ 1;
        const event_t next_event_id = next_slot == 0 ? EVENT_ID0 : EVENT_ID1;
        const uint32_t next_row_base = row_base + tile_stride;
        const uint32_t next_rows_remaining = rows - next_row_base;
        const uint32_t next_rows_in_tile =
            next_rows_remaining < rows_per_tile ? next_rows_remaining : rows_per_tile;
        __ubuf__ half* next_input_tile_ub = next_slot == 0 ? input_tile_ub0 : input_tile_ub1;

        asc_sync_wait(PIPE_V, PIPE_MTE2, next_event_id);
        asc_copy_gm2ub_align(next_input_tile_ub, mutable_gm_ptr(input + next_row_base * cols),
            static_cast<uint16_t>(next_rows_in_tile), row_bytes, 0, 0, false,
            asc_load_l2_cache_mode::NORMAL_FIRST_VICTIM, row_bytes, padded_row_bytes);
        asc_sync_notify(PIPE_MTE2, PIPE_V, next_event_id);
    }

    asc_sync_wait(PIPE_MTE3, PIPE_V, event_id);
    asc_vf_call<softmax_ub_vf>(
        dim3(WARP_SIZE, rows_in_tile), output_tile_ub, input_tile_ub, rows_in_tile, cols, ub_stride);
    asc_sync_notify(PIPE_V, PIPE_MTE2, event_id);

    asc_sync_notify(PIPE_V, PIPE_MTE3, event_id);
    asc_sync_wait(PIPE_V, PIPE_MTE3, event_id);
    asc_copy_ub2gm_align(output + row_base * cols, output_tile_ub, static_cast<uint16_t>(rows_in_tile),
        row_bytes, asc_store_l2_cache_mode::NORMAL_FIRST_VICTIM, row_bytes, padded_row_bytes);
    asc_sync_notify(PIPE_MTE3, PIPE_V, event_id);
}
```

**性能数据**：

| Task Duration(μs) | aiv_time(μs) | aiv_total_cycles | aiv_vec_time(μs) | aiv_vec_ratio | aiv_scalar_time(μs) | aiv_scalar_ratio | aiv_mte2_time(μs) | aiv_mte2_ratio | aiv_mte3_time(μs) | aiv_mte3_ratio |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| 11.203 | 9.782 | 16140.2 | 7.235 | 0.741 | 0.521 | 0.053 | 2.878 | 0.294 | 0.587 | 0.060 |

**分析**：

Case 1把GM访问交给MTE流水并引入双缓冲，SIMT VF不再直接读GM：`aiv_mte2_time` 和 `aiv_mte3_time` 从接近0变为2.878μs和0.587μs，说明GM访问已经转移到MTE搬运路径；`aiv_vec_time` 从11.048μs降至7.235μs，下降约34.5%；`Task Duration` 从13.039μs降至11.203μs，下降约14.1%。此时 `aiv_vec_ratio` 达到0.741，是各分项中占比最高的开销，瓶颈已从GM访存转移到行内计算，下一步需要优化计算耗时。

Case 1的计算耗时主要集中在行内计算的组织方式上：行内计算按max、exp/sum、归一化三轮循环组织，每轮都重新读取一遍UB输入行，`exp` 被计算两次。消除重复读取与重复计算的直接做法，是把每个线程分到的元素缓存到局部数组中复用 `exp` 结果。但遍历数组的循环以cols为边界：cols是运行时参数，循环次数和每次迭代访问的数组下标在编译期均无法确定，编译器不能展开循环，动态下标的数组无法映射到寄存器，只能分配在栈空间，而栈空间的访问效率低于寄存器。若列数在编译期可知，循环次数和下标均为常量，编译器即可展开循环、将数组分配到寄存器，一轮读入就能完成max、exp/sum和归一化三步计算。cols取值任意，按每个取值实例化kernel并不可行；改用少量列上界（64/128/256/384/512）覆盖全部取值，并在计算中屏蔽超出真实cols的尾部列，即可同时满足编译期定长与全取值覆盖。

---

### Case 2：编译期bucket与动态线程选择

**优化目标**：消除Case 1行内计算对输入行的重复读取和对 `exp` 的重复计算。第一步先引入静态数组，把整行元素从UB缓存下来，一次读入完成max、exp/sum和归一化；第二步再设法让数组驻留寄存器，进一步降低访存开销。为此将实现收敛为编译期bucket模板：按cols上界64/128/256/384/512生成特化kernel，使定长数组 `elements[kWarpIterations]` 驻留寄存器；Host侧按shape动态选择线程数。

**核心实现**：

- 静态数组缓存整行：本样例覆盖的cols不超过512，一个Warp处理一行时，每个线程分到的元素最多为512/32 = 16个，行内数据量小，适合直接在寄存器中完成计算。为此引入静态数组 `elements[kWarpIterations]` 缓存线程分到的元素，引导编译器将其分配到寄存器，而不是每轮循环重新读取UB输入行。
- 编译期定长：cols是运行时参数，访问数组的循环边界和下标在编译期均无法确定，编译器不能把动态下标的数组映射到寄存器。将cols按上界划分为64/128/256/384/512五档bucket，按档位实例化 `softmax_bucket_kernel<kThreadCount, kBucketCols>` 模板：`kBucketCols` 为编译期常量，`kWarpIterations = kBucketCols / 32` 为定值，访问数组的循环可完全展开，`elements` 得以驻留寄存器；尾部无效列用 `-ASCRT_INF_F` 屏蔽，不参与归约与归一化。
- 一轮读入完成全部计算：整行元素一次读入 `elements` 后，max归约、exp、sum归约和归一化写回全部在寄存器上完成，输入行只读一次，`exp` 只计算一次。
- UB布局：tile缓冲区按bucket容量 `kBucketCols` 申请，MTE按运行时的真实cols搬运每行数据，真实元素之后的余量空置；SIMT VF在padded UB上计算，MTE3按真实cols将结果搬回GM。bucket分桶与UB布局如下图所示。

  <img src="./figures/case2.png">

- 输入/输出各两份UB缓冲区按tile轮换，配合事件机制管理缓冲区生命周期，使相邻tile的MTE2搬入、SIMT VF计算与MTE3搬出重叠。
- 动态线程选择：Host按 `rows_per_core = ceil(rows / block_count)` 估算每核行数，`needed_threads = rows_per_core * 32`：不超过512选512线程，不超过1024选1024线程；超过1024且 `cols <= 256` 时选2048线程；`cols > 256` 时考虑寄存器压力，SIMT VF的 `__launch_bounds__` 配置为1024，最多启动1024个线程。一个Warp处理一行，512/1024/2048线程对应每个tile最多处理16/32/64行。

SIMT VF将整行元素读入定长数组，在寄存器中完成全部计算：

```cpp
template <uint32_t kThreadCount, uint32_t kBucketCols>
__simt_vf__ __launch_bounds__(kThreadCount) inline void softmax_bucket_vf(
    __ubuf__ half* output_ub, __ubuf__ const half* input_ub, uint32_t row_count, uint32_t cols)
{
    // 每个线程分到的元素个数，为编译期常量。
    constexpr uint32_t kWarpIterations = (kBucketCols + WARP_SIZE - 1) / WARP_SIZE;
    const uint32_t row_in_tile = threadIdx.x / WARP_SIZE;
    const uint32_t thread_in_warp = threadIdx.x % WARP_SIZE;
    if (row_in_tile >= row_count) {
        return;
    }

    // tile内每行按kBucketCols的步长存放，真实数据之外的余量空置。
    const uint32_t row_offset = row_in_tile * kBucketCols;
    // 定长数组：长度和访问下标均为编译期常量，可驻留寄存器。
    float elements[kWarpIterations];

    // 一轮读入线程分到的元素，尾部无效列用-inf屏蔽，不参与后续计算。
    for (uint32_t iter = 0; iter < kWarpIterations; ++iter) {
        const uint32_t col = thread_in_warp + iter * WARP_SIZE;
        elements[iter] = col < cols ? static_cast<float>(input_ub[row_offset + col]) : -ASCRT_INF_F;
    }

    // 在寄存器上完成max，再经Warp归约得到行最大值。
    float max_value = elements[0];
    for (uint32_t iter = 1; iter < kWarpIterations; ++iter) {
        max_value = max(max_value, elements[iter]);
    }
    max_value = asc_reduce_max(max_value);

    // exp只计算一次，结果写回数组复用，并经Warp归约得到行指数和。
    float sum_value = 0.0F;
    for (uint32_t iter = 0; iter < kWarpIterations; ++iter) {
        elements[iter] = __expf(elements[iter] - max_value);
        sum_value += elements[iter];
    }
    sum_value = asc_reduce_add(sum_value);

    // 归一化写回，只处理真实cols内的列。
    for (uint32_t iter = 0; iter < kWarpIterations; ++iter) {
        const uint32_t col = thread_in_warp + iter * WARP_SIZE;
        output_ub[row_offset + col] = static_cast<half>(elements[iter] / sum_value);
    }
}
```

**性能数据**：

| Task Duration(μs) | aiv_time(μs) | aiv_total_cycles | aiv_vec_time(μs) | aiv_vec_ratio | aiv_scalar_time(μs) | aiv_scalar_ratio | aiv_mte2_time(μs) | aiv_mte2_ratio | aiv_mte3_time(μs) | aiv_mte3_ratio |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| 8.465 | 7.055 | 11640.3 | 4.520 | 0.642 | 0.467 | 0.066 | 2.885 | 0.410 | 0.589 | 0.084 |

**分析**：

Case 2相对Case 1的主要变化是“运行时cols”变为“编译期bucket上界”：定长数组的长度和下标在编译期确定，编译器将整行中间值暂存在寄存器中，输入行与 `exp` 均只处理一次。`aiv_vec_time` 从7.235μs进一步降至4.520μs，下降约37.5%；`Task Duration` 从11.203μs降至8.465μs，下降约24.4%。bucket只影响UB布局的步长，MTE仍按真实cols搬运，`aiv_mte2_time` 与Case 1基本持平；总耗时下降使 `aiv_vec_ratio` 从0.741降至0.642、`aiv_mte2_ratio` 从0.294升至0.410，行内计算与MTE2搬运的耗时占比已接近。相对Case 0，`Task Duration` 从13.039μs降至8.465μs，整体下降约35.1%。

## 性能对比总结

下面给出各Case在默认shape [8192, 197]下、Ascend 950PR上的性能数据。本样例中的性能数据基于64个AIV核的测试环境采集得到，线程块数量和线程数按该核数计算。

| Case | 优化手段                   | Task Duration(μs) | aiv_time(μs) | aiv_vec_time(μs) | aiv_vec_ratio | aiv_mte2_time(μs) | aiv_mte3_time(μs) |
| ---- | -------------------------- | :---: | :---: | :---: | :---: | :---: | :---: |
| 0    | SIMT直接读GM基线           | 13.039 | 11.513 | 11.048 | 0.960 | 0.005 | 0.002 |
| 1    | UB中转与MTE双缓冲          | 11.203 | 9.782  | 7.235  | 0.741 | 2.878 | 0.587 |
| 2    | 编译期bucket与动态线程选择 | 8.465  | 7.055  | 4.520  | 0.642 | 2.885 | 0.589 |

调优主路径Case 0 -> Case 1 -> Case 2，`Task Duration` 由13.039μs依次降至11.203μs、8.465μs，Case 2相对Case 0整体下降约35.1%。

从指标变化可以观察各阶段瓶颈的转移：Case 0的耗时集中在SIMT线程直接读写GM（`aiv_vec_ratio` 0.960，MTE接近0）；Case 1把GM访问转移到MTE2后，`aiv_vec_time` 下降34.5%，但三轮循环仍重复读取UB输入行并重复计算 `exp`；Case 2通过编译期bucket让整行中间值驻留寄存器后，`aiv_vec_time` 再下降37.5%，`aiv_vec_ratio` 降至0.642，同时 `aiv_mte2_ratio` 升至0.410，行内计算与MTE2搬运的耗时占比已接近。

固定rows为8192、逐渐增大一行数据量（cols从64到512），Case 1与Case 2的 `Task Duration` 变化如下图所示，图中竖直虚线为bucket档位边界。

<img src="./figures/perf_trend.png">

Case 1的 `Task Duration` 随cols增大持续上升：行内计算三轮串行遍历，每个线程处理的元素数与cols成正比。Case 2的增长呈阶梯状：同一bucket档位内基本持平——每线程的计算量由 `kWarpIterations` 决定、保持不变，只有MTE按真实cols搬运的数据量随cols小幅增长；跨过bucket边界时计算量跳升一档，`Task Duration` 随之跳升。全部cols区间Case 2均优于Case 1，且cols越大优势越明显，cols=512时 `Task Duration` 约为Case 1的54%；bucket上界超出真实cols较多的档位中部（如cols=96、197），Case 2需要多处理尾部无效列，相对优势小于档位边界处（如cols=128、384、512）。

## 调优建议

1. 归约类计算按归约维度组织线程：归约维度内的数据交给一个Warp，用Warp归约原语完成归约；维度之间天然并行，不需要跨Warp同步。
2. 循环边界或数组下标依赖运行时参数、且循环次数较少时，可将参数按少量上界分档并固化为编译期常量，使循环完全展开、定长数组驻留寄存器，避免重复读取输入和重复计算。

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。

- 配置环境变量

  请参考[环境变量配置](../../../../docs/zh/quick_start.md#cann-env-setup)，配置当前环境上的CANN开发套件包环境变量。

  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **说明：** `${install_path}` 为CANN包安装目录，未指定安装目录时默认安装至 `/usr/local/Ascend` 下。
  >
- 样例执行

  在本样例目录下执行如下命令，默认执行shape为[8192, 197]的样例。

  ```bash
  SCENARIO_NUM=2                                                                # 选择执行场景，可选0-2
  mkdir -p build && cd build;                                                   # 创建并进入build目录
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=$SCENARIO_NUM ..;make -j;  # 编译工程
  python3 ../scripts/gen_data.py                                                # 生成输入数据和真值数据
  ./softmax                                                                     # 执行样例
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin       # 校验真值数据
  ```

  也支持自定义shape：先执行 `python3 ../scripts/gen_data.py <rows> <cols>`，再执行 `./softmax <rows> <cols>`，其中cols取值范围为1~512。

- 编译选项说明

  | 选项                        | 可选值       | 说明                                                       |
  | --------------------------- | ------------ | ---------------------------------------------------------- |
  | CMAKE_ASC_ARCHITECTURES | dav-3510 | NPU架构：本样例仅支持dav-3510（Ascend 950PR/Ascend 950DT） |
  | SCENARIO_NUM            | 0-2    | 样例类型，默认为2：0=SIMT直接读GM，1=UB中转与MTE双缓冲，2=编译期bucket |

  执行结果如下，说明精度对比成功。

  ```text
  [Success] Case accuracy verification passed.
  ```

## 性能调试

### msOpProf工具介绍

msOpProf工具是单算子性能分析工具。包含msopprof和msopprof simulator两种使用方式。该工具协助用户定位算子内存、算子代码以及算子指令的异常，实现全方位的算子调优。当前支持基于不同运行模式（上板或仿真）和不同文件形式（可执行文件或算子二进制.o文件）进行性能数据的采集和自动解析。

通过上板性能采集，可以直接测定算子在昇腾AI处理器上的运行时间。该方式适合在板环境中快速定位算子性能问题。

基于可执行文件softmax通过msopprof执行算子调优：

```bash
msopprof ./softmax
```

命令完成后，会在默认目录下生成以“OPPROF_{timestamp}_XXX”命名的性能数据文件夹，文件夹结构示例如下：

```text
├──dump                       # 原始的性能数据，用户无需关注
├──ArithmeticUtilization.csv  # cube/vector指令cycle占比
├──L2Cache.csv                # L2 Cache命中率，影响MTE2，建议合理规划数据搬运逻辑，增加命中率
├──Memory.csv                 # UB，L1和主存储器读写带宽速率
├──MemoryL0.csv               # L0A，L0B，和L0C读写带宽速率
├──MemoryUB.csv               # Vector和Scalar到UB的读写带宽速率
├──OpBasicInfo.csv            # 算子基础信息
├──PipeUtilization.csv        # 采集计算单元和搬运单元耗时和占比
├──ResourceConflictRatio.csv  # UB上的bank group、bank conflict和资源冲突率在所有指令中的占比
└──visualize_data.bin         # MindStudio Insight呈现文件
```

查看具体的性能分析结果：

```bash
# 查看Task Duration 以及各项数据
cat ./OPPROF_*/PipeUtilization.csv
```
