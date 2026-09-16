# 新大纲：注意每一个标题中的（1）、（2）等代表逻辑上继续细分的点。
## 总述
（1）
引出A5新增支持mutex可以用来做核内多流水间同步，从而解决之间的数据依赖问题。
（2）
说明本文以Basic API中的[AscendC::Mutex::Lock](../../../../api/SIMD-API/basic_api/sync_control/intra_core_sync/Mutex_ISASI.md)/[AscendC::Mutex::Unlock](../../../../api/SIMD-API/basic_api/sync_control/intra_core_sync/Mutex_ISASI.md)为例说明Mutex实现多流水同步的原理，相关结论同样适用于C API中的[asc_lock](../../../../api/SIMD-API/c_api/sync/asc_lock.md)/[asc_unlock](../../../../api/SIMD-API/c_api/sync/asc_unlock.md)接口。C API同样也有双缓冲且使用Mutex的样例，可跳转到examples/02_simd_c_api/02_features/01_reg_vector_compute/00_add_double_buffer/README.md。
（3）
根据后续章节的内容，让读者知道本文档后续各个部分介绍的内容。

## 标题待定

双缓冲算子的多流水依赖（[Mutex核内流水线同步样例](../../../../../../examples/01_simd_cpp_api/03_basic_api/05_sync_control/mutex)。）：以MTE2、V、MTE3为例，说明数据搬入、计算、搬出之间的正向依赖与反向依赖关系。（注意这里的说明是以同步层面来介绍，并不涉及具体的接口）。

此部分可以跳转到[DoubleBuffer场景](double_buffer_scenario.md)，说明本文不考虑尾块。
还可以跳转到以下文档：
- [静态Tensor编程方式的同步管理](../../../programming_guide/programming_model/ai_core_simd_programming/cpp_tensor_programming/static_tensor_programming.md#section4338114663610)

  说明静态Tensor编程中正向同步和反向同步的划分，并给出`MTE2_V`、`V_MTE3`、`V_MTE2`和`MTE3_V`等事件的使用位置。

- [内存一致性中的反向依赖](../../../programming_guide/advanced_programming/memory_model/memory_consistency.md#反向依赖)

  从Buffer复用角度说明反向依赖的产生原因。可进一步参考[单Buffer示例](../../../programming_guide/advanced_programming/memory_model/memory_consistency.md#单buffer示例)和[Double Buffer示例](../../../programming_guide/advanced_programming/memory_model/memory_consistency.md#double-buffer示例)，了解不同Buffer复用方式下的等待关系。
## 待定

如下素材主要是为了让读者知道：开发者需要根据同步依赖关系规划`mutex_id`，同一组同步依赖需要使用相同的`mutex_id`，不同同步建议使用不同的`mutex_id`

双缓冲使用两个Buffer和两个MutexID。循环第`i`轮选择`bufferIndex = i % 2`，使同一个MutexID只管理其对应Buffer的依赖。第`i + 2`轮再次选择相同的Buffer和MutexID，其`PIPE_MTE2`上的Lock会等待此前所有使用该MutexID的流水均已Unlock；其中包括第`i`轮`PIPE_MTE3`的Unlock。由此保证Buffer能够安全复用。

| 轮次 | Buffer | MutexID | 关键约束 |
| --- | --- | --- | --- |
| `i` | `buffer0` | `mutexId0` | MTE2、V、MTE3按顺序处理当前tile。 |
| `i + 1` | `buffer1` | `mutexId1` | 使用另一组资源，可与`i`轮的异步流水重叠。 |
| `i + 2` | `buffer0` | `mutexId0` | MTE2搬入前等待`i`轮MTE3完成，避免覆盖`buffer0`。 |

接着说明在不同编程范式中，该ID的获取以及释放方式不同，跳转到docs/zh/api/SIMD-API/basic_api/sync_control/intra_core_sync/Lock.md的约束说明。

## 待定

### 待定

介绍lock/unlock、setflag/waitflag各自的功能，可以跳转到接口，功能必须照抄接口的功能说明。功能说明可以配合各自的时序图（需要润色如何引出时序图）。

### 待定

主要目的是对比两组接口在解决正向和反向依赖的区别，突出mutex的优势。

（1）
重点说明反向同步在 Mutex 下为什么可以被同一个 mutexId 表达：mutexId 绑定 Buffer，Lock<当前pipe> 等待同一 mutexId 此前所有流水的 Unlock。（附上代码段，来自Mutex核内流水线同步样例）

说明如果不用 Mutex、改用 SetFlag/WaitFlag，需要显式维护 V -> MTE2、MTE3 -> V 等事件和循环边界判断，从而凸显 Mutex 的简化点。（附上代码段，来自Mutex核内流水线同步样例，只是将mutex改造为setflag/waitflag，需要你帮我生成）
（2）
接着说明，由于Mutex不需要传入源流水、目的流水组合，因此也不依赖对应的HardEvent。在不同硬件架构上，HardEvent支持的取值可能不同；使用SetFlag/WaitFlag时，需要针对目标架构确认所需的流水组合是否存在（这里可以跳转到docs/zh/api/SIMD-API/basic_api/sync_control/intra_core_sync/intra_core_sync_overview.md的表2和表3）

## 使用约束

对lock和asc_lock的重要的约束进行简述，然后跳转到上述接口的约束说明处。