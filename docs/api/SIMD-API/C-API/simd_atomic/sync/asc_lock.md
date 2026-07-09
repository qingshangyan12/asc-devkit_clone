# asc_lock

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT：支持
<!-- end id1 -->

## 功能说明

- 接口声明文件：`c_api/sync/sync.h`
- 核心功能：根据MutexID获取Mutex，若Mutex已被锁定，将阻塞后续指定流水指令队列，直到当前流水的前序指令中对应MutexID的Mutex被[asc_unlock](asc_unlock.md)释放。该接口用于AI Core内部异步流水线之间的同步，可按需阻塞指定流水线的执行。
- 实现原理/公式：通过Mutex机制实现流水线间的互斥访问。每个mutex_id对应一个硬件锁，asc_lock尝试获取该锁，若锁已被占用则阻塞当前流水线，直到对应的asc_unlock释放锁。

## 函数原型

<!-- npu="950" id2 -->
针对Ascend 950PR/Ascend 950DT：

```cpp
enum ascMutexExecuteMode {
    ASC_LOCK_BLOCK = 0,
    ASC_LOCK_NON_BLOCK = 1
};

__aicore__ inline void asc_lock(pipe_t pipe, uint8_t mutex_id)
__aicore__ inline void asc_lock(pipe_t pipe, uint8_t mutex_id, const ascMutexExecuteMode mode)
```

<!-- end id2 -->

## 参数说明

**表 1**  参数说明

<!-- npu="950" id3 -->

| 参数名 | 输入/输出 | 描述 |
| :---  | :--- | :--- |
| pipe | 输入 | 指定当前接口所属的流水类型。 |
| mutex_id | 输入 | mutex标号，取值范围为[0, 31]。 |
| mode | 输入 | 可选参数，用于指定是否阻塞流水线，默认值为ASC_LOCK_BLOCK。<br>&bull; ASC_LOCK_BLOCK：阻塞pipe对应流水线的执行，直到代码中位于当前asc_lock之前且mutex_id相同的所有asc_unlock调用均已执行完成。<br>&bull; ASC_LOCK_NON_BLOCK：不阻塞pipe对应流水线的执行。 |

<!-- end id3 -->

## 返回值说明

无

## 流水类型

PIPE_S

## 资源占用情况

- 临时空间占用：不占用临时空间。
- 同步资源占用：占用Mutex硬件资源，每个mutex_id对应一个Mutex锁，取值范围为[0, 31]。
- 资源占用与参数/数据量关系：资源占用与数据量无关，与mutex_id参数取值有关。

## 约束说明

- asc_lock与asc_unlock必须严格成对使用，并使用相同的mutex_id与mode。此外，对应的asc_unlock必须始终写在asc_lock之后，否则硬件行为无定义。
- 对于mutex_id相同的asc_lock与asc_unlock组合，无论pipe和mode是否相同，都不得在代码中嵌套使用，否则硬件行为将不可预测。
- 成对使用的asc_lock和asc_unlock必须配置相同的mode，否则硬件行为将不可预测。
- 如果代码中连续出现多个pipe和mutex_id均相同的调用，则后一个asc_lock不会再阻塞流水线运行。若需要表达同一流水线内的依赖关系，必须使用接口asc_sync_pipe。
- 在采用静态Tensor编程范式时，mutex_id由开发者自行管理，建议使用0-27，28-31为系统内部规划预留，不建议使用。

## 调用示例

<!-- npu="950" id4 -->
针对Ascend 950PR/Ascend 950DT：

```cpp
uint8_t mutex_id = 1;
asc_lock(PIPE_S, mutex_id);
asc_unlock(PIPE_S, mutex_id);
```

- 完整样例链接：`https://gitcode.com/cann/asc-devkit/tree/master/examples/01_simd_cpp_api/03_basic_api/05_sync_control/mutex`

<!-- end id4 -->
