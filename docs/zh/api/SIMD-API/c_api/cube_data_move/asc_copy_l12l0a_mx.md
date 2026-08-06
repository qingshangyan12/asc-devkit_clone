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

将L1 Buffer中的`fp8_e8m0_t`缩放矩阵（Scale Matrix A）按2D分形搬运到L0A MX Buffer缓冲，供后续矩阵乘使用。本接口为矩阵引擎（MX）2D加载专用接口，分形大小固定为32字节（即16个`fp8_e8m0_t`元素），按X维（K方向）与Y维（M方向）的起点与步长，从源矩阵中提取若干32字节对齐的子矩阵写入L0A MX Buffer。本接口仅在AIC上执行有效，在AIV上调用为空操作。

## 函数原型

- 常规搬运

    ```cpp
    __aicore__ inline void asc_copy_l12l0a_mx(uint64_t dst, __cbuf__ fp8_e8m0_t* src, uint16_t x_start_pos, uint16_t y_start_pos, uint8_t x_step, uint8_t y_step, uint16_t src_stride, uint16_t dst_stride)
    ```
- 同步搬运

    ```cpp
    __aicore__ inline void asc_copy_l12l0a_mx_sync(uint64_t dst, __cbuf__ fp8_e8m0_t* src, uint16_t x_start_pos, uint16_t y_start_pos, uint8_t x_step, uint8_t y_step, uint16_t src_stride, uint16_t dst_stride)
    ```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| dst | 输出 | 目的操作数，存储位置为L0A MX Buffer。目的地址需512字节对齐。L0A MX Buffer总容量4KB，dst偏移 + 搬运大小不可越界。 |
| src | 输入 | 源操作数，存储位置为L1 Buffer，数据类型固定为`fp8_e8m0_t`。源地址需32字节对齐。 |
| x_start_pos | 输入 | X维（K方向）起始分形位置，单位1（即一个32字节分形）。取值范围 [0, 2^16−1]。 |
| y_start_pos | 输入 | Y维（M方向）起始分形位置，单位32字节。取值范围 [0, 2^16−1]。 |
| x_step | 输入 | X维（K方向）提取步长，即一次提取的32字节分形个数。取值范围 [1, 255]。 |
| y_step | 输入 | Y维（M方向）提取步长，即一次提取的32字节分形个数。取值范围 [1, 255]。 |
| src_stride | 输入 | 相邻X步在源端（L1 Buffer）的步长，单位32字节。取值范围 [0, 2^16−1]，即使x_step为1也不可为0。 |
| dst_stride | 输入 | 相邻Y步在目的端（L0A MX Buffer）的步长，单位32字节。取值范围 [0, 2^16−1]，即使y_step为1也不可为0。 |

## 返回值说明

无

## 流水类型

PIPE_MTE1

## 约束说明

- 各存储单元的空间大小和对齐要求请参考[存储单元说明](../general_description_and_constraints.md#存储单元说明)。
- scale矩阵的分形固定为`(16,2,16)`，对应L0A Buffer的分形为`(16,32,16)`，占L0A Buffer地址的1/16，需要按照这种地址约束存放地址。
- dst（L0A Buffer的地址）的数据类型支持fp4x2_e2m1_t、fp4x2_e1m2_t、fp8_e5m2_t、fp8_e4m3fn_t，src的数据类型支持fp8_e8m0_t。

### 通用约束

- 本接口仅在AIC上生效，非AIC调用直接返回。
- dst起始地址需512字节对齐（L0A Buffer对齐要求），否则触发地址对齐异常。
- src起始地址需32字节对齐（L1 Buffer对齐要求），否则触发地址对齐异常。
- 如果本指令与其他指令存在目的地址重叠，需要插入同步指令（[asc_sync_notify](../sync/asc_sync_notify.md)和[asc_sync_wait](../sync/asc_sync_wait.md)），保证多个指令串行化，防止出现异常数据。
- L0A MX Buffer容量上限：L0A MX Buffer总容量4KB，dst偏移+搬运大小不可越界，否则触发地址溢出异常。
- L1 Buffer容量上限：L1 Buffer总容量512KB，src偏移+源矩阵占用大小不可越界，否则触发地址溢出异常。
- Cube指令对L0A MX/L0B MX缓冲区有特定的布局与地址对齐约束，dst与dst_stride需按要求设置以匹配对应约束，否则触发搬运异常。
- 本接口不支持HSET FLAG与HWAIT FLAG机制。

### 2D加载模式约束

- x_step不可为0，否则指令被置为NOP不执行并打印警告。
- y_step不可为0，否则指令被置为NOP不执行并打印警告。
- src_stride即使x_step为1也不可为0，否则触发异常。
- dst_stride即使y_step为1也不可为0，否则触发异常。
- dtype固定为`fp8_e8m0_t`，本接口仅用于缩放矩阵搬运，传入其他dtype的源指针会触发异常。
- dst与dst_stride需按Cube指令在L0A MX Buffer上的分形排布与地址对齐约束正确配置，否则影响后续矩阵乘的正确性。

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
