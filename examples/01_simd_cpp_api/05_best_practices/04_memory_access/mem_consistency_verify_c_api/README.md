# mem_consistency_verify_c_api

本样例用于验证《内存一致性》文档中可以在910B1/A2环境上稳定复现的结论。样例优先使用C API，例如`asc_sync_notify`、`asc_sync_wait`、`asc_sync_pipe`、`asc_sync_data_barrier`、`asc_set_atomic_add_int`；绕过DCache读写观测通道继续使用基础API。

## 编译运行

```bash
cd /data/hxj/code/2/asc-devkit_0616/examples/01_simd_cpp_api/05_best_practices/04_memory_access/mem_consistency_verify_c_api
cmake -S . -B build -DCMAKE_ASC_ARCHITECTURES=dav-2201
cmake --build build -j
./build/memory_consistency_demo_c_api
./build/demo_c_api
./build/cacheline_overwrite_demo_c_api
python3 scripts/verify_result.py output
```

运行后会生成`output/host_stream_output.bin`、`output/pipeline_output.bin`、`output/atomic_output.bin`。校验脚本会检查正向用例是否符合预期，同时打印未同步反例的观察值。

> 说明：当前910B1环境中，ASC可执行文件使用部分新增target名时会在`aclInit`阶段失败。因此本样例复用已有样例中稳定的target名：`memory_consistency_demo_c_api`对应Host/Stream用例，`demo_c_api`对应核内流水用例，`cacheline_overwrite_demo_c_api`对应原子用例。

## 用例说明

| 编号 | 验证内容 | 对应文档结论 | 预期 |
| --- | --- | --- | --- |
| 0 | 同一Stream先写后读 | 同一Stream中的任务按入队顺序执行，Host使用`aclrtSynchronizeStream`等待结果 | 读到`11` |
| 1 | 跨Stream用Event建立依赖 | 不同Stream默认无依赖，需要`aclrtRecordEvent`和`aclrtStreamWaitEvent`建立任务级顺序 | 读到`22` |
| 2 | Host等待Event后读结果 | `aclrtSynchronizeEvent`只等待指定Event完成 | 读到`33` |
| 3 | Host等待Device后读结果 | `aclrtSynchronizeDevice`等待当前Device/Context下所有已下发任务完成 | 读到`44` |
| 4 | 同一Stream异步H2D后Kernel读 | 同一Stream中的异步拷贝和Kernel按入队顺序执行 | 读到`55` |
| 5 | Host不等待Stream完成立即读 | Host不等待Device任务完成时，读取结果不可靠 | 观察值，不固定断言 |
| 6 | Host等待Stream后再读 | `aclrtSynchronizeStream`等待指定Stream任务完成 | 读到`101` |
| 7 | Host不等待Event完成立即读 | 只记录Event不会阻塞Host，读取结果不可靠 | 观察值，不固定断言 |
| 8 | Host等待Event后再读 | `aclrtSynchronizeEvent`等待指定Event完成 | 读到`102` |
| 9 | Host不等待Device完成立即读 | Host不等待Device任务完成时，读取结果不可靠 | 观察值，不固定断言 |
| 10 | Host等待Device后再读 | `aclrtSynchronizeDevice`等待当前Context下所有已下发任务完成 | 读到`103` |
| 11 | 跨Stream不使用Event直接读 | 不同Stream默认无依赖，读任务可能早于写任务完成 | 观察值，不固定断言 |
| 12 | 跨Stream无Event写任务完成后再读 | 同步所有相关Stream后，写任务最终完成 | 读到`104` |
| 13 | 核内跨流水`MTE2 -> MTE3` | 跨流水存在数据依赖时，需要`asc_sync_notify`/`asc_sync_wait`建立生产者到消费者顺序 | 读到`42` |
| 14 | 核内跨流水不做`notify/wait` | 不建立跨流水同步时，后序流水可能消费未完成数据 | 观察值，不固定断言 |
| 15 | 同一MTE2流水重叠写 | 同流水重叠地址访问需要`asc_sync_pipe(PIPE_MTE2)`等待前序访问完成 | 最终读到第二次写入的`66` |
| 16 | 同一MTE2流水重叠写不做`sync_pipe` | 不等待前序MTE2写完成时，重叠区域结果不受软件保证 | 观察值，不固定断言 |
| 17 | 同一MTE2流水重叠写不做`sync_pipe`压力观察 | 多轮重复反例，每轮检查64个`int32`组成的一片地址是否全部来自第二次写入 | 观察值，不固定断言 |
| 18 | 两个AI Core原子累加同一GM地址 | 原子操作保证同一地址读-改-写不可分割，避免并发累加丢更新 | `1 + 1 = 2` |
| 19 | 两个AI Core普通写同一GM地址 | 多核普通`STORE`不是累加操作，多个核写同一地址会互相覆盖 | 观察值通常为`1`，小于累加语义下的理论值`2` |

## 常见错误反例

本目录也按《内存一致性》文档中“常见一致性错误与调试”表构造了若干反向例子：

| 文档现象 | 反向例子 | 观察方式 |
| --- | --- | --- |
| 结果偶发错误 | 跨流水不做`notify/wait`、跨Stream不做Event、同MTE2重叠写不做`sync_pipe` | 查看`host_stream_output.bin`的`[11]`和`pipeline_output.bin`的`[2]`、`[3]`、`[5]`到`[9]` |
| 多核写同一GM地址结果偏小 | 两个AI Core并发普通`STORE`同一GM地址 | 查看`atomic_output.bin`的`[1]`，累加语义下理论值为`[2] = 2` |
| Host未读到就绪结果 | Host下发延迟写Kernel后不等待Stream/Event/Device完成就立即读 | 查看`host_stream_output.bin`的`[5]`、`[7]`、`[9]` |
| 跨Stream数据错乱 | `stream0`延迟写，`stream1`不等待Event直接读 | 查看`host_stream_output.bin`的`[11]`，同步后写端最终值见`[12]` |
| 本核计算异常或下一个核timeout | 构造不匹配`notify/wait`或保留EventId会导致Kernel可能timeout | 不作为默认运行用例，避免样例执行时挂住 |
| 偶发挂死 | 使用保留EventId 6或7 | 不作为默认运行用例，避免样例执行时挂住 |
| Scalar读到GM旧值 | DCache Dirty未写回或读端旧副本 | 已在`cache_consistency_verify_c_api`中验证，本目录不重复构造 |

## 覆盖边界

本样例同时包含正向用例和观察型反例。正向用例由校验脚本固定断言；未同步反例只打印观察值，不作为固定断言。原因是缺少同步后的行为不受软件保证，结果可能是旧值、未完成值，也可能碰巧正确。

`asc_atomic_add`是Scalar原子接口，当前910B1/A2不支持。本样例用A2支持的MTE3随路`asc_set_atomic_add_int`验证“同地址原子累加不丢更新”的原子性结论；Scalar原子接口本身需要在支持的NPU架构版本3510环境上另行验证。
