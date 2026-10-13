# asc_copy_gm2l1_nd2nz_arch_9201

## 产品支持情况

<!-- npu="960" id1 -->
- Ascend 960PR/Ascend 960DT：支持
<!-- end id1 -->
<!-- npu="950" id2 -->
- Ascend 950PR/Ascend 950DT：不支持
<!-- end id2 -->
<!-- npu="A3" id3 -->
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：不支持
<!-- end id3 -->
<!-- npu="910b" id4 -->
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：不支持
<!-- end id4 -->
<!-- npu="310b" id5 -->
- Atlas 200I/500 A2 推理产品：不支持
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas 推理系列产品AI Core：不支持
<!-- end id6 -->
<!-- npu="310p" id7 -->
- Atlas 推理系列产品Vector Core：不支持
<!-- end id7 -->
<!-- npu="910" id8 -->
- Atlas 训练系列产品：不支持
<!-- end id8 -->

## 功能说明

头文件路径为：`"c_api/cube_datamove/cube_datamove.h"`。

将数据从Global Memory搬运到L1 Buffer，并在搬运过程中将源端按N维连续排布的ND格式数据转换为L1 Buffer侧的Nz分形排布。该接口也适用于将卷积场景的NHWC（ND）格式转换为NC1HWC0格式。

调用本接口前，需通过[asc_set_gm2l1_nz_para](../cube_load_aux_config/asc_set_gm2l1_nz_para.md)配置目的Nz矩阵步长和ND矩阵搬运个数。

本接口仅在AIC上执行有效。

## 函数原型

```cpp
__aicore__ inline void asc_copy_gm2l1_nd2nz(__cbuf__ <dtype>* dst,
                                            __gm__ <dtype>* src,
                                            uint64_t src_d_value,
                                            asc_load_l2_cache_mode l2_cache_mode,
                                            uint16_t n_value,
                                            uint32_t d_value,
                                            uint64_t src_nd_matrix_stride,
                                            bool enable_small_c0,
                                            bool enable_pre_load_ignore_sync)
```

`dtype`支持的数据类型为`int8_t`、`uint8_t`、`hifloat8_t`、`fp8_e4m3fn_t`、`fp8_e5m2_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float`。

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| dst | 输出 | 目的操作数，存储位置为L1 Buffer。起始地址需32字节对齐。 |
| src | 输入 | 源操作数，存储位置为GM。起始地址需1字节对齐。 |
| src_d_value | 输入 | 源矩阵相邻行起始地址之间的偏移，单位为字节。取值范围：[0, $2^{40}-1$]。 |
| l2_cache_mode | 输入 | 配置源端GM数据在L2 Cache中的管理策略，类型为[asc_load_l2_cache_mode](../../defs/enum/asc_load_l2_cache_mode.md)，具体取值及含义请参见该枚举说明。 |
| n_value | 输入 | 单个ND矩阵的行数，单位为元素。取值范围：[1, 65535]。 |
| d_value | 输入 | 单个ND矩阵的列数，单位为元素。取值范围：[1, $2^{21}-1$]。当$d\_value \times \mathrm{sizeof}(\mathrm{dtype})$不满足32字节对齐时，硬件在目的矩阵中自动补0至32字节对齐。 |
| src_nd_matrix_stride | 输入 | 源端相邻ND矩阵起始地址之间的偏移，单位为字节。取值范围：[0, $2^{40}-1$]。搬运一个ND矩阵时设置为0；搬运多个ND矩阵且设置为0时，将重复读取第一个ND矩阵。 |
| enable_small_c0 | 输入 | SmallC0模式开关。`false`表示按标准Nz格式的C0（32字节）排布；`true`表示开启SmallC0模式，仅当$d\_value \le 4$时可开启。 |
| enable_pre_load_ignore_sync | 输入 | 是否开启预加载忽略同步功能。`false`表示关闭；`true`表示开启。 |

## 返回值说明

无

## 流水类型

PIPE_MTE2

## 约束说明

- 本接口非AIC调用直接返回。
- 调用本接口前，需先调用[asc_set_gm2l1_nz_para](../cube_load_aux_config/asc_set_gm2l1_nz_para.md)配置目的Nz矩阵步长与ND矩阵搬运个数。
- `dst`起始地址需32字节对齐，`src`起始地址需1字节对齐。
- `src_d_value`和`src_nd_matrix_stride`取值范围为[0, $2^{40}-1$]，超出取值范围时高位会被截断。
- `n_value`取值范围为[1, 65535]，`d_value`取值范围为[1, $2^{21}-1$]，超出取值范围时高位会被截断。
- `enable_small_c0`设置为`true`时，需满足$d\_value \le 4$。
- 开启`enable_pre_load_ignore_sync`前，需确保该搬运与前序访问相同地址的指令不存在数据依赖，否则可能产生异常数据。
- `dst`偏移量与搬运大小之和不可超出L1 Buffer容量。

## 调用示例

以下示例将一个16 x 16的`half`类型ND矩阵搬运到L1 Buffer并转换为Nz格式，显式关闭SmallC0模式和预加载忽略同步功能。

```cpp
#include "c_api/asc_simd.h"

__global__ __cube__ void CopyGm2l1Nd2nzKernel(__gm__ half* src)
{
    asc_init();
    __cbuf__ half dst[256];

    // 配置目的Nz矩阵布局，并将一个16 x 16的ND矩阵搬入L1 Buffer。
    asc_set_gm2l1_nz_para(1, 1, 16, 0);
    asc_copy_gm2l1_nd2nz(dst, src, 16 * sizeof(half),
        asc_load_l2_cache_mode::NORMAL_FIRST_VICTIM,
        16, 16, 0, false, false);
    asc_sync_pipe(PIPE_MTE2);
}
```
