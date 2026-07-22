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
├── CMakeLists.txt                 # 编译配置
├── data_utils.h                   # 输入输出文件工具
├── reg_load_store_align.asc       # Ascend C API实现
├── README.md                      # 中文说明
├── README_en.md                   # English documentation
└── scripts/
    ├── gen_data.py                # 生成输入和golden数据
    └── verify_result.py           # 校验输出数据
```

## 场景说明

| 场景 | 寄存器读写方式 | 输出长度 | 说明 |
| --- | --- | --- | --- |
| 1 | 偏移量 | 1024 | 通过显式地址偏移读写1021个元素，最后3个元素保持为0。 |
| 2 | PostUpdate | 1024 | 使用PostUpdate模式在每次读写后更新地址。 |
| 3 | AddrReg | 1024 | 使用地址寄存器配置每次读写的偏移。 |
| 4 | DataBlock | 512 | 每个32B DataBlock中读取前16B数据并连续写出。 |
| 5 | BRC B16 | 1024 | 将每个256B数据段的第一个`half`元素广播到一个向量。 |
| 6 | Upsample B16 | 2048 | 将每个输入`half`元素重复一次后写出。 |

## 编译与运行

在样例目录下执行以下命令。`SCENARIO_NUM`取值为1到6，默认为1。

```bash
export SCENARIO_NUM=1
mkdir -p build && cd build
cmake -DSCENARIO_NUM=${SCENARIO_NUM} -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..
make -j
cd ..
python3 scripts/gen_data.py -scenarioNum=${SCENARIO_NUM}
./build/demo
python3 scripts/verify_result.py output/output.bin output/golden.bin
```

默认使用NPU运行模式。使用NPU仿真模式时，在CMake命令中增加`-DCMAKE_ASC_RUN_MODE=sim`：

```bash
cmake -DSCENARIO_NUM=${SCENARIO_NUM} -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DCMAKE_ASC_RUN_MODE=sim ..
```

切换场景或运行模式后，请删除`build`目录或重新执行CMake，避免复用缓存配置。

## 编译选项

| 选项 | 说明 | 默认值 |
| --- | --- | --- |
| `SCENARIO_NUM` | 选择运行场景，可取1到6。 | 1 |
| `CMAKE_ASC_ARCHITECTURES` | 指定NPU架构，仅支持`dav-3510`。 | dav-3510 |
| `CMAKE_ASC_RUN_MODE` | 运行模式，可取`npu`或`sim`。 | npu |

运行成功时，程序输出`test pass!`，校验脚本输出`verification passed`。
