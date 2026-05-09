# ND2NZ Bank 冲突样例计划

## Summary
实现一个 `ND2NZ` 随路转换样例，用 `SCENARIO=1/2` 对比 `dstNzC0Stride` 的 bank group 分布差异，展示 UB bank 写写冲突与规避方式。实现必须采用静态 tensor 编程风格，参考 best practices 目录下的 `00/01/02`，不要使用 `TPipe/TQue`。输入规模为 `8192x8192 half`，按 `256x256` tile 切分，共 `32x32 = 1024` 个 tile，分配给 `48` 个核并行处理。

## Key Changes
- tile 固定为 `256x256 half`，单 tile 大小 `256 * 256 * 2 = 128KB`，可放入 `192KB` UB，且 `256` 为 `16` 的倍数，天然对齐 NZ/C0 分块。
- 总 tile 数为 `1024`。按 `core_id` 均分任务：
  - `base = 1024 / 48 = 21`
  - `remain = 1024 % 48 = 16`
  - 前 `16` 个核各处理 `22` 个 tile
  - 后 `32` 个核各处理 `21` 个 tile
- tile 坐标和输入偏移：
  ```text
  tile_row = tile_id / 32
  tile_col = tile_id % 32
  src_offset = tile_row * 256 * 8192 + tile_col * 256
  ```
- `Nd2NzParams` 统一配置：
  ```cpp
  params.ndNum = 1;
  params.nValue = 256;
  params.dValue = 256;
  params.srcNdMatrixStride = 0;
  params.srcDValue = 8192;
  params.dstNzNStride = 1;
  params.dstNzMatrixStride = 0;
  ```
- `dstNzC0Stride` 作为对照参数：
  - `SCENARIO=1`: `dstNzC0Stride = 256`
  - `SCENARIO=2`: `dstNzC0Stride = 257`
- `SCENARIO=1` 作为冲突组，`SCENARIO=2` 作为优化组；两者只改变 `dstNzC0Stride`，其余流程一致。

## Core Diagrams

### Bank 结构和地址映射

| 参数 | 值 |
|------|-----|
| UB 总大小 | 192KB |
| Bank 数 | 48 |
| Bank 大小 | 4KB |
| Bank group 数 | 16 |
| Bank group 构成 | `bank_i, bank_{i+16}, bank_{i+32}` |

```text
bank_id       = (addr / 32B) % 48
bank_group_id = bank_id % 16

地址相差 512B  = 16 * 32B  -> 同一 bank_group
地址相差 1536B = 48 * 32B  -> 同一 bank
```

### NZ/C0 分形块

```text
half 类型下：

1 个 C0 行        = 16 个 half = 32B
1 个 NZ 分形块    = 16 行 * 32B = 512B
1 个 256x256 tile = 16 * 16 个 NZ 分形块 = 128KB
```

### 关键误解：ND2NZ 不是一次性写连续 512B

```text
错误理解：ND2NZ 一次写入一个连续 512B 分形块

    [================ 512B ================]
     bank0 bank1 bank2 ... bank15

如果真是连续写，512B 会自然铺到 16 个 bank 上。


实际行为：ND2NZ 按 C0 行跳写，每次写 32B

    C0行0 -> addr = 0 * dstNzC0Stride * 32B
    C0行1 -> addr = 1 * dstNzC0Stride * 32B
    C0行2 -> addr = 2 * dstNzC0Stride * 32B
    ...

相邻 C0 行的目的地址间隔由 dstNzC0Stride 决定。
```

### SCENARIO=1：`dstNzC0Stride=256` 产生冲突

```text
stride = 256 C0_SIZE = 256 * 32B = 8192B

C0行0  addr = 0 * 256 * 32B  -> bank_id = 0              -> bank_group_0
C0行1  addr = 1 * 256 * 32B  -> bank_id = 256 % 48 = 16  -> bank_group_0
C0行2  addr = 2 * 256 * 32B  -> bank_id = 512 % 48 = 32  -> bank_group_0
C0行3  addr = 3 * 256 * 32B  -> bank_id = 768 % 48 = 0   -> bank_group_0
...

bank group 视角：

bank_group_0   ████████████████████████████  <- C0 行集中写入
bank_group_1
bank_group_2
bank_group_3
...
bank_group_15

结论：256 % 16 == 0，相邻 C0 行始终回到同一个 bank group，容易触发写写冲突。
```

### SCENARIO=2：`dstNzC0Stride=257` 打散 bank group

```text
stride = 257 C0_SIZE = 257 * 32B = 8224B

C0行0  addr = 0 * 257 * 32B   -> bank_group_0
C0行1  addr = 1 * 257 * 32B   -> bank_group_1
C0行2  addr = 2 * 257 * 32B   -> bank_group_2
C0行3  addr = 3 * 257 * 32B   -> bank_group_3
...
C0行15 addr = 15 * 257 * 32B  -> bank_group_15

bank group 视角：

bank_group_0   ██
bank_group_1   ██
bank_group_2   ██
bank_group_3   ██
...
bank_group_15  ██

结论：257 % 16 == 1，相邻 C0 行按 bank group 轮转，写入更均匀。
```

### 48 核 Tile 分配图

```text
8192x8192 half
  = 32 * 32 个 256x256 tile
  = 1024 个 tile

tile_id:

row0:   0    1    2  ...  31
row1:  32   33   34  ...  63
...
row31: 992  993  994 ... 1023

48 核均分：

core  0 ~ 15: 每核 22 个 tile
core 16 ~ 47: 每核 21 个 tile

每个核处理自己的连续 tile_id 区间，tile 内部始终执行固定 256x256 ND2NZ。
```

## Implementation Changes
- 单核流程保持固定，且使用静态 `LocalTensor` 直接管理 UB：
  1. 计算当前核负责的 tile 范围
  2. 计算 tile 的 GM 源地址偏移
  3. 在 UB 中先 `Duplicate` 清零，再执行 `GM -> UB` 的 ND2NZ 搬运；当前实现采用 `255 + 1` 的补尾方式，先搬 255 行，再单独补最后 1 行
  4. 将 UB 中 NZ 原始布局写回 GM
  5. 循环处理下一个 tile
- 输出按 tile 连续存放，验证直接比对 NZ 格式数据。
- 输出 GM 偏移按场景分别计算：
  ```text
  d_c0 = 256 / 16 = 16
  tile_out_c0_blocks = (d_c0 - 1) * dstNzC0Stride + 256
  tile_out_elements = tile_out_c0_blocks * 16
  dst_offset = tile_id * tile_out_elements
  ```
- 文档中明确说明：
  - ND2NZ 不是一次性写连续 512B，而是按 C0 行跳写
  - `dstNzC0Stride` 的单位是 `C0_SIZE(32B)`
  - `257` 只是用于演示 bank group 分散的实验参数，不是标准 NZ 紧凑布局要求

## Test Plan
- 使用固定随机种子 `2026` 生成 `8192x8192 half` 输入数据，取值范围为 `[-128, 128]`，避免全 1 数据掩盖地址映射错误。
- 分别运行 `SCENARIO=1` 和 `SCENARIO=2`。
- 为两个场景分别生成对应 golden，按 NZ 原始布局逐字节比对。
- 如果看性能，优先查看 `./PROF_*/mindstudio_profiler_output/op_summary_*.csv` 里的 `aiv_mte2_time(us)`、`aiv_mte2_ratio`、`aiv_mte2_wait_ratio`、`aiv_gm_to_ub_bw(GB/s)`、`aiv_mte2_active_bw(GB/s)`，再结合 `ResourceConflictRatio.csv` 看 `aiv_vec_bankgroup_cflt_ratio` 和 `aiv_vec_bank_cflt_ratio`。

## Assumptions
- 使用 `48` 个核并行，核号范围为 `[0, 47]`。
- 不做尾块处理，因为 `8192` 可被 `256` 整除。
- `dstNzC0Stride=257` 仅用于样例内展示 bank conflict 规避，不要求作为后续通用 NZ 布局参数复用。

## Debug Notes
- 已用固定随机种子 `2026` 生成输入，输入内容不是全 1；脚本会打印前 16 个元素，便于复现。
- 已验证 `SCENARIO=1/2` 的功能正确性，`output/output_s1.bin`、`output/output_s2.bin` 与对应 golden 均通过 `verify_result.py`。
- 当前实现已切到静态 tensor 写法，核心是一个 UB `LocalTensor`，先 `Duplicate` 清零，再做两段 `ND2NZ` 搬运，避免空洞位置残留旧值。
- 最终可稳定通过验证的实现是：每个 tile 先搬 255 行，再补最后 1 行，然后整体写回 GM；单次搬 256 行时，最后一行会偏。
- 已跑过两种 profiling 方式：
  - `msprof op ./demo`
  - `msprof op --launch-count=10 ./demo`
- 常用结果目录：
  - `./PROF_*/mindstudio_profiler_output/op_summary_*.csv`
  - `./OPPROF_*/ResourceConflictRatio.csv`
  - `./OPPROF_*/Memory.csv`
  - `./OPPROF_*/PipeUtilization.csv`
- `op_summary_*.csv` 里重点关注过这些 MTE2 相关列：
  - `aiv_mte2_time(us)`
  - `aiv_mte2_ratio`
  - `aiv_mte2_wait_ratio`
  - `aiv_gm_to_ub_bw(GB/s)`
  - `aiv_mte2_active_bw(GB/s)`
- `ResourceConflictRatio.csv` 里看过：
  - `aiv_vec_bankgroup_cflt_ratio`
  - `aiv_vec_bank_cflt_ratio`
  - `aiv_mte2_wait_ratio`
  - `aiv_mte3_wait_ratio`
- `SCENARIO=2` 的 raw NZ 布局包含空洞，`tileLocal` 需要先 `Duplicate` 清零，再做 ND2NZ 两段搬运，否则空洞位置会残留旧数据并导致 `verify_result.py` 失败。
- 当前结论：
  - `aiv_vec_bankgroup_cflt_ratio` 和 `aiv_vec_bank_cflt_ratio` 均为 `0`
  - `SCENARIO=1/2` 的 `aiv_mte2_time` 差异很小，且多次采样后没有稳定拉开
  - 把 kernel 内部人为重复 `8` 次虽然能放大耗时，但不符合样例语义，已撤回
- 目前这版样例更像“ND2NZ 参数/布局示例”，还不足以稳定展示 bank conflict 的性能差异；后续如果继续做，需要重新设计能直接放大 MTE2 冲突的样例形态，而不是在 kernel 里叠加重复次数。

## Performance Profiling
- 常用采集命令：
  ```bash
  msprof op ./demo
  msprof op --launch-count=10 ./demo
  msprof ./demo
  ```
- 常用结果目录：
  - `./PROF_*/mindstudio_profiler_output/op_summary_*.csv`
  - `./PROF_*/mindstudio_profiler_output/`
- 常用查看方式：
  ```bash
  cat ./PROF_*/mindstudio_profiler_output/op_summary_*.csv
  cat ./OPPROF_*/ResourceConflictRatio.csv
  cat ./OPPROF_*/Memory.csv
  cat ./OPPROF_*/PipeUtilization.csv
  ```
- MTE2 侧重点：
  - `aiv_mte2_time(us)`
  - `aiv_mte2_ratio`
  - `aiv_mte2_wait_ratio`
  - `aiv_gm_to_ub_bw(GB/s)`
  - `aiv_mte2_active_bw(GB/s)`
- 冲突侧重点：
  - `aiv_vec_bankgroup_cflt_ratio`
  - `aiv_vec_bank_cflt_ratio`
  - `aiv_mte2_wait_ratio`
  - `aiv_mte3_wait_ratio`
