# asc_mmad_mx

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：不支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：不支持
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

本接口用于完成带有量化系数的矩阵乘加计算。左矩阵A和右矩阵B分别具有对应的量化系数矩阵ScaleA和ScaleB，计算公式如下：

$$
C = (ScaleA \otimes A) \times (ScaleB \otimes B) + C_{init}
$$

其中，$\otimes$表示广播乘法。K方向上每32个元素共享一个量化因子。左矩阵A的形状为M x K，ScaleA矩阵的形状为M x (K / 32)；右矩阵B的形状为K x N，ScaleB矩阵的形状为(K / 32) x N；结果矩阵C的形状为M x N。$C_{init}$可通过`c_matrix_init_val`和`c_matrix_source`配置为全0矩阵、L0C Buffer中的矩阵C或Bias矩阵。使用Bias矩阵时，形状为1 x N，并沿M方向广播，Bias矩阵位于BT Buffer（BiasTable Buffer）。

ScaleA和ScaleB的数据类型均为`fp8_e8m0_t`。本接口不单独传入Scale矩阵地址，而是根据`a_matrix`和`b_matrix`的起始地址，按照固定的地址映射关系从L0A_MX Buffer和L0B_MX Buffer读取对应的Scale矩阵。

常规计算接口`asc_mmad_mx`只下发矩阵乘加指令；同步计算接口`asc_mmad_mx_sync`在完成矩阵乘加指令下发后执行`PIPE_ALL`同步，等待此前提交到各流水的指令执行完成。

**表1** 矩阵说明（Ascend 950PR/Ascend 950DT）

| 矩阵 | 物理存储位置 | 形状 | 数据格式 |
| --- | --- | --- | --- |
| A | L0A Buffer | M x K | Nz |
| ScaleA | L0A_MX Buffer | M x (K / 32) | Zz，分形大小为16 x 2 |
| B | L0B Buffer | K x N | Zn |
| ScaleB | L0B_MX Buffer | (K / 32) x N | Nn，分形大小为2 x 16 |
| C | L0C Buffer | M x N | Nz |
| Bias | BT Buffer | 1 x N | ND |

矩阵C的分形大小为16 x 16。按照逻辑元素计算，FP4场景下矩阵A的分形大小为16 x 64，矩阵B的分形大小为64 x 16；FP8场景下矩阵A的分形大小为16 x 32，矩阵B的分形大小为32 x 16。
<!-- end id8 -->

## 函数原型

<!-- npu="950" id9 -->
### 模板原型

- 常规计算

  ```c
  __aicore__ inline void asc_mmad_mx(__cc__ float* c_matrix,
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
  __aicore__ inline void asc_mmad_mx_sync(__cc__ float* c_matrix,
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

### 数据类型<a id="asc_mmad_mx_data_type"></a>

**表2** 输入矩阵、Scale矩阵、Bias矩阵和结果矩阵支持的数据类型组合（Ascend 950PR/Ascend 950DT）

| `a_dtype` | `b_dtype` | ScaleA/ScaleB数据类型 | Bias数据类型 | 结果矩阵C数据类型 |
| --- | --- | --- | --- | --- |
| `fp4x2_e1m2_t` | `fp4x2_e1m2_t` | `fp8_e8m0_t` | `float` | `float` |
| `fp4x2_e1m2_t` | `fp4x2_e2m1_t` | `fp8_e8m0_t` | `float` | `float` |
| `fp4x2_e2m1_t` | `fp4x2_e1m2_t` | `fp8_e8m0_t` | `float` | `float` |
| `fp4x2_e2m1_t` | `fp4x2_e2m1_t` | `fp8_e8m0_t` | `float` | `float` |
| `fp8_e4m3fn_t` | `fp8_e4m3fn_t` | `fp8_e8m0_t` | `float` | `float` |
| `fp8_e4m3fn_t` | `fp8_e5m2_t` | `fp8_e8m0_t` | `float` | `float` |
| `fp8_e5m2_t` | `fp8_e4m3fn_t` | `fp8_e8m0_t` | `float` | `float` |
| `fp8_e5m2_t` | `fp8_e5m2_t` | `fp8_e8m0_t` | `float` | `float` |
<!-- end id9 -->

## 参数说明

<!-- npu="950" id10 -->
**表3** 参数说明（Ascend 950PR/Ascend 950DT）<a id="asc_mmad_mx_param_table"></a>

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| c_matrix | 输出 | 目的操作数，结果矩阵C在L0C Buffer中的起始地址，数据类型为`float`，需按照1024字节对齐。<br>当`c_matrix_init_val`为false且`c_matrix_source`为true时，该参数低32位为矩阵C在L0C Buffer中的起始地址，高32位为Bias矩阵在BT Buffer中的起始地址；Bias矩阵起始地址需按照64字节对齐。 |
| a_matrix | 输入 | 源操作数，左矩阵A在L0A Buffer中的起始地址。数据类型由接口重载决定，具体请参见[表2](#asc_mmad_mx_data_type)。FP4场景需按照512字节对齐，FP8场景需按照1024字节对齐。<br>ScaleA矩阵在L0A_MX Buffer中的起始地址为`a_matrix`起始地址除以16。 |
| b_matrix | 输入 | 源操作数，右矩阵B在L0B Buffer中的起始地址。数据类型由接口重载决定，具体请参见[表2](#asc_mmad_mx_data_type)。FP4场景需按照512字节对齐，FP8场景需按照1024字节对齐。<br>ScaleB矩阵在L0B_MX Buffer中的起始地址为`b_matrix`起始地址除以16。 |
| left_height | 输入 | 左矩阵A和结果矩阵C的M维大小，单位为逻辑元素，取值范围为[0, 4095]。 |
| n_dim | 输入 | 左矩阵A的K维大小，也是右矩阵B的K维大小，单位为逻辑元素，取值范围为[0, 4095]，并且需为64的倍数。FP4数据类型的每个存储元素包含两个FP4逻辑元素。 |
| right_width | 输入 | 右矩阵B和结果矩阵C的N维大小，单位为逻辑元素，取值范围为[0, 4095]。 |
| unit_flag | 输入 | 用于控制矩阵乘加指令和Fixpipe指令细粒度并行。取值说明如下：<br>&nbsp;&nbsp;&bull; 0：不开启UnitFlag。<br>&nbsp;&nbsp;&bull; 2：开启UnitFlag，硬件执行完指令后不复位单元标记位。<br>&nbsp;&nbsp;&bull; 3：开启UnitFlag，硬件执行完指令后复位单元标记位。<br>不支持取值1。开启UnitFlag时，矩阵乘加指令与配套Fixpipe指令的UnitFlag均需设置为2或3。 |
| disable_gemv | 输入 | M为1时，配置是否关闭[GEMV](../../basic_api/cube_compute_ISASI/mmad_compute_key_features/GEMV.md)模式。<br>&nbsp;&nbsp;&bull; false：开启GEMV模式。<br>&nbsp;&nbsp;&bull; true：关闭GEMV模式。<br>M不为1时，该参数不生效。 |
| c_matrix_source | 输入 | 当`c_matrix_init_val`为false时，配置矩阵C的初值来源。<br>&nbsp;&nbsp;&bull; false：矩阵C的初值来源于L0C Buffer。<br>&nbsp;&nbsp;&bull; true：矩阵C的初值来源于BT Buffer，Bias矩阵沿M方向广播。 |
| c_matrix_init_val | 输入 | 配置是否将矩阵C的初值设置为0。<br>&nbsp;&nbsp;&bull; true：将矩阵C的初值设置为0，`c_matrix_source`不生效。<br>&nbsp;&nbsp;&bull; false：不执行清零操作，矩阵C的初值由`c_matrix_source`配置。 |
<!-- end id10 -->

## 返回值说明

无

## 流水类型

PIPE_M

## 约束说明

<!-- npu="950" id11 -->
- 本接口的矩阵乘加计算仅在AIC上生效，在AIV上调用时不执行矩阵乘加计算。
- 结果矩阵C只支持位于L0C Buffer，左矩阵A只支持位于L0A Buffer，ScaleA矩阵只支持位于L0A_MX Buffer，右矩阵B只支持位于L0B Buffer，ScaleB矩阵只支持位于L0B_MX Buffer；使用Bias矩阵时，Bias只支持位于BT Buffer。
- L0C Buffer大小为256KB，L0A Buffer和L0B Buffer大小均为64KB，L0A_MX Buffer和L0B_MX Buffer大小均为4KB，BT Buffer大小为4KB。矩阵的起始地址和占用空间不能超出对应Buffer的范围。
- 各矩阵的起始地址需满足[表3](#asc_mmad_mx_param_table)中的对齐要求。操作数的其他地址约束请参考[存储单元说明](../general_description_and_constraints.md#存储单元说明)。
- `left_height`、`n_dim`、`right_width`中的任意一个值为0时，接口将被视为NOP（空操作）。
- `n_dim`需为64的倍数。原始K不是64的倍数时，需将K向上补齐到64的倍数，并将补齐区域的矩阵A和矩阵B数据设置为0；传入接口的`n_dim`、矩阵存储空间以及Scale矩阵的K维大小均需按照补齐后的K计算。
- 调用本接口前，需通过[asc_copy_l12l0a_mx](../cube_data_move/asc_copy_l12l0a_mx.md)和[asc_copy_l12l0b_mx](../cube_data_move/asc_copy_l12l0b_mx.md)等接口，将ScaleA和ScaleB矩阵分别搬入L0A_MX Buffer和L0B_MX Buffer。ScaleA和ScaleB矩阵的数据类型必须为`fp8_e8m0_t`，K方向上每32个矩阵元素对应一个Scale元素。
- ScaleA矩阵在L0A_MX Buffer中的起始地址为`a_matrix`起始地址除以16，ScaleB矩阵在L0B_MX Buffer中的起始地址为`b_matrix`起始地址除以16。搬入Scale矩阵时需使用与矩阵A和矩阵B匹配的目的地址。
- Scale矩阵需覆盖本次矩阵乘加所需的全部数据。ScaleA的M维需向上补齐到16的倍数，K维大小为K / 32；ScaleB的K维大小为K / 32，N维需向上补齐到16的倍数。对应的起始地址与占用空间不能超出MX Buffer的范围。
- 当M为1且`disable_gemv`为false时，默认开启GEMV模式。此时从L0A Buffer读取矩阵A时按照ND格式读取，矩阵A需按照ND格式排布，起始地址仍需满足[表3](#asc_mmad_mx_param_table)中的对齐要求。
- 矩阵存储空间的M、N维需分别向上补齐到16的倍数，`left_height`和`right_width`仍传入矩阵的有效M、N值。补齐部分为无效数据，不参与结果矩阵有效区域的计算。
- 使用常规计算接口沿K轴分块并将结果累加到同一块L0C Buffer时，当`(left_height / 16) * (right_width / 16) < 10`，需在相邻两次矩阵乘加之间调用[asc_sync_pipe](../sync/asc_sync_pipe.md)，并将`pipe`设置为`PIPE_M`；也可以使用同步计算接口完成同步。
- 开启UnitFlag时，矩阵乘加指令与配套Fixpipe指令需同时开启UnitFlag。连续操作同一块L0C Buffer时，前n-1条指令的`unit_flag`设置为2，最后一条指令设置为3。
- 开启UnitFlag时，矩阵计算方向需与Fixpipe读取顺序保持一致。Fixpipe开启Nz2ND随路格式转换，或未进行随路格式转换但开启B8/B4量化并触发Channel Merge功能时，调用[asc_set_mmad_direction_n](./asc_set_mmad_direction_n.md)；其他场景调用[asc_set_mmad_direction_m](./asc_set_mmad_direction_m.md)。
- 开启UnitFlag后，建议矩阵乘加的计算数据量与Fixpipe搬出的数据量保持一致。两者不一致可能导致执行异常。需要清除UnitFlag产生的残留状态时，可调用[asc_set_l0c2gm_config](./asc_set_l0c2gm_config.md)，并将`enable_unit_flag`设置为true，将L0C Buffer中所有内存块的单元标志位设置为0。调用时`relu_pre`需传入0；后续搬出不使用Vector量化时，`quant_pre`也传入0，否则传入对应的Vector量化参数地址索引。
- 浮点类型的输入或输出包含INF/NaN时，可通过[asc_set_ctrl](../sys_var/asc_set_ctrl.md)配置CTRL[48]：设置为0时使用饱和模式，INF输出饱和为正负MAX、NaN输出饱和为0；设置为1时使用非饱和模式，INF/NaN保持原输出。Mmad指令应避免NaN输入。
<!-- end id11 -->

## 调用示例

<!-- npu="950" id12 -->
以下示例仅展示矩阵乘加接口的调用。调用前需将与矩阵A、B对应的ScaleA、ScaleB矩阵分别搬入L0A_MX Buffer和L0B_MX Buffer。

```cpp
constexpr uint16_t M = 16;
constexpr uint16_t K = 64;
constexpr uint16_t N = 16;
__ca__ fp8_e4m3fn_t a_matrix[M * K];
__cb__ fp8_e5m2_t b_matrix[K * N];
__cc__ float c_matrix[M * N];

uint8_t unit_flag = 0;
bool disable_gemv = false;
bool c_matrix_source = false;
bool c_matrix_init_val = true;
asc_mmad_mx_sync(c_matrix, a_matrix, b_matrix, M, K, N, unit_flag,
                 disable_gemv, c_matrix_source, c_matrix_init_val);
```

完整使用样例请参考[asc_mmad_mx样例](../../../../../../examples/02_simd_c_api/03_c_api/03_matrix_compute/mmad_mx)。
<!-- end id12 -->
