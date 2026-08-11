# asc_set_atomic_add

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

原子累加过程：将待搬运到GM的数据和GM上已有数据进行求和，然后将求和结果写入GM。本接口对后续目的地址为GM的数据搬运指令开启原子累加，不同产品支持的数据搬运通路请参考[约束说明](#约束说明)。

接口可选择不同的函数原型来设定不同的累加数据类型。

## 函数原型

```c
__aicore__ inline void asc_set_atomic_add_int8()
__aicore__ inline void asc_set_atomic_add_int16()
__aicore__ inline void asc_set_atomic_add_int32()
__aicore__ inline void asc_set_atomic_add_float16()
__aicore__ inline void asc_set_atomic_add_bfloat()
__aicore__ inline void asc_set_atomic_add_float()
```

## 参数说明

无

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

- 各个产品由于硬件架构不同，支持的数据通路也不同，具体情况如下：
    <!-- npu="950" id7 -->
    - Ascend 950PR/Ascend 950DT，支持的数据通路为UB/L0C Buffer-\>GM。
    <!-- end id7 -->

    <!-- npu="A3" id8 -->
    - Atlas A3 训练系列产品/Atlas A3 推理系列产品，支持的数据通路为UB/L0C Buffer/L1 Buffer-\>GM。
    <!-- end id8 -->

    <!-- npu="910b" id9 -->
    - Atlas A2 训练系列产品/Atlas A2 推理系列产品，支持的数据通路为UB/L0C Buffer/L1 Buffer-\>GM。
    <!-- end id9 -->
- 本接口调用后会对后续所有目的地址为GM的搬运指令开启原子操作，可以调用[asc_set_atomic_none](./asc_set_atomic_none.md)接口关闭原子操作。 
- 该接口执行前不会自动将GM上已有数据置零，如果在原子累加之前，需要保证GM上已有数据全为零，开发者需要手动清零。
- AIC与AIV中均可调用本接口。
- 本接口仅对后续目的地址为GM的搬运指令（通过MTE1/MTE2/MTE3单元搬运）生效，对于标量写GM的指令（例如[asc_store_dev](../scalar_compute/asc_store_dev.md)）不生效。
- 本接口与紧邻的后续搬运指令之间的同步由硬件保证，因此以下示例中插入的多流水同步是不必要的：

    ```cpp
    asc_set_atomic_add_int8();

    /*
    asc_set_atomic_add_int8与asc_copy_ub2gm之间的同步由硬件保证，因此以下同步是不必要的。
    asc_sync_notify(PIPE_S, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_S, PIPE_MTE3, EVENT_ID0);
    */

    asc_copy_ub2gm(dst, src1, total_length * sizeof(int8_t));
    // 关闭原子操作
    asc_set_atomic_none();
    ```

- 后续搬运指令的操作数据类型需与所选接口设置的数据类型一致。
- 本接口不能保证后续搬运指令的执行顺序，若需保证确定性的执行顺序请参考[关键特性说明](key_features.md)。
- 本接口不支持纯SIMT编译模式（设置编译选项`--enable-simt`），在该模式下包含头文件`atomic.h`会导致编译失败。

## 调用示例

```cpp
//total_length指参与搬运的数据总长度。dst是外部输入的int8_t类型的GM内存。
constexpr uint32_t total_length = 256;
__ubuf__ int8_t src0[total_length];
__ubuf__ int8_t src1[total_length];
asc_copy_ub2gm(dst, src0, total_length * sizeof(int8_t));
asc_sync_pipe(PIPE_MTE3);
asc_set_atomic_add_int8();
asc_copy_ub2gm(dst, src1, total_length * sizeof(int8_t));
asc_set_atomic_none();
```

结果示例：

```text
输入数据src0：[1, 1, 1, ..., 1]  // int8_t类型
输入数据src1：[2, 2, 2, ..., 2]  // int8_t类型
输出数据dst：[3, 3, 3, ..., 3]   // int8_t类型
```
