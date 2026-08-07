# asc_mmad

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：支持
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2 推理产品：不支持
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas 推理系列产品AI Core：不支持
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas 推理系列产品Vector Core：不支持
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas 训练系列产品：不支持
<!-- end id7 -->

## 功能说明

<!-- npu="950" id8 -->
头文件路径：`"c_api/cube_compute/cube_compute.h"`。

本接口用于完成矩阵乘加计算，计算公式如下：

$$
C = A \times B + C_{init}
$$

其中，左矩阵A的形状为M x K，右矩阵B的形状为K x N，结果矩阵C的形状为M x N。$C_{init}$可通过`c_matrix_init_val`和`c_matrix_source`配置为全0矩阵、L0C Buffer中的矩阵C或Bias矩阵。使用Bias矩阵时，形状为1 x N，并沿M方向广播，Bias矩阵位于BT Buffer（BiasTable Buffer）。

常规计算接口`asc_mmad`只下发矩阵乘加指令；同步计算接口`asc_mmad_sync`在完成矩阵乘加指令下发后执行`PIPE_ALL`同步，等待此前提交到各流水的指令执行完成。

**表1** 矩阵说明（Ascend 950PR/Ascend 950DT）

| 矩阵 | 物理存储位置 | 形状 | 数据格式 |
| --- | --- | --- | --- |
| A | L0A Buffer | M x K | Nz |
| B | L0B Buffer | K x N | Zn |
| C | L0C Buffer | M x N | Nz |
| Bias | BT Buffer | 1 x N | ND |

矩阵C的分形大小为16 x 16，矩阵A的分形大小为16 x K0，矩阵B的分形大小为K0 x 16。输入数据类型为`half`或`bfloat16_t`时，K0为16；输入数据类型为`float`时，K0为8；输入数据类型为`int8_t`、`fp8_e4m3fn_t`、`fp8_e5m2_t`或`hifloat8_t`时，K0为32。
<!-- end id8 -->

<!-- npu="A3,910b" id9 -->
本接口是Ascend C面向昇腾AI芯片的矩阵乘加核心计算接口，专为高性能算子开发设计，封装了昇腾NPU硬件的矩阵乘加计算能力，广泛用于神经网络层（如全连接层、卷积层）、数值计算类算子的开发。

本接口实现昇腾NPU矩阵乘计算能力，其数学表达式为：

$$
C = A \times B + C
$$

针对如下产品型号，矩阵说明见下表：
<!-- npu="A3" id10 -->
Atlas A3 训练系列产品/Atlas A3 推理系列产品
<!-- end id10 -->
<!-- npu="910b" id11 -->
Atlas A2 训练系列产品/Atlas A2 推理系列产品
<!-- end id11 -->

**表2** 矩阵计算矩阵A、B、C解释说明（[NPU架构版本3510](../../../../guide/编程指南/语言扩展层/SIMD-BuiltIn关键字.md)）

| 矩阵计算逻辑 | 矩阵计算物理位置 | 维度 | 输入/输出数据格式 |
| --- | --- | --- | --- |
| A | L0A Buffer | M x K | Zz |
| B | L0B Buffer | K x N | Zn |
| C | L0C Buffer | M x N，可支持使用偏置矩阵Bias进行初始化，维度为1 x N | Nz |
<!-- end id9 -->

## 函数原型

<!-- npu="950" id12 -->
### 模板原型

- 常规计算

  ```c
  __aicore__ inline void asc_mmad(__cc__ <c_dtype>* c_matrix,
                                  __ca__ <a_dtype>* a_matrix,
                                  __cb__ <b_dtype>* b_matrix,
                                  uint16_t left_height,
                                  uint16_t n_dim,
                                  uint16_t right_width,
                                  uint8_t unit_flag,
                                  bool disable_gemv,
                                  bool c_matrix_source,
                                  bool c_matrix_init_val)
  ```

- 同步计算

  ```c
  __aicore__ inline void asc_mmad_sync(__cc__ <c_dtype>* c_matrix,
                                       __ca__ <a_dtype>* a_matrix,
                                       __cb__ <b_dtype>* b_matrix,
                                       uint16_t left_height,
                                       uint16_t n_dim,
                                       uint16_t right_width,
                                       uint8_t unit_flag,
                                       bool disable_gemv,
                                       bool c_matrix_source,
                                       bool c_matrix_init_val)
  ```

### 数据类型<a id="asc_mmad_data_type"></a>

**表3** 输入矩阵、Bias矩阵和结果矩阵支持的数据类型组合（Ascend 950PR/Ascend 950DT）

| `a_dtype` | `b_dtype` | Bias数据类型 | `c_dtype` |
| --- | --- | --- | --- |
| `int8_t` | `int8_t` | `int32_t` | `int32_t` |
| `half` | `half` | `float` | `float` |
| `float` | `float` | `float` | `float` |
| `bfloat16_t` | `bfloat16_t` | `float` | `float` |
| `fp8_e4m3fn_t` | `fp8_e4m3fn_t` | `float` | `float` |
| `fp8_e4m3fn_t` | `fp8_e5m2_t` | `float` | `float` |
| `fp8_e5m2_t` | `fp8_e4m3fn_t` | `float` | `float` |
| `fp8_e5m2_t` | `fp8_e5m2_t` | `float` | `float` |
| `hifloat8_t` | `hifloat8_t` | `float` | `float` |
<!-- end id12 -->

<!-- npu="A3,910b" id13 -->
- 常规计算

  ```cpp
  __aicore__ inline void asc_mmad_s4(__cc__ int32_t* c_matrix, __ca__ int4b_t* a_matrix, __cb__ int4b_t* b_matrix, uint16_t left_height, uint16_t n_dim, uint16_t right_width, uint8_t unit_flag, bool k_direction_align, bool c_matrix_source, bool c_matrix_init_val)
  __aicore__ inline void asc_mmad_s4(__cc__ int32_t* c_matrix, __ca__ int4b_t* a_matrix, __cb__ int4b_t* b_matrix, uint16_t left_height, uint16_t n_dim, uint16_t right_width, uint8_t feat_offset, uint8_t smask_offset, uint8_t unit_flag, bool k_direction_align, bool is_weight_offset, bool c_matrix_source, bool c_matrix_init_val)
  __aicore__ inline void asc_mmad(__cc__ int32_t* c_matrix, __ca__ int8_t* a_matrix, __cb__ int8_t* b_matrix, uint16_t left_height, uint16_t n_dim, uint16_t right_width, uint8_t unit_flag, bool k_direction_align, bool c_matrix_source, bool c_matrix_init_val)
  __aicore__ inline void asc_mmad(__cc__ int32_t* c_matrix, __ca__ int8_t* a_matrix, __cb__ int8_t* b_matrix, uint16_t left_height, uint16_t n_dim, uint16_t right_width, uint8_t feat_offset, uint8_t smask_offset, uint8_t unit_flag, bool k_direction_align, bool is_weight_offset, bool c_matrix_source, bool c_matrix_init_val)
  __aicore__ inline void asc_mmad(__cc__ float* c_matrix, __ca__ half* a_matrix, __cb__ half* b_matrix, uint16_t left_height, uint16_t n_dim, uint16_t right_width, uint8_t unit_flag, bool k_direction_align, bool c_matrix_source, bool c_matrix_init_val)
  __aicore__ inline void asc_mmad(__cc__ float* c_matrix, __ca__ half* a_matrix, __cb__ half* b_matrix, uint16_t left_height, uint16_t n_dim, uint16_t right_width, uint8_t feat_offset, uint8_t smask_offset, uint8_t unit_flag, bool k_direction_align, bool is_weight_offset, bool c_matrix_source, bool c_matrix_init_val)
  __aicore__ inline void asc_mmad(__cc__ float* c_matrix, __ca__ float* a_matrix, __cb__ float* b_matrix, uint16_t left_height, uint16_t n_dim, uint16_t right_width, uint8_t unit_flag, bool k_direction_align, bool c_matrix_source, bool c_matrix_init_val)
  __aicore__ inline void asc_mmad(__cc__ float* c_matrix, __ca__ float* a_matrix, __cb__ float* b_matrix, uint16_t left_height, uint16_t n_dim, uint16_t right_width, uint8_t feat_offset, uint8_t smask_offset, uint8_t unit_flag, bool k_direction_align, bool is_weight_offset, bool c_matrix_source, bool c_matrix_init_val)
  __aicore__ inline void asc_mmad(__cc__ float* c_matrix, __ca__ bfloat16_t* a_matrix, __cb__ bfloat16_t* b_matrix, uint16_t left_height, uint16_t n_dim, uint16_t right_width, uint8_t unit_flag, bool k_direction_align, bool c_matrix_source, bool c_matrix_init_val)
  __aicore__ inline void asc_mmad(__cc__ float* c_matrix, __ca__ bfloat16_t* a_matrix, __cb__ bfloat16_t* b_matrix, uint16_t left_height, uint16_t n_dim, uint16_t right_width, uint8_t feat_offset, uint8_t smask_offset, uint8_t unit_flag, bool k_direction_align, bool is_weight_offset, bool c_matrix_source, bool c_matrix_init_val)
  ```

- 同步计算

  ```cpp
  __aicore__ inline void asc_mmad_s4_sync(__cc__ int32_t* c_matrix, __ca__ int4b_t* a_matrix, __cb__ int4b_t* b_matrix, uint16_t left_height, uint16_t n_dim, uint16_t right_width, uint8_t unit_flag, bool k_direction_align, bool c_matrix_source, bool c_matrix_init_val)
  __aicore__ inline void asc_mmad_s4_sync(__cc__ int32_t* c_matrix, __ca__ int4b_t* a_matrix, __cb__ int4b_t* b_matrix, uint16_t left_height, uint16_t n_dim, uint16_t right_width, uint8_t feat_offset, uint8_t smask_offset, uint8_t unit_flag, bool k_direction_align, bool is_weight_offset, bool c_matrix_source, bool c_matrix_init_val)
  __aicore__ inline void asc_mmad_sync(__cc__ int32_t* c_matrix, __ca__ int8_t* a_matrix, __cb__ int8_t* b_matrix, uint16_t left_height, uint16_t n_dim, uint16_t right_width, uint8_t unit_flag, bool k_direction_align, bool c_matrix_source, bool c_matrix_init_val)
  __aicore__ inline void asc_mmad_sync(__cc__ int32_t* c_matrix, __ca__ int8_t* a_matrix, __cb__ int8_t* b_matrix, uint16_t left_height, uint16_t n_dim, uint16_t right_width, uint8_t feat_offset, uint8_t smask_offset, uint8_t unit_flag, bool k_direction_align, bool is_weight_offset, bool c_matrix_source, bool c_matrix_init_val)
  __aicore__ inline void asc_mmad_sync(__cc__ float* c_matrix, __ca__ half* a_matrix, __cb__ half* b_matrix, uint16_t left_height, uint16_t n_dim, uint16_t right_width, uint8_t unit_flag, bool k_direction_align, bool c_matrix_source, bool c_matrix_init_val)
  __aicore__ inline void asc_mmad_sync(__cc__ float* c_matrix, __ca__ half* a_matrix, __cb__ half* b_matrix, uint16_t left_height, uint16_t n_dim, uint16_t right_width, uint8_t feat_offset, uint8_t smask_offset, uint8_t unit_flag, bool k_direction_align, bool is_weight_offset, bool c_matrix_source, bool c_matrix_init_val)
  __aicore__ inline void asc_mmad_sync(__cc__ float* c_matrix, __ca__ float* a_matrix, __cb__ float* b_matrix, uint16_t left_height, uint16_t n_dim, uint16_t right_width, uint8_t unit_flag, bool k_direction_align, bool c_matrix_source, bool c_matrix_init_val)
  __aicore__ inline void asc_mmad_sync(__cc__ float* c_matrix, __ca__ float* a_matrix, __cb__ float* b_matrix, uint16_t left_height, uint16_t n_dim, uint16_t right_width, uint8_t feat_offset, uint8_t smask_offset, uint8_t unit_flag, bool k_direction_align, bool is_weight_offset, bool c_matrix_source, bool c_matrix_init_val)
  __aicore__ inline void asc_mmad_sync(__cc__ float* c_matrix, __ca__ bfloat16_t* a_matrix, __cb__ bfloat16_t* b_matrix, uint16_t left_height, uint16_t n_dim, uint16_t right_width, uint8_t unit_flag, bool k_direction_align, bool c_matrix_source, bool c_matrix_init_val)
  __aicore__ inline void asc_mmad_sync(__cc__ float* c_matrix, __ca__ bfloat16_t* a_matrix, __cb__ bfloat16_t* b_matrix, uint16_t left_height, uint16_t n_dim, uint16_t right_width, uint8_t feat_offset, uint8_t smask_offset, uint8_t unit_flag, bool k_direction_align, bool is_weight_offset, bool c_matrix_source, bool c_matrix_init_val)
  ```
<!-- end id13 -->

## 参数说明

<!-- npu="950" id14 -->
**表4** 参数说明（Ascend 950PR/Ascend 950DT）<a id="asc_mmad_param_table"></a>

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| c_matrix | 输出 | 目的操作数，结果矩阵C在L0C Buffer中的起始地址，需按照1024字节对齐。数据类型由接口重载决定，具体请参见[表3](#asc_mmad_data_type)。<br>当`c_matrix_init_val`为false且`c_matrix_source`为true时，该参数低32位为矩阵C在L0C Buffer中的起始地址，高32位为Bias矩阵在BT Buffer中的起始地址；Bias矩阵起始地址需按照64字节对齐。 |
| a_matrix | 输入 | 源操作数，左矩阵A在L0A Buffer中的起始地址，需按照512字节对齐。数据类型由接口重载决定，具体请参见[表3](#asc_mmad_data_type)。 |
| b_matrix | 输入 | 源操作数，右矩阵B在L0B Buffer中的起始地址，需按照512字节对齐。数据类型由接口重载决定，具体请参见[表3](#asc_mmad_data_type)。 |
| left_height | 输入 | 左矩阵A和结果矩阵C的M维大小，单位为元素，取值范围为[0, 4095]。 |
| n_dim | 输入 | 左矩阵A的K维大小，也是右矩阵B的K维大小，单位为元素，取值范围为[0, 4095]。 |
| right_width | 输入 | 右矩阵B和结果矩阵C的N维大小，单位为元素，取值范围为[0, 4095]。 |
| unit_flag | 输入 | 用于控制矩阵乘加指令和Fixpipe指令细粒度并行。取值说明如下：<br>&nbsp;&nbsp;&bull; 0：不开启UnitFlag。<br>&nbsp;&nbsp;&bull; 2：开启UnitFlag，硬件执行完指令后不复位单元标记位。<br>&nbsp;&nbsp;&bull; 3：开启UnitFlag，硬件执行完指令后复位单元标记位。<br>不支持取值1。开启UnitFlag时，矩阵乘加指令与配套Fixpipe指令的UnitFlag均需设置为2或3。 |
| disable_gemv | 输入 | M为1时，配置是否关闭[GEMV](../../basic_api/cube_compute_ISASI/mmad_compute_key_features/GEMV.md)模式。<br>&nbsp;&nbsp;&bull; false：开启GEMV模式。<br>&nbsp;&nbsp;&bull; true：关闭GEMV模式。<br>M不为1时，该参数不生效。 |
| c_matrix_source | 输入 | 当`c_matrix_init_val`为false时，配置矩阵C的初值来源。<br>&nbsp;&nbsp;&bull; false：矩阵C的初值来源于L0C Buffer。<br>&nbsp;&nbsp;&bull; true：矩阵C的初值来源于BT Buffer，Bias矩阵沿M方向广播。 |
| c_matrix_init_val | 输入 | 配置是否将矩阵C的初值设置为0。<br>&nbsp;&nbsp;&bull; true：将矩阵C的初值设置为0，`c_matrix_source`不生效。<br>&nbsp;&nbsp;&bull; false：不执行清零操作，矩阵C的初值由`c_matrix_source`配置。 |
<!-- end id14 -->

<!-- npu="A3,910b" id15 -->
**表5** 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| c_matrix | 输出 | 目的操作数，结果矩阵。 |
| a_matrix | 输入 | 源操作数，左矩阵A。 |
| b_matrix | 输入 | 源操作数，右矩阵B。 |
| left_height | 输入 | 左矩阵height，取值范围为[0,4095]。 |
| n_dim | 输入 | 左矩阵width、右矩阵height，取值范围为[0,4095]。 |
| right_width | 输入 | 右矩阵width，取值范围为[0,4095]。 |
| feat_offset | 输入 | 保留参数。 |
| smask_offset | 输入 | 权重矩阵的偏移位。 |
| unit_flag | 输入 | unit_flag是一种asc_mmad接口和Fixpipe指令细粒度的并行，开启该功能后，硬件每计算完一个分形，计算结果就会被搬出，该功能不适用于L0C Buffer累加的场景。取值说明如下：<br>&bull; 0：关闭unit_flag；<br>&bull; 1：保留值；<br>&bull; 2：开启unit_flag，硬件执行完指令后，不会关闭unit_flag功能；<br>&bull; 3：开启unit_flag，硬件执行完指令后，会关闭unit_flag功能。<br>开启该功能时，矩阵计算的unit_flag在最后一个分形设置为3，其余分形计算设置为2即可。 |
| k_direction_align | 输入 | 当源操作数和目的操作数为float时，L0A和L0B中的矩阵在right_width方向上按ceil(right_width/16)*16方式都对齐到48，对于right_width=44，L0A/L0B中的所有12个分形都会被读取到Cube中，而对于right_width=36，只有L0A/L0B中的10个分形会被读取到Cube中。 |
| is_weight_offset | 输入 | 启用weight matrix offset。 |
| c_matrix_source | 输入 | 配置C矩阵初始值是否来源于Bias Table Buffer。取值说明如下：<br>&bull; true：来源于Bias Table Buffer。<br>&bull; false：来源于L0C Buffer。 |
| c_matrix_init_val | 输入 | 配置C矩阵初始值是否为0。取值说明如下：<br>&bull; true：C矩阵初始值为0。<br>&bull; false：C矩阵初始值通过c_matrix_source参数进行配置。 |
<!-- end id15 -->

## 返回值说明

无

## 流水类型

PIPE_M

## 约束说明

<!-- npu="950" id16 -->
- 本接口的矩阵乘加计算仅在AIC上生效，在AIV上调用时不执行矩阵乘加计算。
- 结果矩阵C只支持位于L0C Buffer，左矩阵A只支持位于L0A Buffer，右矩阵B只支持位于L0B Buffer；使用Bias矩阵时，Bias只支持位于BT Buffer。
- L0C Buffer大小为256KB，L0A Buffer和L0B Buffer大小均为64KB，BT Buffer大小为4KB。矩阵的起始地址和占用空间不能超出对应Buffer的范围。
- 各矩阵的起始地址需满足[表4](#asc_mmad_param_table)中的对齐要求。操作数的其他地址约束请参考[存储单元说明](../general_description_and_constraints.md#存储单元说明)。
- `left_height`、`n_dim`、`right_width`中的任意一个值为0时，接口将被视为NOP（空操作）。
- 当M为1且`disable_gemv`为false时，默认开启GEMV模式。此时从L0A Buffer读取矩阵A时按照ND格式读取，矩阵A需按照ND格式排布，起始地址仍需按照512字节对齐。
- 矩阵存储空间需按照分形大小补齐：M、N分别向上补齐到16的倍数，K向上补齐到K0的倍数。K0的取值请参见功能说明中的分形说明。`left_height`、`n_dim`和`right_width`仍传入矩阵的有效M、K、N值；补齐部分为无效数据，不参与结果矩阵有效区域的计算。
- 使用常规计算接口沿K轴分块并将结果累加到同一块L0C Buffer时，当`(left_height / 16) * (right_width / 16) < 10`，需在相邻两次矩阵乘加之间调用[asc_sync_pipe](../sync/asc_sync_pipe.md)，并将`pipe`设置为`PIPE_M`；也可以使用同步计算接口完成同步。
- 开启UnitFlag时，矩阵乘加指令与配套Fixpipe指令需同时开启UnitFlag。连续操作同一块L0C Buffer时，前n-1条指令的`unit_flag`设置为2，最后一条指令设置为3。
- 开启UnitFlag时，矩阵计算方向需与Fixpipe读取顺序保持一致。Fixpipe开启Nz2ND随路格式转换，或未进行随路格式转换但开启B8/B4量化并触发Channel Merge功能时，调用[asc_set_mmad_direction_n](./asc_set_mmad_direction_n.md)；其他场景调用[asc_set_mmad_direction_m](./asc_set_mmad_direction_m.md)。
- 开启UnitFlag后，建议矩阵乘加的计算数据量与Fixpipe搬出的数据量保持一致。两者不一致可能导致执行异常。需要清除UnitFlag产生的残留状态时，可调用[asc_set_l0c2gm_config](./asc_set_l0c2gm_config.md)，并将`enable_unit_flag`设置为true，将L0C Buffer中所有内存块的单元标志位设置为0。调用时`relu_pre`需传入0；后续搬出不使用Vector量化时，`quant_pre`也传入0，否则传入对应的Vector量化参数地址索引。
- 浮点类型的输入或输出包含INF/NaN时，可通过[asc_set_ctrl](../sys_var/asc_set_ctrl.md)配置CTRL[48]：设置为0时使用饱和模式，INF输出饱和为正负MAX、NaN输出饱和为0；设置为1时使用非饱和模式，INF/NaN保持原输出。Mmad指令应避免NaN输入。整数类型仅支持饱和模式。
<!-- end id16 -->

<!-- npu="A3,910b" id17 -->
- 当left_height、right_width、n_dim中的任意一个值为0时，该指令不会被执行。
- 操作数地址对齐约束请参考[存储单元说明](../general_description_and_constraints.md#存储单元说明)。
<!-- end id17 -->

## 调用示例

<!-- npu="950" id18 -->
```cpp
__cc__ int32_t c_matrix[128 * 128];
__ca__ int8_t a_matrix[128 * 64];
__cb__ int8_t b_matrix[64 * 128];
uint16_t left_height = 128;
uint16_t n_dim = 64;
uint16_t right_width = 128;
uint8_t unit_flag = 0;
bool disable_gemv = false;
bool c_matrix_source = false;
bool c_matrix_init_val = true;
asc_mmad_sync(c_matrix, a_matrix, b_matrix, left_height, n_dim, right_width,
              unit_flag, disable_gemv, c_matrix_source, c_matrix_init_val);
```

完整使用样例请参考[asc_mmad样例](../../../../../../examples/02_simd_c_api/03_c_api/03_matrix_compute/mmad)。
<!-- end id18 -->
