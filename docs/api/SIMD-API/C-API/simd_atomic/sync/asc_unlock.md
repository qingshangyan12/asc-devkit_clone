# asc_unlock

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT：支持
<!-- end id1 -->

## 功能说明

- 接口声明文件：`c_api/sync/sync.h`
- 核心功能：用于释放由[asc_lock](asc_lock.md)指令申请的Mutex。直到当前流水的前序指令执行完成后，根据mutex_id释放对应Mutex。
- 实现原理/公式：与asc_lock配合使用，通过释放MutexID对应的硬件锁，解除对对应流水线的阻塞，允许后续指令继续执行。

## 函数原型

<!-- npu="950" id2 -->
针对Ascend 950PR/Ascend 950DT：

```cpp
enum ascMutexExecuteMode {
    ASC_LOCK_BLOCK = 0,
    ASC_LOCK_NON_BLOCK = 1
};

__aicore__ inline void asc_unlock(pipe_t pipe, uint8_t mutex_id)
__aicore__ inline void asc_unlock(pipe_t pipe, uint8_t mutex_id, const ascMutexExecuteMode mode)
```

<!-- end id2 -->

## 参数说明

**表 1**  参数说明

<!-- npu="950" id3 -->

| 参数名 | 输入/输出 | 描述 |
| :---  | :--- | :--- |
| pipe | 输入 | 该指令的执行流水线。 |
| mutex_id | 输入 | buffer标号。取值范围[0, 31]。 |
| mode | 输入 | 可选参数，用于指定是否阻塞流水线，默认值为ASC_LOCK_BLOCK。<br>&bull; ASC_LOCK_BLOCK：该指令等待pipe所对应的流水线中所有前置指令完成后执行。<br>&bull; ASC_LOCK_NON_BLOCK：该指令等待pipe所对应的流水线中所有前置指令完成且相同mutex_id的所有asc_unlock指令执行完成后执行。 |

<!-- end id3 -->

## 返回值说明

无

## 流水类型

PIPE_S

## 资源占用情况

- 临时空间占用：不占用临时空间。
- 同步资源占用：释放Mutex硬件资源，与对应的asc_lock使用相同的mutex_id。
- 资源占用与参数/数据量关系：资源占用与数据量无关，与mutex_id参数取值有关。

## 约束说明

- 必须与asc_lock搭配使用，且需要使用相同的mutex_id与mode。此外，asc_unlock必须始终插在对应的asc_lock指令之后，否则会出现未定义的硬件行为。
- 具有相同mutex_id的asc_lock与asc_unlock组合，无论pipe与mode是否相同，均不得在编程顺序中嵌套，否则硬件行为将不可预测。
- 对于程序顺序中连续出现的，具有相同pipe与mutex_id的指令对，后一个asc_lock将不再阻塞流水线运行，若需实现同一流水线的依赖关系，则必须使用指令asc_sync_pipe。
- 在采用静态Tensor编程范式时，mutex_id由开发者自行管理，建议使用0-27，28-31为系统内部规划预留，不建议使用。

## 调用示例

<!-- npu="950" id4 -->
针对Ascend 950PR/Ascend 950DT：

```cpp
//buffer标号为1
uint8_t mutex_id = 1;
//等待PIPE_S中所有前置指令完成后释放标号为1的缓存
asc_lock(PIPE_S, mutex_id);
asc_unlock(PIPE_S, mutex_id);
```

- 完整样例链接：`https://gitcode.com/cann/asc-devkit/tree/master/examples/01_simd_cpp_api/03_basic_api/05_sync_control/mutex`

<!-- end id4 -->
