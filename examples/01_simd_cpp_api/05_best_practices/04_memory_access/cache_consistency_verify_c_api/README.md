# 缓存一致性验证样例（尽量使用C API）

本目录基于`../cache_consistency_verify`创建，用于验证缓存一致性文档中的关键结论。设备侧代码尽量使用`docs/zh/api/SIMD-API/C-API`中的`asc_*`接口；如果当前资料中找不到等价C API，或者C API在A2/A3上不支持，则保留基础API，并在代码中注明原因。

## C API替换范围

已替换为C API的典型接口如下：

| 原基础API/写法 | C API版本 |
| --- | --- |
| `AscendC::InitSocState()` | `asc_init()` |
| `AscendC::GetBlockIdx()` | `asc_get_block_idx()` |
| `AscendC::GetSystemCycle()` | `asc_get_system_cycle()` |
| `DataCacheCleanAndInvalid` | `asc_dcci_single()`、`asc_dcci_entire_ub()` |
| `DataSyncBarrier` | `asc_sync_data_barrier()` |
| `PipeBarrier<PIPE_ALL>` | `asc_sync_pipe(PIPE_ALL)` |
| `SetFlag/WaitFlag` | `asc_sync_notify()`、`asc_sync_wait()` |
| `DataCopy(GM->UB/UB->GM)` | `asc_copy_gm2ub()`、`asc_copy_ub2gm()` |
| `SetAtomicAdd<int32_t>()` | `asc_set_atomic_add_int()` |
| `DisableDmaAtomic()` | `asc_set_atomic_none()` |

保留基础API的地方：

- `ReadGmByPassDCache`：C API目录中未找到等价的绕过DCache读接口。
- `WriteGmByPassDCache`：C API有`asc_store_dev`，但资料显示该接口不支持A2/A3；为了在910B1等A2机器上运行，结果写入和绕过DCache写仍保留基础API。
- `AscendC::Duplicate`和`LocalTensor/LocalMemAllocator`：`ub_dcache_241_demo_c_api.asc`需要Vector流水写UB来验证`V->S`顺序；C API中的`asc_duplicate`寄存器接口不支持A2/A3，因此保留基础API。

## 样例说明

| 可执行文件 | 源文件 | 验证内容 |
| --- | --- | --- |
| `demo_c_api` | `cache_consistency_verify_c_api.asc` | 综合验证DCache与GM的RAW、读端旧副本、WAW、绕过DCache路径和MTE3随路原子一致性行为。 |
| `scalar_gm_211_demo_c_api` | `scalar_gm_211_demo_c_api.asc` | 专门验证Scalar访问GM带来的写端Dirty副本和读端旧副本问题。 |
| `cacheline_overwrite_demo_c_api` | `multi_core_cacheline_overwrite_c_api.asc` | 验证多个核写同一条64B DCache Cache Line中的不同字段时，DCCI按整条Cache Line写回可能造成随机覆盖。 |
| `ub_dcache_241_demo_c_api` | `ub_dcache_241_demo_c_api.asc` | 验证`CTRL[49]`控制Scalar读UB是否经过DCache，以及开启DCache模式后需要UB DCCI处理旧副本。 |
| `memory_consistency_demo_c_api` | `memory_consistency_verify_c_api.asc` | 验证同Stream保序、跨Stream Event依赖，以及同Stream异步H2D拷贝先于后序Kernel生效。 |

## 编译运行

在本目录下执行：

```bash
source /usr/local/Ascend/cann/set_env.sh
mkdir -p build
cd build
cmake -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..
make -j
```

如果在Ascend 950PR/950DT环境验证，可将架构参数改为：

```bash
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..
```

运行综合样例：

```bash
source /usr/local/Ascend/cann/set_env.sh
./demo_c_api
python3 ../scripts/verify_result.py output/output.bin
```

如果已经回到源码目录，可以这样校验：

```bash
python3 scripts/verify_result.py build/output/output.bin
```

运行其他样例：

```bash
./scalar_gm_211_demo_c_api
./cacheline_overwrite_demo_c_api
./ub_dcache_241_demo_c_api
./memory_consistency_demo_c_api
```

这些程序默认会把输出写到当前工作目录的`output/`目录下。若在`build`目录中运行，可在`build/output/`下查看二进制结果文件。

## 结果阅读

`demo_c_api`中，`positive checks pass`表示加了必要DCCI、DSB或同步后的正向用例通过。负向用例用于观察风险场景，是否复现依赖具体时序；负向用例本次未复现不代表风险不存在。

`cacheline_overwrite_demo_c_api`中，期望每条Cache Line最终为：

```text
1 2 3 4 5 6 7 8
```

如果打印`same Cache Line multi-core overwrite mismatches`大于0，说明观察到了同Cache Line多核写回覆盖。

`ub_dcache_241_demo_c_api`中重点观察：

- `CTRL[49]=0`时，Vector写UB后Scalar通常直接读到新值。
- `CTRL[49]=1`时，Vector写UB后如果不做UB DCCI，Scalar可能继续读到旧值；执行`asc_dcci_entire_ub`后应读到新值。

`memory_consistency_demo_c_api`中重点观察：

- 同一Stream内前序Kernel写入后，后序Kernel应读取到该写入。
- 不同Stream之间通过Event建立依赖后，等待方Stream应读取到被等待方Stream写入的数据。
- 同一Stream内异步H2D拷贝先于后序Kernel完成，Kernel应读取到Host异步拷贝写入的数据。
