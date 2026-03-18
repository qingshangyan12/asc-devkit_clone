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

### 地址分配策略

```cpp
constexpr uint32_t srcAddr = 0;
constexpr uint32_t offsetAddr = srcAddr + size * sizeof(T) * 2 + 256;  // 256字节对齐
constexpr uint32_t dstAddr = offsetAddr + size * sizeof(U) + 256;
```

## 编译运行

在本样例根目录下执行如下步骤，编译并执行算子。

### 配置环境变量

```bash
source /usr/local/Ascend/cann/set_env.sh
```

### 编译执行

```bash
mkdir -p build && cd build
cmake -DSCENARIO_NUM=1 -DSIZE=16384 ..
make -j
python3 ../scripts/gen_data.py -scenarioNum 1
./demo
python3 ../scripts/verify_result.py -scenarioNum 1 output/output.bin output/golden.bin
```

### 执行结果

```bash
test pass!
```

## 与动态内存版本对比

| 特性 | 动态内存版本 | 静态内存版本 (本样例) |
|------|-------------|---------------------|
| 内存管理 | TPipe + TQue | 静态 LocalTensor |
| 编程风格 | 类封装 | 函数式模板 |
| 地址分配 | 运行时动态 | 编译时静态 |
| 同步机制 | 队列自动同步 | SetFlag/WaitFlag |
