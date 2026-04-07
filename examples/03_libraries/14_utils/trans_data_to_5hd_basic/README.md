# TransDataTo5HD Basic样例

## 概述

本样例介绍了调用基础TransDataTo5HD API实现数据格式转换算子，采用静态内存版本。支持将数据进行5HD格式转换，常用于卷积神经网络的数据布局转换。

## 支持的产品

- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍

```
├── trans_data_to_5hd_basic
│   ├── scripts
│   │   ├── gen_data.py         // 输入数据和真值数据生成脚本
│   │   └── verify_result.py    // 验证输出数据和真值数据是否一致的验证脚本
│   ├── CMakeLists.txt          // 编译工程文件
│   ├── data_utils.h            // 数据读入写出函数
│   └── trans_data_to_5hd_basic.asc  // Ascend C算子实现 & 调用样例（静态内存版本）
```

## 算子描述

本样例提供四种数据格式转换场景：

### 场景1: half类型数据转换

- 算子功能：将数据进行5HD格式转换
- 输入shape：[256]（16块 × 16元素/块）
- 输出shape：[256]
- 数据类型：half

### 场景2: float类型数据转换

- 算子功能：将数据进行5HD格式转换
- 输入shape：[128]（16块 × 8元素/块）
- 输出shape：[128]
- 数据类型：float

### 场景3: int8_t类型数据转换

- 算子功能：将数据进行5HD格式转换
- 输入shape：[512]（16块 × 32元素/块）
- 输出shape：[512]
- 数据类型：int8_t
- 特殊说明：b8类型需要两次scatter完成完整转置
  - 第一次: srcHighHalf=false, dstHighHalf=false (处理前半部分)
  - 第二次: srcHighHalf=true, dstHighHalf=true (处理后半部分)

### 场景4: NCHW → NC1HWC0 格式转换

- 算子功能：将NCHW格式数据转换为NC1HWC0格式
- 输入shape：[N=2, C=32, H=16, W=16]
- 输出shape：[N=2, C1=2, H=16, W=16, C0=16]
- 数据类型：half

## 算子实现

### 实现流程

1. 数据搬入：将Global Memory数据搬运到LocalTensor
2. 同步：等待数据搬运完成
3. 计算：调用TransDataTo5HD接口执行格式转换
4. 同步：等待计算完成
5. 数据搬出：将结果搬运到Global Memory

## 编译运行

在本样例根目录下执行如下步骤，编译并执行算子。

### 配置环境变量

请根据当前环境上CANN开发套件包的安装方式，选择对应配置环境变量的命令。

```bash
# 默认路径，root用户安装CANN软件包
source /usr/local/Ascend/cann/set_env.sh

# 或指定路径安装
source ${install_path}/cann/set_env.sh
```

### 场景1: half类型数据转换

```bash
mkdir -p build && cd build
cmake -DSCENARIO_NUM=1 ..
make -j
python3 ../scripts/gen_data.py -scenarioNum 1
./demo
python3 ../scripts/verify_result.py -scenarioNum 1 output/output.bin output/golden.bin
```

### 场景2: float类型数据转换

```bash
mkdir -p build && cd build
cmake -DSCENARIO_NUM=2 ..
make -j
python3 ../scripts/gen_data.py -scenarioNum 2
./demo
python3 ../scripts/verify_result.py -scenarioNum 2 output/output.bin output/golden.bin
```

### 场景3: int8_t类型数据转换

```bash
mkdir -p build && cd build
cmake -DSCENARIO_NUM=3 ..
make -j
python3 ../scripts/gen_data.py -scenarioNum 3
./demo
python3 ../scripts/verify_result.py -scenarioNum 3 output/output.bin output/golden.bin
```

### 场景4: NCHW → NC1HWC0 格式转换

```bash
mkdir -p build && cd build
cmake -DSCENARIO_NUM=4 ..
make -j
python3 ../scripts/gen_data.py -scenarioNum 4
./demo
python3 ../scripts/verify_result.py -scenarioNum 4 output/output.bin output/golden.bin
```

### 执行结果

执行成功后输出：
```bash
test pass!
```
