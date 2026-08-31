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

在AI Core内部，Scalar单元和DMA单元都可能对Global Memory进行访问。

![](../figures/dcci.png "DataCache内存层次示意图") \\

如上图所示：

DMA搬运单元读写Global Memory，数据通过[asc_copy_ub2gm](../vector_data_move/asc_copy_ub2gm/asc_copy_ub2gm.md)和[asc_copy_gm2ub](../vector_data_move/asc_copy_gm2ub/asc_copy_gm2ub.md)等接口在UB等Local Memory和Global Memory间交互，没有Cache一致性问题；
Scalar单元访问Global Memory，首先会访问每个核内的Data Cache，因此存在Data Cache与Global Memory的Cache一致性问题。
该接口用来刷新Cache，保证Cache的一致性，使用场景如下：

读取Global Memory的数据，但该数据可能在外部被其余核修改，此时需要使用asc_dcci接口，直接访问Global Memory，获取最新数据；
用户通过Scalar单元写Global Memory的数据，希望立刻写出，也需要使用asc_dcci接口。

Scalar单元访问UB数据时，该接口需配合[asc_set_ctrl()](../sys_var/asc_set_ctrl.md)接口使用，将CTRL[49]设置为1'b1，开启datacache模式。

<!-- npu="950" id8 -->
针对Ascend 950PR/Ascend 950DT，不支持asc_dcci_entire_ub接口。
<!-- end id8 -->

<font color="red">

头文件路径：`"c_api/cache_ctrl/cache_ctrl.h"`。

数据缓存清理与失效接口，用于维护DCache数据一致性。本接口对DCache中的缓存行执行clean（将脏数据写回到Global Memory或Unified Buffer）与invalidate（置缓存行为无效）操作，按接口形态分为两类：按地址清理单条Cache Line，或按目标域清理整个DCache。

本接口在以下场景下使用：当Scalar单元读取可能被其他核修改的Global Memory数据前，需先失效对应地址的Cache Line，避免读到过期数据；当Scalar单元写入Global Memory后要求写入立即对外可见时，需清理对应Cache Line，使脏数据写回到下级内存。由于硬件不保证多核间DCache一致性，本接口以软件方式维护一致性。

本接口提供两种操作模式，用户按使用场景选择对应重载：

- **按地址清理模式**：对`dst`指定地址所在的单条Cache Line执行clean + invalidate。适用于已知具体地址的一致性维护场景。
- **全清理模式**：对整个DCache中按目标域（ALL/OUT/ATOMIC）过滤的所有Cache Line执行clean + invalidate。适用于需要批量清理的场景，目标域决定哪些内存域对应的Cache Line参与操作。

本接口作用于本核DCache，DCache为多核独立缓存，不涉及核间共享。本接口不能与`--enable-simt`编译选项同时使用。

</font>

## 函数原型

   ```cpp
   // 刷新传入地址所在的单个Cache Line
   __aicore__ inline void asc_dcci_single(__gm__ void* dst)

   // 刷新传入的UB地址空间地址所在的单个Cache Line
   __aicore__ inline void asc_ub_dcci_single(__ubuf__ void* dst)

   // 刷新所有Cache Line
   __aicore__ inline void asc_dcci_entire_all()
   __aicore__ inline void asc_dcci_entire_ub()
   __aicore__ inline void asc_dcci_entire_out()
   __aicore__ inline void asc_dcci_entire_atomic()
   ```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
| :--- | :--- | :--- |
| dst | 输入 | 需要刷新缓存的向量的起始地址。 |

<font color="red">

### 按地址清理模式

**表1** 按地址清理模式参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| dst | 输入 | 待清理与失效的Cache Line所在地址。`asc_dcci_single`的`dst`为Global Memory地址（`__gm__`限定），`asc_ub_dcci_single`的`dst`为Unified Buffer地址（`__ubuf__`限定）。硬件按Cache Line边界对齐定位目标行，传入地址无需满足Cache Line对齐。 |

### 全清理模式

**表2** 全清理模式参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| （无参数） | — | 全清理模式重载无入参，目标域由接口名决定：`asc_dcci_entire_all`操作整个DCache（目标域ALL），`asc_dcci_entire_out`操作Global Memory域对应的全部Cache Line（目标域OUT），`asc_dcci_entire_atomic`操作原子写域对应的全部Cache Line（目标域ATOMIC）。 |

**表3** 全清理模式目标域取值说明

| 接口名 | 目标域 | 含义 |
|---|---|---|
| `asc_dcci_entire_all` | ALL | 操作整个DCache中所有valid行，不按内存域过滤。 |
| `asc_dcci_entire_out` | OUT | 仅操作Global Memory域对应的Cache Line。 |
| `asc_dcci_entire_atomic` | ATOMIC | 仅操作原子写域对应的Cache Line。 |

</font>

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

无

<font color="red">

### 通用约束

- 本接口面向AIC与AIV上由Scalar单元发起的Global Memory访问场景，AIC与AIV均可调用，无AIC/AIV守护限制。
- 本接口作用于本核DCache，DCache为多核独立缓存，多核间不共享；若其他核可能修改同一Global Memory地址的数据，需在各核分别调用本接口或配合其他缓存维护机制保证一致性。
- 本接口为强同步指令，发起后阻塞本核DCache后续请求，直到当前clean + invalidate操作完成；调用前应确认本接口的阻塞语义可接受，避免在高吞吐访存密集区段频繁调用导致流水停顿。
- 本接口与`--enable-simt`编译选项互斥，启用该编译选项时不可引入`cache_ctrl.h`，否则编译报错。C4
- 按地址清理模式下，若目标Cache Line命中且为脏，硬件会先生成回写请求将脏数据写回到对应内存域，再置该行为无效；命中且非脏时直接置为无效；未命中时本接口直接完成，不产生任何操作。
- 全清理模式下，硬件遍历整个DCache，按目标域过滤valid行：脏行先回写再置无效，非脏行直接置无效；目标域为ALL时不按内存域过滤，操作所有valid行。

### 按地址清理模式约束

- `dst`需为有效地址。C7
- 按地址清理模式的tag查找放宽内存域匹配，仅按地址匹配：只要某内存域中存在持有该地址的Cache Line，均会被清理与失效，不区分该行属于哪个内存域。

### 全清理模式约束

- 全清理模式无入参，目标域由接口名决定，不可在运行时切换；若需对不同内存域分别清理，需分别调用对应接口。C9
- 全清理模式下若整个DCache中无任何valid行需处理，硬件直接解除阻塞，不产生任何操作。

</font>

## 调用示例

```cpp
// 示例：SINGLE_CACHE_LINE模式（x_gm 64B对齐场景）
// 假设x为外部传入的地址，每个核计算16个数据，block_idx为内置变量，通过block_idx计算偏移实现多核间数据隔离
__gm__ float* x_gm = x + block_idx * 16;
for (int i = 0; i < 16; i++) {
    x_gm[i] = static_cast<float>(i + block_idx);
}
// 由于首地址64B对齐，调用asc_dcci_single指令后，数据会立刻刷新到Global Memory
asc_dcci_single(reinterpret_cast<__gm__ uint64_t*>(x_gm));
```
