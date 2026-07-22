# Reg Load Store Mask样例

## 概述

本样例演示Ascend C API中MaskReg的加载、存储和使用方式。场景1从UB加载MaskReg后执行掩码复制和MaskReg存储；场景2按MaskReg位选择两个`float`输入中的一个元素写出。

## 支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
| --- | --- |
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |

## 目录结构

```text
reg_load_store_mask/
├── CMakeLists.txt                 # 编译配置
├── data_utils.h                   # 输入输出文件工具
├── reg_load_store_mask.asc        # Ascend C API实现
├── README.md                      # 中文说明
├── README_en.md                   # English documentation
├── figures/
│   └── reg_load_store_mask.png    # 场景示意图
└── scripts/
    ├── gen_data.py                # 生成输入和golden数据
    └── verify_result.py           # 校验输出数据
```

![MaskReg操作示意图](./figures/reg_load_store_mask.png)

## 场景说明

| 场景 | 数据类型和长度 | 说明 |
| --- | --- | --- |
| 1 | `uint8_t[1024]` | 从输入的前32B加载MaskReg；掩码位置写入值2，并将MaskReg写入输出的第256B开始位置。 |
| 2 | `float[256]`和`uint8_t[128]`掩码 | 每次处理64个`float`元素，按MaskReg位从两个输入中选择元素并写出。 |

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
