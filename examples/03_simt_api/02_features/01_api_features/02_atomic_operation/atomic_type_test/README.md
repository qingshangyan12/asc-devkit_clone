# asc_atomic_add 浮点类型性能样例

## 概述

本样例用于通过 `msprof op` 采集 `asc_atomic_add()` 在 `float`、`half`、`bfloat16_t`、`half2`、`bfloat16x2_t` 上的性能数据。每次编译只选择一个数据类型和一个场景，运行时只启动一个目标 kernel，便于 profiling 数据聚焦到单一 workload。

接口支持范围和头文件要求来自 `docs/api/SIMT-API/原子操作/asc_atomic_add.md`：

- `float`、`half`、`bfloat16_t`、`half2`、`bfloat16x2_t` 均支持 Unified Buffer 和 Global Memory。
- `float` 使用 `simt_api/device_atomic_functions.h`。
- `half`、`half2` 使用 `simt_api/asc_fp16.h`。
- `bfloat16_t`、`bfloat16x2_t` 使用 `simt_api/asc_bf16.h`。
- 文档说明 `half` 和 `bfloat16_t` 类型的返回值不准确，因此本样例的带返回值场景只用于触发返回值路径，不依赖返回值做正确性判断。

## 测试矩阵

| 编译参数 | 取值 | 说明 |
|---|---:|---|
| `TYPE_KIND` | 0 | `float` |
| `TYPE_KIND` | 1 | `half` |
| `TYPE_KIND` | 2 | `bfloat16_t` |
| `TYPE_KIND` | 3 | `half2`，两个 lane 都累加 1.0 |
| `TYPE_KIND` | 4 | `bfloat16x2_t`，两个 lane 都累加 1.0 |

| 编译参数 | 取值 | 说明 |
|---|---:|---|
| `SCENARIO_NUM` | 1 | GM，使用 `asc_atomic_add` 返回值 |
| `SCENARIO_NUM` | 2 | GM，丢弃 `asc_atomic_add` 返回值 |
| `SCENARIO_NUM` | 3 | UB，使用 `asc_atomic_add` 返回值 |
| `SCENARIO_NUM` | 4 | UB，丢弃 `asc_atomic_add` 返回值 |

## 编译运行

在本样例目录下执行：

```bash
mkdir -p build && cd build
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DTYPE_KIND=0 -DSCENARIO_NUM=1 -DSKIP_VALIDATION=ON ..
make -j
./atomic_type_test
```

默认 workload：

| 参数 | 默认值 | 说明 |
|---|---:|---|
| `BLOCK_NUM` | 48 | ThreadBlock 数量 |
| `THREADS_PER_BLOCK` | 256 | 每个 ThreadBlock 的线程数 |
| `ITERATIONS` | 256 | 每个线程执行的原子加次数 |
| `SLOTS` | 1024 | GM/UB counter slot 数量，源码固定 |

默认 workload 为 `48×256×256=3145728` 次原子加，用于和其他 atomic 性能样例的 profiling 配置对齐。

## 性能数据

以下数据均使用 `msprof op --launch-count=1` 采集，workload 为 `BLOCK_NUM=48`、`THREADS_PER_BLOCK=256`、`ITERATIONS=256`、`SLOTS=1024`，总原子加次数为 `3145728`。指标为 `OpBasicInfo.csv` 中的 `Task Duration(us)`。

浮点族数据来自本样例；整数数据来自同目录 `atomic_add_perf` 样例的 `1..6` 场景。`atomic_add_perf` 仅包含 UB int32 对照，不包含 UB int64 场景。

| 类型 | GM return（us） | GM no-return（us） | UB return（us） | UB no-return（us） |
|---|---:|---:|---:|---:|
| `int32_t` | 22344.602 | 136.744 | 59.577 | 58.064 |
| `int64_t` | 11239.149 | 11281.082 | - | - |
| `float` | 138386.203 | 139.927 | 107.113 | 58.123 |
| `half` | 613.570 | 291.880 | 67.901 | 59.135 |
| `bfloat16_t` | 613.066 | 291.855 | 67.078 | 58.302 |
| `half2` | 104079.648 | 145.236 | 103.620 | 107.712 |
| `bfloat16x2_t` | 34108.656 | 145.196 | 102.003 | 106.420 |

> 说明：GM 场景在 `msprof op` replay 下建议保持 `SKIP_VALIDATION=ON`。`half` 和 `bfloat16_t` 的返回值在接口文档中已说明不准确，本表中的 return 场景只表示触发返回值路径，正确性不依赖返回值。

## msprof op 采集

建议 profiling 时保持 `SKIP_VALIDATION=ON`，避免可选校验 kernel 和 host 拷贝混入采集流程，也避免 `msprof op` warmup/replay 对 GM counter 重复累加导致校验失败。

示例：采集 `half2` 在 GM 不使用返回值场景的性能：

```bash
mkdir -p build_half2_gm_noret && cd build_half2_gm_noret
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DTYPE_KIND=3 -DSCENARIO_NUM=2 -DSKIP_VALIDATION=ON ..
make -j
msprof op --launch-count=1 ./atomic_type_test
```

批量采集时为每个 `TYPE_KIND` 和 `SCENARIO_NUM` 使用独立 build 目录，或重新运行 `cmake` 后重新编译。

## 可选正确性校验

裸跑 smoke test 可关闭 `SKIP_VALIDATION`：

```bash
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DTYPE_KIND=0 -DSCENARIO_NUM=1 -DSKIP_VALIDATION=OFF ..
make -j
./atomic_type_test
```

`SKIP_VALIDATION=OFF` 时：

- GM 场景会在目标 kernel 后追加一个转换 kernel，把计数器转换为 `float` 后拷回 host 校验。
- UB 场景会在目标 kernel 内把每个 block 的 UB counter 转换为 `float` 写回 GM，再由 host 校验。
- profiling 数据不要使用关闭校验的运行结果。
- 对 `half`、`bfloat16_t`、`half2`、`bfloat16x2_t` 做校验时建议临时调小 `ITERATIONS`，例如 `-DITERATIONS=16`，避免低精度累加超过精确可表达范围。
