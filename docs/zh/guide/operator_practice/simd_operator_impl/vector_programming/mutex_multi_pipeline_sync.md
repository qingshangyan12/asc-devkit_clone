# 基于Mutex实现多流水同步

在Ascend 950PR/Ascend 950DT上，矢量算子可通过Mutex管理AI Core内多条异步流水之间的依赖。本文以使用双缓冲的Add算子为例，说明如何使用`AscendC::Mutex::Lock`和`AscendC::Mutex::Unlock`串联数据搬入、矢量计算和数据搬出，并在Buffer复用时建立反向同步。

`Lock<当前pipe>(mutexId)`只指定需要阻塞的当前流水。其解除阻塞的条件是：此前所有流水中使用相同MutexID的Mutex均已执行`Unlock`。因此，开发者不需要判断或传入前序流水类型；无论前序阶段是`PIPE_MTE2`、`PIPE_V`还是`PIPE_MTE3`，均由同一MutexID统一约束。

## 前置说明

- 本文适用于Ascend 950PR/Ascend 950DT。Mutex为NPU架构版本3510新增能力，其他产品应使用[SetFlag/WaitFlag](../../../../api/SIMD-API/basic_api/sync_control/intra_core_sync/SetFlag-WaitFlag.md)等可用同步接口。
- 需要了解[DoubleBuffer场景](double_buffer_scenario.md)和[内存一致性](../../../programming_guide/advanced_programming/memory_model/memory_consistency.md)中关于跨流水依赖与Buffer复用的说明。
- 本文示例采用TPipe-TQue框架编程范式，MutexID通过[AllocMutexID](../../../../api/SIMD-API/basic_api/sync_control/intra_core_sync/AllocMutexID_ISASI.md)申请，并在使用结束后通过[ReleaseMutexID](../../../../api/SIMD-API/basic_api/sync_control/intra_core_sync/ReleaseMutexID_ISASI.md)释放。

## 适用场景

一个典型的矢量算子会将每个数据块分为数据搬入、计算和数据搬出三个阶段：`PIPE_MTE2 -> PIPE_V -> PIPE_MTE3`。当数据量超过单个Unified Buffer（UB）可容纳范围时，算子通常按tile循环处理；采用双缓冲后，当前tile的计算可以与相邻tile的搬运重叠执行。

对于同一个Buffer，下一次搬入前必须等待上一次搬出完成，否则`PIPE_MTE2`可能覆盖仍被`PIPE_MTE3`读取的数据。MutexID可以表示一个Buffer的完整生命周期：同一Buffer上的MTE2、V、MTE3阶段使用同一个MutexID；两个Buffer使用不同的MutexID。调用`Lock<当前pipe>(mutexId)`时，当前流水会等待此前所有流水中相同MutexID的`Unlock`全部完成。开发者无需指定该流水依赖哪一条前序流水，也无需判断前序流水类型。

```mermaid
sequenceDiagram
    participant MTE2 as PIPE_MTE2
    participant V as PIPE_V
    participant MTE3 as PIPE_MTE3
    Note over MTE2,MTE3: Tile i使用buffer0和mutexId0
    MTE2->>MTE2: Lock(mutexId0)，搬入buffer0，Unlock(mutexId0)
    V->>V: Lock(mutexId0)，计算buffer0，Unlock(mutexId0)
    MTE3->>MTE3: Lock(mutexId0)，搬出buffer0，Unlock(mutexId0)
    Note over MTE2,MTE3: Tile i+1使用buffer1和mutexId1，可与Tile i重叠
    Note over MTE2,MTE3: Tile i+2复用buffer0时，Lock(mutexId0)等待Tile i的MTE3解锁
```

Mutex不限定为上述三段矢量流水。`Mutex::Lock/Unlock`的流水模板参数支持`PIPE_S`、`PIPE_M`、`PIPE_V`、`PIPE_MTE1`、`PIPE_MTE2`、`PIPE_MTE3`和`PIPE_FIX`。因此，除矢量算子的`MTE2 -> V -> MTE3`外，也可用同一模式描述矩阵算子的`MTE2 -> MTE1 -> M -> FIX`依赖。本文聚焦矢量算子；矩阵算子的完整调用方式请参考[矩阵计算同步机制](../../../programming_guide/programming_model/ai_core_simd_programming/cpp_tensor_programming/cube_matrix_computation.md#同步机制)。

### MutexID管理方式

MutexID应与其管理的Buffer或资源生命周期一一对应。不同编程范式的获取方式不同。

| 编程范式 | MutexID管理方式 | 使用建议 |
| --- | --- | --- |
| TPipe-TQue框架编程范式 | 使用`AllocMutexID`申请，并在使用结束后调用`ReleaseMutexID`释放。 | 按Buffer数量申请ID。双缓冲通常申请两个ID，分别管理两个Buffer。 |
| 静态Tensor编程范式 | 由开发者自行指定和管理。 | 建议使用0-27，避免使用系统内部规划预留的28-31。 |

## 算子实现

### MutexID与Buffer对应关系

双缓冲使用两个Buffer和两个MutexID。循环第`i`轮选择`bufferIndex = i % 2`，使同一个MutexID只管理其对应Buffer的依赖。第`i + 2`轮再次选择相同的Buffer和MutexID，其`PIPE_MTE2`上的Lock会等待此前所有使用该MutexID的流水均已Unlock；其中包括第`i`轮`PIPE_MTE3`的Unlock。由此保证Buffer能够安全复用。

| 轮次 | Buffer | MutexID | 关键约束 |
| --- | --- | --- | --- |
| `i` | `buffer0` | `mutexId0` | MTE2、V、MTE3按顺序处理当前tile。 |
| `i + 1` | `buffer1` | `mutexId1` | 使用另一组资源，可与`i`轮的异步流水重叠。 |
| `i + 2` | `buffer0` | `mutexId0` | MTE2搬入前等待`i`轮MTE3完成，避免覆盖`buffer0`。 |

### 核函数核心流程

以下代码展示双缓冲Add算子的核心同步逻辑。`xLocal`、`yLocal`和`zLocal`各包含两个UB Buffer，`DataCopy`和`Add`的参数省略了与同步无关的初始化细节。

```cpp
constexpr uint32_t BUFFER_NUM = 2;

uint8_t mutexId0 = AscendC::AllocMutexID();
uint8_t mutexId1 = AscendC::AllocMutexID();

for (uint32_t i = 0; i < loopCount; ++i) {
    uint32_t bufferIndex = i % BUFFER_NUM;
    uint8_t mutexId = (bufferIndex == 0) ? mutexId0 : mutexId1;

    // 只指定当前PIPE_MTE2。Lock自动等待此前所有流水对该MutexID的Unlock；
    // 复用同一Buffer时，这里会等待该Buffer上一轮的MTE3搬出完成。
    AscendC::Mutex::Lock<PIPE_MTE2>(mutexId);
    AscendC::DataCopy(xLocal[bufferIndex], xGm[tileLength * i], tileLength);
    AscendC::DataCopy(yLocal[bufferIndex], yGm[tileLength * i], tileLength);
    AscendC::Mutex::Unlock<PIPE_MTE2>(mutexId);

    // 当前tile的数据搬入完成后，Vector流水才开始计算。
    AscendC::Mutex::Lock<PIPE_V>(mutexId);
    AscendC::Add(zLocal[bufferIndex], xLocal[bufferIndex], yLocal[bufferIndex], tileLength);
    AscendC::Mutex::Unlock<PIPE_V>(mutexId);

    // 当前tile的计算完成后，MTE3流水才开始搬出结果。
    AscendC::Mutex::Lock<PIPE_MTE3>(mutexId);
    AscendC::DataCopy(zGm[tileLength * i], zLocal[bufferIndex], tileLength);
    AscendC::Mutex::Unlock<PIPE_MTE3>(mutexId);
}

AscendC::ReleaseMutexID(mutexId0);
AscendC::ReleaseMutexID(mutexId1);
```

在每条流水上，`Lock<当前pipe>`后的指令会等待此前所有流水中相同MutexID的`Unlock`全部完成。MTE2、V和MTE3使用同一MutexID时，代码顺序即表达了当前tile的正向依赖；再次使用该MutexID的MTE2时，会同时检查该MutexID此前全部阶段的完成状态，从而自然表达Buffer复用前的反向依赖。

完整的可编译示例请参考仓内[Mutex核内流水线同步样例](../../../../../../examples/01_simd_cpp_api/03_basic_api/05_sync_control/mutex)。

## 与SetFlag/WaitFlag的选择

`SetFlag/WaitFlag`和Mutex都可以建立核内多流水依赖。对于双缓冲算子中按Buffer生命周期组织的依赖，Mutex能将同步状态聚合到对应的MutexID：开发者在当前阶段只需调用`Lock<当前pipe>(mutexId)`和`Unlock<当前pipe>(mutexId)`，无需关心或传入前序生产者流水。Lock阻塞当前流水，直至此前所有流水中相同MutexID的Mutex均已Unlock；它等待的是该资源此前所有阶段的完成状态，而不是某一条指定源流水。因此，不需要像SetFlag/WaitFlag一样传入或维护源流水、目的流水组合及其事件ID。

| 对比项 | Mutex Lock/Unlock | SetFlag/WaitFlag |
| --- | --- | --- |
| 依赖表达 | 调用`Lock<当前pipe>(mutexId)`即可等待此前所有流水对该MutexID的Unlock，以MutexID关联同一Buffer生命周期内的多个阶段。 | 以源流水、目标流水和事件ID明确配对。 |
| 解除阻塞条件 | 此前所有流水中相同MutexID的Mutex均已Unlock，不需要识别前序流水类型。 | 指定源流水已通过对应事件通知指定目标流水。 |
| 反向同步 | 后续复用Buffer时再次Lock同一MutexID即可等待前序使用者释放，无需在当前调用中指定MTE3到下一轮MTE2的流水组合。 | 需要为反向依赖额外设置与等待相应的事件。 |
| 同步信号量 | MutexID可用数量更多，适合管理多个并发Buffer。 | 事件ID数量受限，需要规划复用时机。 |
| 架构相关配置 | 不依赖HardEvent，不需要根据源、目的流水组合选择事件类型。 | HardEvent的取值随硬件架构变化，需要根据目标架构支持的事件选择源、目的流水组合。 |
| 跨硬件版本 | 仅适用于支持Mutex的Ascend 950PR/Ascend 950DT。 | TQueSync的SetFlag/WaitFlag可保证跨硬件版本兼容。 |

Mutex并不用于同一条流水内的指令排序。例如，同一`PIPE_MTE2`上两次UB地址重叠的数据搬运，需要使用[PipeBarrier](../../../../api/SIMD-API/basic_api/sync_control/intra_core_sync/PipeBarrier_ISASI.md)等待前一次搬运完成。

由于Mutex不需要传入源流水、目的流水组合，因此也不依赖对应的HardEvent。在不同硬件架构上，HardEvent支持的取值可能不同；使用SetFlag/WaitFlag时，需要针对目标架构确认所需的流水组合是否存在。使用Mutex时，开发者只需关注当前流水和MutexID，可减少这部分事件配置。Mutex不提供跨硬件版本兼容性，仍需以Ascend 950PR/Ascend 950DT的产品支持范围为准。

### 反向同步原理

双缓冲的正向依赖是当前tile的`PIPE_MTE2 -> PIPE_V -> PIPE_MTE3`。反向依赖发生在Buffer复用时：第`i + 2`轮与第`i`轮使用相同的Buffer，必须等第`i`轮的MTE3从该Buffer搬出完成后，才能开始第`i + 2`轮的MTE2搬入。因此，反向同步表达的是`PIPE_MTE3 -> PIPE_MTE2`，而不是当前tile内部的正向依赖。

使用SetFlag/WaitFlag时，开发者需要为这条反向依赖显式维护一组源流水为MTE3、目的流水为MTE2的事件，并在下一次复用该Buffer前等待该事件。除反向依赖外，当前tile的`MTE2 -> V`和`V -> MTE3`也分别需要事件配对。

反向事件是否需要插入还取决于循环位置：前两个tile首次使用两个Buffer，没有前序MTE3需要等待；最后两个tile后没有下一轮复用该Buffer，也不应再为反向依赖设置事件。开发者需要根据`loopCount`和Buffer数量维护这些边界判断，并保证MTE3设置事件与后续MTE2等待事件严格配对。这会增加循环代码的分支和事件ID管理负担。

仓内矩阵算子样例在复用工作空间前使用`HardEvent::MTE3_MTE2`获取事件ID，并显式插入对应的SetFlag/WaitFlag。这说明反向同步不仅出现在双缓冲矢量算子中；只要后续流水需要复用仍被前序流水访问的存储资源，就需要表达这类依赖。详见[matmul_partial_output样例](../../../../../../examples/01_simd_cpp_api/04_advanced_api/00_matmul/matmul_partial_output/matmul_partial_output.asc)。

```text
SetFlag/WaitFlag伪代码：需要分别表达正向和反向的流水组合。
for (uint32_t i = 0; i < loopCount; ++i) {
    uint32_t bufferIndex = i % 2;
    if (i >= BUFFER_NUM) {
        // 等待前两轮的MTE3释放当前要复用的Buffer：MTE3 -> MTE2。
        wait_mte3_to_mte2(bufferIndex);
    }
    // MTE2搬入bufferIndex。
    set_mte2_to_v(bufferIndex);
    wait_mte2_to_v(bufferIndex);
    // V计算bufferIndex。
    set_v_to_mte3(bufferIndex);
    wait_v_to_mte3(bufferIndex);
    // MTE3搬出bufferIndex。
    if (i + BUFFER_NUM < loopCount) {
        // 仅当后续仍会复用该Buffer时，设置MTE3 -> MTE2事件。
        set_mte3_to_mte2(bufferIndex);
    }
}
```

使用Mutex时，`mutexId`与Buffer绑定，MutexID记录的是该Buffer此前所有阶段的完成状态，而非某个指定生产者的完成状态。每个阶段只锁定当前流水；`Lock<PIPE_MTE2>(mutexId)`会阻塞`PIPE_MTE2`后续指令，直至此前`PIPE_MTE2`、`PIPE_V`、`PIPE_MTE3`等所有流水中同一MutexID的Mutex均已Unlock。因此，第`i + 2`轮复用Buffer时，MTE2上的Lock自然包含对第`i`轮MTE3的等待，而无需额外表达`PIPE_MTE3 -> PIPE_MTE2`的源、目的流水组合。

这正是Mutex能够简化反向同步的原因：第`i + 2`轮的MTE2不需要知道前一次使用该Buffer的是哪一条流水，也不需要为反向依赖维护专用事件。只要所有访问该Buffer的阶段复用同一MutexID，Lock便会统一等待其此前全部阶段完成。相较之下，WaitFlag只能等待指定源流水发出的事件，开发者需要显式维护`PIPE_MTE3 -> PIPE_MTE2`组合，并根据循环次数增加首轮跳过等待、末轮跳过通知等边界分支。

```cpp
// Mutex：只指定当前流水，MutexID表达整个Buffer生命周期。
for (uint32_t i = 0; i < loopCount; ++i) {
    uint32_t bufferIndex = i % 2;
    uint8_t mutexId = (bufferIndex == 0) ? mutexId0 : mutexId1;

    AscendC::Mutex::Lock<PIPE_MTE2>(mutexId);
    // MTE2搬入bufferIndex。
    AscendC::Mutex::Unlock<PIPE_MTE2>(mutexId);
    AscendC::Mutex::Lock<PIPE_V>(mutexId);
    // V计算bufferIndex。
    AscendC::Mutex::Unlock<PIPE_V>(mutexId);
    AscendC::Mutex::Lock<PIPE_MTE3>(mutexId);
    // MTE3搬出bufferIndex。
    AscendC::Mutex::Unlock<PIPE_MTE3>(mutexId);
}
```

## 使用约束

- 对同一个MutexID，Lock和Unlock必须严格成对出现，且使用相同的流水类型和MutexID；Unlock必须位于对应Lock之后。
- 不得嵌套使用相同MutexID的Lock/Unlock组合，否则行为未定义。
- 在TPipe-TQue框架编程范式中，使用结束后应释放申请的MutexID，避免ID耗尽。
- 不要用连续的同一MutexID、同一流水类型的Lock/Unlock实现单流水同步；此场景应使用PipeBarrier。
- 本文示例为C++ Tensor接口的阻塞式`Mutex::Lock/Unlock`用法。C API的`asc_lock/asc_unlock`另提供阻塞和非阻塞模式，调用约束和头文件请参见[asc_lock](../../../../api/SIMD-API/c_api/sync/asc_lock.md)和[asc_unlock](../../../../api/SIMD-API/c_api/sync/asc_unlock.md)。

具体接口约束请参见[Mutex](../../../../api/SIMD-API/basic_api/sync_control/intra_core_sync/Mutex_ISASI.md)、[Lock](../../../../api/SIMD-API/basic_api/sync_control/intra_core_sync/Lock.md)和[Unlock](../../../../api/SIMD-API/basic_api/sync_control/intra_core_sync/Unlock.md)。
