# asc_copy_l12l0b_mx

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

将Cube计算所需的MX量化系数矩阵数据从L1 Buffer搬运到L0B Buffer的MX专用区域。以32字节的分形为单位，按X维与Y维的起始位置、步长从L1 Buffer中较大的源矩阵切分提取多个32字节对齐的分形矩阵写入L0B MX Buffer区域，用于Cube计算中随路量化系数矩阵的加载。源矩阵在L1 Buffer中以列主序分形存放，分形内元素按行序排列。

本接口仅在AIC上执行有效，在AIV上调用为空操作。

## 函数原型

- 常规搬运

    ```cpp
    __aicore__ inline void asc_copy_l12l0b_mx(uint64_t dst, __cbuf__ fp8_e8m0_t* src, uint16_t x_start_pos, uint16_t y_start_pos, uint8_t x_step, uint8_t y_step, uint16_t src_stride, uint16_t dst_stride)
    ```

- 同步搬运

    ```cpp
    __aicore__ inline void asc_copy_l12l0b_mx_sync(uint64_t dst, __cbuf__ fp8_e8m0_t* src, uint16_t x_start_pos, uint16_t y_start_pos, uint8_t x_step, uint8_t y_step, uint16_t src_stride, uint16_t dst_stride)
    ```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| dst | 输出 | 目的操作数，存储位置为L0B MX Buffer，以32字节为单位寻址。目的地址需32字节对齐。取值范围[0, 2^64−1]，但实际有效范围受L0B MX Buffer容量约束（见约束说明）。 |
| src | 输入 | 源操作数，存储位置为L1 Buffer，类型固定为`__cbuf__ fp8_e8m0_t*`。源地址需32字节对齐。 |
| x_start_pos | 输入 | 源矩阵中首个分形在X维的起始位置，单位1个32字节分形。取值范围[0, 2^16−1]。 |
| y_start_pos | 输入 | 源矩阵中首个分形在Y维的起始位置，单位32字节。取值范围[0, 2^16−1]。 |
| x_step | 输入 | 提取矩阵在X维的加载步长，即一次加载的分形行数，单位1个32字节分形。取值范围[1, 255]。 |
| y_step | 输入 | 提取矩阵在Y维的加载步长，即一次加载的分形列数，单位32字节。取值范围[1, 255]。 |
| src_stride | 输入 | 源端（L1 Buffer）相邻X步之间的步长，单位32字节。取值范围[0, 2^16−1]。 |
| dst_stride | 输入 | 目的端（L0B MX Buffer）相邻X步之间的步长，单位32字节。取值范围[0, 2^16−1]。 |

## 返回值说明

无

## 流水类型

PIPE_MTE1

## 约束说明

- 各存储单元的空间大小和对齐要求请参考[存储单元说明](../general_description_and_constraints.md#存储单元说明)。
- scale矩阵的分形固定为`(16,2,16)`，对应L0B Buffer的分形为`(16,32,16)`，占L0B Buffer地址的1/16，需要按照这种地址约束存放地址。
- dst（L0B Buffer的地址）的数据类型支持fp4x2_e2m1_t、fp4x2_e1m2_t、fp8_e5m2_t、fp8_e4m3fn_t，src的数据类型支持fp8_e8m0_t。

### 通用约束

- **本接口仅在AIC上生效，非AIC调用直接返回。**
- dst起始地址需32字节对齐（L0B MX对齐要求），否则触发异常。
- src起始地址需32字节对齐（L1 Buffer对齐要求），否则触发异常。
- 如果本指令与其他指令存在目的地址重叠，需要插入同步指令（[asc_sync_notify](../sync/asc_sync_notify.md)和[asc_sync_wait](../sync/asc_sync_wait.md)），保证多个指令串行化，防止出现异常数据。
- L1 Buffer容量上限：L1 Buffer总容量512KB，src偏移+源矩阵占用大小不可越界，否则触发地址溢出异常。
- L0B MX容量上限：L0B MX专用区域总容量4KB，dst偏移+目的矩阵占用大小不可越界，否则触发地址溢出异常。
- Cube指令对L0A MX/L0B MX缓冲区有特定的布局与地址对齐约束，dst与dst_stride需按要求设置以匹配对应约束，否则触发异常。
- 本接口不支持基于FLAG置位与等待的同步机制。

### 加载步长约束

- x_step不可为0，否则指令被置为NOP并打印警告，不再执行后续搬运。
- y_step不可为0，否则指令被置为NOP并打印警告，不再执行后续搬运。
- 源矩阵在L1 Buffer中的占用不可超过L1 Buffer容量（含起始地址偏移），目的矩阵在L0B MX中的占用不可超过L0B MX容量（含起始地址偏移），否则触发地址溢出异常。

## 调用示例

```cpp
__cbuf__ fp8_e8m0_t src[1024];
__cb__ fp8_e5m2_t dst[1024];
uint16_t x_start_pos = 0;
uint16_t y_start_pos = 0;
uint8_t x_step = 0;
uint8_t y_step = 0;
uint16_t src_stride = 8;
uint16_t dst_stride = 8;
uint64_t mx_dst_addr = static_cast<uint64_t>(reinterpret_cast<uintptr_t>(dst)) / 16;
asc_copy_l12l0b_mx(mx_dst_addr, src, x_start_pos, y_start_pos, x_step, y_step, src_stride, dst_stride);
```
