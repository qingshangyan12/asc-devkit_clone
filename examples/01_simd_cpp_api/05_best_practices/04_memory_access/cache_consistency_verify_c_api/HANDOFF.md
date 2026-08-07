# 缓存一致性C API验证样例交接

## 目录目的

本目录是`../cache_consistency_verify`的C API改写版本，用来验证缓存一致性文档中的关键结论。设备侧代码尽量使用`docs/zh/api/SIMD-API/C-API`中的`asc_*`接口；没有等价C API，或者C API在A2/A3上不支持的地方，保留基础API并在代码注释中说明。

当前样例主要面向910B1等A2机器，默认编译架构为`dav-2201`。

## 文件说明

| 文件 | 作用 |
| --- | --- |
| `README.md` | 面向使用者的简要说明，包括编译、运行和结果阅读。 |
| `build.sh` | 快速构建并运行`demo_c_api`的脚本片段。 |
| `CMakeLists.txt` | 定义4个可执行样例目标。 |
| `data_utils.h` | Host侧工具函数，例如创建目录、二进制文件读写。 |
| `scripts/verify_result.py` | 校验`demo_c_api`的输出结果。 |
| `cache_consistency_verify_c_api.asc` | 综合样例，验证RAW、读端旧副本、WAW、绕过DCache路径和随路原子相关行为。 |
| `scalar_gm_211_demo_c_api.asc` | 专项样例，验证Scalar访问GM带来的写端Dirty副本和读端旧副本问题。 |
| `multi_core_cacheline_overwrite_c_api.asc` | 专项样例，验证多个核写同一条64B Cache Line不同字段时，DCCI写回可能互相覆盖。 |
| `ub_dcache_241_demo_c_api.asc` | 专项样例，验证`CTRL[49]`控制Scalar读UB是否经过DCache，以及UB DCCI对旧副本的影响。 |
| `memory_consistency_verify_c_api.asc` | 专项样例，验证同Stream保序、跨Stream Event依赖和同Stream异步H2D后Kernel读取。 |
| `MEMORY_CONSISTENCY_UNVERIFIED.md` | 记录内存一致性文档中尚未由本目录样例直接验证的结论和后续验证方向。 |

## 已替换的C API

| 原基础API/写法 | C API写法 |
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

## 保留基础API的原因

- `ReadGmByPassDCache`：当前C API目录中没有找到等价的绕过DCache读接口。
- `WriteGmByPassDCache`：C API有`asc_store_dev`，但资料显示该接口不支持A2/A3；为了在910B1上运行，绕过DCache写和结果写入保留基础API。
- `AscendC::Duplicate`、`LocalTensor`、`LocalMemAllocator`：`ub_dcache_241_demo_c_api.asc`需要Vector流水写UB来构造`V->S`场景；C API中的`asc_duplicate`寄存器接口不支持A2/A3，因此保留基础API。

## 关键实现约定

1. 结果写入使用绕过DCache路径。

   `WriteResult()`通过`WriteGmByPassDCache`写`results`，避免结果记录本身停留在Scalar DCache中，影响Host侧拷回结果。结果区使用`RESULT_STRIDE = 32`，即每个结果间隔128B，避免多个结果共享同一条DCache Cache Line。

2. 普通Scalar读写使用`volatile`指针。

   `WriteScalarCache()`和`ReadScalarCache()`里的`volatile __gm__`用于要求编译器真实生成Scalar LOAD/STORE，不改变访问路径；它仍然是普通Scalar访问GM，会经过Scalar DCache。

3. DCCI后的DSB按访问对象区分。

   GM相关DCCI后使用`DSB_DDR`，例如`asc_dcci_single()`后调用`asc_sync_data_barrier(mem_dsb_t::DSB_DDR)`。

   UB相关DCCI后使用`DSB_UB`，例如`asc_dcci_entire_ub()`后调用`asc_sync_data_barrier(mem_dsb_t::DSB_UB)`。

4. 负例用于观察风险，不是稳定判定条件。

   缓存一致性负例依赖具体时序、Cache状态和硬件行为；某次没有复现不代表风险不存在。正例才应作为自动校验依据。

5. 多核样例故意不做核间同步。

   `multi_core_cacheline_overwrite_c_api.asc`的目的不是构造正确多核协作，而是确认“多个核写同一条Cache Line不同字段后，各自DCCI写回可能覆盖其他核字段”这个错误场景存在。

## 编译运行

在本目录下执行：

```bash
source /usr/local/Ascend/cann/set_env.sh
mkdir -p build
cd build
cmake -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..
make -j
```

也可以直接参考`build.sh`中的命令。

如果要在950PR/950DT环境验证，可改为：

```bash
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..
```

## 样例运行与预期

### 1. 综合样例

在`build`目录运行：

```bash
./demo_c_api
python3 ../scripts/verify_result.py output/output.bin
```

预期校验结果：

```text
positive checks pass
```

`negative observations manifested`表示本次观测到了负例现象；没有出现该提示时，也不能说明负例场景不存在。

### 2. Scalar访问GM专项样例

```bash
./scalar_gm_211_demo_c_api
```

重点结果含义：

- 不做写端DCCI时，消费者可能读不到生产者Scalar写的新值。
- 写端做DCCI+DSB后，消费者可以读到新值。
- 消费者先读过旧值后，即使GM已经更新，也可能继续命中本核DCache旧副本。
- 消费者读前做DCCI后，可以读到GM新值。

典型输出：

```text
[0] producer Scalar.ST without DCCI, consumer Scalar.LD: read 0 = 64, read 1 = 0/64
[1] producer Scalar.ST + DCCI + DSB, consumer Scalar.LD: read 0 = 0, read 1 = 64/64
[2] consumer warm Scalar.LD old value count = 64/64
[3] producer published GM, consumer Scalar.LD without read-side DCCI: read 0 = 64, read 1 = 0/64
[4] consumer DCCI before Scalar.LD: read 0 = 0, read 1 = 64/64
```

### 3. 同Cache Line多核覆盖样例

```bash
./cacheline_overwrite_demo_c_api
```

预期是`same Cache Line multi-core overwrite mismatches`大于0，表示观察到同一条64B Cache Line内不同字段被不同核写回覆盖。该样例验证的是错误场景存在，因此“大于0”是符合预期的风险观测。

### 4. UB与DCache专项样例

```bash
./ub_dcache_241_demo_c_api
```

重点结果含义：

- `CTRL[49]=0`时，Scalar默认直接读UB；Vector写UB后，Scalar通常能直接读到新值。
- `CTRL[49]=1`时，Scalar通过DCache读UB；第一次读会形成旧副本。
- Vector写UB后，如果不做UB DCCI，Scalar可能继续读到旧值。
- 执行`asc_dcci_entire_ub()`并配合`DSB_UB`后，Scalar应读到UB新值。

典型关键输出：

```text
[0] default CTRL[49] = 0
[3] CTRL[49]=0, Vector.ST UB then Scalar.LD = 1
[6] CTRL[49]=1, Vector.ST UB then Scalar.LD without UB DCCI = 0
[7] CTRL[49]=1, asc_dcci_entire_ub then Scalar.LD = 1
```

### 5. 内存一致性专项样例

```bash
./memory_consistency_demo_c_api
```

重点结果含义：

- 同一Stream内，前序Kernel写入后，后序Kernel读取到`11`。
- 不同Stream之间通过Event建立依赖后，等待方Stream读取到`22`。
- 同一Stream内，异步H2D拷贝先于后序Kernel完成，Kernel读取到Host写入的`33`。

典型输出：

```text
[0] same Stream kernel write then read = 11
[1] cross Stream Event wait then read = 22
[2] same Stream async H2D then kernel read = 33
[3] progress marker = 3
```

## 后续维护注意点

- 修改访问路径前，先确认是在验证“普通Scalar DCache路径”还是“绕过DCache路径”。这两个路径不能混用，否则样例结论会变模糊。
- 不要把`results`改成普通Scalar写，除非同时对结果区做DCCI并保证Host拷回前写回完成。
- 不要随意去掉`RESULT_STRIDE`、`DATA_STRIDE`等间隔设计，它们用于避免多个观测点共享同一条DCache Cache Line。
- 如果新增C API替换，优先查`docs/zh/api/SIMD-API/C-API`中的产品支持情况。A2/A3不支持的C API不要替换到910B1默认样例里。
- 如果运行时报`libascendcl.so`找不到或`ASCConfig.cmake`找不到，通常是没有执行`source /usr/local/Ascend/cann/set_env.sh`。
- 如果运行时报`aclInit failed`，一般需要在可访问NPU设备的环境中执行，或者确认当前容器/会话是否有设备权限。

## 最近一次验证记录

在910B1环境、`dav-2201`架构下，已验证：

- `cmake --build build -j`编译通过。
- `demo_c_api`运行后，`scripts/verify_result.py`输出`positive checks pass`。
- `scalar_gm_211_demo_c_api`输出符合写端DCCI和读端DCCI预期。
- `cacheline_overwrite_demo_c_api`可观测到同Cache Line多核写回覆盖。
- `ub_dcache_241_demo_c_api`输出符合`CTRL[49]`和UB DCCI预期。
- `memory_consistency_demo_c_api`输出符合Stream顺序、Event依赖和异步H2D同Stream顺序预期。
