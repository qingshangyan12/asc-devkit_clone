# asc_set_atomic_none

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

清空原子操作的状态。一般和[asc_set_atomic_add](asc_set_atomic_add.md)，[asc_set_atomic_max](asc_set_atomic_max.md)，[asc_set_atomic_min](asc_set_atomic_min.md)接口配合使用，用于在完成原子操作后关闭原子操作，避免影响后续功能。

## 函数原型

```cpp
__aicore__ inline void asc_set_atomic_none()
```

## 参数说明

无
## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

- 本接口设置的控制寄存器位在AIC与AIV上均生效，AIC与AIV均可调用。
- 本接口与后续接口之间的同步由硬件保证。
- 本接口仅对后续目的地址为Global Memory的搬出搬运指令生效，对其他搬运动作不生效。
- 本接口配置写入后对后续所有目的地址为Global Memory的搬运指令生效，后续此类搬运按普通搬运语义直接覆盖目的地址原有数据；该配置持续生效，直至再次调用原子操作配置接口覆盖配置。
- 关闭原子操作前的带随路原子操作搬运指令与关闭原子操作后的普通搬运指令之间若发生乱序，会导致Global Memory上已完成原子累加或原子比较的数据被普通搬运的数据覆盖。若需保证确定性的原子操作结果落盘后再关闭原子操作，开发者需在本接口前插入核内同步，确保Global Memory已完成原子操作的数据落盘后再切换为普通搬运。
- 头文件 `atomic.h` 不能与编译选项 `--enable-simt` 同时使用，否则编译报错。

### 确定性计算约束

- 本接口用于关闭数据搬运随路原子操作。在带随路原子操作的搬运指令与本接口之间若发生乱序，会导致Global Memory上已完成原子累加或原子比较的数据被后续普通搬运错误覆盖。若需保证确定性的原子操作结果落盘后再关闭原子操作，开发者需在本接口前插入核内同步。
- 当整个原子操作跨多个核执行时，关闭原子操作的时序需通过核间同步方案协调，确保各核按预期顺序完成原子操作并切换为普通搬运。


## 调用示例

```cpp
//total_length指参与搬运的数据总长度。dst是外部输入的int16_t类型的GM内存。
constexpr uint32_t total_length = 256;
__ubuf__ int16_t src0[total_length];
__ubuf__ int16_t src1[total_length];
asc_set_atomic_add_int16();
asc_copy_ub2gm_sync(dst, src0, total_length * sizeof(int16_t));
asc_copy_ub2gm_sync(dst, src1, total_length * sizeof(int16_t));
asc_set_atomic_none();
```

结果示例：

```
输入数据src0：[1, 1, 1, ..., 1]  // int16_t类型
输入数据src1：[2, 2, 2, ..., 2]  // int16_t类型
输出数据dst：[3, 3, 3, ..., 3]   // int16_t类型
```
