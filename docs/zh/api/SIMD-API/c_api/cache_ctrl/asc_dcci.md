# asc_dcci

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
<!-- npu="910" id4 -->
- Atlas 训练系列产品：不支持
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas 推理系列产品AI Core：不支持
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas 推理系列产品Vector Core：不支持
<!-- end id6 -->
<!-- npu="310b" id7 -->
- Atlas 200I/500 A2 推理产品：不支持
<!-- end id7 -->

## 功能说明

在AI Core内部，Scalar单元和DMA单元都可能对Global Memory进行访问。

**图 1**  DCache内存层次示意图

![](../figures/dcci.png)

如上图所示：

DMA搬运单元读写Global Memory，数据通过[asc_copy_ub2gm](../vector_data_move/asc_copy_ub2gm/asc_copy_ub2gm.md)和[asc_copy_gm2ub](../vector_data_move/asc_copy_gm2ub/asc_copy_gm2ub.md)等接口在Unified Buffer（UB）等Local Memory和Global Memory间交互，没有Cache一致性问题；
Scalar单元访问Global Memory，首先会访问每个核内的DCache，因此存在DCache与Global Memory的一致性问题。
该接口用来刷新Cache，保证Cache的一致性，使用场景如下：

读取Global Memory的数据，但该数据可能在外部被其余核修改，此时需要使用asc_dcci接口，直接访问Global Memory，获取最新数据；
用户通过Scalar单元写Global Memory的数据，希望立刻写出，也需要使用asc_dcci接口。

Scalar单元访问UB数据时，该接口需配合[asc_set_ctrl()](../sys_var/asc_set_ctrl.md)接口使用，将CTRL[49]设置为1'b1，开启DCache模式。

<!-- npu="950" id8 -->
- Ascend 950PR/Ascend 950DT：`asc_dcci_entire_ub`仅支持在Vector Core中调用，Cube Core不包含UB。
<!-- end id8 -->

## 函数原型

```cpp
// 刷新传入地址所在的单个Cache Line。
__aicore__ inline void asc_dcci_single(__gm__ void* dst)

// 刷新传入的UB地址空间地址所在的单个Cache Line。
__aicore__ inline void asc_ub_dcci_single(__ubuf__ void* dst)

// 刷新所有Cache Line。
__aicore__ inline void asc_dcci_entire_all()
__aicore__ inline void asc_dcci_entire_ub()
__aicore__ inline void asc_dcci_entire_out()
__aicore__ inline void asc_dcci_entire_atomic()
```

## 参数说明

**表 1**  参数说明

| 参数名 | 输入/输出 | 描述 |
| :--- | :--- | :--- |
| dst | 输入 | 需要刷新缓存的向量的起始地址。 |

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

使用`asc_dcci_entire_ub`刷新UB对应的DCache后，必须调用`asc_sync_data_barrier(mem_dsb_t::DSB_UB)`，再执行依赖刷新结果的Scalar读操作。

`volatile`不是DCCI接口的调用要求。调用DCCI并执行Data Sync Barrier后，仅当后续读取依赖DCCI结果，并且通过普通GM或UB指针对此前读取过的同一对象再次执行Scalar Load时，才应使用指向`volatile`限定对象的指针执行该读取，避免编译器复用DCCI前的读取结果。对于UB地址，该约束仅适用于`CTRL[49]`设置为`1'b1`，Scalar Load经过DCache的场景。

`volatile`仅约束编译器优化，不能使DCache条目失效，也不能保证硬件访存顺序，因此不能替代DCCI和[Data Sync Barrier](../sync/asc_sync_data_barrier.md)。刷新UB对应的DCache后，应使用`DSB_UB`；刷新GM对应的DCache后，通常使用`DSB_DDR`，如果还需要同时等待其他存储位置的访问完成，则使用范围更大的`DSB_ALL`。

<!-- npu="950" id9 -->
- Ascend 950PR/Ascend 950DT：对于无需经过DCache的GM读取场景，可以使用[asc_load_dev](../scalar_compute/asc_load_dev.md)代替普通GM指针读取。该接口绕过DCache，因此当前读取不需要通过读端DCCI使本核DCache旧副本失效，但写端仍须将数据写回GM并完成所需同步。跨核场景中，写端应在数据写入GM完成后发布同步标志，读端应在观察到同步标志后再读取数据；Data Sync Barrier本身不提供跨核通知。该接口不适用于UB地址。
<!-- end id9 -->

## 调用示例

```cpp
// 示例：SINGLE_CACHE_LINE模式（x_gm 64B对齐场景）。
// 假设x为外部传入的地址，每个核计算16个数据，block_idx为内置变量，通过block_idx计算偏移实现多核间数据隔离。
__gm__ float* x_gm = x + block_idx * 16;
for (int i = 0; i < 16; i++) {
    x_gm[i] = static_cast<float>(i + block_idx);
}
// 由于首地址64B对齐，调用asc_dcci_single指令后，数据会立刻刷新到Global Memory。
asc_dcci_single(reinterpret_cast<__gm__ uint64_t*>(x_gm));
```
