## 使用C_API验证Scalar Cache Mode功能效果样例

## 概述

本样例验证 `asc_set_scalar_cache_mode` 接口设置不同 L2 Cache 模式对标量单元 GM 访问的实际缓存效果，通过 `msopprof` 采集 L2 Cache 命中率指标进行对比验证。

与 [00_scalar_cache_mode](../00_scalar_cache_mode) 的区别：
- `00_scalar_cache_mode`：验证 set→get 寄存器回环一致性（接口契约正确性）
- `01_scalar_cache_mode_perf`：验证不同 cache mode 对标量 GM 访问的实际缓存效果（功能效果验证）

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 目录结构介绍

```
├── 01_scalar_cache_mode_perf
│   ├── scripts
│   │   └── gen_data.py              // 输入数据和真值数据生成脚本
│   ├── CMakeLists.txt               // 编译工程文件
│   ├── scalar_cache_mode_perf.asc   // 样例入口（kernel 调用 + main 函数）
│   ├── scalar_cache_mode_perf.h     // Kernel 实现（4 个 Case）
│   ├── data_utils.h                 // 数据读入写出函数
│   ├── README.md                    // 样例说明文档
│   └── README_en.md                 // 样例说明文档（英文）
```

## 缓存层次与接口配合关系

```
Scalar Unit ──→ Data Cache (L1, 核内私有) ──→ L2 Cache (共享) ──→ GM
                       │                           │
                  asc_dci() 清空            asc_set_scalar_cache_mode() 控制策略
                  asc_dcci_entire_out() 刷新
```

| 接口 | 角色 | 说明 |
|------|------|------|
| `asc_set_scalar_cache_mode` | **被测接口** | 设置 L2 读/写通道缓存策略 |
| `__gm__` 指针直接解引用 | **触发** | 标量 GM 读/写，走完整缓存路径 |
| `asc_dci()` | **隔离** | 清空 L1 Data Cache，确保测试起点干净 |
| `asc_sync_data_barrier(DSB_ALL)` | **前置** | `asc_dci()` 调用前必须等待所有内存访问完成 |
| `asc_dcci_entire_out()` | **刷新** | 标量写后将 L1 脏数据刷到 GM |
| `msopprof --aic-metrics=L2Cache` | **观测** | 采集 L2 读写命中/缺失计数 |

## 样例描述

### Case 1: 读通道 NORMAL_PERSISTENT（复用数据）

- **设计意图**：设置读通道为驻留模式后，重复读取同一批数据。Pass 1 填充 L2，Pass 2 应命中 L2 驻留数据。
- **工作集**：16384 个 float（64KB），远大于 L1 Data Cache，远小于 L2 Cache（~128MB）。
- **L2策略**：`asc_load_l2_cache_mode::NORMAL_PERSISTENT`

### Case 2: 读通道 NOTALLOC_KEEP（流式数据）

- **设计意图**：设置读通道为不分配 L2 Cache Line 后，重复读取同一批数据。由于 L2 从未分配 Cache Line，Pass 2 仍应 L2 miss。
- **L2策略**：`asc_load_l2_cache_mode::NOTALLOC_KEEP`

### Case 3: 写通道 NORMAL_FIRST_VICTIM

- **设计意图**：设置写通道为启用 L2 Cache 后，标量 GM 写入应分配 L2 Cache Line。同时验证写入数据正确性（dst[i] == float(i)）。
- **L2策略**：`asc_store_l2_cache_mode::NORMAL_FIRST_VICTIM`

### Case 4: 写通道 NOTALLOC_CLEAN

- **设计意图**：设置写通道为不启用 L2 Cache 后，标量 GM 写入应绕过 L2。同时验证写入数据正确性。
- **L2策略**：`asc_store_l2_cache_mode::NOTALLOC_CLEAN`

## 性能指标说明

使用 `msopprof --ai-core=on --aic-metrics=L2Cache ./scalar_cache_mode_perf` 采集以下指标：

| 字段名 | 字段含义 |
|:---:|:---|
| Task Duration(μs) | 整个任务执行的总时间 |
| aiv_read_local_l2_hit | 读 Cache 命中的次数 |
| aiv_read_local_l2_miss | 读 Cache 缺失次数 |
| aiv_read_local_l2_victim | 读 Cache 未命中并触发数据被换出的次数 |
| aiv_write_cache_hit | 写 Cache 命中的次数 |
| aiv_write_cache_miss_allocate | 写 Cache 缺失后重新分配缓存的次数 |

### 预期性能对比

**读通道对比（Case 1 vs Case 2）**

| Case | 模式 | aiv_read_local_l2_hit | aiv_read_local_l2_miss | 说明 |
|:---:|:---|:---:|:---:|:---|
| 1 | NORMAL_PERSISTENT | 高 | 低 | Pass 2 命中驻留数据 |
| 2 | NOTALLOC_KEEP | ≈0 | 高 | L2 不分配，每次都 miss |

**写通道对比（Case 3 vs Case 4）**

| Case | 模式 | aiv_write_cache_miss_allocate | 说明 |
|:---:|:---|:---:|:---|
| 3 | NORMAL_FIRST_VICTIM | 非零 | 写入分配 L2 Cache Line |
| 4 | NOTALLOC_CLEAN | ≈0 | 写入绕过 L2，不分配 |

## 编译运行

在本样例根目录下执行如下步骤，编译并执行算子。

- 配置环境变量

  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **说明：** `${install_path}` 为 CANN 包安装目录。

- 样例执行（以 Case 1 为例）

  ```bash
  SCENARIO_NUM=1
  mkdir -p build && cd build
  cmake -DSCENARIO_NUM=$SCENARIO_NUM -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..
  make -j
  python3 ../scripts/gen_data.py -scenarioNum $SCENARIO_NUM
  ./scalar_cache_mode_perf
  ```

- 性能采集

  ```bash
  msopprof --ai-core=on --aic-metrics=L2Cache ./scalar_cache_mode_perf
  cat ./PROF_*/L2Cache.csv
  ```

- 编译选项说明

  | 参数 | 说明 | 可选值 | 默认值 |
  |------|------|--------|--------|
  | `SCENARIO_NUM` | 场景编号 | 1-4 | 1 |
  | `CMAKE_ASC_ARCHITECTURES` | NPU 硬件架构 | `dav-3510`（仅该架构支持） | `dav-3510` |
