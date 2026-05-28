# NPU 架构版本 351x Bank 冲突算子直调样例

## 概述

本样例演示`351x`架构上 UB Bank 冲突的不同场景及其性能影响。通过 11 个 Case 对比同一计算逻辑在不同地址布局、指令模式下的性能差异，总结规避 Bank 冲突的最佳实践。

## 支持的产品

- Ascend 950PR / Ascend 950DT

## 351x UB 关键特性

| 对比 220x | 220x | 351x |
|:----------:|:-----:|:--:|
| UB 总容量 | 192KB | 256KB |
| Bank Group 数 | 16 | 8 |
| 每 BG Bank 数 / 读口数 | 3 / 1 | 2 / **2** |
| 地址交织 | 高位 | 低位 |

351x UB 结构：
- 256KB UB，8 个 bank group，每组 2 个 bank（共 16 bank），每 bank 512 行 × 32B = 16KB
- 每组 2 个读口 + 1 个写口
- 低位地址交织：`ADDR = {DEPTH[8:0], BANK[0], BG[2:0], WIDTH[4:0]}`

## 样例描述

### 算子场景

共 11 个 Case，通过 `CMakeLists.txt` 编译时传入 `-DCASE_NUM=N` 切换。

| Case | VF 函数 | 场景 | 冲突特征 |
|:----:|:--------:|:----|:---------|
| 1 | AddVF | 二读一写 (x+y=z)，无地址间隔 | 读读冲突 + 读写冲突（基准） |
| 2 | AddSingleVF | 同 Case 1，无循环展开 | 同上，对比展开对冲突的影响 |
| 3 | AddVF | 二读一写，地址偏移 0x100 | 读读冲突消除 |
| 4 | AddVF | 地址重叠 (src0==src1) | 读指令减半 |
| 5 | AddWithInstrLoadVF | 双搬入 DINTLV_B32 | 无读读冲突 |
| 6 | AddsVF | 一读一写，同 bank | 读写冲突 |
| 7 | AddsVF | 一读一写，地址偏移 0x100 | 无冲突 |
| 8 | GatherAndScatterVF | 离散访存，idx [0..7] 重复 | 同地址可 coalesce |
| 9 | GatherAndScatterVF | 离散访存，idx 连续 | 跨 bank 自动分散 |
| 10 | GatherAndScatterVF | **subbank 冲突** idx [0,128] 交替 | 同 subbank 不同行，不可 coalesce |
| 11 | GatherAndScatterVF | 无 subbank 冲突 idx 全 0 | 全部 coalesce |

### Case 配置详情

**Case 1-5: 二读一写 / 双搬入**

| Case | src0 地址 | src1 地址 | dst 地址 | TOTAL_LENGTH | 备注 |
|:----:|:---------:|:---------:|:--------:|:------------:|:-----|
| 1 | 0x0000 | 0x4000 | 0x8000 | 4096 | AddVF，循环展开 |
| 2 | 0x0000 | 0x4000 | 0x8000 | 4096 | AddSingleVF，无展开 |
| 3 | 0x0000 | 0x4100 | 0x8900 | 4096 | 地址偏移消除读读冲突 |
| 4 | 0x0000 | **0x0000** | 0x4100 | 4096 | 地址重叠，读指令减半 |
| 5 | 0x0000 | 0x4000 | 0x8000 | 4096 | 双搬入 DINTLV_B32 |

**Case 6-7: 一读一写**

| Case | src0 地址 | src1 地址 | dst 地址 | TOTAL_LENGTH | 备注 |
|:----:|:---------:|:---------:|:--------:|:------------:|:-----|
| 6 | 0x0000 | 0x0000 | 0x4000 | 4096 | 同 bank 读写冲突 |
| 7 | 0x0000 | 0x0000 | 0x4100 | 4096 | 消除读写冲突 |

**Case 8-11: 离散访存**

| Case | index 地址 | data 地址 | index 模式 | 特征 |
|:----:|:----------:|:---------:|:----------|:-----|
| 8 | 0x0000 | 0x0400 | 重复 [0..7] | 同地址可 coalesce |
| 9 | 0x0000 | 0x0400 | 连续 [0..255] | 跨 bank 分散 |
| 10 | 0x0000 | 0x0400 | 交替 [0,128] | **同 subbank 不同行冲突** |
| 11 | 0x0000 | 0x0400 | 全 0 | 全部 coalesce |

**Subbank 冲突机制**：A5 每个 bank 含 4 个 8B subbank，每个 subbank 仅 1 个读口。
- Gather 命中相同地址 → 硬件 coalesce → 无冲突
- Gather 命中同 subbank 不同地址 → 排队串行化 → 可测性能下降

## 目录结构

```text
├── bank_conflict.asc              // 算子 host + device 代码
├── config.h                      // 11 个 Case 的地址/类型配置
├── vf_func.h                     // Vector 函数实现
├── analyse.md                    // 地址解码与冲突分析
├── data_utils.h                  // 数据读写工具
├── CMakeLists.txt                // 编译工程
├── test.sh                       // 测试脚本
└── scripts/
    ├── gen_data.py               // 测试数据生成
    └── verify_result.py          // 精度验证
```

## 运行方式

### 环境准备

```bash
source /path/to/cann/set_env.sh
```

### 编译与运行（单个 Case）

```bash
export CASE_NUM=1          # 1-11 可选
mkdir -p build && cd build
cmake .. -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DCASE_NUM=${CASE_NUM}
make -j
python3 ../scripts/gen_data.py --case ${CASE_NUM}
./demo                     # 精度验证
python3 ../scripts/verify_result.py output/output_z.bin output/golden.bin
msprof op ./demo           # 性能 profiling
```

### 批量运行

```bash
bash test.sh             # 精度验证
bash test.sh profile     # 精度验证 + profiling
```

### CMake 选项

| 选项 | 可选值 | 说明 |
|:----:|:------:|:----|
| `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU 架构版本 |
| `CASE_NUM` | 1-11 | 切换测试场景 |
| `CMAKE_ASC_RUN_MODE` | `npu` | 上板运行模式 |

## 性能分析

### 关键指标说明

| CSV 文件 | 关键字段 | 含义 |
|:--------:|:---------|:-----|
| `OpBasicInfo.csv` | Task Duration(us) | 算子总耗时 |
| `PipeUtilization.csv` | aiv_time(us) | 向量单元总活跃时间 |
| `PipeUtilization.csv` | aiv_vec_time(us) | 向量计算时间 |
| `PipeUtilization.csv` | aiv_scalar_time | 标量操作时间 |
| `PipeUtilization.csv` | aiv_mte2_ratio | MTE2 数据搬运占比 |

### 实测结果

| Case | Task(us) | aiv_time(us) | aiv_vec(us) | 特征 |
|:----:|:--------:|:-----------:|:-----------:|:-----|
| 1 | 82.6 | 81.6 | 0.987 | 三路冲突，循环展开（基准） |
| 2 | 88.0 | 87.1 | 0.988 | 同 Case 1 无展开（+6.5%） |
| 3 | 89.7 | 88.5 | 0.986 | 消除读读冲突 |
| 4 | 70.1 | 69.1 | 0.983 | **地址重叠（最快）** |
| 5 | 91.9 | 90.9 | 0.987 | 双搬入（最慢） |
| 6 | 69.6 | 68.5 | 0.982 | 同 bank 读写冲突 |
| 7 | 70.4 | 69.1 | 0.982 | 消除读写冲突 |
| 8 | 41.5 | 40.3 | 0.977 | idx 重复→coalesce |
| 9 | 41.7 | 40.5 | 0.973 | idx 连续→跨 bank |
| 10 | **47.4** | **46.4** | 0.980 | **subbank 冲突（+14%）** |
| 11 | 41.4 | 40.3 | 0.977 | 全 coalesce |

### 分析结论

1. **aiv_vec 恒定**（~0.98 us）——向量计算管线不受冲突影响
2. **subbank 冲突拉长 aiv_time**（46.4 vs 40.3 us）——Gather 的 subbank 排队让向量单元多活跃 6 us
3. **bank 冲突不拉长 aiv_time**——Case 1/3/6/7 的差异不在向量单元
4. **循环展开对冲突场景有正收益**——Case 1 vs 2（展开 vs 无展开）= 82.6 vs 88.0（+6.5%）
5. **Task ≈ aiv_time + 1 us**——向量单元占满整个算子执行

## 相关文档

- [避免 bank 冲突（NPU 架构版本 3510）](../../../../../docs/guide/算子实践参考/SIMD算子性能优化/内存访问/避免UB的bank冲突/避免bank冲突（NPU架构版本3510）.md)
- [性能调优](../../../../../docs/guide/编程指南/调试调优/性能调优.md)
- [msProf profiling 样例](../../01_utilities/04_profiling/msProf/README.md)
