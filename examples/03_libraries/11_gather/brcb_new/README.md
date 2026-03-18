# Brcb New样例

## 概述

本样例介绍了调用Brcb API实现Brcb算子，采用静态内存版本。Brcb算子从源数据中取8个数据，将每个数据广播成一个32B块，然后连续写入目的地址。

## 支持的产品

- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍

```
├── brcb_new
│   ├── scripts
│   │   ├── gen_data.py         // 输入数据和真值数据生成脚本
│   │   └── verify_result.py    // 验证输出数据和真值数据是否一致的验证脚本
│   ├── CMakeLists.txt          // 编译工程文件
│   ├── data_utils.h            // 数据读入写出函数
│   └── brcb_new.asc            // Ascend C算子实现 & 调用样例（静态内存版本）
```

## 算子描述

### 算子功能
Brcb（Broadcast Block）算子从源操作数中取8个b16/b32数据，将每个数据广播成一个32B块，然后将这8个块连续写入目的地址。

### 支持的场景

| 场景 | 数据类型 | 说明 |
|-----|---------|------|
| 1 | half | 基础测试 |
| 2 | half | 大步长测试 |
| 3 | half | 大数据量测试 |
| 4 | float | 基础测试 |
| 5 | float | 大数据量测试 |

### API说明

```cpp
template <typename T>
__aicore__ inline void Brcb(const LocalTensor<T>& dst, const LocalTensor<T>& src0,
                              const uint8_t repeatTime, const BrcbRepeatParams& repeatParams);
```

### BrcbRepeatParams参数

```cpp
struct BrcbRepeatParams {
    uint16_t dstBlkStride;   // 目的数据块步长
    uint16_t dstRepStride;   // 目的重复步长
};
```

## 实现特点

### 静态内存版本特点

- **静态 LocalTensor**：通过 `TPosition::VECCALC` 和固定地址
- **无动态内存管理**：不使用 TQue 的 AllocTensor/FreeTensor
- **事件同步**：使用 `SetFlag/WaitFlag` 进行同步
- **函数式风格**：使用模板函数而非类封装
- **编译时参数**：使用 constexpr 函数获取场景参数

### 场景参数定义

```cpp
struct BrcbScenarioParams {
    uint32_t srcSize;        // 输入数据大小
    uint32_t dstSize;        // 输出数据大小
    uint8_t repeatTime;      // 重复次数
    uint16_t dstBlkStride;   // 目的数据块步长
    uint16_t dstRepStride;   // 目的重复步长
};
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
| 场景参数 | 运行时获取 | constexpr 编译时计算 |
