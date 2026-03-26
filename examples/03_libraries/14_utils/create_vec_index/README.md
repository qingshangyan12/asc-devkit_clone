# CreateVecIndex样例

## 概述

本样例介绍了调用基础CreateVecIndex API实现CreateVecIndex算子，采用静态内存版本。支持创建从指定值开始的等差序列（步长为1）。

## 支持的产品

- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍

```
├── create_vec_index
│   ├── scripts
│   │   ├── gen_data.py         // 输入数据和真值数据生成脚本
│   │   └── verify_result.py    // 验证输出数据和真值数据是否一致的验证脚本
│   ├── CMakeLists.txt          // 编译工程文件
│   ├── data_utils.h            // 数据读入写出函数
│   └── create_vec_index.asc    // Ascend C算子实现 & 调用样例（静态内存版本）
```

## 算子描述

本样例提供四种等差序列创建场景：

### 场景1: half类型等差序列

- 算子功能：创建从指定值开始的等差序列
- 输出shape：[size]
- 数据类型：half
- 起始值：0

### 场景2: float类型等差序列

- 算子功能：创建从指定值开始的等差序列
- 输出shape：[size]
- 数据类型：float
- 起始值：0

### 场景3: int32_t类型等差序列

- 算子功能：创建从指定值开始的等差序列
- 输出shape：[size]
- 数据类型：int32_t
- 起始值：10

### 场景4: int16_t类型等差序列

- 算子功能：创建从指定值开始的等差序列
- 输出shape：[size]
- 数据类型：int16_t
- 起始值：5

## 算子实现

### 实现流程

1. 计算：调用CreateVecIndex接口创建等差序列
2. 同步：等待计算完成
3. 数据搬出：将结果搬运到Global Memory

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

### 场景1: half类型等差序列

```bash
mkdir -p build && cd build
cmake -DSCENARIO_NUM=1 -DSIZE=256 ..
make -j
python3 ../scripts/gen_data.py -scenarioNum 1 -size 256
./demo
python3 ../scripts/verify_result.py -scenarioNum 1 output/output.bin output/golden.bin
```

### 场景2: float类型等差序列

```bash
mkdir -p build && cd build
cmake -DSCENARIO_NUM=2 -DSIZE=128 ..
make -j
python3 ../scripts/gen_data.py -scenarioNum 2 -size 128
./demo
python3 ../scripts/verify_result.py -scenarioNum 2 output/output.bin output/golden.bin
```

### 场景3: int32_t类型等差序列

```bash
mkdir -p build && cd build
cmake -DSCENARIO_NUM=3 -DSIZE=128 ..
make -j
python3 ../scripts/gen_data.py -scenarioNum 3 -size 128
./demo
python3 ../scripts/verify_result.py -scenarioNum 3 output/output.bin output/golden.bin
```

### 场景4: int16_t类型等差序列

```bash
mkdir -p build && cd build
cmake -DSCENARIO_NUM=4 -DSIZE=256 ..
make -j
python3 ../scripts/gen_data.py -scenarioNum 4 -size 256
./demo
python3 ../scripts/verify_result.py -scenarioNum 4 output/output.bin output/golden.bin
```

### 执行结果

执行成功后输出：
```bash
test pass!
```
