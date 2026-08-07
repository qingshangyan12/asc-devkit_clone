# 内存一致性验证报告

## 验证范围

本目录验证`docs/zh/guide/编程指南/高级编程/内存模型/内存一致性.md`中可在910B1/A2环境稳定复现的正向结论。验证重点是同步机制是否能建立文档描述的执行顺序和可见性边界。

| 文档内容 | 样例用例 | 验证方式 | 预期结果 | 当前结论 |
| --- | --- | --- | --- | --- |
| Host异步下发后，需要显式等待Device任务完成 | 用例0、2、3 | Kernel写GM后，Host分别通过`aclrtSynchronizeStream`、`aclrtSynchronizeEvent`、`aclrtSynchronizeDevice`等待，再读取结果 | 读到写入值`11`、`33`、`44` | 待运行确认 |
| 跨Stream任务不能依赖Host下发顺序，需要Event建立依赖 | 用例1 | `stream0`写GM并记录Event，`stream1`等待Event后读取 | 读到`22` | 待运行确认 |
| 同一Stream中的异步拷贝和Kernel按入队顺序执行 | 用例4 | Host异步拷贝`55`到GM，同一Stream后续Kernel读取 | 读到`55` | 待运行确认 |
| 核内跨流水存在数据依赖时，需要`asc_sync_notify`/`asc_sync_wait` | 用例5 | MTE2搬入输入，MTE3搬出，流水间用notify/wait同步 | 读到`42` | 待运行确认 |
| 同流水存在重叠地址访问时，需要`asc_sync_pipe`等待前序访问完成 | 用例6 | 两次MTE2写同一UB区域，中间插入`asc_sync_pipe(PIPE_MTE2)`，最终搬出结果 | 读到第二次写入值`66` | 待运行确认 |
| 多核同地址读-改-写需要原子操作避免更新丢失 | 用例7 | 两个AI Core用MTE3随路`asc_set_atomic_add_int`向同一GM地址累加`1` | 结果为`2` | 待运行确认 |

## 未覆盖或不作为自动判定的内容

| 内容 | 原因 | 后续验证方向 |
| --- | --- | --- |
| 缺少同步时的跨流水、跨Stream负例 | 负例依赖硬件调度和时序窗口，可能偶发，不适合作为必须失败的自动校验 | 可增加压力循环和统计型脚本，观察无同步时的错误率 |
| `asc_sync_inter_arrive`/`asc_sync_inter_wait`、`asc_sync_block_arrive`/`asc_sync_block_wait`核间同步接口 | 这些接口需要满足group、block/subblock和同步寄存器等约束；本样例先不构造复杂分离模式Kernel | 单独补充核间同步专项样例 |
| SIMT线程间、线程内不同地址内存栅栏 | 当前目录是SIMD C API样例，SIMT线程级行为需要SIMT专项Kernel | 新建SIMT样例验证`asc_threadfence`、`asc_syncthreads`等接口 |
| Scalar原子`asc_atomic_add` | 文档显示该接口A2/A3不支持，当前910B1环境无法验证 | 在NPU架构版本3510环境单独验证 |

## 运行记录

创建样例后执行：

```bash
cmake -S . -B build -DCMAKE_ASC_ARCHITECTURES=dav-2201
cmake --build build -j
./build/host_stream_consistency_c_api
./build/pipeline_consistency_c_api
./build/atomic_consistency_c_api
python3 scripts/verify_result.py output
```

运行结果以实际执行输出为准。若校验脚本输出`all checks pass`，表示本报告中“待运行确认”的正向结论均符合文档预期。
