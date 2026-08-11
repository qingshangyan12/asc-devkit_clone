# asc_copy_l12l0a_mx

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

负责完成MX矩阵计算过程中所需的左矩阵对应的量化系数的搬运，数据通路为L1 Buffer->L0A_MX Buffer。其中左量化系数矩阵以32字节（固定数据类型为`fp8_e8m0_t`、分形大小为16×2的）的数据分形为单位进行搬运。

其中，L0A_MX Buffer的大小为4KB，和L0A Buffer地址的映射关系如下：

$$
L0A\_MX\ Buffer\ Address = L0A\ Buffer\ Address / 16
$$

本接口为MX矩阵计算量化系数搬运接口，需要与对应数据搬运接口[asc_copy_l12l0a](asc_copy_l12l0a/asc_copy_l12l0a_2d_arch_3510.md)配合使用。

本接口仅在AIC上执行有效，非AIC调用直接返回。

## 函数原型

```cpp
__aicore__ inline void asc_copy_l12l0a_mx(uint64_t dst,
                                          __cbuf__ fp8_e8m0_t* src,
                                          uint16_t x_start_pos,
                                          uint16_t y_start_pos,
                                          uint8_t x_step,
                                          uint8_t y_step,
                                          uint16_t src_stride,
                                          uint16_t dst_stride)

__aicore__ inline void asc_copy_l12l0a_mx_sync(uint64_t dst,
                                               __cbuf__ fp8_e8m0_t* src,
                                               uint16_t x_start_pos,
                                               uint16_t y_start_pos,
                                               uint8_t x_step,
                                               uint8_t y_step,
                                               uint16_t src_stride,
                                               uint16_t dst_stride)
```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| dst | 输出 | 目的操作数，存储位置为L0A_MX Buffer。目的地址需32字节对齐。 |
| src | 输入 | 源操作数，存储位置为L1 Buffer，数据类型固定为`fp8_e8m0_t`。源地址需32字节对齐。 |
| x_start_pos | 输入 | 源矩阵X轴起始分形位置，单位32字节（1个分型）。 |
| y_start_pos | 输入 | 源矩阵Y轴起始分形位置，单位32字节。 |
| x_step | 输入 | 源矩阵X轴方向搬运长度，单位为1个分形（1个单位代表一个32B的分形）。取值范围：[1, 255]。 |
| y_step | 输入 | 源矩阵Y轴方向搬运长度，单位为32B。取值范围：[1, 255]。 |
| src_stride | 输入 | 源矩阵X轴方向前一个分形起始地址与后一个分形起始地址的间隔，单位为32B。 |
| dst_stride | 输入 | 目的矩阵X轴方向前一个分形起始地址与后一个分形起始地址的间隔。单位为32B。 |

假设左矩阵A矩阵shape为（M，K），则ScaleA矩阵shape为（M，K/32），ScaleA数据类型为fp8_e8m0_t，ScaleA矩阵分形排布见[图1](#fig177256256134)。

**图 1** ScaleA在L0A Buffer的分形排布<a id="fig177256256134"></a>

![](../../../figures/scalea_l0a_fractal.png "ScaleA在L0A Buffer的分形排布")

下图为ScaleA从L1 Buffer搬运至L0A Buffer过程中的配置参数示意。每一行为32字节，对应着[图1](#fig177256256134)中的一个分形。xStep为X轴分形的个数，如图中的xStep = M / 16 = 3，yStep为Y轴32字节的个数，如图中的yStep = K / 32 / 2 = 21，srcStride和dstStride同理，表示在Y轴上32字节的个数。

**图 2**  ScaleA从L1 Buffer搬运至L0A Buffer的配置参数示意

![](../../../figures/LoadData_2D_MX_nd2nz.png "ScaleA从L1 Buffer搬运至L0A Buffer的配置参数示意")

## 返回值说明

无

## 流水类型

PIPE_MTE1

## 约束说明

- 各存储单元的空间大小和对齐要求请参考[存储单元说明](../general_description_and_constraints.md#存储单元说明)。

### 通用约束

- 本接口仅在AIC上生效，非AIC调用直接返回。
- dst起始地址需32字节对齐，否则触发地址对齐异常。
- src起始地址需32字节对齐（L1 Buffer对齐要求），否则触发地址对齐异常。
- 如果本指令与其他指令存在目的地址重叠，需要插入同步指令（[asc_sync_notify](../sync/asc_sync_notify.md)和[asc_sync_wait](../sync/asc_sync_wait.md)），保证多个指令串行化，防止出现异常数据。
- L0A_MX Buffer容量上限：L0A_MX Buffer总容量4KB，dst偏移+搬运大小不可越界，否则触发地址溢出异常。
- L1 Buffer容量上限：L1 Buffer总容量512KB，src偏移+源矩阵占用大小不可越界，否则触发地址溢出异常。

### 搬运约束

- 本接口需要与对应数据搬运接口[asc_copy_l12l0a](asc_copy_l12l0a/asc_copy_l12l0a_2d_arch_3510.md)配合使用。
- x_step/y_step设置为0，该接口将被视为NOP（空操作）。
- 量化系数矩阵的分形固定为`(16,2)`，对应L0A Buffer的分形为`(16,32)`，占L0A Buffer地址的1/16，dst与dst_stride需按地址约束设置，否则触发搬运异常。

## 调用示例

```cpp
__cbuf__ fp8_e8m0_t src[1024];
__ca__ fp8_e5m2_t dst[1024];
uint16_t x_start_pos = 0;
uint16_t y_start_pos = 0;
uint8_t x_step = 0;
uint8_t y_step = 0;
uint16_t src_stride = 8;
uint16_t dst_stride = 8;
uint64_t mx_dst_addr = static_cast<uint64_t>(reinterpret_cast<uintptr_t>(dst)) / 16;
asc_copy_l12l0a_mx(mx_dst_addr, src, x_start_pos, y_start_pos, x_step, y_step, src_stride, dst_stride);
```
