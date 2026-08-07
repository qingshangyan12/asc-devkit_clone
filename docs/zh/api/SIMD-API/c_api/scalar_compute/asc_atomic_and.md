# asc_atomic_and

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

头文件路径为：`"c_api/scalar_compute/scalar_compute.h"`。

对Global Memory中`address`指向的单个元素执行原子按位与操作：读取该地址上的旧值`old_value`，将`old_value & val`的结果写回同一地址，并返回`old_value`。整个“读取—按位与—写回”过程通过原子锁保证串行化，多个AI Core或多线程并发调用同一`address`时不会相互覆盖，最终按位与结果符合预期。

本接口为标量原子操作，操作数为单个标量元素（不支持向量或连续多元素），数据类型由重载决定（4种重载分别支持`int32_t`、`uint32_t`、`int64_t`、`uint64_t`）。本接口只对当前一次单点读改写操作原子化，不影响后续任何搬运指令。

本接口为标量运算，运行在标量流水上，AIC与AIV均可调用，行为一致。

## 函数原型

### 模板原型（占位符形式）

```c
__aicore__ inline <dtype> asc_atomic_and(__gm__ <dtype>* address,
                                         <dtype> val)
```

### dtype支持数据类型

dtype支持的数据类型为`int32_t`、`uint32_t`、`int64_t`、`uint64_t`。

### 典型示例

```c
// 示例：int32_t类型原子按位与。
__aicore__ inline int32_t asc_atomic_and(__gm__ int32_t* address,
                                         int32_t val)
```

## 参数说明

**表 1**  参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| `address` | 输入/输出 | 待执行原子按位与的Global Memory标量地址。<br>• `int32_t`重载：`__gm__ int32_t*`，按4字节访问；<br>• `uint32_t`重载：`__gm__ uint32_t*`，按4字节访问；<br>• `int64_t`重载：`__gm__ int64_t*`，按8字节访问；<br>• `uint64_t`重载：`__gm__ uint64_t*`，按8字节访问。地址需按所访问数据类型的字节大小自然对齐（4字节重载4字节对齐、8字节重载8字节对齐），否则访问异常。地址必须落在Global Memory地址空间，传入UB、L1 Buffer、L0C Buffer、栈等非GM地址空间时硬件不保证原子语义且可能触发异常。 |
| `val` | 输入 | 与GM旧值执行按位与的标量值。数据类型与`address`指向元素类型一致（即重载决定dtype）。`int32_t`重载取值范围为[−2^31，2^31−1]、`uint32_t`重载取值范围为[0，2^32−1]、`int64_t`重载取值范围为[−2^63，2^63−1]、`uint64_t`重载取值范围为[0，2^64−1]。参数类型与底层操作数位宽一致，无字段截断风险。 |

## 返回值说明

返回`address`修改前的旧值`old_value`，类型由重载决定（与`val`同dtype）：

- `int32_t`重载返回`int32_t`，取值范围为[−2^31，2^31−1]。
- `uint32_t`重载返回`uint32_t`，取值范围为[0，2^32−1]。
- `int64_t`重载返回`int64_t`，取值范围为[−2^63，2^63−1]。
- `uint64_t`重载返回`uint64_t`，取值范围为[0，2^64−1]。

调用方接收返回值即可拿到本接口生效前的GM原值，可用于实现位标志清零、位掩码裁剪、无锁位字段更新等需要“读旧值+写新值”原子语义的场景。

## 流水类型

PIPE_S

## 约束说明

### 通用约束

- 本接口为标量单元素原子按位与运算，操作数固定为单个标量（不支持向量或连续多元素），dtype由4种重载决定（`int32_t`、`uint32_t`、`int64_t`、`uint64_t`），不可配置其他数据类型。
- `address`必须落在Global Memory地址空间。传入UB、L1 Buffer、L0C Buffer、栈等非GM地址空间时，硬件不保证原子语义且可能触发异常。
- `address`需按所访问数据类型的字节大小自然对齐：4字节重载（`int32_t`、`uint32_t`）需4字节对齐，8字节重载（`int64_t`、`uint64_t`）需8字节对齐，否则访问异常。
- `val`与`address`指向元素的数据类型由重载决定，两者类型一致，无需额外dtype配置。
- 本接口通过原子锁对“读取—按位与—写回”全过程串行化，多个AI Core或多线程并发调用同一`address`时不会相互覆盖。按位与运算满足交换律与结合律，多次按位与的最终结果与执行顺序无关，并发结果确定性可保证。
- 按位与运算为纯位级运算，不产生溢出、进位、NaN/Inf等异常，不触发任何浮点或整数异常上报；对负数按补码位模式参与运算，结果按各自C类型位宽解释。
- 本接口运行在标量流水上，与后续依赖该返回值或同一GM地址的指令之间存在标量数据依赖，由标量流水本身的顺序执行保证；但若同一核内存在与本接口访问同一GM地址的数据搬运指令，且两者执行顺序影响最终结果，需由调用方按需插入`asc_sync_pipe`或`asc_sync_notify`与`asc_sync_wait`同步指令保证执行顺序。
- 即使开启编译器自动同步功能，编译器也无法为`asc_atomic_and`访问GM与`PIPE_MTE2`/`PIPE_MTE3`数据搬运之间的数据依赖自动插入同步。存在依赖时，需根据实际流水关系手动插入[asc_sync_pipe](../sync/asc_sync_pipe.md)，或成对使用[asc_sync_notify](../sync/asc_sync_notify.md)和[asc_sync_wait](../sync/asc_sync_wait.md)保证执行顺序。
- 标量原子操作会旁路DCache直接访问GM，不维护DCache一致性。若其他通路通过DCache读写同一GM地址，需按实际读写关系调用[asc_dcci](../cache_ctrl/asc_dcci.md)清理或失效相应Cache Line；需要保证缓存维护与原子访存的顺序和可见性时，还需使用[asc_sync_data_barrier](../sync/asc_sync_data_barrier.md)。

## 调用示例

```cpp
// dst为外部输入的uint32_t类型的GM内存，此示例的初始值为0xF。
asc_dcci_entire_all();            // 手动同步。
uint32_t old = asc_atomic_and(dst, 0x6);  // 计算后dst = 0x6，old = 0xF。
```
