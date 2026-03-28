# fixpipe_co12gm 样例

## 概述

本样例介绍如何使用组合API Fixpipe或基础API DataCopy将矩阵乘的结果从L0C搬出到GM，并支持多种随路能力：NZ2ND转换、channel split、量化/反量化、ReLU以及MMAD与Fixpipe的并行（unitFlag）。用户可通过编译选项灵活配置输入输出类型、输出格式、量化模式等，满足不同场景需求。

## 支持的产品

- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构

```
fixpipe_co12gm/
├── CMakeLists.txt
├── README.md
├── data_utils.h
├── include/
│   └── matmul_common.h          // 公共矩阵乘搬入、拆分、计算逻辑
├── src/
│   └── fixpipe_co12gm.asc       // 主核函数，实现Fixpipe搬出
└── scripts/
    ├── gen_data.py               // 统一数据生成入口
    ├── gen_data_base.py          // 普通矩阵乘真值生成
    ├── gen_data_quant.py         // 量化真值生成
    ├── gen_data_dequant.py       // 反量化真值生成
    └── verify_result.py          // 结果验证
```

## 算子功能

本样例实现固定形状[M=128, K=128, N=256]的矩阵乘：C = A * B，其中A形状[M, K]，B形状[K, N]，C形状[M, N]。A、B为ND格式输入，输出C可根据配置输出ND或NZ格式，并支持随路变换。

## 核心能力：Fixpipe 搬出

Fixpipe指令用于将L0C上的计算结果搬出到GM，同时可完成多种随路操作。本样例展示了以下组合：

- **输出格式控制**：
  - `OUTPUT_FORMAT=1`：输出为ND（行主序），自动使能NZ2ND转换。
  - `OUTPUT_FORMAT=2`：输出为NZ（分形），int8输出默认开启channel merge。

- **Channel Split**（`IS_CHANNEL_SPLIT=true`）：
  当输出为float32且为NZ格式时，可将L0C上16x16的小Z分形拆分为两个独立的16x8分形输出，提高带宽利用率。

- **量化/反量化**：
  - `PRE_QUANT_MODE=1`：标量量化，整个C矩阵使用一个量化参数（deqScalar）。
  - `PRE_QUANT_MODE=2`：向量量化，C矩阵的每一列使用一个量化参数（从GM读取）。
  - 支持浮点输出量化到int8（QF322B8_PRE），或int8输入反量化到浮点（DEQF16、DEQF16）。

- **ReLU**（`PRE_RELU_MODE=true`）：使能Norm ReLU随路。

- **UnitFlag并行**（`EN_UNIT_FLAG=true`）：
  使能MMAD与Fixpipe指令的并行执行。此时L0C上的LocalTensor需通过`TBuf`获取，且MMAD与Fixpipe的unitFlag需同时设置为3。

- **基础API DataCopy**（`USE_DATA_COPY=true`）：
  使用DataCopy替代Fixpipe，功能相同但需手动设置更多参数。

## 编译运行

### 环境准备

配置CANN环境变量（参考CANN文档）。

### 编译选项说明

| 选项 | 取值 | 说明 |
|------|------|------|
| INPUT_TYPE | half / int8 | A、B矩阵数据类型 |
| OUTPUT_TYPE | float32 / bfloat16 / float16 / int8 | C矩阵数据类型 |
| OUTPUT_FORMAT | 1 / 2 | 1=ND输出，2=NZ输出 |
| IS_CHANNEL_SPLIT | true / false | 仅当OUTPUT_TYPE=float32且OUTPUT_FORMAT=NZ时有效 |
| PRE_QUANT_MODE | 0 / 1 / 2 | 0=无量化，1=标量量化，2=向量量化 |
| PRE_RELU_MODE | true / false | 使能Norm ReLU |
| EN_UNIT_FLAG | true / false | 使能unitFlag并行 |
| USE_DATA_COPY | true / false | 使用DataCopy代替Fixpipe |

### 示例执行

```bash
# 1. 普通FP16乘法，输出ND格式
mkdir -p build && cd build
cmake .. -DINPUT_TYPE=half -DOUTPUT_TYPE=float32 -DOUTPUT_FORMAT=1
make -j
python3 ../scripts/gen_data.py --input_dtype half --output_dtype float32 --output_format 1
./demo
python3 ../scripts/verify_result.py output/output.bin output/golden.bin

# 2. 使能channel split，输出NZ格式
rm -rf * && cmake .. -DINPUT_TYPE=half -DOUTPUT_TYPE=float32 -DOUTPUT_FORMAT=2 -DIS_CHANNEL_SPLIT=true
make -j
python3 ../scripts/gen_data.py --input_dtype half --output_dtype float32 --output_format 2 --channel_split true
./demo
python3 ../scripts/verify_result.py output/output.bin output/golden.bin

# 3. 量化输出int8，向量量化模式
rm -rf * && cmake .. -DINPUT_TYPE=half -DOUTPUT_TYPE=int8 -DOUTPUT_FORMAT=2 -DPRE_QUANT_MODE=2
make -j
python3 ../scripts/gen_data.py --input_dtype half --output_dtype int8 --output_format 2 --pre_quant_mode 2
./demo
python3 ../scripts/verify_result.py output/output.bin output/golden.bin

# 4. 使能unitFlag并行与DataCopy
rm -rf * && cmake .. -DINPUT_TYPE=int8 -DOUTPUT_TYPE=half -DOUTPUT_FORMAT=1 -DEN_UNIT_FLAG=true -DUSE_DATA_COPY=true
make -j
python3 ../scripts/gen_data.py --input_dtype int8 --output_dtype half --output_format 1 --pre_quant_mode 1 --pre_relu_mode false
./demo
python3 ../scripts/verify_result.py output/output.bin output/golden.bin
```

所有配置下，若验证通过将打印 `test pass!`。

## 注意事项

- 向量量化参数文件 `quant_pre.bin` 仅在 `PRE_QUANT_MODE=2` 时生成和使用。
- 当 `EN_UNIT_FLAG=true` 时，L0C上的Tensor必须通过 `TBuf` 获取，且MMAD与Fixpipe/DataCopy需同时设置 `unitFlag=3`。
- 输出为NZ格式时，`nSize` 需对齐到C0_SIZE（half:16, int8:32），搬出时再恢复原始N值。
- 标量量化的deqScalar需手动构建为uint64格式，包含M1（20位）、offset（9位）、sign（1位）等。