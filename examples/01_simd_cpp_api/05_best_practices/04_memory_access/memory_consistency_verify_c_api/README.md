# memory_consistency_verify_c_api

本样例用于验证《内存一致性》文档中可以在910B1/A2环境上稳定复现的结论。样例优先使用C API，例如`asc_sync_notify`、`asc_sync_wait`、`asc_sync_pipe`、`asc_sync_data_barrier`、`asc_set_atomic_add_int`；绕过DCache读写观测通道继续使用基础API。

## 编译运行

```bash
cd /data/hxj/code/2/asc-devkit_0616/examples/01_simd_cpp_api/05_best_practices/04_memory_access/memory_consistency_verify_c_api
cmake -S . -B build -DCMAKE_ASC_ARCHITECTURES=dav-2201
cmake --build build -j
./build/host_stream_consistency_c_api
./build/pipeline_consistency_c_api
./build/atomic_consistency_c_api
python3 scripts/verify_result.py output
```

运行后会生成`output/host_stream_output.bin`、`output/pipeline_output.bin`、`output/atomic_output.bin`，校验脚本会检查正向用例是否符合预期。

## 用例说明

| 编号 | 验证内容 | 对应文档结论 | 预期 |
| --- | --- | --- | --- |
| 0 | 同一Stream先写后读 | 同一Stream中的任务按入队顺序执行，Host使用`aclrtSynchronizeStream`等待结果 | 读到`11` |
| 1 | 跨Stream用Event建立依赖 | 不同Stream默认无依赖，需要`aclrtRecordEvent`和`aclrtStreamWaitEvent`建立任务级顺序 | 读到`22` |
| 2 | Host等待Event后读结果 | `aclrtSynchronizeEvent`只等待指定Event完成 | 读到`33` |
| 3 | Host等待Device后读结果 | `aclrtSynchronizeDevice`等待当前Device/Context下所有已下发任务完成 | 读到`44` |
| 4 | 同一Stream异步H2D后Kernel读 | 同一Stream中的异步拷贝和Kernel按入队顺序执行 | 读到`55` |
| 5 | 核内跨流水`MTE2 -> MTE3` | 跨流水存在数据依赖时，需要`asc_sync_notify`/`asc_sync_wait`建立生产者到消费者顺序 | 读到`42` |
| 6 | 同一MTE2流水重叠写 | 同流水重叠地址访问需要`asc_sync_pipe(PIPE_MTE2)`等待前序访问完成 | 最终读到第二次写入的`66` |
| 7 | 两个AI Core原子累加同一GM地址 | 原子操作保证同一地址读-改-写不可分割，避免并发累加丢更新 | `1 + 1 = 2` |

## 覆盖边界

本样例验证的是正向维护方式是否有效，不把缺少同步的负例作为自动判定。负例通常依赖硬件调度窗口，可能本次运行没有暴露，不适合写成必须失败的CI条件。

`asc_atomic_add`是Scalar原子接口，当前910B1/A2不支持。本样例用A2支持的MTE3随路`asc_set_atomic_add_int`验证“同地址原子累加不丢更新”的原子性结论；Scalar原子接口本身需要在支持的NPU架构版本3510环境上另行验证。
