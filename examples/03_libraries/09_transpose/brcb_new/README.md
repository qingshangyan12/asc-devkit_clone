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

### 外部配置参数

本样例支持通过 CMake 参数和 Python 脚本参数进行外部配置：

| 参数名 | 类型 | 说明 |
|-------|------|------|
| SIZE | uint32_t | 输入数据大小 |
| REPEAT_TIME | uint8_t | 重复次数 |
| DST_BLK_STRIDE | uint16_t | 目的数据块步长 |
| DST_REP_STRIDE | uint16_t | 目的重复步长 |

各场景默认参数：

| 场景 | SIZE | REPEAT_TIME | DST_BLK_STRIDE | DST_REP_STRIDE |
|-----|------|-------------|----------------|----------------|
| 1 | 64 | 8 | 1 | 8 |
| 2 | 64 | 8 | 2 | 16 |
| 3 | 1024 | 128 | 1 | 8 |
| 4 | 64 | 8 | 1 | 8 |
| 5 | 1024 | 128 | 1 | 8 |

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
cmake -DSCENARIO_NUM=1 -DSIZE=64 -DREPEAT_TIME=8 -DDST_BLK_STRIDE=1 -DDST_REP_STRIDE=8 ..
make -j
python3 ../scripts/gen_data.py -scenarioNum 1
./demo
python3 ../scripts/verify_result.py -scenarioNum 1 output/output.bin output/golden.bin
```

或使用自定义参数：

```bash
# 自定义参数编译
cmake -DSCENARIO_NUM=1 -DSIZE=128 -DREPEAT_TIME=16 -DDST_BLK_STRIDE=2 -DDST_REP_STRIDE=16 ..
make -j
python3 ../scripts/gen_data.py -scenarioNum 1 -size 128 -repeatTime 16 -dstBlkStride 2 -dstRepStride 16
./demo
python3 ../scripts/verify_result.py -scenarioNum 1 output/output.bin output/golden.bin
```

### 执行结果

```bash
test pass!
```

