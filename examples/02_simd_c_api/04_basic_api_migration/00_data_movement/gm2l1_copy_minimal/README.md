# GM 到 L1 搬运极简 C API 样例

## 概述

本样例从完整迁移样例 [`data_copy_gm2l1`](../data_copy_gm2l1/README.md) 中提取最简单的连续搬运场景，仅演示数据的搬入和搬出，不包含 ND/Nz/DN 格式转换、L0 搬运、矩阵乘、Bias 或量化。

输入为 256 个 `uint16_t` 元素（512 B）。设备侧执行以下固定链路：

```text
GM input --asc_copy_gm2l1--> L1 --asc_copy_l12ub--> UB --asc_copy_ub2gm--> GM output
```

核函数采用`__mix__(1, 2)`，每个group包含1个AIC和2个AIV。`asc_copy_l12ub`仅支持AIC:AIV为1:2的Mix场景；AIC负责GM->L1 Buffer->UB，并通过`sub_blockid=false`将数据搬运到AIV0的UB。AIV侧使用`asc_get_sub_block_id()`筛选AIV0，仅AIV0等待同步信号并执行UB->GM，AIV1不参与回写，从而避免两个AIV同时等待单次通知或写入同一GM地址。

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
AIC: PIPE_MTE2 (GM->L1 Buffer) ──asc_sync_notify/wait──> PIPE_MTE1 (L1 Buffer->AIV0 UB)
                                                        │
                                      asc_sync_block_arrive/wait
                                                        │
AIV0:                                               PIPE_MTE3 (UB->GM)
AIV1:                                               不参与搬运
```

MTE2->MTE1是同一AIC内的流水依赖，使用`asc_sync_notify`和`asc_sync_wait`建立。L1 Buffer->UB的AIC侧生产与UB->GM的AIV0侧消费属于跨核依赖，使用`asc_sync_block_arrive(PIPE_MTE1, CROSS_CORE_FLAG_ID)`和`asc_sync_block_wait(PIPE_S, CROSS_CORE_FLAG_ID)`交接。不能使用`asc_sync_notify(PIPE_MTE1, PIPE_MTE3, ...)`：该组合会展开为目标不支持的`set_flag`参数组合。最终GM写回只由AIV0的MTE3流水发起，因此在AIV0分支内使用`asc_sync_pipe(PIPE_MTE3)`等待写回完成，不阻塞无关流水。

## API 依据

| API | 文档 | 本样例用法 |
|---|---|---|
| `asc_init` | [`misc/asc_init.md`](../../../../../docs/zh/api/SIMD-API/C-API/misc/asc_init.md) | 初始化 NPU 状态。 |
| `asc_copy_gm2l1` | [`cube_datamove/asc_copy_gm2l1/asc_copy_gm2l1_arch_3510.md`](../../../../../docs/zh/api/SIMD-API/C-API/cube_datamove/asc_copy_gm2l1/asc_copy_gm2l1_arch_3510.md) | 1 个连续 burst，16 个 32 B 数据块，无 padding。 |
| `asc_copy_l12ub` | [`cube_datamove/asc_copy_l12ub.md`](../../../../../docs/zh/api/SIMD-API/C-API/cube_datamove/asc_copy_l12ub.md) | 在AIC:AIV为1:2的Mix场景中向AIV0搬运1个连续burst，共16个DataBlock。 |
| `asc_copy_ub2gm` | [`vector_datamove/asc_copy_ub2gm/asc_copy_ub2gm_arch_3510.md`](../../../../../docs/zh/api/SIMD-API/C-API/vector_datamove/asc_copy_ub2gm/asc_copy_ub2gm_arch_3510.md) | 以 512 B 前 n 个数据搬运形式回写 GM。 |
| `asc_get_sub_block_id` | [`sys_var/asc_get_sub_block_id.md`](../../../../../docs/zh/api/SIMD-API/C-API/sys_var/asc_get_sub_block_id.md) | 在1:2的Mix场景中筛选AIV0，避免AIV1等待同一同步信号或写入同一输出地址。 |
| `asc_sync_notify` / `asc_sync_wait` | [`sync`](../../../../../docs/zh/api/SIMD-API/C-API/sync/) | 约束 AIC 内 GM→L1 与 L1→UB 的生产者—消费者关系。 |
| `asc_sync_block_arrive` / `asc_sync_block_wait` | [`sync`](../../../../../docs/zh/api/SIMD-API/C-API/sync/) | 使用 flag 0 完成 AIC→AIV 的核间交接；flag 的有效范围为 0 至 15。 |
| `asc_sync_pipe` | [`sync`](../../../../../docs/zh/api/SIMD-API/C-API/sync/) | AIV0仅等待最终的MTE3流水写回完成。 |

## 编译和执行

在具备 Ascend 950PR/Ascend 950DT（`dav-3510`）和对应 CANN 环境的机器上，可在本目录执行：

```bash
mkdir -p build && cd build
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..
make -j
./gm2l1_copy_minimal
```

本次新增仅做了静态文档、声明、实现和现有样例核对；未在当前机器上编译或执行。
