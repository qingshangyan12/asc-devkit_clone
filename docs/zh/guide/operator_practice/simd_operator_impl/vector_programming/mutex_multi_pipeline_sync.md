# 基于Mutex实现多流水同步

## 总述

Ascend 950PR/Ascend 950DT新增了使用Mutex实现核内多流水同步的能力。矢量算子可以通过Mutex协调数据搬入、矢量计算和数据搬出等异步流水，解决不同流水访问同一资源时产生的数据依赖问题。

本文以Basic API中的[AscendC::Mutex::Lock](../../../../api/SIMD-API/basic_api/sync_control/intra_core_sync/Mutex_ISASI.md)和[AscendC::Mutex::Unlock](../../../../api/SIMD-API/basic_api/sync_control/intra_core_sync/Mutex_ISASI.md)为例，说明使用Mutex实现双缓冲多流水同步的原理。相关同步原理同样适用于C API中的[asc_lock](../../../../api/SIMD-API/c_api/sync/asc_lock.md)和[asc_unlock](../../../../api/SIMD-API/c_api/sync/asc_unlock.md)。C API双缓冲样例请参考[add_double_buffer样例](../../../../../../examples/02_simd_c_api/02_features/01_reg_vector_compute/00_add_double_buffer/README.md)。

本文首先介绍双缓冲算子中的正向依赖和反向依赖，然后说明如何规划MutexID，接着通过Lock/Unlock与SetFlag/WaitFlag的对比说明Mutex的使用方式，最后总结使用约束。

## 双缓冲算子的多流水依赖

一个典型的矢量算子包含数据搬入、矢量计算和数据搬出三个阶段，分别使用`PIPE_MTE2`、`PIPE_V`和`PIPE_MTE3`流水。数据量超过Unified Buffer（UB）容量时，算子通常将数据切分为多个tile，并循环处理这些tile。

采用双缓冲后，两组Buffer资源交替使用。当前tile使用一组资源进行计算时，其他流水可以使用另一组资源处理相邻tile。


本文以[Mutex核内流水线同步样例](../../../../../../examples/01_simd_cpp_api/03_basic_api/05_sync_control/mutex)中的`MTE2 -> V -> MTE3`数据流为例，只从同步关系说明问题，不展开具体接口实现。

同一tile中的流水存在正向依赖：

- `PIPE_MTE2 -> PIPE_V`：数据搬入完成后，`PIPE_V`才能读取该tile的数据并进行计算。
- `PIPE_V -> PIPE_MTE3`：计算完成后，`PIPE_MTE3`才能搬出计算结果。

Buffer复用时还会产生反向依赖。第`i + 2`轮重新使用第`i`轮的同一组Buffer资源时，需要满足以下约束：

- `PIPE_V -> PIPE_MTE2`：第`i`轮的计算完成后，第`i + 2`轮才能覆盖输入Buffer。
- `PIPE_MTE3 -> PIPE_V`：第`i`轮的结果搬出完成后，第`i + 2`轮才能覆盖输出Buffer。

正向依赖保证同一tile内的处理顺序，反向依赖保证Buffer复用时不会覆盖仍被前序流水访问的数据。双缓冲场景和流水重叠关系请参考[DoubleBuffer场景](double_buffer_scenario.md)；静态Tensor编程中的正向同步、反向同步及事件使用位置请参考[静态Tensor编程方式的同步管理](../../../programming_guide/programming_model/ai_core_simd_programming/cpp_tensor_programming/static_tensor_programming.md#section4338114663610)。从存储资源复用角度理解反向依赖时，请参考[内存一致性中的反向依赖](../../../programming_guide/advanced_programming/memory_model/memory_consistency.md#反向依赖)以及其中的[单Buffer示例](../../../programming_guide/advanced_programming/memory_model/memory_consistency.md#单buffer示例)和[Double Buffer示例](../../../programming_guide/advanced_programming/memory_model/memory_consistency.md#double-buffer示例)。

## MutexID规划

开发者需要根据同步依赖关系规划MutexID：同一组同步依赖使用相同的MutexID，不同的同步资源建议使用不同的MutexID。这里的一组资源是指同一轮次中需要共同管理生命周期的Buffer资源，例如Add算子的`xLocal`、`yLocal`和`zLocal`组成一组资源。

双缓冲使用两组Buffer资源和两个MutexID。循环第`i`轮选择`bufferIndex = i % 2`，使同一个MutexID只管理其对应资源组的依赖。第`i + 2`轮再次选择相同的资源组和MutexID，此时`PIPE_MTE2`上的Lock会等待此前所有使用该MutexID的流水均已Unlock，其中包括第`i`轮`PIPE_MTE3`的Unlock，从而保证资源能够安全复用。

| 轮次 | Buffer资源组 | MutexID | 关键约束 |
| --- | --- | --- | --- |
| `i` | `buffer0` | `mutexId0` | `MTE2`、`V`、`MTE3`按顺序处理当前tile。 |
| `i + 1` | `buffer1` | `mutexId1` | 使用另一组资源，可以与第`i`轮的异步流水重叠。 |
| `i + 2` | `buffer0` | `mutexId0` | `MTE2`搬入前等待第`i`轮的资源释放，避免覆盖`buffer0`。 |

MutexID的获取和释放方式与编程范式有关。TPipe-TQue框架编程范式需要通过[AllocMutexID](../../../../api/SIMD-API/basic_api/sync_control/intra_core_sync/AllocMutexID_ISASI.md)申请，并在使用结束后通过[ReleaseMutexID](../../../../api/SIMD-API/basic_api/sync_control/intra_core_sync/ReleaseMutexID_ISASI.md)释放。静态Tensor编程范式下，MutexID由开发者自行管理，具体取值和保留范围请参考[Lock约束说明](../../../../api/SIMD-API/basic_api/sync_control/intra_core_sync/Lock.md#section184751024101111)。

## Mutex与其他核内同步接口

### Lock/Unlock与SetFlag/WaitFlag的功能

`Lock`和`Unlock`必须配合使用。`Lock`根据MutexID获取Mutex；如果Mutex已被锁定，将阻塞指定流水上的后续指令，直到此前所有流水中使用相同MutexID的Mutex都已通过`Unlock`释放。`Unlock`在指定流水的前序指令执行完成后，根据MutexID释放对应Mutex。完整功能说明、函数原型和调用约束请参考[Mutex接口说明](../../../../api/SIMD-API/basic_api/sync_control/intra_core_sync/Mutex_ISASI.md)、[Lock接口](../../../../api/SIMD-API/basic_api/sync_control/intra_core_sync/Lock.md)和[Unlock接口](../../../../api/SIMD-API/basic_api/sync_control/intra_core_sync/Unlock.md)。

`SetFlag`和`WaitFlag`通过事件建立源流水和目的流水之间的同步关系。`SetFlag`在源流水的前序指令完成后设置对应标志位，但不会阻塞源流水的后续指令；`WaitFlag`执行时，如果标志位为0，则阻塞目的流水的后续指令，如果标志位为1，则清除标志位并继续执行。完整功能说明请参考[SetFlag/WaitFlag接口](../../../../api/SIMD-API/basic_api/sync_control/intra_core_sync/SetFlag_WaitFlag_ISASI.md)。

两组接口都可以实现多流水同步，但表达依赖的方式不同。以下时序图分别展示了两组接口的执行过程。

**图1**  SetFlag/WaitFlag同步时序示意图  
![](../../../figures/set_flag_wait_flag_sequence_diagram.png)

**图2**  Lock/Unlock同步时序示意图  
![](../../../figures/mutex_lock_unlock_sequence_diagram.png)

### 两组接口对正向依赖和反向依赖的表达差异

Mutex将MutexID与一组Buffer资源绑定，记录该资源此前各阶段的完成状态。每个阶段只需要锁定当前流水：

```cpp
for (uint32_t i = 0; i < loopCount; ++i) {
    uint32_t bufferIndex = i % 2;
    uint8_t mutexId = (bufferIndex == 0) ? mutexId0 : mutexId1;

    // 等待当前资源组此前的访问完成，然后搬入当前tile。
    AscendC::Mutex::Lock<PIPE_MTE2>(mutexId);
    // MTE2搬入当前tile的数据。
    AscendC::Mutex::Unlock<PIPE_MTE2>(mutexId);

    // 等待MTE2完成，然后计算当前tile。
    AscendC::Mutex::Lock<PIPE_V>(mutexId);
    // V计算当前tile。
    AscendC::Mutex::Unlock<PIPE_V>(mutexId);

    // 等待V完成，然后搬出计算结果。
    AscendC::Mutex::Lock<PIPE_MTE3>(mutexId);
    // MTE3搬出当前tile的结果。
    AscendC::Mutex::Unlock<PIPE_MTE3>(mutexId);
}
```

在第`i + 2`轮复用同一组资源时，`PIPE_MTE2`和`PIPE_V`再次Lock同一个MutexID，会统一等待此前所有使用该MutexID的流水完成。因此，代码不需要在当前调用中指定前序流水，也不需要分别维护`V -> MTE2`和`MTE3 -> V`等反向依赖。

如果使用SetFlag/WaitFlag，则需要分别表达每一组源流水到目的流水的依赖。下面代码只保留同步相关的核心逻辑，数据搬入、计算和搬出操作用注释表示：

```cpp
for (uint32_t i = 0; i < loopCount; ++i) {
    uint32_t bufferIndex = i % 2;

    if (i >= BUFFER_NUM) {
        // 复用输入资源前，等待上一轮V完成：V -> MTE2。
        AscendC::WaitFlag<AscendC::HardEvent::V_MTE2>(eventVToMTE2[bufferIndex]);
    }

    // MTE2搬入当前tile的数据。
    AscendC::SetFlag<AscendC::HardEvent::MTE2_V>(eventMTE2ToV[bufferIndex]);
    AscendC::WaitFlag<AscendC::HardEvent::MTE2_V>(eventMTE2ToV[bufferIndex]);

    if (i >= BUFFER_NUM) {
        // 复用输出资源前，等待上一轮MTE3完成：MTE3 -> V。
        AscendC::WaitFlag<AscendC::HardEvent::MTE3_V>(eventMTE3ToV[bufferIndex]);
    }

    // V计算当前tile。
    if (i + BUFFER_NUM < loopCount) {
        // 只有后续仍会复用输入资源时，才设置V -> MTE2事件。
        AscendC::SetFlag<AscendC::HardEvent::V_MTE2>(eventVToMTE2[bufferIndex]);
    }

    AscendC::SetFlag<AscendC::HardEvent::V_MTE3>(eventVToMTE3[bufferIndex]);
    AscendC::WaitFlag<AscendC::HardEvent::V_MTE3>(eventVToMTE3[bufferIndex]);

    // MTE3搬出当前tile的结果。
    if (i + BUFFER_NUM < loopCount) {
        // 只有后续仍会复用输出资源时，才设置MTE3 -> V事件。
        AscendC::SetFlag<AscendC::HardEvent::MTE3_V>(eventMTE3ToV[bufferIndex]);
    }
}
```

SetFlag/WaitFlag需要为正向依赖和反向依赖分别选择HardEvent并管理事件ID，还需要根据循环的开始和结束位置判断反向事件是否存在。并且，SetFlag和WaitFlag的HardEvent、事件ID必须完全匹配，详细约束请参考[SetFlag/WaitFlag约束说明](../../../../api/SIMD-API/basic_api/sync_control/intra_core_sync/SetFlag_WaitFlag_ISASI.md#section633mcpsimp)。相比之下，Mutex不需要传入源流水和目的流水组合，只需在当前流水上使用相同MutexID，即可表达同一组资源的多流水生命周期。

此外，SetFlag/WaitFlag依赖HardEvent表示源流水到目的流水的同步关系。不同硬件架构支持的HardEvent组合可能不同，使用SetFlag/WaitFlag时需要根据目标架构确认对应的流水组合是否存在。相关说明和架构示例请参考[核内同步能力概述中的表2和表3](../../../../api/SIMD-API/basic_api/sync_control/intra_core_sync/intra_core_sync_overview.md#核内同步分类)。Mutex不依赖具体的HardEvent，但仅适用于支持Mutex的Ascend 950PR/Ascend 950DT。

## 使用约束

使用Mutex时需要遵守以下约束，完整内容请以[Lock约束说明](../../../../api/SIMD-API/basic_api/sync_control/intra_core_sync/Lock.md#section184751024101111)和[Unlock约束说明](../../../../api/SIMD-API/basic_api/sync_control/intra_core_sync/Unlock.md#section184751024101111)为准：

- 对同一个MutexID，Lock和Unlock必须严格成对出现，并使用相同的流水类型和MutexID；对应的Unlock必须位于Lock之后。
- 对于相同MutexID的Lock/Unlock组合，无论流水类型是否相同，都不得嵌套使用，否则属于未定义行为。
- 连续使用相同MutexID和相同流水类型的Lock/Unlock，不能实现同一流水内不同指令之间的同步。同一流水内的指令依赖应使用[PipeBarrier](../../../../api/SIMD-API/basic_api/sync_control/intra_core_sync/PipeBarrier_ISASI.md)。
- TPipe-TQue框架编程范式下，通过AllocMutexID申请的MutexID使用结束后应及时通过ReleaseMutexID释放，避免MutexID耗尽。
- 同一组Buffer资源的所有相关流水应使用相同MutexID；不同资源组应使用不同MutexID，避免无关流水之间产生等待。
- C API的[asc_lock](../../../../api/SIMD-API/c_api/sync/asc_lock.md)和[asc_unlock](../../../../api/SIMD-API/c_api/sync/asc_unlock.md)支持阻塞和非阻塞模式，调用时还需要遵守接口对`mode`、`pipe`和`mutex_id`的配对约束。

Mutex的完整产品支持范围、函数原型、参数说明和调用示例，请参考[Mutex接口说明](../../../../api/SIMD-API/basic_api/sync_control/intra_core_sync/Mutex_ISASI.md)。
