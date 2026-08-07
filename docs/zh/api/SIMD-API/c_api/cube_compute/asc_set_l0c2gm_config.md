# asc_set_l0c2gm_config
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

头文件路径：`"c_api/cube_compute/cube_compute.h"`。

<!-- npu="950" id8 -->
本接口用于配置L0C Buffer搬出时的Fixpipe控制信息，包括Vector量化参数在Fixpipe Buffer中的起始地址和L0C Buffer的unitFlag清零控制。配置写入后，由后续的[asc_copy_l0c2gm](../cube_data_move/asc_copy_l0c2gm/asc_copy_l0c2gm_arch_3510.md)等L0C Buffer搬出接口使用。

当前版本仅支持配置Vector量化参数，`relu_pre`为预留参数。Vector量化参数需先搬运到Fixpipe Buffer，再通过本接口设置其地址。
<!-- end id8 -->

<!-- npu="A3,910b" id9 -->
数据搬运过程中进行随路量化时，通过调用该接口设置量化流程中的矢量量化参数。
<!-- end id9 -->

## 函数原型

```c
__aicore__ inline void asc_set_l0c2gm_config(uint64_t relu_pre,
                                             uint64_t quant_pre,
                                             bool enable_unit_flag)
```

## 参数说明

**表1** 参数说明

<!-- npu="950" id10 -->
| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| relu_pre | 输入 | Vector ReLU参数在Fixpipe Buffer中的起始地址索引，单位为64字节。当前版本为预留参数，请传入0。该参数仅低8 bit有效。 |
| quant_pre | 输入 | Vector量化参数在Fixpipe Buffer中的起始地址索引，单位为128字节，取值范围为[0, 255]。该参数仅低8 bit有效。Vector量化参数的地址需128字节对齐，地址索引的计算方式为：Fixpipe Buffer物理地址/128。仅当后续L0C Buffer搬出接口选择Vector量化模式时，本参数生效。 |
| enable_unit_flag | 输入 | 是否清零L0C Buffer所有存储块的unitFlag。取值为true时清零，取值为false时保持当前状态。本参数是一次性的清零控制，不用于开启后续搬出接口的unitFlag功能。 |
<!-- end id10 -->

<!-- npu="A3,910b" id11 -->
|参数名|输入/输出|描述|
|------------|------------|-----------|
| relu_pre | 输入     | ReLU操作前矢量的起始地址。|
| quant_pre | 输入     | 量化操作前矢量的起始地址。|
| enable_unit_flag | 输入     | 是否启用unit_flag。unit_flag是一种矩阵计算指令和矩阵搬运指令细粒度的并行，开启该功能后，硬件每计算完一个分形，计算结果就会被搬出，该功能不适用于L0C Buffer累加的场景。|
<!-- end id11 -->

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

<!-- npu="950" id12 -->
- 本接口仅在AIC上生效，在AIV上调用不生效。
- `relu_pre`为预留参数，需传入0。
- Vector量化参数需先搬运到Fixpipe Buffer。调用本接口后，需在后续搬出接口读取量化参数前完成PIPE_FIX流水同步。`quant_pre`超出取值范围时仅低8 bit生效，将导致硬件从错误的地址读取量化参数。
- 本接口的配置会持续生效，直至再次调用本接口覆盖。不同搬出任务使用的配置不同时，需在对应搬出接口执行前重新配置；每次调用均会同时覆盖三个参数。
- `enable_unit_flag`用于消除L0C Buffer中残留unitFlag对后续任务的影响。是否使用unitFlag搬出数据，由后续搬出接口的unitFlag相关参数控制。
<!-- end id12 -->

<!-- npu="A3,910b" id13 -->
支持以下三种传参形式：
- 同时设置relu_pre和quant_pre。
- 仅传入relu_pre，quant_pre传入0。
- 仅传入quant_pre，relu_pre传入0。
<!-- end id13 -->

## 调用示例

<!-- npu="950" id14 -->
```cpp
// quant_fb为已写入Vector量化参数且按128字节对齐的Fixpipe Buffer地址
uint64_t quant_pre = reinterpret_cast<uintptr_t>(quant_fb) / 128;
asc_set_l0c2gm_config(0, quant_pre, false);
asc_sync_pipe(PIPE_FIX);
```
<!-- end id14 -->

<!-- npu="A3,910b" id15 -->
```cpp
constexpr uint64_t relu_pre = 0;
constexpr uint64_t quant_pre = 0x1000;// 假设量化操作有效地址为0x1000
asc_set_l0c2gm_config(relu_pre, quant_pre, true);
```
<!-- end id15 -->
