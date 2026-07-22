# Reg Load Gather样例

## 概述

本样例演示Ascend C API中从UB非连续读取数据到向量寄存器的接口。样例使用`asc_gather`按元素索引读取数据，并使用`asc_gather_datablock`按DataBlock索引读取数据。

## 支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
| --- | --- |
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |

## 目录结构

```text
reg_load_gather/
├── CMakeLists.txt                 # 编译配置
├── data_utils.h                   # 输入输出文件工具
├── reg_load_gather.asc            # Ascend C API实现
├── README.md                      # 中文说明
├── README_en.md                   # English documentation
└── scripts/
    ├── gen_data.py                # 生成输入和golden数据
    └── verify_result.py           # 校验输出数据
```

## 场景说明

输入数据类型为`half`，源数据长度为1024，输出数据长度为128。

| 场景 | 接口 | 索引类型 | 说明 |
| --- | --- | --- | --- |
| 1 | `asc_gather` | `uint16_t[128]` | 索引表示源数据中的元素下标，按元素读取128个数据。 |
| 2 | `asc_gather_datablock` | `uint32_t[8]` | 索引表示源数据中的32B DataBlock编号，读取8个DataBlock。 |

## 编译与运行

在样例目录下执行以下命令。`SCENARIO_NUM`取值为1或2，默认为1。

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
| `SCENARIO_NUM` | 选择运行场景，可取1或2。 | 1 |
| `CMAKE_ASC_ARCHITECTURES` | 指定NPU架构，仅支持`dav-3510`。 | dav-3510 |
| `CMAKE_ASC_RUN_MODE` | 运行模式，可取`npu`或`sim`。 | npu |

运行成功时，程序输出`test pass!`，校验脚本输出`verification passed`。
