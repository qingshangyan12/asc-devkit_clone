# fixpipe_co12c1样例

## 概述

本样例介绍如何使用组合API Fixpipe或基础API DataCopy将矩阵乘的结果从L0C搬出到L1，然后再从L1搬出到GM，并支持unitFlag、随路量化与Relu能力组合。

完整的矩阵乘法流程涉及四个阶段的数据搬运和计算：
1. **GM → L1**：通过DataCopy随路转换ND2NZ指令，将A、B矩阵从GM搬运到L1，同时完成ND到NZ格式的转换
2. **L1 → L0A/L0B**：通过LoadData系列指令，将A、B矩阵从L1搬运到L0A/L0B，完成NZ到ZZ/ZN格式的转换
3. **L0A × L0B → L0C**：通过Mmad指令执行矩阵乘法计算，结果存储在L0C中
4. **L0C → L1 → GM**：通过Fixpipe或DataCopy指令将结果从L0C搬出到L1，再通过DataCopy指令从L1搬出到GM，支持unitFlag、随路量化、Relu等操作

本样例重点介绍**阶段4：L0C → L1 → GM**的Fixpipe和DataCopy的使用方法。

## 支持的产品

- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍

```
├── fixpipe_co12c1
│   ├── scripts
│   │   ├── gen_data_f322bf16.py      // F32 Cast到 BF16 数据生成脚本
│   │   ├── gen_data_f322f16.py       // F32 Cast到F16 数据生成脚本
│   │   ├── gen_data_f322s8.py        // F32 量化到 S8数据生成脚本
│   │   ├── gen_data_s322f16.py       // S32 量化到 F16数据生成脚本
│   │   ├── gen_data_s322s8.py        // S32 量化到 S8数据生成脚本
│   │   └── verify_result.py          // 验证输出数据和真值数据是否一致的验证脚本
│   ├── CMakeLists.txt                // 编译工程文件
│   ├── data_utils.h                  // 数据读入写出函数
│   ├── cube_common.h                 // 矩阵搬入和计算通用头文件
│   ├── fixpipe_co12c1.asc            // 矩阵搬出实现样例
│   └── run_example.sh                // 样例执行脚本
```

## 样例描述

### 样例功能

本样例中实现了 [M, N, K] 固定为 [128, 256, 128] 的矩阵乘法，对应的数学表达式为：
```
C = A * B
```
其中A的形状为 [M, K], B的形状为 [K, N], C的形状为 [M, N]。

### 样例规格

<table>
<tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="4" align="center">矩阵乘法</td></tr>
<tr><td rowspan="3" align="center">样例输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
<tr><td align="center">A</td><td align="center">[128, 128]</td><td align="center">half/int8_t</td><td align="center">ND</td></tr>
<tr><td align="center">B</td><td align="center">[128, 256]</td><td align="center">half/int8_t</td><td align="center">ND</td></tr>
<tr><td rowspan="1" align="center">样例输出</td><td align="center">C</td><td align="center">[128, 256]</td><td align="center">half/bfloat16_t/int8_t</td><td align="center">NZ</td></tr>
<tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">fixpipe_co12c1</td></tr>
</table>

### 功能配置说明

本样例支持配置使用AscendC提供的组合API Fixpipe或基础API DataCopy与寄存器配置实现矩阵从L0C搬出到L1，并支持FIXPIPE指令多种随路能力组合：

| 配置项 | 取值 | 说明 |
|--------|------|------|
| QUANT_MODE | 1 | F322BF16，half输入，L0C上数据类型为float，bfloat16_t输出 |
| QUANT_MODE | 2 | F322F16，half输入，L0C上数据类型为float，half输出 |
| QUANT_MODE | 3 | (V)QF322B8_PRE，half输入，L0C上数据类型为float，int8_t输出 |
| QUANT_MODE | 4 | (V)DEQF16，int8_t输入，L0C上数据类型为int32_t，half输出 |
| QUANT_MODE | 5 | (V)REQ8，int8_t输入，L0C上数据类型为int32_t，int8_t输出 |
| PRE_QUANT_MODE | 1 | Scalar量化模式，整个C矩阵使用一个量化参数（QUANT_MODE为1/2时该参数无效） |
| PRE_QUANT_MODE | 2 | Vector量化模式，C矩阵的每一列使用一个量化参数（QUANT_MODE为1/2时该参数无效） |
| PRE_RELU_MODE | 0 | 不使能Relu |
| PRE_RELU_MODE | 1 | 使能NormRelu能力 |
| EN_UNIT_FLAG | true | 开启unitFlag能力 |
| EN_UNIT_FLAG | false | 不开启unitFlag能力 |
| USE_DATA_COPY | true | 使用基础API DataCopy与寄存器配置实现矩阵搬出 |
| USE_DATA_COPY | false | 使用组合API Fixpipe实现矩阵搬出 |

**注意**：当使能UnitFlag时，MMAD与FIXPIPE需同时使能unitFlag，此时L0C上的LocalTensor不能用[TQue](https://www.hiascend.com/document/detail/zh/canncommercial/850/API/ascendcopapi/atlasascendc_api_07_0137.html)获取，需要改用[TBuf](https://www.hiascend.com/document/detail/zh/canncommercial/850/API/ascendcopapi/atlasascendc_api_07_0161.html)。

## Fixpipe搬出详细说明

### 搬运路径

本样例展示的搬运路径为：**L0C → L1 → GM**

### 数据排布变化

- **L0C**：NZ格式，数据按分形Z字形排布
- **L1**：NZ格式，数据按分形排布

### Fixpipe参数配置

#### 使用组合API Fixpipe

使用[Fixpipe](https://www.hiascend.com/document/detail/zh/canncommercial/850/API/ascendcopapi/atlasascendc_api_07_0251.html)接口时，需要配置FixpipeParamsV220结构体：

```cpp
AscendC::FixpipeParamsV220 fixpipeParams;
fixpipeParams.nSize = CeilAlign(n, CUBE_BLOCK);       // N轴对齐后的长度
fixpipeParams.mSize = m;                              // M轴长度
fixpipeParams.srcStride = CeilAlign(m, CUBE_BLOCK);   // 源操作数步长
fixpipeParams.dstStride = m * C0_SIZE / AscendC::ONE_BLK_SIZE; // 目的操作数步长
fixpipeParams.quantPre = QuantMode_t::XXX;            // 量化模式
fixpipeParams.deqScalar = deqScalar;                  // 标量量化参数（PRE_QUANT_MODE=1时）
fixpipeParams.unitFlag = 3;                           // unitFlag使能,取值为2或3
fixpipeParams.reluEn = true;                          // ReLU使能

AscendC::Fixpipe<outputType, l0cType, AscendC::CFG_NZ>(c1Local, c, fixpipeParams);
```

#### 使用基础API DataCopy

使用[DataCopy](https://www.hiascend.com/document/detail/zh/canncommercial/850/API/ascendcopapi/atlasascendc_api_07_00130.html)接口时，需要配置DataCopyCO12DstParams结构体：

```cpp
AscendC::DataCopyCO12DstParams intriParams;
intriParams.nSize = CeilAlign(n, CUBE_BLOCK);       // N轴对齐后的长度
intriParams.mSize = m;                              // M轴长度
intriParams.srcStride = CeilAlign(m, CUBE_BLOCK);   // 源操作数步长
intriParams.dstStride = m * C0_SIZE / AscendC::ONE_BLK_SIZE; // 目的操作数步长
intriParams.quantPre = QuantMode_t::XXX;            // 量化模式
intriParams.unitFlag = 3;                           // unitFlag使能,取值为2或3
intriParams.reluPre = true;                         // ReLU使能

AscendC::DataCopy(c1Local, c, intriParams);
```

### L1到GM的数据搬运

fixpipe完成后，数据存储在L1上，需要使用DataCopy指令将数据搬运到GM：

```cpp
event_t eventIDFIXToMTE3 = static_cast<event_t>(GetTPipePtr()->FetchEventID(AscendC::HardEvent::FIX_MTE3));
AscendC::SetFlag<AscendC::HardEvent::FIX_MTE3>(eventIDFIXToMTE3);
AscendC::WaitFlag<AscendC::HardEvent::FIX_MTE3>(eventIDFIXToMTE3);

AscendC::DataCopyParams dataCopyInfo;
dataCopyInfo.blockCount = 1;
dataCopyInfo.blockLen = n * m * sizeof(outputType) / AscendC::ONE_BLK_SIZE;
AscendC::DataCopy(cGM, c1Local, dataCopyInfo);
```

## 随路能力组合说明

### 随路量化 (QUANT_MODE)

本样例支持多种量化模式，通过QUANT_MODE宏定义控制：

| QUANT_MODE | 输入类型 | 输出类型 | L0C类型 | 量化模式 | 说明 |
|------------|----------|----------|---------|----------|------|
| 1 | half | bfloat16_t | float | F322BF16 | float32 Cast为 bfloat16 |
| 2 | half | half | float | F322F16 | float32 Cast为 float16 |
| 3 | half | int8_t | float | (V)QF322B8_PRE | float32 量化为 int8 |
| 4 | int8_t | half | int32_t | (V)DEQF16 | int32 量化为 float16 |
| 5 | int8_t | int8_t | int32_t | (V)REQ8 | int32 量化为 int8 |

#### Scalar量化模式 (PRE_QUANT_MODE=1)

整个C矩阵对应一个量化参数。

**Fixpipe配置：**
```cpp
fixpipeParams.quantPre = QuantMode_t::REQ8;
uint64_t deqScalar = static_cast<uint64_t>(*reinterpret_cast<int32_t *>(&quantScalar));
constexpr bool sign = (AscendC::IsSameType<outputType, int8_t>::value) ? true : false;
deqScalar = (deqScalar & ~(static_cast<uint64_t>(1) << 46)) | (static_cast<uint64_t>(sign) << 46);
fixpipeParams.deqScalar = deqScalar;
```

**DataCopy配置：**
```cpp
intriParams.quantPre = QuantMode_t::REQ8;
uint64_t deqScalar = static_cast<uint64_t>(*reinterpret_cast<int32_t *>(&quantScalar));
constexpr bool sign = (AscendC::IsSameType<outputType, int8_t>::value) ? true : false;
deqScalar = (deqScalar & ~(static_cast<uint64_t>(1) << 46)) | (static_cast<uint64_t>(sign) << 46);
AscendC::SetFixpipePreQuantFlag(deqScalar);
```

#### Vector量化模式 (PRE_QUANT_MODE=2)

C矩阵的每一列对应一个量化参数。

**数据搬运流程：**
1. 量化参数从GM搬运到L1（需要128B对齐）
2. 量化参数从L1搬运到Fixpipe Buffer

**Fixpipe配置：**
```cpp
fixpipeParams.quantPre = QuantMode_t::VREQ8;
AscendC::Fixpipe<outputType, l0cType, AscendC::CFG_NZ>(c1Local, c, quantAlphaTensor, fixpipeParams);
```

**DataCopy配置：**
```cpp
intriParams.quantPre = QuantMode_t::VREQ8;

AscendC::LocalTensor<uint64_t> fbTensor = inQueueFB.AllocTensor<uint64_t>();
inQueueFB.EnQue<uint64_t>(fbTensor);
fbTensor = inQueueFB.DeQue<uint64_t>();
uint16_t burstLen = CeilAlign(n * sizeof(uint64_t), 128) / 128;
AscendC::DataCopyParams dataCopyParams(1, burstLen, 0, 0);
AscendC::DataCopy(fbTensor, quantAlphaTensor, dataCopyParams);
AscendC::SetFixPipeConfig(fbTensor);
inQueueFB.FreeTensor(fbTensor);
```

### 随路Relu (PRE_RELU_MODE=1)

使能NormRelu能力，对输出结果进行Relu激活。

**Fixpipe配置：**
```cpp
fixpipeParams.reluEn = true;
```

**DataCopy配置：**
```cpp
intriParams.reluPre = true;
```

### UnitFlag并行能力 (EN_UNIT_FLAG=true)

开启MMAD指令与FIXPIPE指令的细粒度并行，硬件每计算完一个分形，计算结果就会被搬出。

**关键点：**
1. MMAD和FIXPIPE都需要使能unitFlag
2. L0C上的LocalTensor需要用TBuf获取，不能用TQue
3. MMAD和FIXPIPE的unitFlag取值为2或3

**MMAD配置：**
```cpp
if constexpr (EN_UNIT_FLAG == true) {
    if (kIndex != K_ROUND - 1) {
        mmadParams.unitFlag = 2;
    } else {
        mmadParams.unitFlag = 3;
    }
}
```

**FIXPIPE配置：**
```cpp
if constexpr (EN_UNIT_FLAG == true) {
    fixpipeParams.unitFlag = 3;
}
AscendC::Fixpipe<outputType, l0cType, AscendC::CFG_NZ>(c1Local, c, fixpipeParams);

if constexpr (EN_UNIT_FLAG == true && QUANT_MODE >= 1) {
    bool isUnitFlag = true;
    AscendC::SetFixPipeConfig<l0cType>(c, isUnitFlag);
}
```

**L0C Tensor获取：**
```cpp
if constexpr (EN_UNIT_FLAG == true) {
    c = l0cBuf.Get<l0cType>();
} else {
    c = outQueueCO1.AllocTensor<l0cType>();
}
```

### Fixpipe与DataCopy搬出方式对比

| 对比项 | Fixpipe | DataCopy |
|--------|---------|----------|
| 调用方式 | 一次调用完成搬出+随路操作 | 需要配合多个辅助接口 |
| 量化参数传递 | 直接作为函数参数 | 需要SetFixPipeConfig/SetFixpipePreQuantFlag |
| 代码简洁性 | 更简洁 | 需要更多配置步骤 |

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。

- 配置环境变量
  请根据当前环境上CANN开发套件包的[安装方式](../../../../docs/quick_start.md#prepare&install)，选择对应配置环境变量的命令。

  - 默认路径，root用户安装CANN软件包
    ```bash
    source /usr/local/Ascend/cann/set_env.sh
    ```

  - 默认路径，非root用户安装CANN软件包
    ```bash
    source $HOME/Ascend/cann/set_env.sh
    ```

  - 指定路径install_path，安装CANN软件包
    ```bash
    source ${install_path}/cann/set_env.sh
    ```
- 通过run_example.sh脚本执行样例，可通过参数自由配置场景和功能组合，参数顺序为QUANT_MODE、PRE_QUANT_MODE、PRE_RELU_MODE、EN_UNIT_FLAG、USE_DATA_COPY
  ```bash
  ./run_example.sh f322bf16 1 0 false false
  ./run_example.sh f322f16 1 0 true false
  ./run_example.sh f322s8 1 0 false false
  ./run_example.sh s322f16 2 1 true false
  ./run_example.sh s322s8 2 1 true true
  ```
  执行结果如下，说明精度对比成功。
  ```bash
  test pass!
  ```

- 样例执行
  ```bash
  QUANTMODE=3 PREQUANTMODE=1 PRERELUMODE=0 ENUNITFLAG=false USEDATACOPY=false
  # QUANTMODE: 1-5; PREQUANTMODE: 1:SCALAR 2:VECTOR; PRERELUMODE: 0:disable 1:Norm relu
  mkdir -p build && cd build;
  cmake .. -DQUANT_MODE=$QUANTMODE -DPRE_QUANT_MODE=$PREQUANTMODE -DPRE_RELU_MODE=$PRERELUMODE -DEN_UNIT_FLAG=$ENUNITFLAG -DUSE_DATA_COPY=$USEDATACOPY;
  make -j;
  python3 ../scripts/gen_data_f322s8.py -pre_quant_mode=$PREQUANTMODE -pre_relu_mode=$PRERELUMODE;
  ./demo
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin -quant_mode=$QUANTMODE
  ```
  执行结果如下，说明精度对比成功。
  ```bash
  test pass!
  ```
