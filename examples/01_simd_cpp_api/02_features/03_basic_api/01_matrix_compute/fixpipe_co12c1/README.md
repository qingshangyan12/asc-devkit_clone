# fixpipe_co12c1样例

## 概述

本样例介绍如何使用组合API Fixpipe或基础API DataCopy将矩阵乘的结果从L0C搬出到L1，然后再从L1搬出到GM，并支持随路量化、Relu能力组合。

完整的矩阵乘法流程涉及四个阶段的数据搬运和计算：
1. **GM → L1**：通过DataCopy随路转换ND2NZ指令，将A、B矩阵从GM搬运到L1，同时完成ND到NZ格式的转换
2. **L1 → L0A/L0B**：通过LoadData系列指令，将A、B矩阵从L1搬运到L0A/L0B，完成NZ到Zz/Zn格式的转换
3. **L0A × L0B → L0C**：通过Mmad指令执行矩阵乘法计算，结果存储在L0C中
4. **L0C → L1 → GM**：通过Fixpipe或DataCopy指令将结果从L0C搬出到L1，再通过DataCopy指令从L1搬出到GM，支持随路量化、Relu等操作

本样例重点介绍**阶段4：L0C → L1 → GM**的fixpipe使用方法。

## 支持的产品

- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍

```
├── fixpipe_co12c1
│   ├── scripts
│   │   ├── gen_data_f322bf16.py      // F32到BF16数据生成脚本
│   │   ├── gen_data_f322f16.py       // F32到F16数据生成脚本
│   │   ├── gen_data_f322s8.py        // F32到S8数据生成脚本
│   │   ├── gen_data_s322f16.py       // S32到F16数据生成脚本
│   │   ├── gen_data_s322s8.py        // S32到S8数据生成脚本
│   │   └── verify_result.py          // 验证输出数据和真值数据是否一致的验证脚本
│   ├── CMakeLists.txt                // 编译工程文件
│   ├── data_utils.h                  // 数据读入写出函数
│   ├── cube_common.h                 // 矩阵计算通用头文件
│   └── fixpipe_co12c1.asc            // Ascend C算子实现 & 调用样例
```

## 算子描述

### 算子功能

本样例中实现的是[M, N, K]固定为[128, 256, 128]的Matmul乘算子，对应的数学表达式为：
```
C = A * B
```
其中A的形状为[M, K], B的形状为[K, N], C的形状为[M, N]。

本样例中，A矩阵不转置，B矩阵不转置（逻辑上需要转置），因此：
- A矩阵：GM(ND) → L1(NZ) → L0A(Zz)
- B矩阵：GM(ND) → L1(NZ) → L0B(Zn) （需要通过LoadDataWithTranspose转置）
- C矩阵：L0C(Nz) → L1(NZ) → GM(ND)

### 算子规格

<table>
<tr><td rowspan="1" align="center">算子类型(OpType)</td><td colspan="4" align="center">Matmul</td></tr>
<tr><td rowspan="3" align="center">算子输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
<tr><td align="center">A</td><td align="center">(128, 128)</td><td align="center">half/int8_t</td><td align="center">ND</td></tr>
<tr><td align="center">B</td><td align="center">(128, 256)</td><td align="center">half/int8_t</td><td align="center">ND</td></tr>
<tr><td rowspan="1" align="center">算子输出</td><td align="center">C</td><td align="center">(128, 256)</td><td align="center">float/half/bfloat16_t/int8_t</td><td align="center">ND</td></tr>
<tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">fixpipe_co12c1</td></tr>
</table>

### 功能配置说明

本样例支持配置使用AscendC提供的组合API Fixpipe或基础API DataCopy与寄存器配置实现矩阵从L0C搬出到L1，并支持FIXPIPE指令多种随路能力组合：

| 配置项 | 取值 | 说明 |
|--------|------|------|
| QUANT_MODE | 0 | 无量化，half输入，float输出 |
| QUANT_MODE | 1 | F322BF16，half输入，bfloat16_t输出 |
| QUANT_MODE | 2 | F322F16，half输入，half输出 |
| QUANT_MODE | 3 | QF322B8_PRE，half输入，int8_t输出 |
| QUANT_MODE | 4 | DEQF16，int8_t输入，half输出 |
| QUANT_MODE | 5 | REQ8，int8_t输入，int8_t输出 |
| PRE_QUANT_MODE | 1 | Scalar量化模式，整个C矩阵使用一个量化参数 |
| PRE_QUANT_MODE | 2 | Tensor/Vector量化模式，C矩阵的每一列使用一个量化参数 |
| PRE_RELU_MODE | 0 | 不使能Relu |
| PRE_RELU_MODE | 1 | 使能NormRelu能力 |
| EN_UNIT_FLAG | true | 开启MMAD指令与FIXPIPE指令并行能力 |
| EN_UNIT_FLAG | false | 不开启并行能力 |
| USE_DATA_COPY | true | 使用基础API DataCopy与寄存器配置实现矩阵搬出 |
| USE_DATA_COPY | false | 使用组合API Fixpipe实现矩阵搬出 |

**注意**：当使能UnitFlag时，MMAD与FIXPIPE需同时使能unitFlag，此时L0C上的LocalTensor不能用[TQue](https://www.hiascend.com/document/detail/zh/canncommercial/850/API/ascendcopapi/atlasascendc_api_07_0137.html)获取，需要改用[TBuf](https://www.hiascend.com/document/detail/zh/canncommercial/850/API/ascendcopapi/atlasascendc_api_07_0161.html)。

## Fixpipe搬出详细说明

### 搬运路径

本样例展示的搬运路径为：**L0C → L1 → GM**

与直接从L0C搬运到GM相比，先搬运到L1再搬运到GM的方式适用于以下场景：
- 需要在L1上进行额外处理（如数据重排、拼接等）
- L0C空间有限，需要及时释放
- 多次矩阵乘法结果需要在L1上进行累加或其他操作

### 数据排布变化

- **L0C**：NZ格式，数据按分形（16x16）Z字形排布
- **L1**：NZ格式，数据按分形排布
- **GM**：ND格式，数据线性连续存储

### Fixpipe参数配置

#### 使用组合API Fixpipe

使用[Fixpipe](https://www.hiascend.com/document/detail/zh/canncommercial/850/API/ascendcopapi/atlasascendc_api_07_0251.html)接口时，需要配置FixpipeParamsV220结构体：

```cpp
AscendC::FixpipeParamsV220 fixpipeParams;
fixpipeParams.nSize = CeilAlign(n, CUBE_BLOCK);      // N轴对齐后的长度
fixpipeParams.mSize = m;                              // M轴长度
fixpipeParams.srcStride = CeilAlign(m, CUBE_BLOCK);   // 源操作数步长
fixpipeParams.dstStride = m * CUBE_BLOCK * sizeof(outputType) / AscendC::ONE_BLK_SIZE; // 目的操作数步长
fixpipeParams.quantPre = QuantMode_t::XXX;           // 量化模式
fixpipeParams.deqScalar = deqScalar;                  // 标量量化参数（PRE_QUANT_MODE=1时）
fixpipeParams.unitFlag = 3;                           // Unit Flag使能（累加模式）
fixpipeParams.reluEn = true;                          // ReLU使能

AscendC::Fixpipe<outputType, l0cType, AscendC::CFG_NZ>(c1Local, c, fixpipeParams);
```

#### 使用基础API DataCopy

使用[DataCopy](https://www.hiascend.com/document/detail/zh/canncommercial/850/API/ascendcopapi/atlasascendc_api_07_00130.html)接口时，需要配置DataCopyCO12DstParams结构体：

```cpp
AscendC::DataCopyCO12DstParams intriParams;
intriParams.nSize = CeilAlign(n, CUBE_BLOCK);
intriParams.mSize = m;
intriParams.srcStride = CeilAlign(m, CUBE_BLOCK);
intriParams.dstStride = m * CUBE_BLOCK * sizeof(outputType) / AscendC::ONE_BLK_SIZE;
intriParams.quantPre = QuantMode_t::XXX;
intriParams.unitFlag = 3;
intriParams.reluPre = true;

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
| 0 | half | float | float | 无 | 无量化，仅数据搬运 |
| 1 | half | bfloat16_t | float | F322BF16 | float32 转 bfloat16 |
| 2 | half | half | float | F322F16 | float32 转 float16 |
| 3 | half | int8_t | float | QF322B8_PRE | float32 量化为 int8 |
| 4 | int8_t | half | int32_t | DEQF16 | int32 反量化为 float16 |
| 5 | int8_t | int8_t | int32_t | REQ8 | int32 重量化为 int8 |

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

开启MMAD指令与FIXPIPE指令并行能力，提升性能。

**关键点：**
1. MMAD和FIXPIPE都需要使能unitFlag
2. L0C上的LocalTensor需要用TBuf获取，不能用TQue
3. MMAD的unitFlag：最后一个K分块为3，其余为2
4. FIXPIPE的unitFlag：设置为3

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
| API类型 | 组合API | 基础API |
| 调用方式 | 一次调用完成搬出+随路操作 | 需要配合多个辅助接口 |
| 量化参数传递 | 直接作为函数参数 | 需要SetFixPipeConfig/SetFixpipePreQuantFlag |
| 代码简洁性 | 更简洁 | 需要更多配置步骤 |
| 灵活性 | 相对固定 | 更灵活 |

## 编译运行

在本样例根目录下执行如下步骤，编译并执行算子。

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

- 样例执行
  ```bash
  QUANTMODE=2 PREQUANTMODE=1 PRERELUMODE=0 ENUNITFLAG=false USEDATACOPY=false
  # QUANTMODE: 0-5; PREQUANTMODE: 1:SCALAR 2:VECTOR; PRERELUMODE: 0:disable 1:Norm relu
  mkdir -p build && cd build;
  cmake .. -DQUANT_MODE=$QUANTMODE -DPRE_QUANT_MODE=$PREQUANTMODE -DPRE_RELU_MODE=$PRERELUMODE -DEN_UNIT_FLAG=$ENUNITFLAG -DUSE_DATA_COPY=$USEDATACOPY;
  make -j;
  python3 ../scripts/gen_data_f322f16.py -pre_quant_mode=$PREQUANTMODE -pre_relu_mode=$PRERELUMODE;
  ./demo
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin
  ```
  执行结果如下，说明精度对比成功。
  ```bash
  test pass!
  ```
