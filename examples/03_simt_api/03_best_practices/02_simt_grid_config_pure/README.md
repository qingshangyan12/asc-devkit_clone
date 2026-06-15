体现一下一个核一个block
# gridDim配置与UB Stage样例（Pure SIMT版）

## 概述

本样例以Gather+Adds融合计算为例，介绍Ascend C **Pure SIMT编程** 模型下的gridDim配置与UB（Unified Buffer）共享内存使用思路。样例包含4个空壳核函数场景（测试不同核数下的调度头开销）以及2个实际计算场景（对比核数匹配前后的性能），从核数远超物理核的基础场景，逐步优化至核数匹配的最优场景。

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 目录结构介绍

```text
├── 02_simt_grid_config
│   ├── CMakeLists.txt         // 编译工程文件
│   ├── grid_config.asc        // 样例实现
│   └── README.md
```

## 样例描述

- 计算公式：

  $$
  output[i][j] = input[index[i][j]] + 1.0
  $$

  - input为输入张量，形状为[1100000]，数据类型为float
  - index为索引张量，形状为[1024, 1024]，数据类型为uint32，索引范围为[0, 1100000)
  - output为输出张量，形状为[1024, 1024]，数据类型为float
  - 计算过程：根据index[i][j]索引从input中采集对应位置的数据，加1.0后写入output[i][j]位置

- 样例规格：

  <table>
  <tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="4" align="center">GatherAdds</td></tr>
  <tr><td rowspan="2" align="center">样例输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">input</td><td align="center">[1100000]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="2" align="center">样例输入</td><td align="center">index</td><td align="center">[1024, 1024]</td><td align="center">uint32</td><td align="center">ND</td></tr>
  <tr><td align="center">output</td><td align="center">[1024, 1024]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">empty_kernel / gather_adds_kernel</td></tr>
  </table>

## 样例实现

### Case实现说明

本样例通过6个独立场景展示不同的gridDim配置策略。

| Case   | 实现特点                            | 使用的核函数        | 优化特性                          |
| ------ | ----------------------------------- | ------------------- | --------------------------------- |
| Case 1 | 36核，空壳核函数                    | empty_kernel        | 调度基线（核数 < 物理核）         |
| Case 2 | 64核（物理核数），空壳核函数        | empty_kernel        | 调度基线（核数 = 物理核）         |
| Case 3 | 128核，空壳核函数                   | empty_kernel        | 调度基线（核数 > 物理核）         |
| Case 4 | 1024核，空壳核函数                  | empty_kernel        | 调度基线（核数远超物理核）        |
| Case 5 | 1024核，每核1024数据，gather+adds   | gather_adds_kernel  | 计算基线（核数远超物理核）        |
| Case 6 | 64核（物理核数），每核16384数据，gather+adds | gather_adds_kernel | 核数匹配物理核（最优配置）|

#### 核心概念：逻辑核与物理核

在SIMT编程模型中：

- **物理核**：硬件实际拥有的物理核数，本样例目标平台Ascend 950PR/Ascend 950DT的AIV核数为64
- **逻辑核**：通过gridDim配置启动的逻辑核数量，即`gridDim.x`
- **核数匹配原则**：当逻辑核数量小于等于物理核数量时，各核可并行执行；当逻辑核数量超过物理核数量时，多余逻辑核需要串行等待执行

本样例通过Case 1-4建立调度开销基线，Case 5与Case 6对比1024核与64核两种配置下的实际计算性能。

#### 运行期查询物理核数

样例中 `AIV_CORE_NUM = 64` 是按目标平台 Ascend 950PR/950DT 写死的教学常量。在实际工程中，建议通过 AscendCL Runtime API 在运行期向硬件查询，以避免硬编码：

```cpp
#include "acl/acl.h"

int32_t device_id = 0;
int64_t physical_aiv_core_num = 0;
aclError ret = aclrtGetDeviceInfo(
    static_cast<uint32_t>(device_id),
    ACL_DEV_ATTR_VECTOR_CORE_NUM,
    &physical_aiv_core_num);
if (ret == ACL_SUCCESS) {
    printf("Physical AIV (Vector Core) num: %ld\n", physical_aiv_core_num);
}
```

调用前需先完成 `aclInit` 与 `aclrtSetDevice`。`aclrtDevAttr` 常用枚举：

| 枚举值                          | 含义                |
| ------------------------------- | ------------------- |
| `ACL_DEV_ATTR_AICORE_CORE_NUM`  | AI Core 数量        |
| `ACL_DEV_ATTR_VECTOR_CORE_NUM`  | Vector Core (AIV) 数量 |
| `ACL_DEV_ATTR_AICPU_CORE_NUM`   | AI CPU 数量         |

接口文档：
- [aclrtGetDeviceInfo / acl.rt.get_device_info](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/900/API/runtimeapi/aclpythondevg_01_1029.html)
- [aclrtDevAttr 枚举](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/900/API/runtimeapi/aclpythondevg_01_1111.html)

#### 核心概念：动态UB（Unified Buffer）

Pure SIMT中，每个thread block拥有独立的Unified Buffer（共享内存）。本样例的`gather_adds_kernel`通过动态UB接收每核的工作集：

```cpp
extern __ubuf__ float ub_scratch[];
```

实际大小由核函数启动时的第三槽 `dyn_ubuf_size` 在运行期决定：

```cpp
uint32_t dyn_ubuf_size = (index_total_length / num_blocks) * sizeof(float);
gather_adds_kernel<<<num_blocks, THREAD_NUM, dyn_ubuf_size, stream>>>(...);
```

每核UB占用 = `rows_per_block * sizeof(float)`：
- Case 5（1024核）：`1024 * 4 = 4KB`
- Case 6（64核）：`16384 * 4 = 64KB`

均在UB容量约束（256KB - 静态UB - 8KB预留 ≥ 32KB Data Cache下限）之内。

### Case 1-4：空壳核函数测试场景

**样例目标**：建立调度开销基线，测试不同核数下纯空壳核函数的Task调度耗时

**核心实现**：

- 核函数 `empty_kernel` 函数体为空
- 4个场景对应 36 / 64 / 128 / 1024 四种核数配置
- Host侧先通过 `aclrtMemcpy(DEVICE_TO_DEVICE)` 把 input 拷贝到 output（保证精度校验通过），再启动空 kernel 仅测调度开销

**关键代码**：

```cpp
__global__ __launch_bounds__(THREAD_NUM) void empty_kernel(
    float* input, uint32_t* index, float* output,
    uint32_t input_total_length, uint32_t index_total_length)
{
}
```

### Case 5-6：Gather+Adds计算场景

**样例目标**：通过对比1024核与64核两种gridDim配置，展示核数匹配物理核对性能的影响

**核心实现**：

| 配置项               | Case 5             | Case 6             |
| -------------------- | ------------------ | ------------------ |
| 逻辑核数             | 1024（远超物理核） | 64（等于物理核）   |
| 每核处理数据量       | 1024               | 16384              |
| 每核UB占用           | 4KB                | 64KB               |

两个场景共用同一个核函数 `gather_adds_kernel`，通过 stride 模式自适应每核工作集：每条线程在 `[threadIdx.x, rows_per_block)` 区间按 `THREAD_NUM` 跨步处理，循环展开为"全 gather+add → 全 store"两阶段。

**关键代码**：

```cpp
__global__ __launch_bounds__(THREAD_NUM) void gather_adds_kernel(
    float* input, uint32_t* index, float* output,
    uint32_t input_total_length, uint32_t index_total_length)
{
    extern __ubuf__ float ub_scratch[];

    uint32_t rows_per_block = index_total_length / gridDim.x;
    uint32_t index_start = blockIdx.x * rows_per_block;

    // 阶段1: gather + 加常数 → UB
    for (uint32_t local_idx = threadIdx.x; local_idx < rows_per_block; local_idx += THREAD_NUM) {
        uint32_t gather_idx = index[index_start + local_idx];
        if (gather_idx >= input_total_length) { gather_idx = 0; }
        ub_scratch[local_idx] = input[gather_idx] + ADDS_ADDEND;
    }

    // 阶段2: UB → GM
    for (uint32_t local_idx = threadIdx.x; local_idx < rows_per_block; local_idx += THREAD_NUM) {
        output[index_start + local_idx] = ub_scratch[local_idx];
    }
}
```

每条线程只读写自己负责的 `ub_scratch[local_idx]` 槽位，无跨线程通信，因此**不需要 `asc_syncthreads()`**。

**Host 侧 launch 配置**：

```cpp
uint32_t dyn_ubuf_size = (index_total_length / num_blocks) * sizeof(float);
gather_adds_kernel<<<num_blocks, THREAD_NUM, dyn_ubuf_size, stream>>>(
    input_device, index_device, output_device,
    input_total_length, index_total_length);
```

**预期优化收益**：

- **Case 5 → Case 6**：核数从 1024 降到 64 匹配物理核，消除多余逻辑核的串行等待开销，每核工作集从 1024 增加到 16384，调度头开销摊薄到更多有效计算上

**优化手段**：

将逻辑核数调整为 64，匹配物理核数，各核并行执行。

## 调优建议

1. **核数匹配物理核**：逻辑核数量应尽量匹配物理核数量，避免过多逻辑核串行执行带来的调度开销
2. **避免无效同步**：每条线程只访问自己 UB 槽位时，不应调用 `asc_syncthreads()`；只有跨线程读写同一 UB 区间时才需要同步
3. **UB空间规划**：动态UB大小由launch第三槽决定，需确保静态UB + 动态UB + 预留8KB之后剩余的Data Cache不小于32KB

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。

- 配置环境变量
  请根据当前环境上CANN开发套件包的[安装方式](../../../../docs/quick_start.md#prepare&install)，选择对应配置环境变量的命令。

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

  ```bash
  SCENARIO_NUM=5                       # 选择执行场景，可选1-6
  mkdir -p build && cd build;          # 创建并进入build目录
  cmake -DSCENARIO_NUM=$SCENARIO_NUM ..;make -j;  # 编译工程
  ./grid_config                        # 执行样例
  ```

- 编译选项说明

  | 选项           | 可选值   | 说明              |
  | -------------- | -------- | ----------------- |
  | `SCENARIO_NUM` | `1`-`6`  | 样例类型，默认为1 |

  执行结果如下，说明精度对比成功。

  ```text
  [Success] Case accuracy is verification passed.
  ```

## 性能分析

使用 `msprof` 工具获取详细性能数据：

```bash
msprof ./grid_config   # 分析性能
```

当前目录下会生成PROF_前缀的文件夹，`mindstudio_profiler_output`目录保存Host和各个Device的性能数据汇总，性能数据分析推荐查看该目录下文件

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

查看具体的性能分析结果：

```bash
# 查看Task Duration 以及各项数据
cat ./PROF_*/mindstudio_profiler_output/op_summary_*.csv
```
