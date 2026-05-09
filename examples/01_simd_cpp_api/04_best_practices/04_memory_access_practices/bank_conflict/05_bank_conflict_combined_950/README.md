# MTE2 与 Vec 双侧 bank 冲突直调样例（Ascend 950PR/Ascend 950DT）

## 概述

本样例在完整数据通路 `GM → UB → Reg → UB → GM` 上同时演示 MTE2 与 Vec 两条流水线的 bank 冲突机制：

```
GM(ND) ──MTE2 ND2NZ──▶ buf1(UB, NZ格式) ──__simd_vf__──▶ buf2(UB) ──MTE3──▶ GM
            [冲突点①]                          [冲突点②]
```

两处 bank 冲突的成因和控制手段都不同：

| 冲突点 | 流水线 | 触发条件 | 控制参数 |
|--------|--------|---------|---------|
| ① | MTE2（ND2NZ） | `dstNzC0Stride` 是 8 的倍数，同行各 C0 写入同一 bank | `DST_NZ_C0_STRIDE` |
| ② | Vec（LoadAlign/StoreAlign 跨迭代双发） | buf1/buf2 地址差为 16 DataBlocks 的半周期（256B），双发时覆盖相同的 8 个 bank | `VEC_PADDING_BYTES` |

三个场景逐步消除冲突：

| Scenario | stride | MTE2 写入分布（row 0） | VEC_PADDING | d | Vec 冲突 |
|----------|--------|---------------------|------------|---|---------|
| **S1** | 128 | **16 写全打 bank 0**（16-way 冲突） | 256B | 8 | **有** |
| **S2** | 129 | 16 写分散到 bank 0~15（无冲突） | 256B | 8 | **有** |
| **S3** | 129 | 同 S2 | 0 | 0 | 无 |

**递进**：S1（两冲突都堵）→ S2（修 MTE2，仅 Vec 堵）→ S3（都通）。

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 目录结构介绍

```
├── 05_bank_conflict_combined_950
│   ├── scripts
│   │   ├── gen_data.py                       // 输入数据和真值数据生成脚本
│   │   └── verify_result.py                  // 验证输出数据与真值是否一致
│   ├── CMakeLists.txt                        // 编译工程文件
│   ├── data_utils.h                          // 数据读入写出函数
│   └── bank_conflict_combined_950.asc        // Ascend C 算子实现 & 调用样例
```

## 算子描述

- 算子功能：将输入 ND 矩阵经 ND2NZ 格式转换搬入 UB，再通过 `__simd_vf__` 函数在 UB 内部搬运，最后写回 GM。完整覆盖 MTE2→Vec→MTE3 数据通路，演示两个独立的 bank 冲突点及其规避。

- 算子规格：

  <table>
  <tr><td align="center">算子类型(OpType)</td><td colspan="4" align="center">AIV算子</td></tr>
  <tr><td rowspan="2" align="center">算子输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">x</td><td align="center">8192×8192</td><td align="center">half</td><td align="center">ND</td></tr>
  <tr><td align="center">算子输出</td><td align="center">y</td><td align="center">取决于 Scenario（见性能说明）</td><td align="center">half</td><td align="center">NZ</td></tr>
  <tr><td align="center">核函数名</td><td colspan="4" align="center">bank_conflict_combined_950</td></tr>
  </table>

- 算子实现：
  - 8192×8192 矩阵切分为 2048 个 128×256 tile（TILE_C0_COLS=16，与 example 03 一致），64 个 AIV 核各处理 32 个 tile。
  - 每个 tile 内部：`Duplicate` 清零 → `DataCopy`(ND2NZ，两段) → `CopyVF`(`__simd_vf__`) → `DataCopy`(MTE3 写回)。
  - UB 内 buf1（ND2NZ 目标）和 buf2（VF 搬运目标）地址固定，跨 tile 复用。UB 用量约 130KB（50%）。

## 编译运行

在本样例根目录下执行如下步骤，编译并执行算子。

- 配置环境变量

  - 默认路径，root 用户安装 CANN 软件包
    ```bash
    source /usr/local/Ascend/cann/set_env.sh
    ```

  - 默认路径，非 root 用户安装 CANN 软件包
    ```bash
    source $HOME/Ascend/cann/set_env.sh
    ```

  - 指定路径 install_path 安装 CANN 软件包
    ```bash
    source ${install_path}/cann/set_env.sh
    ```

- 生成输入数据和真值（三个 scenario 共用同一份输入；golden 按 scenario 区分）
  ```bash
  python3 scripts/gen_data.py
  ```

- 场景 1 编译运行（MTE2 + Vec 双冲突）
  ```bash
  mkdir -p build_s1 && cd build_s1
  cmake .. -DSCENARIO=1
  make -j
  ./demo
  python3 ../scripts/verify_result.py output/output_s1.bin output/golden_s1.bin
  cd ..
  ```

- 场景 2 编译运行（修了 MTE2，仅剩 Vec 冲突）
  ```bash
  mkdir -p build_s2 && cd build_s2
  cmake .. -DSCENARIO=2
  make -j
  ./demo
  python3 ../scripts/verify_result.py output/output_s2.bin output/golden_s2.bin
  cd ..
  ```

- 场景 3 编译运行（两个冲突均已消除）
  ```bash
  mkdir -p build_s3 && cd build_s3
  cmake .. -DSCENARIO=3
  make -j
  ./demo
  python3 ../scripts/verify_result.py output/output_s3.bin output/golden_s3.bin
  cd ..
  ```

执行结果如下，说明精度对比成功。
```bash
test pass!
```

## 性能说明

### Ascend 950PR/DT UB Bank 结构

| 参数 | 值 |
|------|----|
| UB 总大小 | 256KB |
| Bank 数 | 16 |
| Bank group 数 | 8 |
| 每个 bank group | 2 个 bank（bank_i 和 bank_{i+8}，i = 0..7） |
| Bank group 周期 | 8 × 32B = **256B** |
| 每 bank_group 端口 | 2 组读口 + 2 组写口 |

地址映射规则：

```
bank_id       = (addr / 32B) % 16
bank_group_id = bank_id % 8
```

### 冲突点①：ND2NZ MTE2 写写冲突

NZ 紧凑布局的自然 stride = TILE_H = 128（NZ 内存中相邻 C1 列组的间距 = N1×N0 = TILE_H/C0 × C0 = TILE_H）。

ND2NZ 把一行的 16 个 C0 块写入 UB 时，第 c 列的位置 = `n × dstNzNStride + c × dstNzC0Stride`。

**SCENARIO=1（stride=128）：每行 16 个写全打 bank 0**

```
c     位置          bank_id           bank_group
─────────────────────────────────────────────────
 0       0     0%16=0          0
 1     128   128%16=0          0
 2     256   256%16=0          0
...
15    1920  1920%16=0          0
              ↑                ↑
          全是 bank 0      全是 bank_group 0
```

**16 个写抢同一个 bank 的同一个端口 → 16-way 串行 → 性能急剧下降**。

**SCENARIO=2/3（stride=129）：每行 16 个写分散到 16 个不同 bank**

```
c     位置          bank_id           bank_group
─────────────────────────────────────────────────
 0       0           0               0
 1     129           1               1
 2     258           2               2
...
 7     903           7               7
 8    1032           8               0  ← bank_group 0 的另一个 bank
 9    1161           9               1
...
15    1935          15               7
              ↑                       ↑
          16 个不同 bank        8 个 bank_group 各用 2 个 bank
```

**16 个写打到 16 个不同 bank（每个 bank_group 用尽两个端口）→ 1 拍并行完成**。

代价：每两相邻列间多 1 DataBlock=32B 空洞（共 15 个，480B/tile），需要 Duplicate 预清零。

### 冲突点②：Vec 侧 LoadAlign/StoreAlign 跨迭代双发冲突

`__simd_vf__` 循环中，相邻迭代的 LoadAlign/StoreAlign 无数据依赖，编译器把迭代 i 的 StoreAlign（写 buf2 + i×VL）与迭代 i+1 的 LoadAlign（读 buf1 + (i+1)×VL）双发到同一拍。

950PR/DT 上 VL=256B=8 DataBlocks，一次 LoadAlign/StoreAlign 精确覆盖 **8 个连续 bank**。双发时是否冲突取决于：

```
d = (buf2_UbAddr / 32 - buf1_UbAddr / 32) % 16

d = 8 → 二者覆盖完全相同的 8 个 bank（同一物理端口被抢）→ 冲突
d = 0 → 二者覆盖互补的 8 个 bank（{0..7} 与 {8..15}）→ 无冲突
```

### UB 地址设计

```
SCENARIO=1（stride=128，buf1=2048 DataBlocks）：
  ┌──────────────────────────────────────┐
  │ buf1 [0, 65536B)     DataBlock 0~2047│ ← ND2NZ 输出（紧凑无洞）
  ├──[ 256B gap ]────────────────────────┤
  │ buf2 [65792B, ...)   DataBlock 2056  │ ← 2056%16=8，d=8 → Vec 冲突
  └──────────────────────────────────────┘
  UB 用量 ≈ 128.25KB

SCENARIO=2（stride=129，buf1_alloc=2064 DataBlocks）：
  ┌──────────────────────────────────────┐
  │ buf1 [0, 66048B)     DataBlock 0~2063│ ← ND2NZ 输出（15 个 32B 空洞）
  ├──[ 256B gap ]────────────────────────┤
  │ buf2 [66304B, ...)   DataBlock 2072  │ ← 2072%16=8，d=8 → Vec 冲突
  └──────────────────────────────────────┘
  UB 用量 ≈ 129.25KB

SCENARIO=3（stride=129，无 gap）：
  ┌──────────────────────────────────────┐
  │ buf1 [0, 66048B)     DataBlock 0~2063│
  ├──────────────────────────────────────┤
  │ buf2 [66048B, ...)   DataBlock 2064  │ ← 2064%16=0，d=0 → 无冲突
  └──────────────────────────────────────┘
  UB 用量 ≈ 129.00KB
```

### 输出规格

| | SCENARIO=1（stride=128） | SCENARIO=2/3（stride=129） |
|--|--|--|
| 每 tile 输出 C0 块数 | 2048 | 2063 |
| 每 tile 输出大小 | 32768 half（64KB） | 33008 half（64.4KB） |
| NZ 列间空洞 | 无 | 每 tile 15 个 32B 空洞（填 0） |
| 总输出元素数 | 2048 × 32768 = 67,108,864 | 2048 × 33008 = 67,600,384 |
| 输出文件大小 | 128MB | 128.9MB |

### Profiling 时关注的指标

```bash
msprof op ./demo
cat ./PROF_*/mindstudio_profiler_output/op_summary_*.csv
cat ./OPPROF_*/ResourceConflictRatio.csv
```

| 指标 | SCENARIO=1 | SCENARIO=2 | SCENARIO=3 |
|------|:----------:|:----------:|:----------:|
| `aiv_mte2_bankgroup_cflt_ratio` | **> 0** | ≈ 0 | ≈ 0 |
| `aiv_vec_bankgroup_cflt_ratio` | > 0 | > 0 | ≈ 0 |
| `aiv_mte2_time(μs)` | 较长 | 较短 | 较短 |
| `aiv_vec_time(μs)` | 较长 | 较长 | 较短 |

## 与其他 bank conflict 样例的关系

| 样例 | 冲突流水线 | API 风格 | 冲突控制手段 |
|------|-----------|---------|------------|
| `01_bank_conflict_950` | Vec（Add） | LocalTensor + TPipe | InitBuffer 大小控制 UB 地址 |
| `02_bank_conflict_nd2nz` | MTE2（ND2NZ） | Static tensor | dstNzC0Stride（Atlas A3/A2） |
| `03_bank_conflict_nd2nz_950` | MTE2（ND2NZ） | Static tensor | dstNzC0Stride（Ascend 950） |
| `04_bank_conflict_vec_reg_950` | Vec（LoadAlign/StoreAlign 双发） | `__simd_vf__` + Reg:: API | src/dst UB 地址差 |
| **`05_bank_conflict_combined_950`** | **MTE2 + Vec 双侧** | **Static tensor + `__simd_vf__`** | **dstNzC0Stride × src/dst 地址差（独立控制，3 场景递进）** |
