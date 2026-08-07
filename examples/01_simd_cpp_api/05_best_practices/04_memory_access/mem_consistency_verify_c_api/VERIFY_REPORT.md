# 内存一致性验证报告

## 验证范围

本目录验证`docs/zh/guide/编程指南/高级编程/内存模型/内存一致性.md`中可在910B1/A2环境稳定复现的正向结论。验证重点是同步机制是否能建立文档描述的执行顺序和可见性边界。

| 文档内容 | 样例用例 | 验证方式 | 预期结果 | 当前结论 |
| --- | --- | --- | --- | --- |
| Host异步下发后，需要显式等待Device任务完成 | 用例0、2、3 | Kernel写GM后，Host分别通过`aclrtSynchronizeStream`、`aclrtSynchronizeEvent`、`aclrtSynchronizeDevice`等待，再读取结果 | 读到写入值`11`、`33`、`44` | 通过 |
| Host异步下发后，不等待就读取结果存在风险 | 反例5、7、9 | 下发延迟写Kernel后，Host不等待Stream/Event/Device完成就立即读Device内存 | 观察值不固定；本次均读到旧值`0` | 已观察到风险 |
| Host显式同步后读取结果可靠 | 用例6、8、10 | 未同步立即读后，再分别等待Stream/Event/Device完成并读取 | 读到写入值`101`、`102`、`103` | 通过 |
| 跨Stream任务不能依赖Host下发顺序，需要Event建立依赖 | 用例1 | `stream0`写GM并记录Event，`stream1`等待Event后读取 | 读到`22` | 通过 |
| 跨Stream不建立Event依赖时存在顺序风险 | 反例11、用例12 | `stream0`下发延迟写Kernel，`stream1`不等待Event直接读同一地址；随后同步相关Stream确认写任务最终完成 | 无Event读值不固定；同步后读到写入值`104` | 已观察到风险 |
| 同一Stream中的异步拷贝和Kernel按入队顺序执行 | 用例4 | Host异步拷贝`55`到GM，同一Stream后续Kernel读取 | 读到`55` | 通过 |
| 核内跨流水存在数据依赖时，需要`asc_sync_notify`/`asc_sync_wait` | 用例11 | MTE2搬入输入，MTE3搬出，流水间用notify/wait同步 | 读到`42` | 通过 |
| 核内跨流水不建立同步时存在风险 | 反例12 | MTE2搬入UB后，MTE3不等待MTE2完成就搬出 | 观察值不固定；本次读到异常值`903854027` | 已观察到风险 |
| 同流水存在重叠地址访问时，需要`asc_sync_pipe`等待前序访问完成 | 用例13 | 两次MTE2写同一UB区域，中间插入`asc_sync_pipe(PIPE_MTE2)`，最终搬出结果 | 读到第二次写入值`66` | 通过 |
| 同一MTE2流水重叠写不插入`asc_sync_pipe`时，软件不保证结果 | 反例14 | 两次MTE2写同一UB区域，中间不插入`asc_sync_pipe` | 观察值不固定；本次碰巧读到第二次写入值`99` | 已记录观察值 |
| 同一MTE2流水重叠写不插入`asc_sync_pipe`时，多轮重复观察 | 反例压力统计 | 重复执行256轮无`sync_pipe`重叠写；每轮检查64个`int32`组成的一片地址是否全部来自第二次写入 | 观察值不固定；本次未观察到非期望值 | 已记录统计值 |
| 多核同地址读-改-写需要原子操作避免更新丢失 | 用例15 | 两个AI Core用MTE3随路`asc_set_atomic_add_int`向同一GM地址累加`1` | 结果为`2` | 通过 |
| 多核普通写同一GM地址会互相覆盖 | 反例16 | 两个AI Core并发执行普通`STORE`，每个核都向同一GM地址写入`1` | 若业务语义是累加，理论值应为`2`；普通写通常只留下其中一次写入 | 本次观察为`1`，符合偏小现象 |

## 常见错误表反向验证

| 文档表格现象 | 本目录反例 | 是否默认运行 | 说明 |
| --- | --- | :---: | --- |
| 结果偶发错误 | 跨流水不做`notify/wait`、跨Stream不做Event、同MTE2重叠写不做`sync_pipe` | 是 | 观察未同步读到旧值、异常值或碰巧正确的情况 |
| 本核计算异常或下一个核timeout | 不匹配`notify/wait`、不匹配event_id | 否 | 这类反例可能使Kernel timeout或挂住，不适合作为默认可运行样例 |
| 偶发挂死 | 手动使用保留EventId 6或7 | 否 | 这类反例可能导致挂死，不适合作为默认可运行样例 |
| 多核写同一GM地址结果偏小 | 两个AI Core普通`STORE`同一GM地址 | 是 | 对比实际值和累加语义下的理论值`2` |
| Scalar读到GM旧值 | 见`cache_consistency_verify_c_api` | 否 | 该现象属于缓存一致性，已由缓存一致性样例验证 |
| Host未读到就绪结果 | Host不等待Stream/Event/Device完成就读 | 是 | 延迟写Kernel扩大窗口，立即读取通常观察到旧值 |
| 跨Stream数据错乱 | `stream0`延迟写，`stream1`不等待Event直接读 | 是 | 无Event读值与同步后的最终写入值对比 |

## 未覆盖或不作为自动判定的内容

| 内容 | 原因 | 后续验证方向 |
| --- | --- | --- |
| `asc_sync_inter_arrive`/`asc_sync_inter_wait`、`asc_sync_block_arrive`/`asc_sync_block_wait`核间同步接口 | 这些接口需要满足group、block/subblock和同步寄存器等约束；本样例先不构造复杂分离模式Kernel | 单独补充核间同步专项样例 |
| SIMT线程间、线程内不同地址内存栅栏 | 当前目录是SIMD C API样例，SIMT线程级行为需要SIMT专项Kernel | 新建SIMT样例验证`asc_threadfence`、`asc_syncthreads`等接口 |
| Scalar原子`asc_atomic_add` | 文档显示该接口A2/A3不支持，当前910B1环境无法验证 | 在NPU架构版本3510环境单独验证 |

## 运行记录

创建样例后执行：

```bash
cmake -S . -B build -DCMAKE_ASC_ARCHITECTURES=dav-2201
cmake --build build -j
./build/memory_consistency_demo_c_api
./build/demo_c_api
./build/cacheline_overwrite_demo_c_api
python3 scripts/verify_result.py output
```

本次运行结果：

```text
host_stream_output.bin:
  [00] same Stream write then read = 11
  [01] cross Stream Event wait then read = 22
  [02] aclrtSynchronizeEvent then Host read = 33
  [03] aclrtSynchronizeDevice then Host read = 44
  [04] same Stream async H2D then kernel read = 55
  [06] after aclrtSynchronizeStream Host read = 101
  [08] after aclrtSynchronizeEvent Host read = 102
  [10] after aclrtSynchronizeDevice Host read = 103
  [12] cross Stream no Event writer final value after sync = 104
pipeline_output.bin:
  [00] MTE2 -> MTE3 notify/wait result = 42
  [01] same MTE2 pipeline sync_pipe result = 66
atomic_output.bin:
  [00] two-core DMA atomic add result = 2
  [02] two-core normal STORE theoretical sum = 2
host_stream_output.bin observations:
  [05] no Stream sync immediate Host read = 0
  [07] no Event sync immediate Host read = 0
  [09] no Device sync immediate Host read = 0
  [11] cross Stream no Event read result = 0
pipeline_output.bin observations:
  [02] no MTE2 -> MTE3 sync observed result = 0
  [03] no same-MTE2 sync_pipe observed result = 99
  [05] no same-MTE2 sync_pipe stress total = 256
  [06] no same-MTE2 sync_pipe stress all-expected-slice-count = 256
  [07] no same-MTE2 sync_pipe stress mismatch-slice-count = 0
  [08] no same-MTE2 sync_pipe stress first-mismatch-index = -1
  [09] no same-MTE2 sync_pipe stress first-mismatch-value = -777
atomic_output.bin observations:
  [01] two-core normal STORE same address result = 1
  [02] two-core normal STORE theoretical sum = 2

all checks pass
```

结论：已执行的正向用例均符合文档预期；未同步反例已经观察到旧值或异常值，但这些观察值不代表固定语义。

下面按最终可运行目录解释：`examples/01_simd_cpp_api/05_best_practices/04_memory_access/mem_consistency_verify_c_api`。

**公共设计**
这些用例都尽量把“内存一致性”问题和“缓存一致性”问题拆开。代码里用`WriteBypass`/`ReadBypass`绕过DCache读写GM，避免验证内存顺序时被DCache旧副本或Dirty副本干扰；所以这些用例主要验证“同步是否建立了预期的先后关系”。

**用例0：同一Stream先写后读**
代码位置：[host_stream_consistency_c_api.asc](/data/hxj/code/2/asc-devkit_0616/examples/01_simd_cpp_api/05_best_practices/04_memory_access/mem_consistency_verify_c_api/host_stream_consistency_c_api.asc:169)

流程是同一个Stream里先下发写Kernel，把`11`写到GM；再下发读Kernel读同一地址。最后Host调用`aclrtSynchronizeStream`等待Stream完成。结果读到`11`。

对应文档：`## Host-Device内存一致性`里的“异步下发语义”和“Host侧显式同步接口”。验证同一Stream任务按入队顺序执行，但Host仍要显式同步后才能可靠读取结果。

**用例1：跨Stream用Event建立依赖**
代码位置：[host_stream_consistency_c_api.asc](/data/hxj/code/2/asc-devkit_0616/examples/01_simd_cpp_api/05_best_practices/04_memory_access/mem_consistency_verify_c_api/host_stream_consistency_c_api.asc:174)

流程是`stream0`写`22`，然后`aclrtRecordEvent(eventCross, stream0)`；`stream1`通过`aclrtStreamWaitEvent(stream1, eventCross)`等待这个Event，再读同一GM地址。结果读到`22`。

对应文档：`## Host-Device内存一致性`里的“跨Stream任务依赖”。验证不同Stream不能靠Host下发顺序建立依赖，需要Event表达生产者/消费者关系。

**用例2：Host等待Event后读结果**
代码位置：[host_stream_consistency_c_api.asc](/data/hxj/code/2/asc-devkit_0616/examples/01_simd_cpp_api/05_best_practices/04_memory_access/mem_consistency_verify_c_api/host_stream_consistency_c_api.asc:205)

流程是Kernel写`33`，Host在该Stream上记录Event，然后调用`aclrtSynchronizeEvent(eventHost)`等待这个Event完成，再从Device拷回结果。结果读到`33`。

对应文档：`## Host-Device内存一致性`里的“Host侧显式同步接口”。验证`aclrtSynchronizeEvent`可以做更细粒度的Host等待。

**用例3：Host等待Device后读结果**
代码位置：[host_stream_consistency_c_api.asc](/data/hxj/code/2/asc-devkit_0616/examples/01_simd_cpp_api/05_best_practices/04_memory_access/mem_consistency_verify_c_api/host_stream_consistency_c_api.asc:215)

流程是另一个Stream写`44`，Host调用`aclrtSynchronizeDevice()`等待当前Device/Context下任务完成，再读取GM。结果读到`44`。

对应文档：`## Host-Device内存一致性`里的“Host侧显式同步接口”。验证`aclrtSynchronizeDevice`是更大范围的等待。

**用例4：同一Stream异步H2D后Kernel读**
代码位置：[host_stream_consistency_c_api.asc](/data/hxj/code/2/asc-devkit_0616/examples/01_simd_cpp_api/05_best_practices/04_memory_access/mem_consistency_verify_c_api/host_stream_consistency_c_api.asc:185)

流程是Host用`aclrtMemcpyAsync`在某个Stream里异步拷贝`55`到Device GM，随后在同一Stream下发读Kernel。结果读到`55`。

对应文档：`## Host-Device内存一致性`里的“异步下发语义”。验证同一Stream内异步拷贝和Kernel按入队顺序执行。

**用例5：核内跨流水MTE2 -> MTE3同步**
代码位置：[pipeline_consistency_c_api.asc](/data/hxj/code/2/asc-devkit_0616/examples/01_simd_cpp_api/05_best_practices/04_memory_access/mem_consistency_verify_c_api/pipeline_consistency_c_api.asc:82)

流程是MTE2先把GM中的`42`搬入UB，然后通过`asc_sync_notify(PIPE_MTE2, PIPE_MTE3, EVENT_ID0)`和`asc_sync_wait(...)`通知MTE3等待MTE2完成，MTE3再把UB搬回GM。结果读到`42`。

对应文档：`## 核内内存一致性`里的“跨流水内存一致性”和“核内同步原语”。验证跨流水不能只靠代码顺序，需要`notify/wait`建立生产者到消费者顺序。

**用例6：同一MTE2流水重叠写**
代码位置：[pipeline_consistency_c_api.asc](/data/hxj/code/2/asc-devkit_0616/examples/01_simd_cpp_api/05_best_practices/04_memory_access/mem_consistency_verify_c_api/pipeline_consistency_c_api.asc:105)

流程是MTE2第一次把`55`搬入同一块UB，然后调用`asc_sync_pipe(PIPE_MTE2)`等待第一次搬入完成，再第二次把`66`搬入同一块UB，最后搬出到GM。结果读到`66`。

对应文档：`## 核内内存一致性`里的“同流水内存一致性”。验证同流水内如果访问同一/重叠地址，也需要等待前序访问完成，才能保证最终结果符合预期。

**用例7：两个AI Core原子累加同一GM地址**
代码位置：[atomic_consistency_c_api.asc](/data/hxj/code/2/asc-devkit_0616/examples/01_simd_cpp_api/05_best_practices/04_memory_access/mem_consistency_verify_c_api/atomic_consistency_c_api.asc:55)

流程是启动两个block，每个block把`1`搬入UB，然后开启`asc_set_atomic_add_int()`，通过MTE3随路原子累加到同一个GM地址。最终Host读到`2`。

对应文档：`## 核间内存一致性`里的“原子操作与内存一致性”。验证原子操作能保证同一地址读-改-写不丢更新。

注意：这里验证的是A2上可用的MTE3随路原子`asc_set_atomic_add_int`，不是Scalar原子`asc_atomic_add`。文档里的`asc_atomic_add`在A2/A3不支持，所以报告里单独标成未覆盖项。
