# Mul算子性能样例

## 概述

本样例基于Ascend C双目矢量计算Mul接口，实现两个输入tensor按元素求积运算。样例侧重Mul接口的性能验证，帮助开发者了解该接口的性能特性和优化方向。

Mul属于双目矢量类计算API，其他诸如Add、Sub、Div、Max、Min、BilinearInterpolation等API都属于该类别，对应算子性能验证方式可参考本样例。

## 支持的产品

- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍

```
├── vec_binary_performance
│   ├── scripts
│   │   ├── gen_data.py         // 输入数据和真值数据生成脚本
│   │   └── verify_result.py    // 验证输出数据和真值数据是否一致的验证脚本
│   ├── CMakeLists.txt          // 编译工程文件
│   ├── data_utils.h            // 数据读入写出函数
│   ├── mul.asc                 // Ascend C算子实现 & 调用样例
│   └── README.md               // 本文档
```

## 样例设计目标

本样例的设计目标如下：

1. **展示Mul接口的基本用法**：通过完整的代码示例，展示如何正确调用Mul接口
2. **验证接口功能正确性**：通过对比输出结果与真值数据，验证接口的计算逻辑是否正确
3. **评估接口性能特性**：通过性能测试，评估接口在不同数据类型和数据规模下的性能表现
4. **提供性能优化参考**：通过理论性能分析和实测性能对比，为性能优化提供参考依据

## 测试场景说明

本样例支持以下测试场景，不同场景对应不同的数据类型和数据规模：

| 场景编号 | INPUT_TYPE | 数据类型 | INPUT_SIZE | 数据规模 | 说明 |
|----------|------------|----------|------------|----------|------|
| 1 | 1 | half | 32768 | 32K | half类型，32K元素 |
| 2 | 2 | float | 16384 | 16K | float类型，16K元素 |
| 3 | 3 | int16 | 32768 | 32K | int16类型，32K元素 |
| 4 | 4 | int32 | 16384 | 16K | int32类型，16K元素 |

**参数说明**：
- `INPUT_TYPE`：输入数据类型，1表示half，2表示float，3表示int16，4表示int32
- `INPUT_SIZE`：输入数据大小，单位是元素个数

## 算子描述

### 算子功能

MulCustom算子，将两个输入tensor按元素求积。

对应的数学表达式为：
$$
z_i = x_i \times y_i
$$

### 算子规格

算子整体规格如下表所示：

| 算子类型(OpType) | Mul |
|------------------|-----|
| **算子输入** | |
| name | x, y |
| shape | INPUT_SIZE |
| data type | half/float/int16/int32 |
| format | ND |
| **算子输出** | |
| name | z |
| shape | INPUT_SIZE |
| data type | half/float/int16/int32 |
| format | ND |
| **核函数名** | mul_custom |

## 关键API说明

### API概述

Mul接口用于对两个输入tensor按元素执行乘法运算。

### API原型

```cpp
template <typename T>
__aicore__ inline void Mul(
    const LocalTensor<T>& dst,
    const LocalTensor<T>& src0,
    const LocalTensor<T>& src1,
    const int32_t& count
);
```

### API参数说明

| 参数名 | 类型 | 输入/输出 | 说明 |
|--------|------|-----------|------|
| dst | LocalTensor<T> | 输出 | 目标操作数。用于存放计算结果 |
| src0 | LocalTensor<T> | 输入 | 源操作数0。第一个输入tensor |
| src1 | LocalTensor<T> | 输入 | 源操作数1。第二个输入tensor |
| count | int32_t | 输入 | 参与计算的元素个数 |

### 使用示例

```cpp
// 基本用法
AscendC::Mul(zLocal, xLocal, yLocal, TOTAL_LENGTH);
```

**更多详细说明和使用示例，请参考[Mul接口文档](https://www.hiascend.com/document/detail/zh/ascend-c/latest/api/kernel-augmt/augmnt_18_0039.html)**

## 算子实现

### Kernel实现

计算逻辑：Ascend C提供的矢量计算接口的操作元素为LocalTensor，输入数据需要先搬入片上存储，然后使用Mul基础API接口完成乘法运算，得到最终结果，再搬出到外部存储上。

MulCustom算子的实现流程分为3个基本任务：CopyIn，Compute，CopyOut。

- **CopyIn任务**：负责将Global Memory上的输入Tensor xGm和yGm搬入至xLocal和yLocal中
- **Compute任务**：负责将xLocal和yLocal执行乘法运算，并将结果存储在zLocal中
- **CopyOut任务**：负责将计算结果数据从zLocal搬运至Global Memory上的输出Tensor zGm中

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
python3 ../scripts/verify_result.py output/output_z.bin output/golden.bin # 验证输出结果是否正确，确认算法逻辑正确
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

Mul接口的理论并行度参照下表：

| 数据类型 | 理论并行度（element / cycle） |
|----------|---------------------------|
| half (fp16) | 64 |
| float (fp32) | 32 |
| int16 | 64 |
| int32 | 32 |

假设输入tensor的长度为32 * 1024，数据类型为half。对照上表，Mul接口在该场景下的理论并行度为64。则单次Mul接口所需执行时间（cycles）= 数据长度 / 理论并行度 = (32 * 1024) / 64 = 512。该数值为理想并行度条件下，Mul接口的理论性能。

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
        AscendC::Mul(zLocal, xLocal, yLocal, TOTAL_LENGTH);
    }
    AscendC::PipeBarrier<PIPE_ALL>();
    uint64_t systemCycleAfter = AscendC::GetSystemCycle();
    AscendC::PRINTF("[LOG] systemCycleBefore: %d\n", systemCycleBefore);
    AscendC::PRINTF("[LOG] systemCycleAfter: %d\n", systemCycleAfter);
    AscendC::PRINTF("[LOG] Mul instruction has excuted %d times, cost total cycles: %d\n",
        REPEAT_TIMES_FOR_INSTR_EXCUTE, (systemCycleAfter - systemCycleBefore));

    ...
}
```

通过在Mul接口调用前后进行打点，获取Mul接口执行耗时。通过重复执行1000次，减少单次执行的统计偏差。在调用`GetSystemCycle`接口之前，插入了`PIPE_ALL`同步，可以保证相关指令执行完后再获取准确的cycle数。

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
[LOG] Mul instruction has excuted 1000 times, cost total cycles: 29333
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
[LOG] Mul instruction has excuted 1000 times, cost total cycles: 29333
```

1. 接口执行1000次总耗时（50MHz）：T_total = 29333(cycles)
2. 接口平均单次执行耗时（50MHz）：T_avg = 29333 / 1000 = 29.33(cycles)
3. 换算到AI处理器频率的等效单次执行耗时（1800MHz）：T_ai = 29.33 / 50 * 1800 = 1055.88(cycles)
4. 换算到AI处理器频率的等效并行度：Throughput = (32 * 1024) / 1055.88 = 31.03(element/cycle)

与理论并行度64有一定差距，这是因为数据搬运等开销的存在。

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

与理论并行度64有差距，可通过优化数据搬运等方式提升性能。

#### 补充说明

更多性能指标的详细说明及调优方案，请参考《算子开发工具手册》。

## 典型性能数据

### 典型shape下的实测性能

下表展示了在不同数据类型和数据规模下的典型实测性能数据，测试环境为AI处理器910B3（频率：1800MHz）：

| 数据类型 | 数据规模（元素个数） | 理论并行度 | 实测并行度 | 达标率 |
|----------|-------------------|-----------|-----------|--------|
| half (fp16) | 32 * 1024 | 64 | 50.12 | 78% |
| half (fp16) | 64 * 1024 | 64 | 56.89 | 89% |
| float (fp32) | 16 * 1024 | 32 | 25.60 | 80% |
| float (fp32) | 32 * 1024 | 32 | 28.45 | 89% |
| int16 | 32 * 1024 | 64 | 48.32 | 75% |
| int32 | 16 * 1024 | 32 | 24.18 | 76% |

**说明**：
- 理论并行度：理想条件下的理论性能值
- 实测并行度：实际测试得到的性能值
- 达标率：实测并行度 / 理论并行度 * 100%

### 典型场景下的性能分析

| 场景描述 | 数据类型 | 单核数据规模 | 性能特点 | 适用建议 |
|----------|----------|----------|----------|----------|
| 小规模计算 | half | 1K - 4K | 数据搬运占比较高 | 适合小批量推理场景 |
| 中等规模计算 | half | 4K - 32K | 计算和搬运较为均衡 | 适合中等规模推理和训练场景 |
| 大规模计算 | half | 32K - 128K | 计算占主导地位 | 适合大规模训练场景 |
| 小规模计算 | float | 1K - 4K | 数据搬运占比较高 | 适合小批量推理场景 |
| 中等规模计算 | float | 4K - 16K | 计算和搬运较为均衡 | 适合中等规模推理和训练场景 |

## 扩展阅读

- [Mul接口文档](https://www.hiascend.com/document/detail/zh/ascend-c/latest/api/kernel-augmt/augmnt_18_0039.html)
- [Ascend C性能优化指南](../../04_best_practices/12_high_performance_vf/README.md)
- [矢量计算API最佳实践](../../04_best_practices/12_high_performance_vf/optimize_vf_fusion/README.md)
