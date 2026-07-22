# Reg Load Store Align样例

## 概述

本样例演示Ascend C API中UB与向量寄存器之间的对齐读写接口。输入为两个长度为1024的`half`数组，向量计算执行逐元素加法；不同场景使用不同的寄存器读写寻址和数据分布模式。

## 支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
| --- | --- |
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |

## 目录结构

```text
reg_load_store_align/
├── scripts/
│   ├── gen_data.py                // 生成输入和golden数据
│   └── verify_result.py           // 校验输出数据
├── CMakeLists.txt                 // 编译配置
├── data_utils.h                   // 输入输出文件工具
├── reg_load_store_align.asc       // Ascend C API实现和调用入口
├── README.md                      // 中文说明
└── README_en.md                   // English documentation
```

## 场景详细说明

所有场景均在一个AIV核上运行。输入数据先由MTE2从GM搬运至UB，Vector流水执行寄存器加载、加法和存储操作，最后由MTE3将输出从UB搬运至GM。

| 场景 | 寄存器读写方式 | 输出长度 | 说明 |
| --- | --- | --- | --- |
| 1 | 偏移量 | 1024 | 通过显式地址偏移读写1021个元素，最后3个元素保持为0。 |
| 2 | PostUpdate | 1024 | 使用PostUpdate模式在每次读写后更新地址。 |
| 3 | AddrReg | 1024 | 使用地址寄存器配置每次读写的偏移。 |
| 4 | DataBlock | 512 | 每个32B DataBlock中读取前16B数据并连续写出。 |
| 5 | BRC B16 | 1024 | 将每个256B数据段的第一个`half`元素广播到一个向量。 |
| 6 | Upsample B16 | 2048 | 将每个输入`half`元素重复一次后写出。 |

## 编译与运行

### 配置环境变量

请根据当前环境中CANN开发套件包的安装方式配置环境变量：

```bash
source ${install_path}/cann/set_env.sh
```

> **说明：** `${install_path}`为CANN包安装目录；root用户默认为`/usr/local/Ascend`，非root用户默认为`${HOME}/Ascend`。

### 运行样例

在样例根目录执行以下命令。`SCENARIO_NUM`可取1到6，默认值为1。

```bash
SCENARIO_NUM=1                                                                    # 选择场景1
mkdir -p build && cd build;                                                        # 创建并进入build目录
cmake -DSCENARIO_NUM=$SCENARIO_NUM -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j; # 编译工程，默认NPU模式
python3 ../scripts/gen_data.py -scenarioNum=$SCENARIO_NUM                         # 生成测试输入数据和golden数据
./demo                                                                             # 执行样例
python3 ../scripts/verify_result.py output/output.bin output/golden.bin           # 校验输出数据与golden数据
```

默认使用NPU运行模式。使用NPU仿真模式时，增加`-DCMAKE_ASC_RUN_MODE=sim`：

```bash
cmake -DSCENARIO_NUM=$SCENARIO_NUM -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j; # NPU仿真模式
```

切换场景或运行模式前，请在`build`目录删除`CMakeCache.txt`后重新执行CMake，避免复用缓存配置。

### 编译选项

| 选项 | 可选值 | 说明 |
| --- | --- | --- |
| `SCENARIO_NUM` | `1`-`6` | 选择寄存器对齐读写场景。 |
| `CMAKE_ASC_RUN_MODE` | `npu`、`sim` | 选择NPU运行或NPU仿真模式。 |
| `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | 指定NPU架构，对应Ascend 950PR/Ascend 950DT。 |

## 运行结果

输出结果与golden数据一致时，校验脚本输出如下信息：

```bash
test pass!
```
