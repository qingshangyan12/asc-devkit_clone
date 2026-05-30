# 随路量化与随路relu场景组合<a name="ZH-CN_TOPIC_0000002538071282"></a>

矩阵搬出支持多种随路量化与随路relu的组合，芯片支持的全量场景组合参考下表，接下来将对下表中的随路量化relu算法逐一介绍。（其中Leaky relu、Prelu、REQ4/VREQ4、QF322S4\_PRE/VQF322S4\_PRE、\(V\)SHIFTS322S16模式AscendC待实现）

<cann-filter npu_type = "A3,910b">

针对Atlas A3 训练系列产品/Atlas A3 推理系列产品和Atlas A2 训练系列产品/Atlas A2 推理系列产品：

**表 1**  随路量化与随路ReLU的组合表

| quantPre\reluPre | no relu | Normal relu | Leaky relu | Prelu |
|------------------|---------|------------|------------|-------|
| &bull; REQ8<br>&bull; DEQF16<br>&bull; QF322B8_PRE<br>&bull; QF322S4_PRE<br>&bull; REQ4 | M1=QUANT_PRE\[31:0\]<br>M2=QUANT_PRE\[31:0\]<br>M1、M2均为量化参数 | M1=QUANT_PRE\[31:0\]<br>M2=0<br>M1为量化参数、M2为Normal relu系数 | M1=QUANT_PRE\[31:0\]<br>M2=LEAKY_ALPHA\[31:0\]<br>M1为量化参数、M2为Leaky relu系数 | M1=QUANT_PRE\[31:0\]<br>M2=ReLU_PRE_ADDR\[i\]\[31:0\]<br>M1为量化参数、M2为Prelu系数，i为原始矩阵的列索引 |
| &bull; VREQ8<br>&bull; VDEQF16<br>&bull; VQF322B8_PRE<br>&bull; VQF322S4_PRE<br>&bull; VREQ4 | M1=Quant_PRE_ADDR\[i\]\[31:0\]<br>M2=Quant_PRE_ADDR\[i\]\[31:0\]<br>M1、M2均为量化参数，i为原始矩阵的列索引 | M1=Quant_PRE_ADDR\[i\]\[31:0\]<br>M2=0<br>M1为量化参数，M2为Normal relu系数，i为原始矩阵的列索引 | M1=Quant_PRE_ADDR\[i\]\[31:0\]<br>M2=LEAKY_ALPHA\[31:0\]<br>M1为量化参数，M2为Leaky relu系数，i为原始矩阵的列索引 | M1=Quant_PRE_ADDR\[i\]\[31:0\]<br>M2=ReLU_PRE_ADDR\[i\]\[31:0\]<br>M1为量化参数、M2为Prelu系数，i为原始矩阵的列索引 |
| SHIFTS322S16 | N=QUANT_PRE\[35:32\]<br>N为量化系数 | N=QUANT_PRE\[35:32\]<br>N为量化系数 | NA | NA |
| VSHIFTS322S16 | N=Quant_PRE_ADDR\[i\]\[35:32\]<br>N为量化系数，i为原始矩阵的列索引 | N=Quant_PRE_ADDR\[i\]\[35:32\]<br>N为量化系数，i为原始矩阵的列索引 | NA | NA |

</cann-filter>

注：M1为原始数据取值为正数时使用的随路系数，M2为原始数据取值为负数时使用的随路系数，N为右移位数，即量化系数；

<cann-filter npu_type = "A3,910b">

针对Atlas A3 训练系列产品/Atlas A3 推理系列产品和Atlas A2 训练系列产品/Atlas A2 推理系列产品：

- \(V\)REQ8量化（\(Vector\)ReQuant \(u\)int8量化）与随路relu组合

    将L0C上int32\_t数据搬出过程中转换成int8\_t/uint8\_t，具体随路量化与ReLU算法的完整Python示例代码请参考[\(V\)REQ8量化算法](https://gitcode.com/cann/asc-devkit/blob/master/examples/01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/fixpipe_l0c2gm/scripts/gen_data_s322s8.py)。

- DEQF16与VDEQF16量化与随路ReLU组合

    将L0C上int32\_t数据搬出过程中转换成half（float16），具体随路量化与ReLU算法的完整Python示例代码请参考[\(V\)DEQF16算法](https://gitcode.com/cann/asc-devkit/blob/master/examples/01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/fixpipe_l0c2gm/scripts/gen_data_s322f16.py)。

- QF322B8\_PRE与VQF322B8\_PRE量化与随路ReLU组合

    将L0C上float数据搬出过程中转换成int8\_t/uint8\_t，具体随路量化与ReLU算法的完整Python示例代码请参考[\(V\)QF322B8\_PRE算法](https://gitcode.com/cann/asc-devkit/blob/master/examples/01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/fixpipe_l0c2gm/scripts/gen_data_f322s8.py)。

</cann-filter>
