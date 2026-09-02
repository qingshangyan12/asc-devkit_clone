# asc_dcci_entire_ub精简对照样例

## 概述

本样例是`dcci_entire_ub`样例的实验性对照版本，用于确认省略核函数中的显式`static_cast`和输出阶段的`asc_sync_data_barrier(mem_dsb_t::DSB_ALL)`后，是否仍能有效验证`asc_dcci_entire_ub`的功能。原样例保持不变。

样例程序一次执行两个Kernel。第一个Kernel传入`useVolatile=0`，通过普通指针读取UB；第二个Kernel传入`useVolatile=1`，通过`volatile`指针读取UB。两次Kernel使用独立的GM输出区域，并分别记录八项观测结果。由于`volatile`是编译期类型限定，运行时变量不能动态改变同一个指针的类型，因此Kernel中同时定义普通指针和`volatile`指针，再由参数选择实际读取路径。

样例仍保留Vector流水与Scalar流水之间的同步，以及`asc_dcci_entire_ub()`后的`asc_sync_data_barrier(mem_dsb_t::DSB_UB)`。其中，流水同步保证Vector写UB完成后Scalar才读取；`DSB_UB`是接口文档规定的必要同步。`volatile`路径强制三组观测分别从UB地址发起Scalar读取，避免编译器复用先前读取结果。

## 对照结论

传入`useVolatile=0`时，当前编译器会复用先前的普通Scalar读取结果，DCCI后的两个观测值仍为旧值`11`。传入`useVolatile=1`时，DCCI后的两个观测值为新值`101`，能够正确验证接口功能。程序一次运行即可输出两组结果，直观展示两种读取路径的差异。

非`volatile`路径读到`11`是当前编译器和优化条件下的实验结果，并非C++标准保证。缺少`volatile`只代表编译器可以复用读取结果，并不要求编译器必须复用。因此，该路径只用于对照，不应作为跨编译器的接口判定依据；接口功能以`useVolatile=1`路径的结果为准。

普通业务代码在按程序数据流读取UB时通常不需要使用`volatile`。本样例需要在Vector改写UB和DCCI刷新DCache前后，对同一地址执行三组不可合并的Scalar读取，因此`volatile`属于测试观测机制的一部分，而不是调用`asc_dcci_entire_ub`接口的一般要求。

显式`static_cast`用于表达无符号位运算和安全窄化的意图，不是接口调用要求，本样例使用对应的隐式转换仍能正确编译和运行。输出前的`DSB_ALL`对当前数据流没有额外依赖需要保护；输出后的`DSB_ALL`用于显式等待绕过DCache的GM写完成，本样例依赖Host等待核函数结束也能正确回读结果。该结果只说明两个`DSB_ALL`对当前验证流程冗余，不能推广到存在后续Device侧访存依赖的业务代码。

## 为什么非volatile路径仍输出旧值

`useVolatile=0`路径在DCCI后输出`11`，不能据此判断DCache中仍然保存旧值，也不能说明`asc_dcci_entire_ub`没有生效。更准确的原因是：不使用`volatile`时，编译器可能没有在DCCI后生成新的Scalar读指令，而是直接复用此前读取并保存在寄存器中的`11`。

具体执行过程如下：

1. Scalar首次读取UB中的旧值`11`。该值进入DCache，同时也可能保存在Scalar寄存器中。
2. Vector流水将底层UB更新为`101`。该写入不会同步更新Scalar侧已有的DCache旧值。
3. DCCI前的Scalar读取从DCache取得旧值`11`，从而建立DCache与UB数据不一致的验证条件。
4. `asc_dcci_entire_ub()`使UB对应的DCache条目失效。
5. `asc_sync_data_barrier(mem_dsb_t::DSB_UB)`等待此次Cache操作完成。
6. 普通指针不要求每个读取表达式都产生实际访存。当前编译器发现Scalar C++数据流中没有通过该指针写入新值，因此可能使用此前保存在寄存器中的`11`替代DCCI后的读取。
7. 最终输出的`11`可能来自寄存器复用，而不是DCCI后重新访问DCache的结果。因此，该结果不能用于判断DCache条目是否已经失效。

`useVolatile=1`路径通过`volatile`限定的指针读取。每次读取都属于编译器必须保留的可观测副作用，DCCI后的读取不能被此前的寄存器值替代。此时处理过程如下：

```text
DCCI使DCache条目失效
        ↓
DSB_UB等待失效操作完成
        ↓
volatile要求执行新的Scalar读
        ↓
DCache Miss
        ↓
从UB读取新值101
```

各机制的作用边界如下：

**表 1**  各机制的作用边界

| 机制 | 作用 |
| --- | --- |
| `volatile` | 约束编译器优化，防止删除、合并或复用该指针的读取。 |
| `asc_dcci_entire_ub()` | 使UB对应的DCache条目失效。 |
| `asc_sync_data_barrier(mem_dsb_t::DSB_UB)` | 等待UB范围内的Cache操作完成。 |
| V->S同步 | 保证Vector写UB完成后再执行Scalar操作。 |

这些机制不能互相替代。DCCI负责处理硬件Cache，`DSB_UB`负责等待硬件操作完成，V->S同步负责跨流水依赖，`volatile`负责确保源码中的再次读取确实被编译为新的读取指令。

## 判断标准

程序输出两组观测值。当前环境下，预期结果如下：

**表 2**  两种读取路径的预期结果

| 观测项 | `useVolatile=0` | `useVolatile=1` |
| --- | --- | --- |
| `original_ctrl_bit49` | 0 | 0 |
| `enabled_ctrl_bit49` | 1 | 1 |
| `warm_line0`、`warm_line1` | 11、11 | 11、11 |
| `before_line0`、`before_line1` | 11、11 | 11、11 |
| `after_line0`、`after_line1` | 11、11 | 101、101 |

如果`before_line0`或`before_line1`提前变为`101`，说明样例没有建立DCache与UB数据不一致的前置条件，即使程序能够运行，也不能用于证明`asc_dcci_entire_ub`生效。

## 编译运行

在本样例目录执行：

```bash
bash test.sh
```

`test.sh`会进行干净构建并在NPU上运行。程序依次输出`use_volatile=0`和`use_volatile=1`两组结果；仅当当前环境中的两组结果均匹配上表时，脚本才输出`test pass!`并返回0。
