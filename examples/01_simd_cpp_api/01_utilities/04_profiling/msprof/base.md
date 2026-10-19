# msOpProf 单算子性能分析工具使用讲解

## 一、工具定位

msOpProf 是 CANN 提供的**单算子**性能分析工具，命令行入口 `msopprof`。它有两种形态：

| 形态 | 命令 | 用途 |
|---|---|---|
| 上板采集 | `msopprof ./demo` | 在真实昇腾处理器上跑，读硬件 PMU 计数器，数据真实 |
| 仿真采集 | `msopprof simulator --soc-version=<VERSION> ./demo` | 在指令仿真器上跑，无需硬件，可拿到指令级流水 |

本文验证环境的版本（`msopprof -v`）：

```
msopprof vesion is 26.1.0-a9b9e9ea5c701cc8939017999fdd02c2e15905c2
msopscommon version is d57576307447c2558540b64c4b67af8d138a92f1
```

工具路径 `${install_path}/cann-9.2.0/tools/msopprof/bin/msopprof`，通过 `bin/msopprof` 软链接暴露。使用前需要 `source ${install_path}/cann/set_env.sh`，且必须是 CANN 商用版或社区版（社区精简包不带此工具）。

---

## 二、参数详解（上板模式）

### 2.1 基础参数

| 参数 | 默认值 | 说明 |
|---|---|---|
| `-v, --version` | — | 打印版本 |
| `-h, --help` | — | 打印帮助 |
| `--application=<FILE>` | — | 可执行文件路径。等价于直接把路径写在命令末尾 |
| `--output=<PATH>` | `./` | 输出目录。结果落在该目录下的 `OPPROF_{timestamp}_{随机串}/` |
| `--config=<FILE>` | — | 用 JSON 配置文件描述算子。**用了 config 模式后 `--kernel-name` 和 `--soc-version` 失效** |
| `--custom-input=<FILE>` | — | JSON 文件，指定算子自定义输入 |

### 2.2 采集范围控制

| 参数 | 默认值 | 说明 |
|---|---|---|
| `--kernel-name=<NAME>` | — | 只采集指定名字的 kernel。config 模式下不生效 |
| `--launch-count=<LIMIT>` | **1** | 采集多少个 kernel。范围 1–5000 |
| `--launch-skip-before-match=<SKIP>` | **0** | 开始分析前跳过多少次 kernel launch。范围 0–1000 |
| `--warm-up=<TIMES>` | **5** | 正式采集前空跑几次预热。范围 0–500 |
| `--kill=<SWITCH>` | `off` | 采满 `launch-count` 后是否直接杀掉算子进程 |

这三个参数配合起来用：一个程序里循环调 100 次 kernel，想采第 51 次那一发，就是 `--launch-skip-before-match=50 --launch-count=1`。

**`--warm-up` 默认是 5，不是 0** —— 这点容易被忽略。不传参数时工具已经在预热了。原因是首轮执行 ICache 冷、L2/DCache 冷、kernel 首次加载有额外开销、频率可能还没爬到额定值，测出来的数会明显偏大且波动大。只有当你**就是想看冷启动开销**（比如评估 ICache miss 代价）时才该 `--warm-up=0`。

### 2.3 采集内容控制

| 参数 | 默认值 | 说明 |
|---|---|---|
| `--aic-metrics=<TYPE>` | `Default` | 指定采集能力项，见下表 |
| `--instr-timeline-pipe=<PIPE>` | — | 指定 instr timeline 的流水。**仅当 `--aic-metrics=InstrTimeline` 时生效** |
| `--replay-mode=<MODE>` | `kernel` | 重放粒度：`application` / `kernel` / `range` |
| `--mstx=<SWITCH>` | `off` | 是否启用 mstx 打点 API |
| `--mstx-include=<RANGE>` | — | 指定要采集的 mstx range |

`--aic-metrics` 可选值：

```
ArithmeticUtilization | MemoryUB | Memory | MemoryL0 | L2Cache
PipeUtilization | ResourceConflictRatio | Default | BasicInfo | Roofline
Occupancy | TimelineDetail | KernelScale | Source | MemoryDetail
PCSampling | PipeTimeline | InstrTimeline
```

**关于 `--replay-mode`：** 硬件 PMU 计数器数量有限，一次执行采不完所有指标，工具需要**重放 kernel** 多轮、每轮采一组计数器。这就是为什么下面例子里 kernel 只调了一次，`dump/` 里却有 6 份 `DeviceProf*.bin`。

### 2.4 仿真模式参数差异

`msopprof simulator` 的参数集不一样，独有的部分：

| 参数 | 默认值 | 说明 |
|---|---|---|
| `--soc-version=<VERSION>` | — | 指定用哪个仿真器，如 `Ascend910B1`。config 模式下不生效 |
| `--export=<PATH>` | — | 指定已有 dump 数据路径，只做解析不重跑 |
| `--core-id=<ID>` | — | 只解析指定 core |
| `--timeout=<MINUTES>` | — | 超时时间，范围 1–2880 分钟（仿真慢，需要这个） |
| `--dump=<SWITCH>` | `off` | 是否落盘 dump。**仅对 A2/A3 有效** |

仿真模式的 `--aic-metrics` 只支持 `PipeUtilization | ResourceConflictRatio | PMSampling | Overhead`，且 **`PipeUtilization` 是必选项**。

仿真模式**没有** `--warm-up`、`--replay-mode`、`--launch-skip-before-match`、`--kill` —— 仿真器是确定性执行，不需要预热和重放。

---

## 三、实例：Add 算子采集

以本目录的 `msprof.asc` 为例。算子逻辑是 `z = x + y`，shape `[8, 2048]`，`float` 类型，`blockNum = 8`，每个 block 处理 2048 个元素。

Kernel 主体：

```cpp
AscendC::DataCopy(xLocal, xGm, blockLength);   // GM → UB
AscendC::DataCopy(yLocal, yGm, blockLength);   // GM → UB
AscendC::PipeBarrier<PIPE_ALL>();
AscendC::Add(zLocal, xLocal, yLocal, blockLength);  // UB → Reg 计算 → UB
AscendC::PipeBarrier<PIPE_ALL>();
AscendC::DataCopy(zGm, zLocal, blockLength);   // UB → GM
```

编译执行：

```bash
mkdir -p build && cd build
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..    # dav-2201 / dav-3510
make -j
msopprof ./demo
```

`CMAKE_ASC_ARCHITECTURES` 取值：`dav-2201` 对应 Atlas A2/A3 训练推理系列，`dav-3510` 对应 Ascend 950PR/950DT。

执行完成后生成 `OPPROF_20260730193410_GHKSZZZLZJGQSMBK/`：

```
├── dump/                        # 原始性能数据
│   ├── aicore_binary.o          # 算子二进制，反查指令/符号用
│   ├── DeviceProf1.bin ~ 6.bin  # 6 轮重放的原始 PMU 数据
│   ├── duration.bin
│   ├── op_basic_info.txt
│   └── pc_start_addr.txt        # PC 起始地址，做 PC 采样/源码关联用
├── OpBasicInfo.csv
├── PipeUtilization.csv
├── ArithmeticUtilization.csv
├── Memory.csv
├── MemoryUB.csv
├── MemoryL0.csv
├── L2Cache.csv
├── ResourceConflictRatio.csv
└── visualize_data.bin           # 给 MindStudio Insight 用
```

---

## 四、CSV 文件逐个解读

所有 CSV 的行组织方式一致：**一行一个 block**，前两列是 `block_id` 和 `sub_block_id`。本例 8 行，`block_id` 0–7，`sub_block_id` 全是 `vector0`。

列名前缀区分核类型：`aic_` 是 AI Cube 核（矩阵），`aiv_` 是 AI Vector 核（向量）。本例是纯 vector 算子，所有 `aic_*` 列都是 `NA` —— **看到大片 NA 不是采集失败，是这个核没参与**。

### 4.1 OpBasicInfo.csv — 先看这个

```
Op Name,Op Type,Task Duration(us),Block Dim,Mix Block Dim,Device Id,Pid,Current Freq,Rated Freq
add_custom,vector,1.936000,8,NA,0,1081967,1650,1650
```

| 字段 | 值 | 含义 |
|---|---|---|
| Op Name / Op Type | `add_custom` / `vector` | 算子名和类型 |
| **Task Duration(us)** | **1.936** | **算子端到端耗时，性能对比以此为准** |
| Block Dim | 8 | 核数，对应 `<<<8, 0>>>` |
| Mix Block Dim | NA | 混合 CV 算子才有值 |
| Current Freq / Rated Freq | 1650 / 1650 | 当前频率与额定频率（MHz） |

两个判断要点：

- **`Current Freq == Rated Freq` 说明没有降频**，数据可信。如果 Current 明显低于 Rated，测的是降频状态，横向对比会失真。
- 1650MHz 是 Ascend 950PR/950DT 的主频（A2/A3 是 1800MHz）。配合 `PipeUtilization.csv` 里出现了 `aiv_vec_simt_vf_ratio` 这个 SIMT 专属列，可以确认**这份数据是 `dav-3510` 编译跑出来的**。

Task Duration 1.936us 比后面单核最大的 `aiv_time` 1.357us 大，差值是 kernel 拉起、多核调度等核外开销。

### 4.2 PipeUtilization.csv — 定位瓶颈的主入口

这是最重要的一张表，给出每条流水的**耗时**和**占比**。本例 AIV 侧数据（取 block 0 和 block 3）：

| 指标 | block 0 | block 3 | 含义 |
|---|---|---|---|
| `aiv_time(us)` | 1.144848 | 1.356970 | 该核总耗时 |
| `aiv_total_cycles` | 1889 | 2239 | 总 cycle 数 |
| `aiv_vec_time(us)` | 0.055152 | 0.055152 | Vector 计算单元耗时 |
| `aiv_vec_ratio` | 0.048174 | 0.040643 | Vector 占比 |
| `aiv_scalar_time(us)` | 0.361818 | 0.410303 | Scalar 单元耗时 |
| `aiv_scalar_ratio` | 0.316040 | 0.302367 | Scalar 占比 |
| `aiv_mte2_time(us)` | 0.461818 | 0.618788 | MTE2（GM→UB 搬入）耗时 |
| `aiv_mte2_ratio` | **0.403388** | **0.456007** | **MTE2 占比，本例最高** |
| `aiv_mte2_active_bw(GB/s)` | 33.556946 | 25.044458 | MTE2 活跃带宽 |
| `aiv_mte3_time(us)` | 0.232121 | 0.238788 | MTE3（UB→GM 搬出）耗时 |
| `aiv_mte3_ratio` | 0.202753 | 0.175971 | MTE3 占比 |
| `aiv_icache_miss_rate` | 0.000000 | 0.000000 | ICache 未命中率 |

先验算一下自洽性：`1848 cycles ÷ 1650 MHz = 1.12 us`，与 block 4 的 `aiv_time = 1.120000` 完全吻合。cycle 和时间是一致的。

**结论怎么读：**

- `aiv_vec_time` 八个核全部是 **0.055152us，一模一样**。这是纯计算量，跟数据无关，所以完全一致。
- **`mte2_ratio` 约 0.40–0.51 是最大项，`vec_ratio` 只有 0.04–0.05**。计算只占 5%，搬运占 40%+ —— 这是典型的**访存瓶颈（memory-bound）算子**。Add 这种逐元素操作本来就该是这个形态：搬 3 份数据只做 1 次加法。
- `scalar_ratio` 约 0.30 偏高，因为 kernel 体量太小，地址计算等标量开销摊不薄。
- `icache_miss_rate = 0`，指令全部命中，代码段没问题。
- 八个核的 `aiv_time` 从 1.120 到 1.357，**离散约 21%**。计算量完全相同却有这个差异，来源是访存竞争（后面 L2Cache 数据能对上）。

**优化方向由此确定**：动 Vector 计算没有意义（只占 5%），要动的是访存。

### 4.3 ArithmeticUtilization.csv — 计算指令占比细分

```
aiv_vec_ratio, aiv_vec_vf_ratio, aiv_vec_sfu_ratio, aiv_vec_simt_vf_ratio
0.048174,      0.048174,         0.000000,          0.000000
```

| 列 | 含义 |
|---|---|
| `aiv_vec_ratio` | Vector 指令总占比 |
| `aiv_vec_vf_ratio` | VF（Vector Function）指令占比 |
| `aiv_vec_sfu_ratio` | SFU（特殊功能单元，超越函数如 exp/log）占比 |
| `aiv_vec_simt_vf_ratio` | SIMT VF 指令占比 |

本例 `vec_ratio` 与 `vec_vf_ratio` 完全相等，说明**全部 Vector 指令都是 VF 类**，没用到 SFU（Add 不涉及超越函数）。`simt_vf_ratio = 0` 说明是纯 SIMD 算子，没走 SIMT 路径。

AIC 侧的 `aic_cube_ratio` / `aic_cube_fp_ratio` / `aic_cube_int_ratio` / `aic_cube_*_instr_number` 全 NA，Cube 未参与。

### 4.4 Memory.csv — 主存带宽与数据量

这张表最有价值的是**可以用来校验理论数据量**。block 0：

| 指标 | 值 | 含义 |
|---|---|---|
| `read_main_memory_datas(KB)` | 16.250000 | 从主存读取的数据量 |
| `write_main_memory_datas(KB)` | 8.000000 | 写入主存的数据量 |
| `GM_to_UB_datas(KB)` | 16.250000 | GM→UB 搬运量 |
| `GM_to_UB_bw_usage_rate(%)` | 8.297965 | GM→UB 带宽利用率 |
| `aiv_gm_to_ub_bw(GB/s)` | 13.536470 | GM→UB 带宽 |
| `aiv_main_mem_read_bw(GB/s)` | 13.536470 | 主存读带宽 |
| `aiv_main_mem_write_bw(GB/s)` | 6.664109 | 主存写带宽 |
| `aiv_mte2_instructions` | 4 | MTE2 指令条数 |
| `aiv_mte3_instructions` | 3 | MTE3 指令条数 |

**理论值校验**：每 block 处理 2048 个 float = 8KB。读 x + y = 16KB，写 z = 8KB。

- 实测读 16.25KB ≈ 理论 16KB（多出的 0.25KB 是对齐/预取开销）✓
- 实测写 8.00KB = 理论 8KB，**精确吻合** ✓

这条校验做完，说明访存量没有冗余，没有重复搬运。**性能分析的第一步就该做这个对比** —— 如果实测远大于理论，说明有重复搬运或访存不合并，那才是首要问题。

`GM_to_UB_bw_usage_rate` 只有 8.3%，带宽利用率很低。原因是数据量太小（每核 16KB），MTE2 还没进入稳态就结束了，启动开销占比过大。这类算子要提升带宽利用率，方向是增大单核处理的数据量。

`aic_*` 列（`aic_l1_read_bw`、`aic_main_mem_*`、`aic_mte1_*`）以及 `GM_to_L1_datas`、`L0C_to_L1_datas`、`L0C_to_GM_datas` 全 NA，都是 Cube 路径的指标。

注意 `UB_to_GM_datas` 也是 NA 而 `GM_to_UB_datas` 有值 —— 这两个方向的统计在不同重放轮次采集，本次配置下只采到了搬入方向。

### 4.5 MemoryUB.csv — UB 读写带宽

```
aiv_ub_read_bw_vector, aiv_ub_write_bw_vector, aiv_ub_read_bw_gm, aiv_ub_write_bw_gm
6.872362,              6.664109,               NA,                13.536470
```

| 列 | 含义 |
|---|---|
| `aiv_ub_read_bw_vector` | Vector 单元从 UB 读的带宽 |
| `aiv_ub_write_bw_vector` | Vector 单元向 UB 写的带宽 |
| `aiv_ub_read_bw_gm` | GM 侧从 UB 读的带宽 |
| `aiv_ub_write_bw_gm` | GM 侧向 UB 写的带宽 |

`ub_write_bw_gm = 13.536470` 与 `Memory.csv` 里的 `aiv_gm_to_ub_bw` 完全相同 —— 同一条数据通路的两个视角。`ub_read_bw_vector` 6.87 GB/s 略高于 `ub_write_bw_vector` 6.66 GB/s，符合 Add 读两份写一份的模式。

这张表和 `Memory.csv` 的分工：`Memory.csv` 看**核外**（GM/主存）带宽，`MemoryUB.csv` 看**核内** UB 带宽。判断是 GM 卡住还是 UB 卡住时对比这两张。

### 4.6 MemoryL0.csv — L0 Buffer 带宽

```
aic_l0a_read_bw, aic_l0a_write_bw, aic_l0b_read_bw, aic_l0b_write_bw, aic_l0c_read_bw_cube, aic_l0c_write_bw_cube
```

L0A（左矩阵）、L0B（右矩阵）、L0C（矩阵乘结果）的读写带宽，**纯 Cube 指标**。本例全部为空 —— vector 算子不碰 L0 Buffer。做 Matmul/Conv 调优时这张表才有内容。

### 4.7 L2Cache.csv — 命中率

L2 Cache 是所有核共享、硬件自动管理的缓存，位于 GM 与各核之间。本例 AIV 侧：

| block | `read_close_hit` | `read_close_miss` | `read_close_victim` | `read_hit_rate(%)` | `write_hit_rate(%)` |
|---|---|---|---|---|---|
| 0 | 0 | 64 | 64 | 0.775194 | 100.000000 |
| 1 | 1 | 56 | 72 | 0.775194 | 93.750000 |
| 3 | 1 | 64 | 64 | 0.775194 | 93.750000 |
| 6 | 0 | 60 | 69 | 0.000000 | 100.000000 |

列名里的 `close` / `far` 指访问的 L2 分片距离当前核的远近，`victim` 是被替换出去的 cache line 数。

**读命中率 0.775%（block 6 是 0）—— 几乎完全 miss。**

这是符合预期的，不是问题：x、y 每个元素只读一次，没有任何数据复用，L2 里不可能有命中。这正是 Add 这类算子的本质。**看到低命中率要先判断算子有没有复用机会**，无复用场景刻意去优化 L2 命中率是无效的。

写命中率 93.75%–100%，输出写入基本都命中。

另外 `read_close_victim` 高达 64–72，说明**核间在互相踢 cache line**（8 个核同时抢 L2）。这与 `PipeUtilization` 里八核耗时离散 21% 可以对上 —— 快慢差异来源于 L2 竞争。

### 4.8 ResourceConflictRatio.csv — 冲突与等待

| 列 | block 0 值 | 含义 |
|---|---|---|
| `aiv_vec_stu_cflt_ratio` | 0.007941 | Vector 存储单元（Store Unit）冲突率 |
| `aiv_vec_ldu_cflt_ratio` | 0.000529 | Vector 加载单元（Load Unit）冲突率 |
| `aiv_vec_sfu_cflt_ratio` | 0.000000 | SFU 冲突率 |
| `aiv_vec_wait_ratio` | 0.000000 | Vector 等待占比 |
| `aiv_mte2_wait_ratio` | 0.000000 | MTE2 等待占比 |
| `aiv_mte3_wait_ratio` | 0.000000 | MTE3 等待占比 |

这张表主要看 **UB bank 冲突**。多个操作同时访问 UB 同一个 bank 或 bank group 时会排队，分读写冲突、写写冲突、读读冲突三类。

本例 STU 冲突率 0.79%、LDU 0.05%、其余全 0，**冲突基本不存在，不是瓶颈**。所有 `wait_ratio` 都是 0，说明没有流水互相阻塞。

如果这里 STU/LDU 冲突率上到几个百分点，就要回去调 UB 上的数据布局（错开 bank）。

### 4.9 分析流程小结

把八张表串起来，标准的看图顺序是：

1. **OpBasicInfo** — 确认没降频、拿到 Task Duration 基线
2. **Memory** — 实测数据量对比理论值，判断有无冗余搬运
3. **PipeUtilization** — 找占比最高的流水，确定 bound 类型
4. 按 bound 类型深入：访存瓶颈看 **MemoryUB** + **L2Cache**；计算瓶颈看 **ArithmeticUtilization**；怀疑排队看 **ResourceConflictRatio**；Cube 算子看 **MemoryL0**

本例的结论链条：数据量吻合理论 → MTE2 占比 40%+ 而 Vector 只占 5% → 访存瓶颈 → L2 无复用机会（读命中率 0.775% 属正常）+ 带宽利用率仅 8.3% + 核间 victim 高 → **真正的问题是单核数据量太小、启动开销占比过大**，优化方向是增大 tiling 粒度，而不是优化 cache 命中。

---

## 五、visualize_data.bin 与 MindStudio Insight

`visualize_data.bin` 是 26KB 的二进制文件，用途是**导入 MindStudio Insight 做图形化呈现**。它是 CSV 之外的独立产物：CSV 是聚合到 block 粒度的统计值，`.bin` 里带的是时间线级别的明细，两者信息量不同。

导入方式是在 MindStudio Insight 中打开整个 `OPPROF_{timestamp}_XXX` 目录（不是单独选 `.bin`），工具会自行索引其中的数据。

> **验证边界说明**：本文的编写环境未安装 MindStudio Insight（`${install_path}/cann-9.2.0/tools/` 下只有 msopprof、msdebug、msaicerr、simulator 等 CLI 工具），因此 Insight 各视图的具体布局、菜单名称和呈现细节未经实机验证，此处不写具体操作步骤以免误导。准确的界面操作流程请查 [MindStudio 工具指南](https://www.hiascend.com/document/redirect/CannCommercialToolOpDev) 中"算子调优（msOpProf）"章节。

### 与 dump 目录的关系

如果需要更底层的分析，`dump/` 下几个文件的用途：

| 文件 | 用途 |
|---|---|
| `aicore_binary.o` | 算子二进制。配合 `msobjdump` 可反汇编，做指令级分析 |
| `DeviceProf1~6.bin` | 6 轮重放的原始 PMU 数据，CSV 由此解析得来 |
| `pc_start_addr.txt` | PC 起始地址。做 `--aic-metrics=PCSampling` 或 `Source` 源码关联时用于地址映射 |
| `op_basic_info.txt` | `OpBasicInfo.csv` 的文本原始形态 |
| `duration.bin` | 耗时原始数据 |

想做源码级热点定位，用 `--aic-metrics=Source` 或 `PCSampling` 重采一次，工具会结合 `aicore_binary.o` 和 `pc_start_addr.txt` 把热点映射回源码行。

---

## 六、常用命令速查

```bash
# 最简用法：默认指标、预热 5 次、采 1 个 kernel
msopprof ./demo

# 带算子参数
msopprof build/demo 1 128 64 128

# 指定输出目录
msopprof --output=./prof_result ./demo

# 只采指定 kernel
msopprof --kernel-name=add_custom ./demo

# 采第 51 次 launch
msopprof --launch-skip-before-match=50 --launch-count=1 ./demo

# 关闭预热，测冷启动开销
msopprof --warm-up=0 ./demo

# 只采流水占比
msopprof --aic-metrics=PipeUtilization ./demo

# 采指令时间线，指定流水
msopprof --aic-metrics=InstrTimeline --instr-timeline-pipe=MTE2 ./demo

# 仿真采集
msopprof simulator --soc-version=Ascend910B1 --aic-metrics=PipeUtilization ./demo

# 只解析已有 dump，不重跑
msopprof simulator --export=./OPPROF_xxx/dump --soc-version=Ascend910B1
```

批量测试可以参考仓内 `tests/perf/load_data_perf/perf.sh` 和 `tests/perf/fixpipe_perf/perf.sh`，里面有完整的「循环编译 → msopprof 采集 → 从 CSV 提取指标 → 汇总 CSV」流程。两个脚本都有一个共同处理：**msopprof 可能返回非 0 退出码，不能被 `set -e` 提前中断**，需要单独兜住返回值。
