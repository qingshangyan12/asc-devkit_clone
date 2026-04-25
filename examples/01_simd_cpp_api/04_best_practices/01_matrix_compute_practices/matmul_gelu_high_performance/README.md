# Matmul Gelu最佳实践样例

## 概述

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 目录结构介绍



## 样例描述


- 样例规格：


## 样例实现

### 性能指标说明

| 指标 | 说明 |
|------|------|
| Task Duration(μs) | 整个任务执行的总时间，算子执行时间以该参数为准 |
| Block Num | 使用的核数（Block数量） |
| aicore_time(μs) | AI Core 的平均执行时间 |
| aic_mac_time(μs) | Cube 计算单元的执行时间 |
| aic_mac_ratio | Cube 计算单元的时间占比，反映计算单元利用率 |
| aic_mte1_time(μs) | MTE1（L1 到 L0A/L0B 搬运）的执行时间 |
| aic_mte1_ratio | MTE1 的时间占比，反映 L1 到 L0 的数据搬运压力 |
| aic_mte2_time(μs) | MTE2（GM 到 L1 搬运）的执行时间 |
| aic_mte2_ratio | MTE2 的时间占比，反映 GM 到 L1 的数据加载压力 |
| aic_fixpipe_time(μs) | FixPipe（L0C 到 GM 搬运）的执行时间 |
| aic_fixpipe_ratio | FixPipe 的时间占比，反映结果写回的访存压力 |




### 核心特性



### 性能数据分析


#### Ascend 950PR芯片性能数据


### 理论性能分析

#### Cube 计算性能分析

#### MTE2 带宽分析


## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。

### 配置环境变量

请根据当前环境上 CANN 开发套件包的[安装方式](../../../../../docs/quick_start.md#prepare&install)，选择对应配置环境变量的命令。

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

### 样例执行

```bash
SCENARIO=1                                                           # 选择执行场景（1为不使能L2Cache切分，2为使能L2Cache切分）
mkdir -p build && cd build;                                          # 创建并进入 build 目录
cmake -DSCENARIO_NUM=$SCENARIO -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j;  # 编译工程（默认npu模式）
python3 ../scripts/gen_data.py                                       # 生成测试输入数据
./demo                                                               # 执行编译生成的可执行程序
python3 ../scripts/verify_result.py output/output.bin output/golden.bin
```

使用 NPU仿真 模式时，添加 `-DCMAKE_ASC_RUN_MODE=sim` 参数即可。

示例：
```bash
cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j;   # NPU 仿真模式
```

> **注意：** 切换编译模式前需清理 cmake 缓存，可在 build 目录下执行 `rm CMakeCache.txt` 后重新 cmake。

### 编译选项说明

| 参数 | 说明 | 可选值 | 默认值 |
|------|------|---------|--------|
| `SCENARIO_NUM` | `1` / `2` | 1: 不使能L2Cache切分；2: 使能L2Cache切分 | `1` |
| `CMAKE_ASC_RUN_MODE` | 运行模式 | `npu`、`sim` | `npu` |
| `CMAKE_ASC_ARCHITECTURES` | NPU 硬件架构 | `dav-2201`、`dav-3510` | `dav-2201` |

执行结果如下，说明精度对比成功。
```bash
test pass!
```

### 性能分析

使用 `msprof` 工具获取详细性能数据：

```bash
msprof ./demo   # 分析样例性能
```

当前目录下会生成 PROF_ 前缀的文件夹，`mindstudio_profiler_output` 目录保存 Host 和各个 Device 的性能数据汇总，性能数据分析推荐查看该目录下文件：
```bash
PROF_xxxx_XXXXXX
├── device_{id}
└── host
└── mindstudio_profiler_log
└── mindstudio_profiler_output    # 保存 Host 和各个 Device 的性能数据汇总
    ├── msprof_*.json
    ├── xx_*.csv
    └── README.txt
```
