# Transpose Basic样例

## 概述

本样例介绍了调用基础Transpose API实现Transpose算子，采用静态内存版本。支持2D矩阵转置和4D张量转置（NCHW/NHWC互转）。

## 支持的产品

- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍

```
├── transpose_basic
│   ├── scripts
│   │   ├── gen_data.py         // 输入数据和真值数据生成脚本
│   │   └── verify_result.py    // 验证输出数据和真值数据是否一致的验证脚本
│   ├── CMakeLists.txt          // 编译工程文件
│   ├── data_utils.h            // 数据读入写出函数
│   └── transpose_basic.asc     // Ascend C算子实现 & 调用样例（静态内存版本）
```

## 算子描述

本样例提供三种转置场景：

### 场景1: 2D矩阵转置

- 算子功能：16x16矩阵转置，dst[i][j] = src[j][i]
- 输入shape：[16, 16]
- 输出shape：[16, 16]
- 数据类型：half

### 场景2: NCHW转NHWC

- 算子功能：4D张量格式转换 [N, C, H, W] -> [N, H, W, C]
- 输入shape：[3, 3, 2, 8] (N=3, C=3, H=2, W=8)
- 输出shape：[3, 2, 8, 3]
- 数据类型：half

### 场景3: NHWC转NCHW

- 算子功能：4D张量格式转换 [N, H, W, C] -> [N, C, H, W]
- 输入shape：[3, 2, 8, 3] (N=3, H=2, W=8, C=3)
- 输出shape：[3, 3, 2, 8]
- 数据类型：half

## 算子实现

### 实现流程

1. 数据搬入：将Global Memory数据搬运到LocalTensor
2. 同步：等待数据搬运完成
3. 计算：调用Transpose接口执行转置
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

### 场景1: 2D矩阵转置

```bash
mkdir -p build && cd build
cmake -DSCENARIO_NUM=1 -DSIZE=256 ..
make -j
python3 ../scripts/gen_data.py -scenarioNum 1
./demo
python3 ../scripts/verify_result.py -scenarioNum 1 output/output.bin output/golden.bin
```

### 场景2: NCHW转NHWC

```bash
mkdir -p build && cd build
cmake -DSCENARIO_NUM=2 -DSIZE=144 ..
make -j
python3 ../scripts/gen_data.py -scenarioNum 2
./demo
python3 ../scripts/verify_result.py -scenarioNum 2 output/output.bin output/golden.bin
```

### 场景3: NHWC转NCHW

```bash
mkdir -p build && cd build
cmake -DSCENARIO_NUM=3 -DSIZE=144 ..
make -j
python3 ../scripts/gen_data.py -scenarioNum 3
./demo
python3 ../scripts/verify_result.py -scenarioNum 3 output/output.bin output/golden.bin
```

### 执行结果

执行成功后输出：
```bash
test pass!
```

