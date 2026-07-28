# GM 到 L1 搬运极简 C API 样例

## 概述

本样例从完整迁移样例 [`data_copy_gm2l1`](../data_copy_gm2l1/README.md) 中提取最简单的连续搬运场景，仅演示数据的搬入和搬出，不包含 ND/Nz/DN 格式转换、L0 搬运、矩阵乘、Bias 或量化。

输入为 256 个 `uint16_t` 元素（512 B）。设备侧执行以下固定链路：

```text
GM input --asc_copy_gm2l1--> L1 --asc_copy_l12ub--> UB --asc_copy_ub2gm--> GM output
```

`dav-3510` 上的 C API 文档将 `asc_copy_l12gm` 标记为不支持，因此本样例不使用直接 L1→GM 回写；经 UB 回写是为了保留 L1 搬入过程并让主机侧能够读取、打印结果。

## 目录结构

```text
gm2l1_copy_minimal
├── CMakeLists.txt
├── gm2l1_copy_minimal.asc
└── README.md
```

## 数据和逻辑

主机侧使用确定性数据初始化输入：第 `i` 个元素为 `i * 3 + 1`。核函数只复制数据，主机在结果回传后打印输入和输出的前 8 个 `uint16_t` 值（十六进制）。因此两行打印内容应一致，例如：

```text
input (first 8): 0x0001 0x0004 0x0007 0x000a 0x000d 0x0010 0x0013 0x0016
output (first 8): 0x0001 0x0004 0x0007 0x000a 0x000d 0x0010 0x0013 0x0016
```

设备侧的依赖关系为：

```text
PIPE_MTE2 (GM→L1) ──> PIPE_MTE1 (L1→UB) ──> PIPE_MTE3 (UB→GM)
```

每一条箭头均使用 `asc_sync_notify` 和 `asc_sync_wait` 建立，最后用 `asc_sync_pipe(PIPE_ALL)` 收敛核内异步搬运。

## API 依据

| API | 文档 | 本样例用法 |
|---|---|---|
| `asc_init` | [`misc/asc_init.md`](../../../../../docs/zh/api/SIMD-API/C-API/misc/asc_init.md) | 初始化 NPU 状态。 |
| `asc_copy_gm2l1` | [`cube_datamove/asc_copy_gm2l1/asc_copy_gm2l1_arch_3510.md`](../../../../../docs/zh/api/SIMD-API/C-API/cube_datamove/asc_copy_gm2l1/asc_copy_gm2l1_arch_3510.md) | 1 个连续 burst，16 个 32 B 数据块，无 padding。 |
| `asc_copy_l12ub` | [`cube_datamove/asc_copy_l12ub.md`](../../../../../docs/zh/api/SIMD-API/C-API/cube_datamove/asc_copy_l12ub.md) | 1 个连续 burst，16 个 DataBlock。该接口不能与 `__cube__` 或 `__vector__` 执行空间修饰符同时使用。 |
| `asc_copy_ub2gm` | [`vector_datamove/asc_copy_ub2gm/asc_copy_ub2gm_arch_3510.md`](../../../../../docs/zh/api/SIMD-API/C-API/vector_datamove/asc_copy_ub2gm/asc_copy_ub2gm_arch_3510.md) | 以 512 B 前 n 个数据搬运形式回写 GM。 |
| `asc_sync_notify` / `asc_sync_wait` / `asc_sync_pipe` | [`sync`](../../../../../docs/zh/api/SIMD-API/C-API/sync/) | 约束三段异步数据搬运的生产者—消费者关系。 |

## 编译和执行

在具备 Ascend 950PR/Ascend 950DT（`dav-3510`）和对应 CANN 环境的机器上，可在本目录执行：

```bash
mkdir -p build && cd build
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..
make -j
./gm2l1_copy_minimal
```

本次新增仅做了静态文档、声明、实现和现有样例核对；未在当前机器上编译或执行。
