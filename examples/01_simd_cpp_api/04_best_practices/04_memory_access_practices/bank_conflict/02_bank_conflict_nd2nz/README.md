# ND2NZ Bank 冲突调优样例

## 概述

本样例以 `8192 × 8192` half ND 矩阵到全局紧凑 NZ 布局的转换为例，介绍基于静态 Tensor 方式编程时，UB 上 scatter 写阶段的 bank 冲突如何产生、如何观测、如何规避。整个调优过程分为两个步骤（case 1-2），通过切换 `dstNzC0Stride` 这一个参数对比性能。

**优化路径**：
- Case 1：`dstNzC0Stride = TILE_H = 128`（与 bank-group 周期同模，触发冲突）
- Case 2：`dstNzC0Stride = TILE_H + 1 = 129`（错开 1 个 DataBlock，规避冲突）

两个架构共用同一份代码，只在 Vec 侧 scatter 的 API 上有差异；冲突机制完全同构：
- Atlas A2/A3 (`dav-2201`)：scatter 走 `AscendC::Copy()` (vcopy)，bank-group 周期 = 16 DataBlock
- Ascend 950PR/DT (`dav-3510`)：scatter 走 `Reg::LoadAlign` + `Reg::StoreAlign<DATA_BLOCK_COPY>`，bank-group 周期 = 8 DataBlock

`128` 是两个架构 bank-group 周期的公倍数，所以两种硬件下都会触发冲突；`129` 同时不被两个周期整除，所以都规避。

## 支持的产品
- Ascend 950PR/Ascend 950DT
- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍

```text
├── 02_bank_conflict_nd2nz
│   ├── scripts
│   │   ├── gen_data.py            // 输入数据和真值数据生成脚本文件
│   │   └── verify_result.py       // 真值对比文件
│   ├── CMakeLists.txt             // 编译工程文件
│   ├── data_utils.h               // 数据读入写出函数
│   └── bank_conflict_nd2nz.asc    // Ascend C 样例实现（含 2 个 case）
```

## 样例描述

**样例功能**：

  样例实现的是固定 shape 为 `8192 × 8192` 的 half ND 矩阵到全局紧凑 NZ 布局的转换。

  ND → NZ 转换的本质是一次大规模 scatter：把每行的 16 个 C0 块（每块 32B）"撒"到 NZ 布局里 16 个不同 C0 列的对应行槽里。dstNzC0Stride 控制 UB 内相邻 C0 列的落点间距，是 bank 冲突的核心旋钮：

  - x：输入，shape `[8192, 8192]`，half，ND 行优先布局
  - z：输出，shape `[8192, 8192]`，half，全局紧凑 NZ 布局
  - tile：固定 `128 × 256`，共 `64 × 32 = 2048` 个 tile
  - 核函数名：`bank_conflict_nd2nz`

**关键参数**：

| 参数 | 值 | 含义 |
|:---:|:---:|:---|
| `TILE_H` | 128 | tile 内 N 方向行数 |
| `TILE_W` | 256 | tile 内 D 方向 half 数 |
| `TILE_C0_COLS` | 16 | tile 内 D 方向 C0 列数 |
| `DST_NZ_N_STRIDE` | 1 | UB 内 N 方向步长（DataBlock） |
| `DST_NZ_C0_STRIDE` | 128 (S1) / 129 (S2) | UB 内 C0 列间步长（DataBlock） |
| `SCATTER_LEN_DBS` | 8 | 单条 Vec scatter 指令处理的 DataBlock 数 |

**ND → NZ scatter 数据流**：

```text
源 ndBuf（一行 16 个 C0 块紧凑相邻，DataBlock 索引 0..15）：
┌──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┐
│c0│c1│c2│c3│c4│c5│c6│c7│c8│c9│cA│cB│cC│cD│cE│cF│
└──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┘
  └────── 一次 scatter (8 块) ──────┘└── 第二次 scatter ──┘

目标 nzBuf（NZ 紧凑布局 + 列尾空洞，按 DST_NZ_C0_STRIDE 散开）：
       DataBlock 索引 →
       0    1   ...  127    128   129  ...  255    256 ...
列 0:  [c0][r1][r2]...[r127] .                                    │
列 1:                       │ [c1][r1][r2]...[r127] .             │  S2 (stride=129)
列 2:                                                │ [c2]...    │  时每段尾部
       ↑                    ↑                       ↑               有 1 块空洞
       第 0 列起点          第 1 列起点              第 2 列起点
       (DB 0)               (DB DST_NZ_C0_STRIDE)   (DB 2*stride)

scatter 的本质 = 源连续读、目的按大 stride 写；落点间距由 DST_NZ_C0_STRIDE 控制。
```

## 样例实现

### 性能指标说明

**表 1 AI Core 性能指标字段说明表**
| 字段名 | 字段含义 |
|:---:|:---|
| Task Duration(μs) | Task 整体耗时，包含调度、加速器执行、响应结束时间。|
| aiv_time | Task 在 AI Vector Core 上的理论执行时间，单位 μs。|
| aiv_vec_time(μs) | vec 类型指令（向量类运算指令）耗时，单位 μs。**本样例的核心观测项**：bank 冲突直接拉高这一项。|
| aiv_vec_ratio | vec 类型指令的 cycle 占比。|
| aiv_scalar_time(μs) | scalar 类型指令耗时，单位 μs。|
| aiv_mte2_time(μs) | mte2 类型指令（GM→UB 搬运）耗时，单位 μs。|
| aiv_mte3_time(μs) | mte3 类型指令（UB→GM 搬运）耗时，单位 μs。|

### Case 1: dstNzC0Stride = 128（触发 Bank 冲突）

**实现方式**：编译时 `-DSCENARIO_NUM=1`。

代码中 `DST_NZ_C0_STRIDE` 取 `TILE_H = 128`，让 UB 内 tile-NZ 布局紧凑无空洞。这是 NZ 布局的"自然写法"，但会触发 bank 冲突。

**关键代码**：
```cpp
constexpr uint32_t DST_NZ_C0_STRIDE = (SCENARIO == 1) ? TILE_H : (TILE_H + 1);
```

**架构特化的 scatter 实现**：

- `dav-2201`：vcopy 的 `Copy()` 自带 repeat，单条 Copy 覆盖 TILE_H=128 行 × 8 个 C0 列。一行 16 个 C0 列要 2 次 Copy。

  ```cpp
  AscendC::CopyRepeatParams copyParams;
  copyParams.dstStride     = DST_NZ_C0_STRIDE;    // = 128，scatter 列间距
  copyParams.srcStride     = 1;
  copyParams.dstRepeatSize = DST_NZ_N_STRIDE;
  copyParams.srcRepeatSize = TILE_W / C0_ELEMS;

  for (uint32_t k = 0; k < SCATTERS_PER_ROW; ++k) {
      AscendC::Copy(nzBuf[k * SCATTER_LEN_DBS * DST_NZ_C0_STRIDE * C0_ELEMS],
                    ndBuf[k * SCATTER_LEN_HALF],
                    static_cast<uint64_t>(SCATTER_LEN_HALF),
                    static_cast<uint8_t>(TILE_H),
                    copyParams);
  }
  ```

- `dav-3510`：950 没有 vcopy/Copy 的 repeat 机制，手写 `__simd_vf__`，外层 scatter 列组、内层行循环。

  ```cpp
  for (uint16_t k = 0; k < SCATTERS_PER_ROW; ++k) {
      for (uint16_t r = 0; r < TILE_H; ++r) {
          AscendC::Reg::LoadAlign<half>(reg, srcAddr + r * TILE_W + k * SCATTER_LEN_HALF);
          AscendC::Reg::StoreAlign<half, AscendC::Reg::DataCopyMode::DATA_BLOCK_COPY>(
              dstRowAddr, reg, DST_NZ_C0_STRIDE, mask);
      }
  }
  ```

**冲突原理**：

单条 Vec scatter 指令一次写 `SCATTER_LEN_DBS = 8` 个 32B 块到 UB，落点间距 = `DST_NZ_C0_STRIDE` 个 DataBlock。一个写命中哪个 bank-group：

```
bank_group_id = (落点 DataBlock 索引) % bank_groups
```

当 `DST_NZ_C0_STRIDE % bank_groups == 0` 时，8 个落点全部落到同一个 bank group：

| 架构 | bank groups | bank-group 周期 | 128 % bank_groups | 结果 |
|:---:|:---:|:---:|:---:|:---:|
| 2201 | 16 | 16 DataBlock | 128 % 16 = 0 | 8 个写全打同一 bank group，**8 拍**完成 1 个 repeat 的写入 |
| 3510 | 8 | 8 DataBlock | 128 % 8 = 0 | 同上，**8 拍**完成 1 条 StoreAlign |

理论上单条指令延迟从 1 拍拉长为 8 拍，aiv_vec_time 显著膨胀。

**S1 落点示意图**（以 950 为例，8 bank groups，stride=128）：

```text
单条 StoreAlign 的 8 个写在 UB 上的落点（DataBlock 索引）：
  写 #0 → DB   0    bank_group = 0 % 8 = 0
  写 #1 → DB 128    bank_group = 128 % 8 = 0   ◀ 冲突
  写 #2 → DB 256    bank_group = 256 % 8 = 0   ◀ 冲突
  写 #3 → DB 384    bank_group = 384 % 8 = 0   ◀ 冲突
  写 #4 → DB 512    bank_group = 512 % 8 = 0   ◀ 冲突
  写 #5 → DB 640    bank_group = 640 % 8 = 0   ◀ 冲突
  写 #6 → DB 768    bank_group = 768 % 8 = 0   ◀ 冲突
  写 #7 → DB 896    bank_group = 896 % 8 = 0   ◀ 冲突

  bank group:    0       1       2       3       4       5       6       7
              ┌──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┐
              │ ●●●●●│      │      │      │      │      │      │      │
              │ ●●●  │      │      │      │      │      │      │      │
              └──────┴──────┴──────┴──────┴──────┴──────┴──────┴──────┘
                 ↑
                 8 个写全堆在 bank group 0 → 排队 8 拍才能写完
```

**性能数据**（Ascend 950PR/DT，64 核）：

| Task Duration(μs) | aiv_vec_time(μs) | aiv_vec_ratio | aiv_mte2_time(μs) | aiv_mte3_time(μs) |
|:---:|:---:|:---:|:---:|:---:|
| 待补充 | **44** | 高 | 待补充 | 待补充 |

**优化效果分析**：
- `aiv_vec_time = 44 μs`，明显高于理论值。VEC_STORE 流水内每条 StoreAlign 都因 bank-group 冲突排队 8 拍。
- VEC_LOAD（LoadAlign 读 ndBuf）紧凑无冲突，不是瓶颈。

**性能瓶颈定位**：
> ⚠️ **bank 冲突在 Vec 子流水内部**
>
> 在 Regbase 架构下，`Reg::LoadAlign` 走 **VEC_LOAD** 子流水、`Reg::StoreAlign` 走 **VEC_STORE** 子流水（出处：`docs/api/context/LocalMemBar.md`），不在 MTE 上。常规 MTE 类 bank-conflict 计数器看不到，需要看 vec 流水或单指令周期数。

---

### Case 2: dstNzC0Stride = 129（规避 Bank 冲突）

**实现方式**：编译时 `-DSCENARIO_NUM=2`。

代码中 `DST_NZ_C0_STRIDE` 取 `TILE_H + 1 = 129`，每个 C0 列尾部多出 1 个 DataBlock 的空洞，scatter 落点在 bank group 间轮转。

**关键代码**：源码完全一样，只是宏 `SCENARIO=2`，编译期推导出不同的 `DST_NZ_C0_STRIDE`。

**规避原理**：

`129 % 8 = 1`、`129 % 16 = 1`，落点 bank-group 索引序列在两个架构下分别是：

| 架构 | bank groups | 落点 bank-group 序列 | 结果 |
|:---:|:---:|:---|:---:|
| 2201 | 16 | 0, 1, 2, ..., 7（8 个落点，每个占一个 group） | 8 个写并发，**1 拍**完成 |
| 3510 | 8 | 0, 1, 2, ..., 7（8 个落点，每个占一个 group） | 同上，**1 拍**完成 1 条 StoreAlign |

**S2 落点示意图**（以 950 为例，8 bank groups，stride=129）：

```text
单条 StoreAlign 的 8 个写在 UB 上的落点（DataBlock 索引）：
  写 #0 → DB   0    bank_group =   0 % 8 = 0
  写 #1 → DB 129    bank_group = 129 % 8 = 1
  写 #2 → DB 258    bank_group = 258 % 8 = 2
  写 #3 → DB 387    bank_group = 387 % 8 = 3
  写 #4 → DB 516    bank_group = 516 % 8 = 4
  写 #5 → DB 645    bank_group = 645 % 8 = 5
  写 #6 → DB 774    bank_group = 774 % 8 = 6
  写 #7 → DB 903    bank_group = 903 % 8 = 7

  bank group:    0       1       2       3       4       5       6       7
              ┌──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┐
              │  ●   │  ●   │  ●   │  ●   │  ●   │  ●   │  ●   │  ●   │
              │      │      │      │      │      │      │      │      │
              └──────┴──────┴──────┴──────┴──────┴──────┴──────┴──────┘
                 ↑      ↑      ↑      ↑      ↑      ↑      ↑      ↑
                 8 个写均匀分散到 8 个 bank groups → 1 拍并发完成
```

**UB 占用变化**：

`nzBuf` 占用 DataBlock 数 = `(TILE_H - 1) * DST_NZ_N_STRIDE + (TILE_C0_COLS - 1) * DST_NZ_C0_STRIDE + 1`

| 场景 | DataBlock 数 | UB 占用 |
|:---:|:---:|:---:|
| S1 (stride=128) | 2048 | 64 KB（紧凑） |
| S2 (stride=129) | 2063 | ≈64.5 KB（含 15 个 32B 空洞） |

空洞在 UB→GM 的 MTE3 阶段由 `outParams.srcStride = DST_NZ_C0_STRIDE - TILE_H` 跳过，不写回 GM。

**性能数据**（Ascend 950PR/DT，64 核）：

| Task Duration(μs) | aiv_vec_time(μs) | aiv_vec_ratio | aiv_mte2_time(μs) | aiv_mte3_time(μs) |
|:---:|:---:|:---:|:---:|:---:|
| 待补充 | **10** | 中 | 待补充 | 待补充 |

**优化效果分析**：
- `aiv_vec_time` 从 44 μs 降到 10 μs，**减少 77%**，与"延迟从 8 拍降到 1 拍"的理论吻合。
- 代价是每 tile 多出 15 × 32B = 480B 的 UB 空洞（约 0.7% 浪费），完全可接受。

**性能优化建议**：
> 💡 **stride 错开 1 个 DataBlock 即可消除 bank 冲突**
>
> 1. 当 scatter 的 stride 是 bank-group 周期（2201 为 16、3510 为 8）的整数倍时必触发冲突；
> 2. 把 stride 改成"周期 + 1"（或任意与周期互质的偏移）即可规避，代价只是每段尾部 1 个 DataBlock 的空洞；
> 3. UB 空洞由 MTE3 的 `srcStride` 跳过，不影响 GM 输出正确性。

---

## 性能对比总结

### Ascend 950 系列性能对比

| Case | dstNzC0Stride | scatter 落点模式 | aiv_vec_time(μs) | 单条指令延迟 |
|:---:|:---:|:---|:---:|:---:|
| 1 | 128 | 8 个 DataBlock 全打 bank group 0 | **44** | 8 拍 |
| 2 | 129 | 8 个 DataBlock 散到 bank groups 0~7 | **10** | 1 拍 |

`aiv_vec_time` 提升 4.4×，与理论冲突倍数（8 拍 → 1 拍）方向一致。差距未达 8× 的原因：VEC_LOAD 与 VEC_STORE 双发会部分掩盖 VEC_STORE 的延迟，且 mask/scalar 等 overhead 不参与 bank 冲突放大。

### Atlas A2/A3 系列性能对比

2201 上的 vcopy `Copy()` 也走 Vec pipe，scatter stride 与 bank-group 周期同模时同样触发写写冲突，理论上 `aiv_vec_bankgroup_cflt_ratio` 应该在 S1 高、S2 接近 0；端到端 `aiv_vec_time` 同样应该呈现 8× 量级的差异。**性能数据待用户在 A2/A3 真机上补充**。

### 优化要点总结

| 优化手段 | 核心原理 | 适用场景 |
|:---|:---:|:---|
| stride + 1 偏移 | 让 scatter 落点的 bank-group 索引轮转，避免 1 拍内多次访问同一 group | ND2NZ / NZ2ND / 转置 / 任何 stride 为 bank-group 周期整数倍的 scatter |

---

## 编译运行

- 切换 Case

  在 cmake 编译时通过 `-DSCENARIO_NUM=N` 指定要编译的 case：

  ```bash
  cmake -DSCENARIO_NUM=1 -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..   # 编译 case 1（可替换为 1 或 2）
  ```

  各 case 说明：
  - `1`: `dstNzC0Stride = 128`，触发 bank 冲突
  - `2`: `dstNzC0Stride = 129`，规避 bank 冲突

- 配置环境变量
  请根据当前环境上 CANN 开发套件包的[安装方式](../../../../../../docs/quick_start.md#prepare&install)，选择对应配置环境变量的命令。
  - 默认路径，root 用户安装 CANN 软件包
    ```bash
    source /usr/local/Ascend/cann/set_env.sh
    ```

  - 默认路径，非 root 用户安装 CANN 软件包
    ```bash
    source $HOME/Ascend/cann/set_env.sh
    ```

  - 指定路径 install_path，安装 CANN 软件包
    ```bash
    source ${install_path}/cann/set_env.sh
    ```

- 样例执行

  ```bash
  python3 scripts/gen_data.py                # 生成测试输入数据（仅需运行一次，input.bin 和 golden.bin 复用）
  mkdir -p build && cd build                 # 创建并进入 build 目录
  cmake -DSCENARIO_NUM=1 -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..   # 编译 case 1
  make -j
  cp -r ../input ../output .                 # 把数据软链/拷贝到 build 目录
  ./demo                                     # 执行
  python3 ../scripts/verify_result.py output/output_s1.bin output/golden.bin  # 精度验证
  ```

  使用 CPU 调试 或 NPU 仿真 模式时，添加 `-DCMAKE_ASC_RUN_MODE=cpu` 或 `-DCMAKE_ASC_RUN_MODE=sim`：

  ```bash
  cmake -DSCENARIO_NUM=1 -DCMAKE_ASC_RUN_MODE=cpu -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..   # CPU 调试模式
  cmake -DSCENARIO_NUM=1 -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..   # NPU 仿真模式
  ```

  > **注意：** 切换编译模式或场景前需清理 cmake 缓存，可在 build 目录下执行 `rm CMakeCache.txt` 后重新 cmake。

- 编译选项说明

  | 选项 | 可选值 | 说明 |
  |------|--------|------|
  | `CMAKE_ASC_RUN_MODE` | `npu`（默认）、`cpu`、`sim` | 运行模式：NPU 运行、CPU 调试、NPU 仿真 |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-2201`（默认）、`dav-3510` | NPU 架构：`dav-2201` 对应 Atlas A2/A3 训练/推理系列，`dav-3510` 对应 Ascend 950PR/Ascend 950DT |
  | `SCENARIO_NUM` | `1`（默认）、`2` | bank 冲突场景编号（编译期映射为 C 宏 `SCENARIO`） |

  执行结果如下，说明精度对比成功：
  ```bash
  test pass!
  ```

## 性能分析

使用 `msprof` 工具获取详细性能数据：

```bash
msprof ./demo
```

当前目录下会生成 `PROF_` 前缀的文件夹，`mindstudio_profiler_output` 目录保存 Host 和各 Device 的性能数据汇总：

```text
PROF_xxxx_XXXXXX
├── device_{id}
└── host
└── mindstudio_profiler_log
└── mindstudio_profiler_output    # 保存 Host 和各 Device 的性能数据汇总
    ├── msprof_*.json
    ├── xx_*.csv
    └── README.txt
```

查看具体的性能分析结果：
```bash
cat ./PROF_*/mindstudio_profiler_output/op_summary_*.csv
```

**本样例重点关注的字段**：

- `aiv_vec_time(μs)`：bank 冲突直接拉高这一项，对比 case1 / case2 应有明显差异。
- 在 Atlas A2/A3 上可额外查看 `aiv_vec_bankgroup_cflt_ratio` 之类 vec bank-group 冲突计数（若 profiler 工具版本支持）。
- 在 Ascend 950 上 `Reg::LoadAlign` / `Reg::StoreAlign` 属于 VEC_LOAD / VEC_STORE 子流水（非 MTE），若 profiler 无独立冲突计数，可直接对比 `aiv_vec_time` 差值。
