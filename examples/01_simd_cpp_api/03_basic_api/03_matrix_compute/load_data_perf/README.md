# LoadData(L1->L0) 性能测试样例

## 概述

本样例用于 LoadData 相关接口的性能测试，覆盖矩阵 A/B 从 L1 搬运到 L0A/L0B 的数据搬运路径。
本样例为无业务测试，没有矩阵相关计算指令。

## 本样例支持的产品及CANN软件版本

| 产品 | 架构代号 | CANN软件版本 |
|------|----------|-------------|
| Ascend 950PR/Ascend 950DT | dav-3510（950） | >= CANN 9.1.0 |
| Atlas A3 训练系列产品/Atlas A3 推理系列产品 | dav-2201（a2/a3） | >= CANN 9.0.0 |
| Atlas A2 训练系列产品/Atlas A2 推理系列产品 | dav-2201（a2/a3） | >= CANN 9.0.0 |

## 目录结构介绍

```
├── load_data_perf
│   ├── CMakeLists.txt              // 编译工程文件
│   ├── load_data_perf.asc          // LoadData 性能测试实现与调用入口
│   └── README.md                   // 样例说明文档
```

## 样例描述

本样例通过运行参数 `SCENARIO_NUM` 选择不同搬运路径，矩阵规格通过 `./demo SCENARIO_NUM M K N` 在运行时传入，核函数名为 `load_data_perf_custom`。

### 场景说明

根据平台架构，支持的测试场景如下：

| 平台架构 | 支持场景 | 说明 |
|----------|----------|------|
| a2/a3（dav-2201） | 1-9 | Atlas A3/A2 平台支持的 LoadData 接口场景 |
| 950（dav-3510） | 11-19 | Ascend 950PR/950DT 平台支持的 LoadData 接口场景 |

### a2/a3 平台场景（1-9）

| SCENARIO_NUM | 输入数据类型 | 数据源 | 执行路径 | 说明 |
|--------------|--------------|--------|----------|------|
| 1 | bfloat16 | A矩阵 | L1 -> L0A | Load2D 从 L1 加载到 L0A |
| 2 | bfloat16 | B矩阵 | L1 -> L0B | Load2D 从 L1 加载到 L0B |
| 3 | bfloat16 | A矩阵 | L1 -> L0A（转置） | LoadDataWithTranspose 从 L1 加载到 L0A（转置） |
| 4 | bfloat16 | B矩阵 | L1 -> L0B（转置） | LoadDataWithTranspose 从 L1 加载到 L0B（转置） |
| 5 | bfloat16 | A矩阵 | L1 -> L0A | Load3Dv2 从 L1 加载到 L0A |
| 6 | bfloat16 | B矩阵 | L1 -> L0B | Load3Dv2 从 L1 加载到 L0B |
| 7 | int8_t | B矩阵 | L1 -> L0B | LoadDataWithSparse 从 L1 加载到 L0B（稀疏加载） |
| 8 | bfloat16 | Bias | L1 -> C2 | 从 L1 加载到 bias buffer |
| 9 | uint64_t | FixPipe | L1 -> C2PIPE2GM | 从 L1 加载到 fixpipe buffer |

### 950 平台场景（11-19）

| SCENARIO_NUM | 输入数据类型 | 数据源 | 执行路径 | 说明 |
|--------------|--------------|--------|----------|------|
| 11 | bfloat16 | A矩阵 | L1 -> L0A | Load2Dv2 从 L1 加载到 L0A |
| 12 | bfloat16 | B矩阵 | L1 -> L0B | Load2Dv2 从 L1 加载到 L0B |
| 13 | fp8_e4m3fn | A矩阵 + ScaleA | L1 -> L0A + L0A_MX | Load2DMx 从 L1 加载到 L0A 和 L0A_MX（带 scale） |
| 14 | fp8_e4m3fn | B矩阵 + ScaleB | L1 -> L0B + L0B_MX | Load2DMx 从 L1 加载到 L0B 和 L0B_MX（带 scale） |
| 15 | bfloat16 | A矩阵 | L1 -> L0A | Load3Dv2 从 L1 加载到 L0A |
| 16 | bfloat16 | B矩阵 | L1 -> L0B | Load3Dv2 从 L1 加载到 L0B |
| 17 | bfloat16 | B矩阵 | L1 -> L0B（转置） | LoadDataWithTranspose 从 L1 加载到 L0B（转置） |
| 18 | bfloat16 | Bias | L1 -> C2 | 从 L1 加载到 bias buffer |
| 19 | uint64_t | FixPipe | L1 -> C2PIPE2GM | 从 L1 加载到 fixpipe buffer |

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。

### 配置环境变量

请根据当前环境上CANN开发套件包的[安装方式](../../../../../../docs/quick_start.md#prepare&install)，配置环境变量。

```bash
source ${install_path}/cann/set_env.sh
```

`${install_path}` 为CANN包安装目录，未指定安装目录时默认安装至 `/usr/local/Ascend` 下。

### 编译样例

```bash
mkdir -p build && cd build
cmake -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j
```

或针对 950 平台编译：

```bash
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j
```

### 运行样例

```bash
# a2/a3 平台示例（场景 1-9）
./demo 1 64 128 128
./demo 2 256 128 128

# 950 平台示例（场景 11-19）
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j
./demo 11 64 128 128
./demo 13 256 128 128
```

运行参数顺序为 `SCENARIO_NUM M K N`：
- `SCENARIO_NUM`：测试场景编号，a2/a3 平台使用 1-9，950 平台使用 11-19
- `M`：矩阵 A 的行数
- `K`：矩阵 A 的列数（矩阵 B 的行数）
- `N`：矩阵 B 的列数

例如 `./demo 2 256 128 512` 表示执行场景 2（Load2DB），A 矩阵为 `[256, 128]`，B 矩阵为 `[128, 512]`。


### 编译选项说明

| 选项 | 可选值 | 说明 |
|------|--------|------|
| `CMAKE_ASC_RUN_MODE` | `npu`（默认） | 运行模式：NPU运行 |
| `CMAKE_ASC_ARCHITECTURES` | `dav-2201`（默认）、`dav-3510` | NPU 架构：dav-2201 对应 a2/a3 平台，dav-3510 对应 950 平台 |


## 性能分析

本样例提供了性能测试脚本，用于批量测试和性能数据采集。

### 使用性能测试脚本

以执行场景 1 的性能测试脚本为例（测试 3 组不同的 shape）：

```bash
# 执行性能测试脚本
./perf_test.sh 1

```

脚本会：
1. 自动编译工程（如果需要）
2. 使用 `msprof op` 采集性能数据
3. 测试 3 组 shape：
   - [32, 32, 32]
   - [64, 64, 64]
   - [128, 128, 128]
4. 从 `PipeUtilization.csv` 提取 aic_mte1_time 性能数据
5. 生成汇总 CSV 文件：`perf_result_scenario1.csv`

### 性能测试结果

测试完成后，结果保存在 `perf_result_scenario1.csv`，包含：
- Test_ID：测试编号
- M/K/N：矩阵维度
- Shape：矩阵规格字符串
- AIC_MTE1_Time(ns)：AIC MTE1 性能数据（纳秒）

原始性能数据保存在 `perf_data/` 目录下，可使用 msprof 工具进一步分析。

### 注意事项

1. 确保已安装并配置 msprof 工具
2. 确保环境变量已正确设置（`source ${install_path}/cann/set_env.sh`）
3. 性能数据提取逻辑可能需要根据实际的 msprof 输出格式调整

### 性能数据获取
使用 `msprof` 工具获取详细性能数据：

```bash
msprof op ./demo 1 128 128 128
```

当前目录下会生成以“OPPROF_{timestamp}_XXX”命名的文件夹,性能数据文件夹结构示例如下：

    ```bash
    ├──dump                       # 原始的性能数据，用户无需关注
    ├──ArithmeticUtilization.csv  # cube/vector指令cycle占比
    ├──L2Cache.csv                # L2 Cache命中率
    ├──Memory.csv                 # UB，L1和主存储器读写带宽速率
    ├──MemoryL0.csv               # L0A，L0B，和L0C读写带宽速率
    ├──MemoryUB.csv               # Vector和Scalar到UB的读写带宽速率
    ├──OpBasicInfo.csv            # 算子基础信息
    ├──PipeUtilization.csv        # 采集计算单元和搬运单元耗时和占比
    ├──ResourceConflictRatio.csv  # UB上的 bank group、bank conflict和资源冲突率在所有指令中的占比
    └──visualize_data.bin         # MindStudio Insight呈现文件
    ```

查看具体的性能分析结果：

```bash
cat ./OPPROF_*/PipeUtilization.csv
```

### 性能指标说明

本样例涉及到的性能指标说明如下：
| 指标 | 说明 |
|------|------|
| aic_mte1_time(us) | MTE1（L1 到 L0A/L0B 搬运或L1到Bias Table Buffer搬运）的执行时间 |
| aic_fixpipe_time(us) | FixPipe（L0C 到 GM 搬运或者L1到FixPipe Buffer搬运）的执行时间 |



## 注意事项

1. 场景编号与平台架构必须匹配：a2/a3 平台使用场景 1-9，950 平台使用场景 11-19
2. 场景 7（LoadSparse）仅支持 dav-2201 架构
3. 场景 13/14（Load2DMx）仅支持 dav-3510 架构
4. 不同场景对矩阵规格有不同的对齐要求，建议使用符合对齐要求的矩阵规格（如 16、32、64 的倍数）

