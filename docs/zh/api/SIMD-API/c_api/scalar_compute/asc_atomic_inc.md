# asc_atomic_inc

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

对Global Memory中`address`指向的单个无符号计数器执行原子递增操作：读取该地址上的旧值`old_value`，按“回绕递增”语义计算新值并写回同一地址，并返回`old_value`。回绕规则为：若`old_value >= val`，新值置为0；否则新值置为`old_value + 1`。计数器在`[0，val]`区间内循环递增：当旧值小于`val`时写回旧值加1，因此`val`可作为新值写回；当旧值大于等于`val`时写回0。整个“读取—判定—递增或清零—写回”过程通过原子锁保证串行化，多个AI Core或多线程并发调用同一`address`时不会相互覆盖，最终计数结果符合预期。

本接口为标量原子操作，操作数为单个无符号标量元素（不支持向量或连续多元素），数据类型由重载决定（2种重载分别支持`uint32_t`、`uint64_t`）。接口与数据搬运随路原子操作接口相互独立：随路原子接口（如`asc_set_atomic_add_*`）会“开启”后续搬运指令的原子累加属性，影响后续写GM的搬运行为；本接口则只对当前一次单点读改写操作原子化，不会影响后续任何搬运指令。

本接口为标量运算，运行在标量流水上，AIC与AIV均可调用，行为一致。

## 函数原型

```c
__aicore__ inline uint32_t asc_atomic_inc(__gm__ uint32_t* address,
                                          uint32_t val)
__aicore__ inline uint64_t asc_atomic_inc(__gm__ uint64_t* address,
                                          uint64_t val)
```

dtype支持的数据类型为`uint32_t`、`uint64_t`。

## 参数说明

**表 1**  参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| `address` | 输入/输出 | 待执行原子递增的Global Memory无符号标量地址。<br>• `uint32_t`重载：`__gm__ uint32_t*`，按4字节访问；<br>• `uint64_t`重载：`__gm__ uint64_t*`，按8字节访问。地址需按所访问数据类型的字节大小自然对齐（4字节重载4字节对齐、8字节重载8字节对齐），否则访问异常。地址必须落在Global Memory地址空间，传入UB、L1 Buffer、L0C Buffer、栈等非GM地址空间时硬件不保证原子语义且可能触发异常。 |
| `val` | 输入 | 计数器回绕阈值（上界）。数据类型与`address`指向元素类型一致（即重载决定dtype）。`uint32_t`重载取值范围`[0，2^32−1]`，`uint64_t`重载取值范围`[0，2^64−1]`。语义：若`*address >= val`则写回0（回绕），否则写回`*address + 1`（递增）。`val`同时决定计数器有效取值区间`[0，val]`，达到`val`即在下一次调用清零。`val = 0`时由于`*address >= 0`恒成立，每次调用均写回0，计数器恒为0（等价于“始终清零”，无实际递增效果）。参数类型与底层操作数位宽一致，无字段截断风险。 |

## 返回值说明

返回`address`修改前的旧值`old_value`，类型由重载决定（与`val`同dtype）：

- `uint32_t`重载返回`uint32_t`，取值范围`[0，2^32−1]`。
- `uint64_t`重载返回`uint64_t`，取值范围`[0，2^64−1]`。

调用方接收返回值即可拿到本接口生效前的GM原值，可用于实现循环计数器、令牌桶、轮询锁、生产者-消费者环形索引等需要“读旧值+按上界回绕递增”原子语义的场景。

## 流水类型

PIPE_S

## 约束说明

### 通用约束

- 本接口为标量单元素原子递增运算，操作数固定为单个无符号标量（不支持向量或连续多元素），dtype由2种重载决定（`uint32_t`、`uint64_t`），不可配置其他数据类型；本接口不支持有符号整数（`int32_t`、`int64_t`）与浮点（`float`）重载，因为底层原子比较交换原语仅支持无符号32/64位整数。
- `address`必须落在Global Memory地址空间。传入UB、L1 Buffer、L0C Buffer、栈等非GM地址空间时，硬件不保证原子语义且可能触发异常。
- `address`需按所访问数据类型的字节大小自然对齐：`uint32_t`重载需4字节对齐，`uint64_t`重载需8字节对齐，否则访问异常。
- `val`与`address`指向元素的数据类型由重载决定，两者类型一致，无需额外dtype配置。本接口与数据搬运随路原子操作接口相互独立，不受`asc_set_atomic_add_*`等随路原子设置影响，也不会影响后续搬运指令。
- 本接口通过原子锁对“读取—判定—递增或清零—写回”全过程串行化，多个AI Core或多线程并发调用同一`address`时不会相互覆盖。各调用使用相同`val`时，多次调用的最终值仅取决于调用次数和该回绕阈值；各调用使用不同`val`时，最终值及各调用的返回值可能随硬件实际串行顺序变化。
- 回绕规则为“达到上界后清零”：若`old_value >= val`则新值为0，否则新值为`old_value + 1`。当`val`大于0且旧值为`val - 1`时，新值为`val`；下一次调用再将其清零，因此计数器实际取值区间为`[0，val]`。`val = 0`时，每次调用均写回0，计数器恒为0；调用方应避免将`val`设为0，除非确实需要“始终清零”语义。
- 本接口仅支持无符号整数计数器，不产生溢出、进位、NaN/Inf等异常，不触发任何浮点或整数异常上报；递增与回绕按各自C类型位宽的无符号语义执行。
- 本接口运行在标量流水上，与后续依赖该返回值或同一GM地址的指令之间存在标量数据依赖，由标量流水本身的顺序执行保证；但若同一核内存在与本接口访问同一GM地址的数据搬运指令，且两者执行顺序影响最终结果，需由调用方按需插入`asc_sync_pipe`或`asc_sync_notify`与`asc_sync_wait`同步指令保证执行顺序。
- 即使开启编译器自动同步功能，编译器也无法为`asc_atomic_inc`访问GM与`PIPE_MTE2`/`PIPE_MTE3`数据搬运之间的数据依赖自动插入同步。存在依赖时，需根据实际流水关系手动插入[asc_sync_pipe](../sync/asc_sync_pipe.md)，或成对使用[asc_sync_notify](../sync/asc_sync_notify.md)和[asc_sync_wait](../sync/asc_sync_wait.md)保证执行顺序。
- 标量原子操作会旁路DCache直接访问GM，不维护DCache一致性。若其他通路通过DCache读写同一GM地址，需按实际读写关系调用[asc_dcci](../cache_ctrl/asc_dcci.md)清理或失效相应Cache Line；需要保证缓存维护与原子访存的顺序和可见性时，还需使用[asc_sync_data_barrier](../sync/asc_sync_data_barrier.md)。

## 调用示例

```cpp
// counter为外部输入的uint32_t类型的GM内存，此示例的初始值为0。
asc_dcci_entire_all();            // 手动同步。
uint32_t old = asc_atomic_inc(counter, 9);  // 计算后counter = 1，old = 0。
```
