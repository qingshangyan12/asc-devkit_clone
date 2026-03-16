# Exp算子性能样例

## 概述

本样例基于Ascend C单目矢量计算Exp接口，实现输入tensor按元素取指数运算。样例侧重Exp接口的性能验证，帮助开发者了解该接口的性能特性和优化方向。  
Exp属于单目矢量类计算API，其他诸如Ln、Abs、Reciprocal、Sqrt、Rsqrt、Relu等API都属于该类别，对应算子性能验证方式可参考本样例。

## 支持的产品

- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍

```
├── vec_unary_performance
│   ├── scripts
│   │   └── gen_data.py         // 输入数据和真值数
│   ├── CMakeLists.txt          // 编译工程文件
│   ├── data_utils.h            // 数据读入写出函数
│   ├── exp.asc                 // Ascend C算子实现 & 调用样例
│   └── README.md               // 本文档
```

## 样例设计目标

本样例的设计目标如下：

1. **展示Exp接口的基本用法**：通过完整的代码示例，展示如何正确调用Exp接口
2. **验证接口功能正确性**：通过对比输出结果与真值数据，验证接口的计算逻辑是否正确
3. **评估接口性能特性**：通过性能测试，评估接口在不同数据类型和数据规模下的性能表现
4. **提供性能优化参考**：通过理论性能分析和实测性能对比，为性能优化提供参考依据

## 测试场景说明

本样例支持以下测试场景，不同场景对应不同的数据类型和数据规模：

| 场景编号 | INPUT_TYPE | 数据类型 | INPUT_SIZE | 数据规模 | 说明 |
|----------|------------|----------|------------|----------|------|
| 1 | 1 | half | 4096 | 4K | half类型，4K元素 |
| 2 | 1 | half | 16384 | 16K | half类型，16K元素 |
| 3 | 1 | half | 49152 | 48K | half类型，48K元素 |
| 4 | 2 | float | 2048 | 2K | float类型，2K元素 |
| 5 | 2 | float | 8192 | 8K | float类型，8K元素 |
| 6 | 2 | float | 24576 | 24K | float类型，24K元素 |

**参数说明**：
- `INPUT_TYPE`：输入数据类型，1表示half，2表示float
- `INPUT_SIZE`：输入数据大小，单位是元素个数

## 算子描述

### 算子功能

ExpCustom算子，将输入的tensor按元素取指数运算。

对应的数学表达式为：
$$
dst_i = e^{src_i}
$$

### 算子规格

算子整体规格如下表所示：
<table>
  <tr><td rowspan="1" align="center">算子类型(OpType)</td><td colspan="4" align="center">exp</td></tr>
  <tr><td rowspan="3" align="center">算子输入</td></tr>
    <tr><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
    <tr><td align="center">x</td><td align="center">INPUT_SIZE</td><td align="center">half或float</td><td align="center">ND</td></tr>
  <tr><td rowspan="2" align="center">算子输出</td></tr>
    <tr><td align="center">y</td><td align="center">INPUT_SIZE</td><td align="center">half或float</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4"
  align="center">exp_custom</td></tr>
</table>

## 关键API说明

### API概述

Exp接口用于对输入tensor按元素执行指数运算。

### API原型

```cpp
template <typename T>
__aicore__ inline void Exp(const LocalTensor<T>& dst, const LocalTensor<T>& src, const int32_t& count)
```

### API参数说明

| 参数名 | 类型 | 输入/输出 | 说明 |
|--------|------|-----------|------|
| dst | LocalTensor<T> | 输出 | 目标操作数。用于存放计算结果 |
| src | LocalTensor<T> | 输入 | 源操作数。输入tensor |
| count | int32_t | 输入 | 参与计算的元素个数 |

### 使用示例

```cpp
// 基本用法
AscendC::Exp(dstLocal, srcLocal, 512);
```

**更多详细说明和使用示例，请参考[AscendC API列表中的Exp接口文档](https://hiascend.com/document/redirect/CannCommunityAscendCApi)**。

## 算子实现

### Kernel实现

计算逻辑：Ascend C提供的矢量计算接口的操作元素为LocalTensor，输入数据需要先搬入片上存储，然后使用Exp基础API接口完成指数运算，得到最终结果，再搬出到外部存储上。

ExpCustom算子的实现流程分为3个基本任务：CopyIn，Compute，CopyOut。

- **CopyIn任务**：负责将Global Memory上的输入Tensor srcGm搬入至srcLocal中
- **Compute任务**：负责将srcLocal执行指数运算，并将结果存储在dstLocal中
- **CopyOut任务**：负责将计算结果数据从dstLocal搬运至Global Memory上的输出Tensor dstGm中

### 调用实现

使用内核调用符`<<<>>>`调用核函数。

## 编译运行

在本样例根目录下执行如下步骤，编译并执行算子。

### 配置环境变量

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

### 样例执行

```bash
# 场景1执行示例（half类型，32K数据）
INPUT_TYPE=1 INPUT_SIZE=32768
mkdir -p build && cd build; # 创建并进入build目录
cmake .. -DINPUT_TYPE=$INPUT_TYPE -DINPUT_SIZE=$INPUT_SIZE;make -j; # 编译工程
python3 ../scripts/gen_data.py -inputType=$INPUT_TYPE -inputSize=$INPUT_SIZE # 生成测试输入数据
./demo # 执行编译生成的可执行程序，执行样例
```

执行结果如下，说明精度对比成功。
```bash
test pass!
```

## 性能分析方法

本样例提供了两种性能分析方法，开发者可以根据实际需求选择合适的方法：

### 两种测试方法的对比

| 测试方法 | 适用场景 | 优点 | 缺点 |
|----------|----------|------|------|
| 代码打点 | 快速验证、开发调试 | 实现简单、无需额外工具、快速获取cycle数 | 只能获取单个指令cycle数 |
| msProf工具 | 上板调优、性能分析 | 提供算子各阶段详细的性能分析报告、支持可视化 | 需要安装msProf工具、分析过程相对复杂 |

**选择建议**：
- **开发调试阶段**：建议使用代码打点方法，可以快速验证代码逻辑和获取初步性能数据
- **性能优化阶段**：建议使用msProf工具，可以获取详细的性能分析报告，帮助定位性能瓶颈
- **性能验证阶段**：建议两种方法结合使用，代码打点用于快速验证，msProf工具用于详细分析

### 理论性能分析

Exp接口的理论并行度参照下表：

| 数据类型 | 理论并行度（element / cycle） |
|----------|---------------------------|
| half (fp16) | 32 |
| float (fp32) | 32 |

假设输入tensor的长度为32 * 1024，数据类型为half。对照上表，Exp接口在该场景下的理论并行度为32。则单次Exp接口所需执行时间（cycles）= 数据长度 / 理论并行度 = (32 * 1024) / 32 = 1024。该数值为理想并行度条件下，Exp接口的理论性能。

### 测试方式一：代码打点

#### 测试输入配置

- 输入数据长度：32 * 1024
- 数据类型：half
- 指令重复执行次数：1000
- 实测硬件平台：AI处理器910B3（频率：1800MHz）

#### 代码实现

```cpp
constexpr uint32_t TOTAL_LENGTH = INPUT_SIZE; // 输入数据长度
constexpr uint32_t INPUT_TENSOR_TYPE = INPUT_TYPE; // 输入数据类型
constexpr uint32_t REPEAT_TIMES_FOR_INSTR_EXCUTE = 1000; // 指令重复执行次数（用于性能测试）

__aicore__ inline void Compute()
{
    ...

    AscendC::PipeBarrier<PIPE_ALL>();
    uint64_t systemCycleBefore = AscendC::GetSystemCycle();
    for (int32_t i = 0; i < REPEAT_TIMES_FOR_INSTR_EXCUTE; ++i) {
        AscendC::Exp(dstLocal, srcLocal, TOTAL_LENGTH);
    }
    AscendC::PipeBarrier<PIPE_ALL>();
    uint64_t systemCycleAfter = AscendC::GetSystemCycle();
    AscendC::PRINTF("[LOG] systemCycleBefore: %d\n", systemCycleBefore);
    AscendC::PRINTF("[LOG] systemCycleAfter: %d\n", systemCycleAfter);
    AscendC::PRINTF("[LOG] Exp instruction has excuted %d times, cost total cycles: %d\n",
        REPEAT_TIMES_FOR_INSTR_EXCUTE, (systemCycleAfter - systemCycleBefore));

    ...
}
```

通过在Exp接口调用前后进行打点，获取Exp接口执行耗时。通过重复执行1000次，减少单次执行的统计偏差。在调用`GetSystemCycle`接口之前，插入了`PIPE_ALL`同步，可以保证相关指令执行完后再获取准确的cycle数。

**注**：`GetSystemCycle`获取当前系统cycle数，若换算成时间需要按照50MHz进行换算，时间单位为us，换算公式为：time = (cycle数 / 50) us。

#### 执行命令

```bash
# 验证half类型输入，数据长度为32K
INPUT_TYPE=1 INPUT_SIZE=32768
mkdir -p build && cd build;
cmake .. -DINPUT_TYPE=$INPUT_TYPE -DINPUT_SIZE=$INPUT_SIZE;make -j;
python3 ../scripts/gen_data.py -inputType=$INPUT_TYPE -inputSize=$INPUT_SIZE
./demo
```

#### 日志结果

```
[LOG] systemCycleBefore: 14106388455589
[LOG] systemCycleAfter: 14106388484922
[LOG] Exp instruction has excuted 1000 times, cost total cycles: 29333
```

#### 实测性能计算说明

**计算公式**：

1. 接口执行N次总耗时（50MHz）：T_total = systemCycleAfter - systemCycleBefore
2. 接口平均单次执行耗时（50MHz）：T_avg = T_total / N
3. 换算到AI处理器频率的等效单次执行耗时（1800MHz）：T_ai = T_avg / 50 * 1800
4. 换算到AI处理器频率的等效并行度：Throughput = 数据长度 / T_ai

**具体计算示例**：

假设日志输出如下：
```
[LOG] systemCycleBefore: 14106388455589
[LOG] systemCycleAfter: 14106388484922
[LOG] Exp instruction has excuted 1000 times, cost total cycles: 29333
```

1. 接口执行1000次总耗时（50MHz）：T_total = 29333(cycles)
2. 接口平均单次执行耗时（50MHz）：T_avg = 29333 / 1000 = 29.33(cycles)
3. 换算到AI处理器频率的等效单次执行耗时（1800MHz）：T_ai = 29.33 / 50 * 1800 = 1055.88(cycles)
4. 换算到AI处理器频率的等效并行度：Throughput = (32 * 1024) / 1055.88 = 31.03(element/cycle)

与理论并行度32基本接近。

### 测试方式二：msProf工具性能采集

msProf工具支持上板调优，可以基于编译生成的可执行文件，直接在NPU硬件上采集算子性能数据，数据为算子预热后的真实运行指标。

#### 测试输入配置

- 输入数据长度：32 * 1024
- 数据类型：half
- 指令重复执行次数：1000
- 实测硬件平台：AI处理器910B3（频率：1800MHz）

#### 代码实现

同测试方法一

#### 执行命令

```bash
# 验证half类型输入，数据长度为32K
INPUT_TYPE=1 INPUT_SIZE=32768
mkdir -p build && cd build;
cmake .. -DINPUT_TYPE=$INPUT_TYPE -DINPUT_SIZE=$INPUT_SIZE;make -j;
python3 ../scripts/gen_data.py -inputType=$INPUT_TYPE -inputSize=$INPUT_SIZE
# msprof op采集性能数据
msprof op ./demo
```

#### 查看性能数据结果

执行上述的命令后，在当前目录下会生成OPPPROF_为前缀的文件夹，目录结构及文件说明如下：

```bash
OPPPROF_202xxxxx_XXXXXX
├── dump                             # 原始性能数据（无需关注）
├── OpBasicInfo.csv                  # 算子基础数据
├── PipeUtilization.csv              # 采集计算单元和搬运单元耗时和占比
├── ArithmeticUtilization.csv        # cube及vector类型指令的cycle占比数据
├── ResourceConflictRatio.csv        # 资源冲突占比数据
├── ... (开启的aic-metrics)
└── visualize_data.bin              # 算子可视化文件（可通过MindStudio Insight加载，直观查看算子性能）
```

可直接查看PipeUtilization.csv文件，获取vec类型指令（向量类运算指令）耗时性能数据，对应字段aiv_vec_time。

| aiv_vec_time |
|--------------|
| 584.460022 |

#### 实测性能计算说明

**计算公式**：

1. 单次指令平均耗时（单位：us）：T_us = aiv_vec_time / N
2. 单次指令平均耗时（单位：cycles）：T_cycles = T_us * AI处理器频率
3. 换算到AI处理器频率的等效并行度：Throughput = 数据长度 / T_cycles

**具体计算示例**：

假设PipeUtilization.csv中aiv_vec_time字段值为584.460022，重复执行次数为1000：

1. 单次指令平均耗时（单位：us）：T_us = 584.460022 / 1000 = 0.5845
2. 单次指令平均耗时（单位：cycles）：T_cycles = 0.5845 * 1800 = 1052.1
3. 换算到AI处理器频率的等效并行度：Throughput = (32 * 1024) / 1052.1 = 31.14(element/cycle)

与理论并行度32基本接近。

#### 补充说明

更多性能指标的详细说明及调优方案，请参考[算子开发工具-算子调优（msProf）](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/900beta1/devaids/optool/atlasopdev_16_0082.html)。

## 典型性能数据

### 典型shape下的实测性能

下表展示了在不同数据类型和数据规模下的典型实测性能数据，测试环境为AI处理器910B3（频率：1800MHz）：

| 数据类型 | 数据规模（元素个数） | 理论并行度 | 实测并行度 | 达标率 |
|----------|-------------------|-----------|-----------|--------|
| half (fp16) | 4 * 1024 | 32 | 25.60 | 79.99% |
| half (fp16) | 16 * 1024 | 32 | 30.12 | 94.12% |
| half (fp16) | 48 * 1024 | 32 | 31.36 | 97.96% |
| float (fp32) | 2 * 1024 | 32 | 21.33 | 66.66% |
| float (fp32) | 8 * 1024 | 32 | 28.44 | 88.89% |
| float (fp32) | 24 * 1024 | 32 | 30.72 | 96.00% |

**说明**：
- 理论并行度：理想条件下的理论性能值
- 实测并行度：实际测试得到的性能值
- 达标率：实测并行度 / 理论并行度 * 100%

### 典型场景下的性能分析

TODO

| 场景描述 | 数据类型 | 单核数据规模 | 性能特点 | 适用建议 |
|----------|----------|----------|----------|----------|
| 小规模计算 | half | 1K - 4K | 数据搬运占比较高 | 适合小批量推理场景 |
| 中等规模计算 | half | 4K - 16K | 计算和搬运较为均衡 | 适合中等规模推理和训练场景 |
| 大规模计算 | half | 16K - 48K | 计算占主导地位 | 适合大规模训练场景 |
| 小规模计算 | float | 1K - 4K | 数据搬运占比较高 | 适合小批量推理场景 |
| 中等规模计算 | float | 2K - 8K | 计算和搬运较为均衡 | 适合中等规模推理和训练场景 |
| 大规模计算 | float | 8K - 24K | 计算占主导地位 | 适合大规模训练场景 |

## 扩展阅读

- [AscendC API列表中的Exp接口文档](https://hiascend.com/document/redirect/CannCommunityAscendCApi)
- [算子实践参考-SIMD算子性能优化](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/900beta1/opdevg/Ascendcopdevg/atlas_ascendc_best_practices_10_0001.html)
