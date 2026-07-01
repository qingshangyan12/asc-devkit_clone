# asc_atomic_add 原子加性能样例

## 概述

本样例以 `asc_atomic_add()` 原子加为例，在相同原子加总次数下，展示内存层级（Global Memory / Unified Buffer）、数据类型（int32 / int64）、是否使用返回值、以及计数器在内存中的排布密度对原子加性能的影响，并给出相应的优化指导。程序本身只输出场景信息和正确性校验结果，性能数据统一通过外部 Profiling 工具（`msprof op`）采集。

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 支持的CANN软件版本

- \>= CANN 9.1.0

## 目录结构介绍

```text
├── atomic_add_perf
│   ├── CMakeLists.txt         // 样例构建脚本
│   ├── atomic_add_perf.asc    // 原子加性能workload实现 & 调用样例
│   ├── README.md
│   └── README_en.md
```

## 样例描述

本样例通过 `SCENARIO_NUM` 选择一个待测场景，每次运行只启动一个目标 kernel，便于外部 Profiling 工具采集单场景性能数据。8 个场景覆盖 4 组对照维度：

| 场景编号 | 内存位置 | 数据类型 | 是否使用返回值 | 对照维度 |
|:---:|:---:|:---:|:---:|---|
| 1 | Global Memory | int32_t | 是 | GM vs UB、返回值、数据类型基准 |
| 2 | Global Memory | int32_t | 否 | 返回值开销对照（对场景1） |
| 3 | Global Memory | int64_t | 是 | 数据类型对照（对场景1） |
| 4 | Global Memory | int64_t | 否 | 返回值开销对照（对场景3） |
| 5 | Unified Buffer | int32_t | 是 | 内存层级对照（对场景1） |
| 6 | Unified Buffer | int32_t | 否 | 返回值开销对照（对场景5） |
| 7 | Global Memory | int32_t | 是 | slot 按 `SLOT_STRIDE` 间距排布，cacheline 伪共享扫描（int32） |
| 8 | Global Memory | int64_t | 是 | slot 按 `SLOT_STRIDE` 间距排布，cacheline 伪共享扫描（int64） |

- 计算逻辑：
  - GM 场景：GM counter 数组长度为 `SLOTS`（默认 1024），每个线程根据 `global_tid % SLOTS` 选择一个 slot 执行原子加；host 侧逐 slot 校验计数并校验总和等于 `TOTAL_ATOMIC_ADDS`。
  - UB 场景：每个 ThreadBlock 内使用长度为 1024 的 UB counter 数组，每个线程根据 `global_tid % 1024` 选择一个 slot 执行原子加，kernel 结束前将每个 Block 的 1024 个 slot 写回 GM，host 侧逐 slot 校验。
  - 带返回值场景：每次 `asc_atomic_add()` 返回的旧值写入线程本地 `volatile` 变量，防止返回值路径被编译器优化掉，同时避免引入额外累加和 GM 写回。
  - 场景 7/8：逻辑分别与场景 1、场景 3 完全相同（带返回值），仅把相邻 slot 在 GM 中拉开 `SLOT_STRIDE` 个本类型元素，用于扫描 cacheline 打包密度对串行化的影响。

- 原子加总次数：

  ```text
  TOTAL_ATOMIC_ADDS = BLOCK_NUM * THREADS_PER_BLOCK * ITERATIONS
  ```

## 样例实现

### 性能指标说明

| 指标 | 说明 |
|------|------|
| Task Duration（μs） | Task 整体耗时，来自 `msprof op` 采集的 `OpBasicInfo.csv`，包含调度到加速器、加速器上执行、以及响应结束的时间 |
| 平均耗时（ns/atomic） | `Task Duration / TOTAL_ATOMIC_ADDS`，单笔原子加的均摊代价 |

> **采集配置说明：** 下文除特别注明的基准表外，性能数据统一采用 `BLOCK_NUM=48`、`THREADS_PER_BLOCK=256`、`ITERATIONS=256`、`SLOTS=1024`（`total_atomic_adds=3145728`），采集命令为 `msprof op --launch-count=1 ./atomic_add_perf`，采集环境为 dav-3510、Device Freq 1650 MHz。默认 workload 已在源码中调小为 `8×64×256=131072`，以保证 `msprof op` 采集稳定快速（详见「Profiling 采集」）。

### Case 1：内存层级——GM 原子加 vs UB 原子加

**样例目标**：相同原子加总次数下，对比原子操作打在 Global Memory 与 Unified Buffer 上的性能差异。

**核心实现**：通过 `SCENARIO_NUM` 取值 1（GM int32 带返回值）与 5（UB int32 带返回值）分别启动对应 kernel，两者原子加总次数、slot 映射一致。

**性能数据**：

| 场景 | 内存 | Task Duration（μs） | 平均耗时（ns/atomic） |
|:---:|:---:|---:|---:|
| 1 | Global Memory | 22344.60 | 7.103 |
| 5 | Unified Buffer | 59.58 | 0.019 |

**分析**：

- 相同的原子加总次数下，UB 场景 Task Duration 约 60μs，GM 场景高达约 22300μs，相差约 **375 倍**。
- GM 原子 read-modify-write 在 L2 原子单元上执行、访问延迟高；`PipeUtilization` 显示 `aiv_vec_ratio ≈ 0.9999`，即 vector 单元几乎一直"忙"，但实际是在原子重试/等待。UB 原子在 AIV（vector core）内部执行，访问延迟低一到两个数量级。

**结论**：原子加应尽量收敛到更靠近计算单元的内存层级。能在 UB（block 内私有）完成的原子累加，不要放到 GM 上做。

> **说明：** GM 场景按 `global_tid % 1024` 分散到 1024 个 GM 地址；UB 场景是每个 ThreadBlock 内部各自维护 1024 个 UB slot。两类场景竞争范围不同，**不能作为同竞争范围下的绝对性能对比**，仅反映"原子加打在不同内存层级"的整体代价量级。

---

### Case 2：返回值开销

**样例目标**：对比 `asc_atomic_add()` 使用返回值（读回旧值）与丢弃返回值的性能差异。

**核心实现**：通过 `SCENARIO_NUM` 取值 1/2（GM int32）与 3/4（GM int64）成对对照，成对场景仅"是否消费返回值"不同。

**性能数据**：

| 数据类型 | 带返回值（μs） | 不带返回值（μs） | 带/不带比值 |
|:---:|---:|---:|:---:|
| int32（场景1 vs 2） | 22344.60 | 136.74 | 163.4× |
| int64（场景3 vs 4） | 11239.15 | 11281.08 | 0.996× |

**分析**：

- **int32**：不带返回值场景仅 137μs——`L2Cache.csv` 显示其 L2 写事务为 0，硬件把单线程 256 次连续累加在本地聚合掉了，不逐笔过 L2；一旦要返回旧值，就退回到 65536 笔逐笔串行 RMW，竞争才被暴露，因此带返回值慢 100 倍以上。
- **int64**：无此本地聚合快路径，带/不带返回值都走完整 RMW，两者基本持平。

**结论**：只在业务确实需要旧值时才使用原子加返回值。对于纯计数场景，丢弃返回值可能触发硬件的本地聚合快路径（如 int32），获得数量级收益。

---

### Case 3：数据类型——int32 与 int64（伪共享陷阱）

**样例目标**：对比 int32 与 int64 原子加的性能，并解释一个反直觉现象。

**核心实现**：通过 `SCENARIO_NUM` 取值 1（GM int32 带返回值）与 3（GM int64 带返回值）对照，两者竞争映射完全相同（12288 线程 `% 1024`，每 slot 12 线程），slot 均按元素紧密排布。

**性能数据**：

| 数据类型 | Task Duration（μs） | 每 128B cacheline 容纳 slot 数 | 同 line 排队线程数 |
|:---:|---:|:---:|:---:|
| int32（场景1） | 22344.60 | 128B/4B = 32 | 32 × 12 = 384 |
| int64（场景3） | 11239.15 | 128B/8B = 16 | 16 × 12 = 192 |

**分析**：

- 反直觉：窄类型 int32 反而比宽类型 int64 慢约 **2 倍**。对比两者 metric，事务笔数、字节数、指令数完全相同，唯一差 2 倍的是 `aiv_total_cycles`（≈36.0M vs ≈18.0M，干净的 2.0×）。
- 根因是 **cacheline 伪共享串行化**：GM 原子 RMW 以一条 **128B cacheline** 为串行化粒度，同一条 line 上的所有原子操作排一条队逐笔执行——哪怕是两个独立地址。int32 每条 line 挤进 32 个计数器、int64 只挤 16 个，i32 的 line 内密度恰为 i64 的 2 倍，队列长一倍，周期翻倍。
- 这与"int32 这个类型本身"无关，纯粹是排布密度差异。Case 4 用 stride 扫描证实了这一点。

**结论**：不要凭"类型更窄应更快"的直觉给高竞争 GM 计数器选 int32。紧密布局下窄类型每条 cacheline 打包更密，竞争反而更重。

---

### Case 4：计数器排布密度——cacheline 伪共享扫描

**样例目标**：通过拉开相邻计数器在 GM 中的间距，验证 GM 原子的串行化粒度，并给出消除伪共享的方法。

**核心实现**：场景 7（int32）/场景 8（int64）逻辑与场景 1/3 完全相同（带返回值、每 slot 12 线程真竞争），仅把相邻 slot 拉开 `SLOT_STRIDE` 个本类型元素，只改变 cacheline 打包密度、不改变真竞争量。

**性能数据**（48×256×256，扫 `SLOT_STRIDE`）：

| `SLOT_STRIDE` | 每 128B line 计数器数 | int32 间距 | int32（μs） | int64 间距 | int64（μs） | 偏差 |
|:---:|:---:|:---:|---:|:---:|---:|:---:|
| 1 | 32（仅 i32） | 4B | 22226.78 | — | — | — |
| 2 | 16 | 8B | 11248.16 | 8B | 11291.87 | 0.4% |
| 4 | 8 | 16B | 5552.79 | 16B | 5548.53 | 0.1% |
| 8 | 4 | 32B | 3000.70 | 32B | 3006.00 | 0.2% |
| 16 | 2 | 64B | 1526.90 | 64B | 1531.57 | 0.3% |
| 32 | 1（独占 128B line） | 128B | 1050.29 | 128B | 1059.55 | 0.9% |

**分析**：

- **耗时只取决于"每条 128B line 装几个计数器"，与数据类型完全无关。** 把横轴从 stride 换成"每 128B line 计数器数"后，int32 与 int64 两条曲线处处重合（偏差 < 1%）。
- **串行化粒度实测为 128B**：间距每翻一倍耗时几乎干净减半，直到每计数器独占一条 128B line 时触底（i32 stride=32→1050μs，i64 stride=16→1060μs）；再拉一倍到 256B 仅降约 3%，证实粒度就是一整条 128B cacheline。
- 耗时线性正比于每 line 计数器数 n，拟合 `T ≈ 275 + 686 × n`。
- 从紧密排布到 128B 隔离，int32 提速约 **21 倍**（1050 vs 22344μs），比 int64 紧密排布还快约 **10.7 倍**。

**结论**：高竞争 GM 原子计数器应按 cacheline 对齐——让每个计数器独占 128B（或填充到 ≥128B 间距），可彻底消除伪共享串行化。

> **UB 无需 cacheline 对齐：** 补充实测表明，UB 上的 `asc_atomic_add` 不存在 cacheline 伪共享，且对真竞争也基本不敏感（同一 block 内 256 线程全砸同一 UB 地址，仅比无竞争慢约 2%）。伪共享规避手段是 GM（L2 原子串行化）专属问题，搬到 UB 上强行按 128B 填充只会白白放大 UB 占用。详见 `PROFILING_REPORT.md` §7 与 `CACHELINE_FALSE_SHARING_CUDA.md`。

---

## 性能对比总结

| 优化维度 | 关键对照 | 收益量级 | 建议 |
|---|---|---|---|
| 内存层级 | UB（60μs） vs GM（22344μs） | ~375× | 原子累加尽量收敛到 UB 等靠近计算单元的层级 |
| 返回值 | int32 丢弃返回值（137μs） vs 带返回值（22344μs） | ~163× | 不需要旧值时丢弃返回值，可触发本地聚合快路径 |
| cacheline 对齐 | int32 128B 独占（1050μs） vs 紧密排布（22344μs） | ~21× | 高竞争 GM 计数器按 128B cacheline 对齐/填充 |
| 数据类型 | 同排布密度下 int32 ≈ int64 | ~1× | 类型选择不影响原子性能，密度才是关键 |

## 调优建议

1. **优先降低原子操作的内存层级**：GM 原子在 L2 上串行执行、延迟高，UB 原子在计算核内执行、快两到三个数量级。能在 block 内私有内存完成的原子累加不要放到 GM。
2. **纯计数场景丢弃返回值**：读回旧值会使部分类型（如 int32）退出本地聚合快路径，退回逐笔串行 RMW，代价可能相差百倍以上。
3. **高竞争 GM 计数器按 128B cacheline 对齐**：GM 原子以 128B cacheline 为串行化粒度，落在同一条 line 上的独立计数器会互相排队（伪共享）。为每个计数器填充到独占 128B 可获得约 20 倍收益。
4. **不要以数据类型宽窄推断原子性能**：性能取决于每条 cacheline 的计数器打包密度，而非类型本身。紧密布局下窄类型密度更高，竞争反而更重。

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。

- 配置环境变量

  请根据当前环境上CANN开发套件包的[安装方式](../../../../../../docs/quick_start.md#prepare&install)，配置环境变量。

  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **说明：** `${install_path}` 为CANN包安装目录，未指定安装目录时默认安装至`/usr/local/Ascend`下。

- 样例执行

  编译并运行场景1：

  ```bash
  mkdir -p build && cd build
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=1 ..
  make -j
  ./atomic_add_perf
  ```

  编译选项说明：

  | 选项 | 可选值 | 说明 |
  |------|--------|------|
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU架构：本样例仅支持dav-3510（Ascend 950PR/Ascend 950DT） |
  | `SCENARIO_NUM` | `1`-`8` | 待测场景编号 |
  | `BLOCK_NUM` | 正整数 | ThreadBlock数量 |
  | `THREADS_PER_BLOCK` | 正整数 | 每个ThreadBlock中的线程数 |
  | `ITERATIONS` | 正整数 | 每个线程执行的原子加次数 |
  | `SLOT_STRIDE` | 正整数 | 相邻slot的元素间距，仅场景7/8生效 |
  | `SKIP_VALIDATION` | `ON`/`OFF` | 是否跳过结果校验，默认`OFF`。使用`msprof op`采集GM场景时建议设为`ON` |

  执行结果如下，说明正确性校验通过。

  ```text
  Scenario 1: GM int32 with return value
  Workload: blocks=8 threads_per_block=64 iterations=256 total_atomic_adds=131072
  Validation passed
  ```

## Profiling 采集

程序不内置计时逻辑。性能数据请使用 `msprof op` 采集单场景运行结果。为避免多个 kernel 混在同一份数据中，建议每次只配置一个 `SCENARIO_NUM` 并单独运行；采集 GM 场景时建议打开 `SKIP_VALIDATION`：

```bash
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=1 -DSKIP_VALIDATION=ON ..
make -j
msprof op --launch-count=1 ./atomic_add_perf
```

完整采集流程、各场景原始数据、metric 分析（含 cacheline 伪共享的 stride 扫描与 UB 对照实验）见同目录 `PROFILING_REPORT.md`。

> **规模与超时说明：** `msprof op` 为采集多种 metric 会对同一 kernel 做 warmup 并多次 replay。若单次 kernel 耗时过长（超过 stream 同步超时，默认 10s），replay 会失败并报 `Stream synchronize timeout`。各场景敏感度差异很大：
> - **GM 场景（1-4、7-8）** 按 `global_tid % SLOTS` 分散竞争，调大 workload 后仍可能因原子竞争触发较长耗时。需要 `msprof op` 采集时建议 `total_atomic_adds` 不超过约 2000 万，`BLOCK_NUM` 也不宜过大（动态插桩对 sub block 总数有限制，过大时会有 `sub blocks exceeding 108` 告警导致部分插桩数据丢失）。
> - **UB 场景（5-6）** 通常明显快于 GM，常规调参基本不会触及超时。
>
> 因此默认 workload 已设为较小的 `total_atomic_adds=131072`。仅做**裸跑压测**（不挂 msProf）时可自由调大。

> **关于 GM 场景的 Validation failed：** GM 场景的 counter slots 仅在 host 侧分配时清零一次，kernel 只累加、不在入口清零。`msprof op` 的 warmup+replay 会在同一块 GM 内存上重复执行 kernel，counter 被累加多次，因此严格校验模式下会报 `Validation failed`（计数为期望值的整数倍）。这是 replay 机制与校验逻辑的固有冲突，**不影响 profiling 数据采集**，metric 仍正常落盘。采集 GM 场景时建议通过 `-DSKIP_VALIDATION=ON` 跳过校验。UB 场景的 counter slots 在 kernel 入口自清零，对 replay 幂等，校验仍 passed。
