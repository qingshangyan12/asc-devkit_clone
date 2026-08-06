# asc_set_l0c_copy_params

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

头文件路径：`"c_api/cube_datamove/cube_datamove.h"`。

设置L0C Buffer搬运到Global Memory或L1 Buffer过程中执行随路格式转换（NZ格式转换为ND格式）所需的ND循环参数，包含ND矩阵搬运块数、源端相邻ND矩阵间的步长、目的端相邻ND矩阵间的步长。配置写入后作用于后续调用的开启NZ2ND随路格式转换的L0C Buffer搬运接口，由对应搬运接口在NZ2ND模式下读取循环参数展开ND矩阵迭代。

本接口仅在AIC上执行有效，在AIV上调用为空操作。

本接口需与配套setter接口按功能分组配合使用，随路量化模式下需预先通过`asc_set_l0c_copy_prequant`配置前级量化参数，再调用本接口配置ND循环参数，最后由L0C Buffer搬运接口在NZ2ND随路格式转换模式下读取并执行搬运。

## 函数原型

```cpp
__aicore__ inline void asc_set_l0c_copy_params(uint16_t nd_num, uint16_t src_nd_stride, uint16_t dst_nd_stride)
```


## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| `nd_num` | 输入 | NZ2ND随路格式转换时ND矩阵搬运块数，即最外层ND矩阵迭代次数。取值范围 [0, 2^16−1]，开启NZ2ND的搬运接口要求该值大于0。 |
| `src_nd_stride` | 输入 | NZ2ND随路格式转换时源端（L0C Buffer）相邻ND矩阵间的步长，单位16个源元素（即一个C0分形列大小，源dtype为`float`/`int32_t`时为64字节，`half`/`bfloat16_t`/`int16_t`时为32字节，`int8_t`/`uint8_t`时为16字节）。取值范围 [0, 2^16−1]。 |
| `dst_nd_stride` | 输入 | NZ2ND随路格式转换时目的端（GM或L1 Buffer）相邻ND矩阵间的步长，单位1个目的元素。取值范围 [0, 2^16−1]，当ND矩阵搬运块数大于1时该值不可为0。 |

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

<!-- npu="950" id8 -->
### 通用约束

- **本接口仅在AIC上生效，非AIC调用直接返回。**
- 本接口仅设置NZ2ND随路格式转换的ND循环参数，实际搬运由后续调用的开启NZ2ND的L0C Buffer搬运接口执行，须先调用本接口写入配置，再调用对应搬运接口并使能NZ2ND，否则搬运接口读取未配置的循环参数产生错误结果。
- 配置写入后对后续所有开启NZ2ND的L0C Buffer搬运接口生效，直至再次调用本接口覆盖配置；不同搬运任务间若需不同ND循环参数，应在调用搬运接口前重新调用本接口设置。
- 本接口须与其他配套setter接口配合使用：先调用配置量化参数等随路能力的配套setter接口，再调用本接口配置ND循环参数，最后调用开启NZ2ND的L0C Buffer搬运接口执行搬运（配套setter名见功能说明章节）。

### ND循环参数约束

- `nd_num`在后续搬运接口开启NZ2ND时须大于0，否则触发搬运接口非法配置异常，搬运指令被置为空操作。
- `dst_nd_stride`在`nd_num`大于1时不应为0，否则相邻ND矩阵在目的端地址重叠，产生错误结果。
- `dst_nd_stride`在`nd_num`大于1时需按目的端dtype满足对齐约束：目的端为UB或L1 Buffer时需与目的dtype字节对齐（`int4b_t`需为64的倍数，其他dtype需为`32/sizeof(dst dtype)`的倍数）；目的端为GM时`int4b_t`需为2的倍数，其他dtype按对应字节对齐。不满足时目的端地址偏移不对齐，可能产生异常结果。
- `nd_num`，参数类型`uint16_t`可表示0~2^16−1，传入>15的值会截断处理（如16→0、17→1）。截断后实际生效值偏小，导致实际迭代次数小于预期，循环提前结束，部分ND矩阵未被搬运；截断后若值为0会同时触发后续搬运接口`nd_num`不可为0的异常，搬运指令被置为空操作。
- `src_nd_stride`，参数类型`uint16_t`可表示0~2^16−1，传入>31的值会截断处理（如32→0、33→1）。截断后实际生效值偏小，导致源端相邻ND矩阵间的地址偏移量小于预期，后一次迭代读到错误位置，与前一次迭代数据重叠或错位。
- `dst_nd_stride`，参数类型`uint16_t`可表示0~2^16−1，传入>63的值会截断处理（如64→0、65→1）。截断后实际生效值偏小，导致目的端相邻ND矩阵间的地址偏移量小于预期，后一次迭代写到错误位置，与前一次迭代数据重叠或错位。

<!-- end id8 -->

## 调用示例

```cpp
uint16_t nd_num = 2;
uint16_t src_nd_stride = 2;
uint16_t dst_nd_stride = 1;
asc_set_l0c_copy_params(nd_num, src_nd_stride, dst_nd_stride);
```
