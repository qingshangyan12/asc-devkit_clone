# DataCopy最佳实践样例

## 概述
本样例介绍DataCopy数据搬运的最佳实践，展示GM到UB、GM到L1的数据搬运优化方法，重点演示不同分块策略对搬运性能的影响，并提供核函数直调方法。

## 支持的产品
- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍
```
├── data_copy
│   ├── scripts
│   │   ├── gen_data.py                    // 输入数据和真值数据生成脚本
│   │   └── verify_result.py               // 验证输出数据和真值数据是否一致的验证脚本
│   ├── CMakeLists.txt                     // 编译工程文件
│   ├── data_utils.h                       // 数据读入写出函数
│   ├── data_copy.h                        // DataCopy算子实现
│   └── data_copy.asc                      // Ascend C算子实现 & 调用样例
```

## 样例描述
本样例实现从Global Memory到UB/L1的数据搬运，通过不同的分块策略（TILE_M × TILE_N）展示搬运效率的差异。

### 数据流路径

```
GM ──(MTE2, DataCopyPad/DataCopy)──> UB/L1
```

### 算子规格

**GM到UB搬运（COPY_DST=UB）**：

| Scenario | M | N | NUM_BLOCKS | TILE_M | TILE_N | 说明 |
|----------|---|---|------------|--------|--------|------|
| 1 | 12288 | 12288 | 48 | 1 | 64 | 小分块，频繁搬运 |
| 2 | 12288 | 12288 | 48 | 64 | 64 | 中等分块 |
| 3 | 12288 | 12288 | 48 | 64 | 1024 | 大分块，搬运次数少 |
| 4 | 12288 | 12287 | 48 | 1 | 64 | 非对齐N，小分块 |
| 5 | 12288 | 12287 | 48 | 64 | 64 | 非对齐N，中等分块 |
| 6 | 12288 | 12287 | 48 | 64 | 1024 | 非对齐N，大分块 |

**GM到L1搬运（COPY_DST=L1）**：

| Scenario | M | N | NUM_BLOCKS | TILE_M | TILE_N | 说明 |
|----------|---|---|------------|--------|--------|------|
| 1 | 12288 | 12288 | 48 | 64 | 64 | 对齐N，ND2NZ搬运 |
| 2 | 12288 | 12288 | 48 | 64 | 256 | 对齐N，大分块 |
| 3 | 12288 | 12287 | 48 | 64 | 64 | 非对齐N，ND2NZ搬运 |
| 4 | 12288 | 12287 | 48 | 64 | 256 | 非对齐N，大分块 |

## 样例实现

### 核心特性

#### 1. 分块搬运策略
通过调整TILE_M和TILE_N参数，可以优化搬运效率：
- **小分块（TILE_M=1, TILE_N=64）**：搬运次数多，每次搬运数据量小，适合UB空间受限场景
- **中等分块（TILE_M=64, TILE_N=64）**：平衡搬运次数和数据量
- **大分块（TILE_M=64, TILE_N=1024）**：搬运次数少，数据搬运效率高

#### 2. GM到UB搬运
使用`DataCopyPad`接口，支持非对齐场景（N非64对齐）：

```cpp
AscendC::DataCopyParams copyInParams{
    static_cast<uint16_t>(tileM),
    static_cast<uint16_t>(tileN * sizeof(half)),
    static_cast<uint16_t>(gmStride),
    0};
AscendC::DataCopyPadParams padParams{false, 0, 0, 0};
AscendC::DataCopyPad(ubLocal, srcGlobal[mIdx * n + nIdx], copyInParams, padParams);
```

#### 3. GM到L1搬运
使用`DataCopy`接口配合`Nd2NzParams`参数，实现ND格式到NZ格式的转换：

```cpp
AscendC::Nd2NzParams nd2nzParams;
nd2nzParams.ndNum = 1;
nd2nzParams.nValue = tileM;
nd2nzParams.dValue = curCols;
nd2nzParams.srcNdMatrixStride = 0;
nd2nzParams.srcDValue = n;
nd2nzParams.dstNzC0Stride = AlignUp(tileM, blockCube);
nd2nzParams.dstNzNStride = 1;
nd2nzParams.dstNzMatrixStride = 0;
AscendC::DataCopy(l1Local, srcGlobal[mIdx * n + nIdx], nd2nzParams);
```

#### 4. 多核并行
按M方向均匀切分数据到多核并行搬运：

```cpp
uint32_t blockIdx = AscendC::GetBlockIdx();
uint32_t mStart = blockIdx * singleCoreM;
```

### 性能指标说明

| 指标 | 说明 |
|------|------|
| Task Duration(μs) | 整个任务执行的总时间，算子执行时间以该参数为准 |
| Block Num | 使用的核数（Block数量） |
| aic_mte2_time(μs) | MTE2（GM 到 UB/L1 搬运）的执行时间 |
| aic_mte2_ratio | MTE2 的时间占比，反映数据加载效率 |

### 性能分析

#### 理论搬运数据量
以M=N=12288，数据类型half为例：

$$搬运数据量 = M \times N \times sizeof(half) = 12288 \times 12288 \times 2B = 288MB$$

#### 搬运效率分析
Atlas A2训练系列芯片HBM带宽约1.8TB/s，理论搬运耗时：

$$理论搬运耗时 = \frac{搬运数据量}{HBM带宽} = \frac{288MB}{1.8TB/s} = 160\mu s$$

不同分块策略的搬运效率存在差异：
- 大分块减少MTE2指令发射次数，降低Scalar开销
- 非对齐场景需要额外的边界处理

## 编译运行
在本样例根目录下执行如下步骤，编译并执行算子。

- 配置环境变量  
  请根据当前环境上CANN开发套件包的[安装方式](../../../../../docs/quick_start.md#prepare&install)，选择对应配置环境变量的命令。
  - 默认路径，root用户安装CANN软件包
    ```bash
    source /usr/local/Ascend/cann/set_env.sh
    ```

  - 默认路径，非root用户安装CANN软件包
    ```bash
    source $HOME/Ascend/cann/set_env.sh
    ```

  - 指定路径install_path，安装CANN软件包
    ```bash
    source ${install_path}/cann/set_env.sh
    ```

- 样例执行

  **GM到UB搬运**：
  ```bash
  SCENARIO_NUM=1                                           # 选择执行场景（1-6）
  COPY_DST=UB                                              # 目标存储为UB
  mkdir -p build && cd build;                              # 创建并进入build目录
  cmake -DSCENARIO_NUM=$SCENARIO_NUM -DCMAKE_ASC_RUN_MODE=npu -DCMAKE_ASC_ARCHITECTURES=dav-2201 -DCOPY_DST=$COPY_DST ..;make -j;  # 编译工程
  python3 ../scripts/gen_data.py -scenarioNum $SCENARIO_NUM -copyDst $COPY_DST   # 生成测试输入数据
  ./demo                                                   # 执行
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin  # 验证结果
  ```

  **GM到L1搬运**：
  ```bash
  SCENARIO_NUM=1                                           # 选择执行场景（1-4）
  COPY_DST=L1                                              # 目标存储为L1
  mkdir -p build && cd build;                              # 创建并进入build目录
  cmake -DSCENARIO_NUM=$SCENARIO_NUM -DCMAKE_ASC_RUN_MODE=npu -DCMAKE_ASC_ARCHITECTURES=dav-2201 -DCOPY_DST=$COPY_DST ..;make -j;  # 编译工程
  python3 ../scripts/gen_data.py -scenarioNum $SCENARIO_NUM -copyDst $COPY_DST   # 生成测试输入数据
  ./demo                                                   # 执行
  # 注意：COPY_DST=L1仅演示GM到L1搬运，不产生可校验输出
  ```

  执行结果如下，说明精度对比成功。
  ```bash
  test pass!
  ```

- 编译选项说明

  | 参数 | 说明 | 可选值 | 默认值 |
  |------|------|---------|--------|
  | `SCENARIO_NUM` | 场景编号 | COPY_DST=UB: 1-6<br>COPY_DST=L1: 1-4 | `1` |
  | `COPY_DST` | 目标存储 | `UB`、`L1` | `UB` |
  | `CMAKE_ASC_RUN_MODE` | 运行模式 | `npu`、`sim` | `npu` |
  | `CMAKE_ASC_ARCHITECTURES` | NPU硬件架构 | `dav-2201` | `dav-2201` |

## 性能分析
使用`msprof`工具获取详细性能数据：

```bash
msprof ./demo   # 分析样例性能
```

当前目录下会生成PROF_前缀的文件夹，`mindstudio_profiler_output`目录保存Host和各个Device的性能数据汇总，性能数据分析推荐查看该目录下文件：
```bash
PROF_xxxx_XXXXXX
├── device_{id}
└── host
└── mindstudio_profiler_log
└── mindstudio_profiler_output    # 保存Host和各个Device的性能数据汇总
    ├── msprof_*.json
    ├── xx_*.csv
    └── README.txt
```