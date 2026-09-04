# asc_get_uintdiv_magic_and_shift

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

根据无符号除数计算与`asc_uintdiv`配套使用的magic和shift参数，避免使用除法指令。调用本接口获得参数后，将其传递给`asc_uintdiv`即可完成对应位宽的快速无符号除法。

## 函数原型

```cpp
__aicore__ inline void asc_get_uintdiv_magic_and_shift(uint32_t* magic, uint32_t* shift, uint32_t divisor)
```

```cpp
__aicore__ inline void asc_get_uintdiv_magic_and_shift(uint64_t* magic, uint64_t* shift, uint64_t divisor)
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| :------ | :--- | :------------ |
| magic | 输出 | 快速除法乘法因子。 |
| shift | 输出 | 快速除法移位量。 |
| divisor | 输入 | 无符号除数，不能为0。 |

## 返回值说明

无返回值。计算结果通过`magic`和`shift`指针参数输出。

## 约束说明

1. 仅支持`uint32_t`和`uint64_t`重载。
2. 当被除数为`uint32_t`数据类型时，支持的被除数最大值为`INT32_MAX`，即2的31次方减1；当被除数为`uint64_t`数据类型时，支持的被除数最大值为`INT64_MAX`，即2的63次方减1。
3. 本接口仅支持Ascend 950（NPU架构`dav-3510`）的SIMD+SIMT新同构编程方式，不支持纯SIMT编程方式。
4. `magic`和`shift`指针必须指向可写的标量存储空间，且不能为`nullptr`。
5. 本接口运行在标量流水（`PIPE_S`）上，同一流水内的数据依赖由指令执行顺序保证，无需额外同步。

## 调用示例

```cpp
uint32_t value = 123;
uint32_t magic = 0;
uint32_t shift = 0;
asc_get_uintdiv_magic_and_shift(&magic, &shift, 7);
uint32_t quotient = asc_uintdiv(value, magic, shift);
```
