# Gather New样例

## 概述

本样例介绍了调用Gather API实现Gather算子，采用静态内存版本。支持多种Gather模式和场景。

## 支持的产品

- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍

```
├── gather_new
│   ├── scripts
│   │   ├── gen_data.py         // 输入数据和真值数据生成脚本
│   │   └── verify_result.py    // 验证输出数据和真值数据是否一致的验证脚本
│   ├── CMakeLists.txt          // 编译工程文件
│   ├── data_utils.h            // 数据读入写出函数
│   └── gather_new.asc          // Ascend C算子实现 & 调用样例（静态内存版本）
```

## 算子描述

### 算子功能
Gather算子根据偏移量从源数据中收集数据。

### 支持的场景

| 场景 | 模式 | 数据类型 | 说明 |
|-----|------|---------|------|
| 1 | USE_COUNT_MODE | half | Count模式 |
| 2 | USE_CONTINUOUS_MODE_OF_MASK | half | 连续Mask模式 |
| 3 | USE_BITWISE_MASK_MODE | half | 逐比特Mask模式 |
| 4 | USE_COUNT_MODE | half | 大数据量测试 half |
| 5 | USE_COUNT_MODE | float | 大数据量测试 float |

### API说明

```cpp
// Count模式
template <typename T, typename U>
__aicore__ inline void Gather(LocalTensor<T>& dst, LocalTensor<T>& src,
                               LocalTensor<U>& offset, uint32_t baseAddr, uint32_t count);

// Mask连续模式
template <typename T, typename U>
__aicore__ inline void Gather(LocalTensor<T>& dst, LocalTensor<T>& src,
                               LocalTensor<U>& offset, uint32_t baseAddr,
                               uint32_t count, uint16_t repeatTimes, uint8_t dstRepStride);

// Mask逐比特模式
template <typename T, typename U>
__aicore__ inline void Gather(LocalTensor<T>& dst, LocalTensor<T>& src,
                               LocalTensor<U>& offset, uint32_t baseAddr,
                               uint64_t mask[2], uint16_t repeatTimes, uint8_t dstRepStride);
```

## 实现特点

### 静态内存版本特点

- **静态 LocalTensor**：通过 `TPosition::VECCALC` 和固定地址
- **无动态内存管理**：不使用 TQue 的 AllocTensor/FreeTensor
- **事件同步**：使用 `SetFlag/WaitFlag` 进行同步
- **函数式风格**：使用模板函数而非类封装

### 外部配置参数

本样例支持通过 CMake 参数和 Python 脚本参数进行外部配置：

| 参数名 | 类型 | 说明 |
|-------|------|------|
| SIZE | uint32_t | 缓冲区大小（用于静态内存分配） |
| CALC_SIZE | uint32_t | 计算数据大小 |

各场景默认参数：

| 场景 | SIZE | CALC_SIZE |
|-----|------|-----------|
| 1-3 | 160 | 128 |
| 4 | 16384 | 16384 |
| 5 | 10240 | 10240 |

## 编译运行

在本样例根目录下执行如下步骤，编译并执行算子。

### 配置环境变量

```bash
source /usr/local/Ascend/cann/set_env.sh
```

### 编译执行

```bash
mkdir -p build && cd build
# 使用默认参数编译
cmake -DSCENARIO_NUM=1 -DSIZE=160 -DCALC_SIZE=128 ..
make -j
python3 ../scripts/gen_data.py -scenarioNum 1
./demo
python3 ../scripts/verify_result.py -scenarioNum 1 output/output.bin output/golden.bin
```

或使用自定义参数：

```bash
# 自定义参数编译
cmake -DSCENARIO_NUM=1 -DSIZE=288 -DCALC_SIZE=256 ..
make -j
python3 ../scripts/gen_data.py -scenarioNum 1 -calcSize 256
./demo
python3 ../scripts/verify_result.py -scenarioNum 1 output/output.bin output/golden.bin
```

### 执行结果

```bash
test pass!
```

## 性能数据
 	 
样例采用msprof工具测试性能数据  

```
基于完整样例编译生成可执行程序，执行以下命令，通过msprof工具获取上板profiling数据。  
```shell
cd build
msprof ./demo
```
在执行上述命令后，会在对应目录下生成op_summary_xxx.csv文件，其中aiv_vec_time代表vec类型指令耗时。
- 单次指令平均耗时(单位: us)： time = aiv_vec_time / REPEAT_TIMES_FOR_PERF_TEST
- 单词指令平均耗时(单位: cycles)： time * frequence（硬件频率）  

| 数据类型 | SIZE（元素数） | 理论性能（cycles） | 实测性能（cycles） |
|:------------:|:------------:|:--------------:|:-----------------:|
| half      | 16K          | 3277         | 3039               |
| float     | 10K          | 2048         | 1964               |

> **说明**：性能数据基于典型硬件环境测得，实际结果可能因硬件、驱动版本等因素有所差异。