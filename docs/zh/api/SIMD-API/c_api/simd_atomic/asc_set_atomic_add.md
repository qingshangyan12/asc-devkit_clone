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

设置对后续的从Unified Buffer/L0C Buffer/L1 Buffer到GM的数据传输开启原子累加。累加的数据类型支持int8_t、int16_t、half、bfloat16_t、int32_t、float。
开启原子累加后，后续执行搬运操作从Unified Buffer/L0C Buffer/L1 Buffer到GM时，GM中原始数据将与新搬运数据进行逐元素累加，累加结果写回GM。
<!-- npu="950" id8 -->
特别地，针对Ascend 950PR/Ascend 950DT，不支持L1 Buffer到GM的通路。
<!-- end id8 -->

## 函数原型

```cpp
__aicore__ inline void asc_set_atomic_add_int8()
__aicore__ inline void asc_set_atomic_add_int16()
__aicore__ inline void asc_set_atomic_add_int32()
__aicore__ inline void asc_set_atomic_add_float16()
__aicore__ inline void asc_set_atomic_add_bfloat()
__aicore__ inline void asc_set_atomic_add_int()
__aicore__ inline void asc_set_atomic_add_float()
```

## 参数说明

无
## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

- 使用完成后，建议清空原子操作的状态（详见[asc_set_atomic_none](./asc_set_atomic_none.md)），以免影响后续相关指令功能。
- 该指令执行前不会对GM的数据做清零操作，开发者可以在需要时手动添加清零操作。
<!-- npu="950" id9 -->
- Ascend 950PR/Ascend 950DT，不支持L1 Buffer到GM的通路。
<!-- end id9 -->


- 本接口设置的控制寄存器位在AIC与AIV上均生效，AIC与AIV均可调用。 <span style="color:red">⚠️ **缺失接口**：`add_float`、`add_float16`、`min_float16`、`max_bfloat`、`max_float16`、`add_bfloat`（`add_bfloat` 写的是"仅在AIC上生效"，与其他接口矛盾）</span>
- 本接口仅对后续目的地址为Global Memory的搬出搬运指令生效，对其他搬运动作不生效。 <span style="color:red">⚠️ **缺失接口**：`add_int`</span>
- 调用本接口时不需要手动插入同步，本接口走Scalar流水写配置寄存器，与后续搬运指令分属不同流水，本接口与紧邻的后续搬运指令之间的同步由硬件保证。
- 后续搬出搬运指令的操作数据类型需与本接口设置的数据类型一致，否则操作结果与预期不符（硬件不报错）。 <span style="color:red">⚠️ **缺失接口**：`add_int`</span>
- 本接口配置写入后对后续所有目的地址为Global Memory的搬运指令生效，直至再次调用本类原子操作配置接口覆盖配置；调用后如需切换为其他数据类型的原子操作、改用其他原子操作配置或关闭原子操作，须显式调用对应接口覆盖配置。
- 开启原子操作前的普通搬运指令与开启原子操作后的搬运指令之间若发生乱序，会导致Global Memory上已完成原子操作的数据被普通搬运的数据覆盖。若需保证确定性的执行顺序，开发者需在开启原子操作的指令前插入核内同步，确保Global Memory初始值符合预期。
- 头文件`atomic.h`不能与编译选项`--enable-simt`同时使用，否则编译报错。 <span style="color:red">⚠️ **缺失接口**：`add_float`、`add_bfloat`、`add_float16`、`min_float`、`min_bfloat`、`max_float`、`max_bfloat`</span>





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

```
输入数据src0：[1, 1, 1, ..., 1]  // int8_t类型
输入数据src1：[2, 2, 2, ..., 2]  // int8_t类型
输出数据dst：[3, 3, 3, ..., 3]   // int8_t类型
```
