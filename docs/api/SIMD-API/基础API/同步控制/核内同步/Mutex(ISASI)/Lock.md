# Lock<a name="ZH-CN_TOPIC_0000002372961532"></a>

## 产品支持情况<a name="section73648168211"></a>

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

## 功能说明<a name="section54681522111017"></a>

头文件路径为： "basic\_api/kernel\_common.h"

根据MutexID获取Mutex，若Mutex已被锁定，将阻塞后续指定流水指令队列，直到前序指令中对应MutexID的Mutex被Unlock。

## 函数原型<a name="section1568410468104"></a>

```cpp
template <pipe_t pipe>
static __aicore__ inline void Lock(MutexID id)
```

## 参数说明<a name="section74061251191017"></a>

**表 1**  模板参数说明

| 参数名 | 描述 |
| --- | --- |
| pipe | 模板参数，表示流水类别。支持的流水类型为PIPE_S/PIPE_M/PIPE_V/PIPE_MTE1/PIPE_MTE2/PIPE_MTE3/PIPE_FIX |

**表 2**  参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| id | 输入 | 进行流水同步管理的MutexID。<ul><li><a href=\"TPipe-TQue编程实践参考.md\">TPipe和TQue编程实践</a>中，MutexID需要通过<a href=\"zh-cn_topic_0000002534327182.md\">AllocMutexID/ReleaseMutexID</a>进行申请释放。</li><li><a href=\"静态Tensor编程实践参考.md\">静态Tensor编程实践</a>中，此时性能最优，MutexID由开发者自行管理，建议使用0-27，28-31为系统内部规划预留，不建议使用。</li></ul> |

## 约束说明<a name="section184751024101111"></a>

-   [TPipe和TQue编程实践](TPipe-TQue编程实践参考.md)中，MutexID需要通过[AllocMutexID/ReleaseMutexID](zh-cn_topic_0000002534327182.md)进行申请释放。
-   [静态Tensor编程实践](静态Tensor编程实践参考.md)中，此时性能最优，MutexID由开发者自行管理，建议使用0-27，28-31为系统内部规划预留，不建议使用。

-   对于同一个MutexID，必须先执行Lock，然后才能执行Unlock，且指定的pipe需要相同。如果Lock和Unlock没有按照这种“成对出现”的顺序排列，硬件行为将不可预测。

    ```cpp
    // 错误写法 1
    AscendC::Mutex::UnLock<PIPE_V>(0);
    AscendC::Mutex::Lock<PIPE_V>(0);
    
    // 错误写法 2
    AscendC::Mutex::Lock<PIPE_V>(0);
    AscendC::Mutex::Lock<PIPE_V>(0);;
    AscendC::Mutex::UnLock<PIPE_V>(0);
    AscendC::Mutex::UnLock<PIPE_V>(0);
    
    // 正确写法
    AscendC::Mutex::Lock<PIPE_V>(0);
    AscendC::Mutex::UnLock<PIPE_V>(0);
    AscendC::Mutex::Lock<PIPE_V>(0);
    AscendC::Mutex::UnLock<PIPE_V>(0);
    ```

-   使用相同MutexID、相同流水的两组Lock/UnLock指令时，第二次Lock不会再阻塞流水，这种情况建议使用[PipeBarrier](PipeBarrier.md)接口，示例如下：

    ```cpp
    // 错误写法
    AscendC::Mutex::Lock<PIPE_V>(0);
    AscendC::Add(zLocal, xLocal, yLocal, TILE_LENGTH);
    AscendC::Mutex::UnLock<PIPE_V>(0);
    AscendC::Mutex::Lock<PIPE_V>(0); //不再阻塞
    AscendC::Add(zLocal, xLocal, yLocal, TILE_LENGTH);
    AscendC::Mutex::UnLock<PIPE_V>(0);
    
    // 正确写法
    AscendC::Add(zLocal, xLocal, yLocal, TILE_LENGTH);
    AscendC::PipeBarrier<PIPE_V>();
    AscendC::Add(zLocal, xLocal, yLocal, TILE_LENGTH);
    ```

## 调用示例<a name="section123275308128"></a>

完整算子样例参考：[Mutex样例](https://gitcode.com/cann/asc-devkit/tree/master/examples/01_simd_cpp_api/03_basic_api/05_sync_control/mutex)。

```cpp
// 锁定 MTE2 流水线互斥锁，确保当前线程独占 MTE2 资源进行数据搬运
AscendC::Mutex::Lock<PIPE_MTE2>(0);
AscendC::DataCopy(xLocal, src0Global[TILE_LENGTH * progress], TILE_LENGTH);
AscendC::DataCopy(yLocal, src1Global[TILE_LENGTH * progress], TILE_LENGTH);
// 解锁 MTE2 流水线，允许其他线程使用 MTE2
AscendC::Mutex::Unlock<PIPE_MTE2>(0);

// 锁定 Vector 流水线互斥锁，确保当前线程独占向量计算资源
AscendC::Mutex::Lock<PIPE_V>(0);
AscendC::Add(zLocal, xLocal, yLocal, TILE_LENGTH);
// 解锁 Vector 流水线，释放计算资源供其他线程使用
AscendC::Mutex::Unlock<PIPE_V>(0);
```
