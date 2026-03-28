# fixpipe_co12l1 样例

## 概述

本样例介绍如何使用组合API Fixpipe或基础API DataCopy将矩阵乘的结果从L0C搬出到L1（随路量化/反量化、ReLU），再由L1搬出到GM，支持多种数据类型和配置。适用于需要两级搬移的场景，例如在L1上做进一步处理后再输出。

## 支持的产品

- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构

```
fixpipe_co12l1/
├── CMakeLists.txt
├── README.md
├── data_utils.h
├── include/
│   └── matmul_common_l1.h   // 公共矩阵乘搬入、拆分、计算逻辑
├── src/
│   └── fixpipe_co12l1.asc   // 主核函数，实现两级搬出
└── scripts/
    ├── gen_data.py           // 统一数据生成入口
    ├── gen_data_base.py      // 普通矩阵乘真值生成
    ├── gen_data_quant.py     // 量化真值生成（浮点->整型）
    ├── gen_data_dequant.py   // 反量化真值生成（整型->浮点）
    └── verify_result.py      // 结果验证
```

## 算子功能

固定形状[M=128, K=128, N=256]的矩阵乘 C = A * B，其中A、B为ND格式输入，输出为NZ格式。支持以下随路能力：

- **量化/反量化**（`PRE_QUANT_MODE`）：
  - `0`：无量化，直接输出（仅当输出类型与L0C类型一致时有效，本样例不直接支持）
  - `1`：标量量化/反量化，整个矩阵使用一个量化参数
  - `2`：向量量化/反量化，每列使用一个量化参数

- **ReLU**（`PRE_RELU_MODE`）：使能Norm ReLU随路

- **UnitFlag**（`EN_UNIT_FLAG`）：使能MMAD与Fixpipe/DataCopy并行

- **DataCopy替代Fixpipe**（`USE_DATA_COPY`）：使用基础API DataCopy代替组合API Fixpipe

## 编译运行

### 环境准备

配置CANN环境变量。

### 编译选项说明

| 选项 | 取值 | 说明 |
|------|------|------|
| INPUT_TYPE | half / int8 | A、B矩阵数据类型 |
| OUTPUT_TYPE | int8 / half | C矩阵数据类型（NZ格式） |
| PRE_QUANT_MODE | 0 / 1 / 2 | 0=无，1=标量，2=向量 |
| PRE_RELU_MODE | true / false | 使能Norm ReLU |
| EN_UNIT_FLAG | true / false | 使能unitFlag并行 |
| USE_DATA_COPY | true / false | 使用DataCopy代替Fixpipe |

### 示例执行

```bash
# 1. FP16输入，量化输出int8（标量量化）
mkdir -p build && cd build
cmake .. -DINPUT_TYPE=half -DOUTPUT_TYPE=int8 -DPRE_QUANT_MODE=1
make -j
python3 ../scripts/gen_data.py --input_dtype half --output_dtype int8 --pre_quant_mode 1
./demo
python3 ../scripts/verify_result.py output/output.bin output/golden.bin int8

# 2. INT8输入，反量化输出half（向量量化，使能ReLU）
cmake .. -DINPUT_TYPE=int8 -DOUTPUT_TYPE=half -DPRE_QUANT_MODE=2 -DPRE_RELU_MODE=true
make -j
python3 ../scripts/gen_data.py --input_dtype int8 --output_dtype half --pre_quant_mode 2 --pre_relu_mode
./demo
python3 ../scripts/verify_result.py output/output.bin output/golden.bin half

# 3. 使能unitFlag与DataCopy
cmake .. -DINPUT_TYPE=int8 -DOUTPUT_TYPE=int8 -DPRE_QUANT_MODE=1 -DEN_UNIT_FLAG=true -DUSE_DATA_COPY=true
make -j
python3 ../scripts/gen_data.py --input_dtype int8 --output_dtype int8 --pre_quant_mode 1
./demo
python3 ../scripts/verify_result.py output/output.bin output/golden.bin int8
```

验证通过打印 `test pass!`。

## 注意事项

- 向量量化参数文件 `quant_pre.bin` 仅在 `PRE_QUANT_MODE=2` 时生成和使用。
- 当 `EN_UNIT_FLAG=true` 时，L0C上的Tensor必须通过 `TBuf` 获取，且MMAD与Fixpipe/DataCopy需同时设置 `unitFlag=3`。
- 输出格式固定为NZ（分形），int8输出默认开启channel merge。
- 两级搬移中，Fixpipe/DataCopy负责L0C→L1，然后手动DataCopy将L1数据搬到GM。