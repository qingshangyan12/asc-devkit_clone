<!--
@source: include/adv_api/hccl/hccl.h
@source: include/adv_api/hccl/hccl_common.h
@source: include/adv_api/hccl/hccl_tiling.h
@source: include/adv_api/hccl/hccl_tilingdata.h
@source: include/adv_api/hccl/internal/hccl.h
@source: impl/adv_api/detail/hccl/impl/hccl_impl_def.h
@source: impl/adv_api/detail/hccl/impl/hccl_impl.h
@source: impl/adv_api/detail/hccl/common/hccl_base.h
@source: impl/adv_api/detail/hccl/common/hccl_aicpu_impl.h
@source: impl/adv_api/detail/hccl/cc/src/ops/
-->

# HCCL 集合通信库设计文档

## 概述

HCCL（Huawei Collective Communication Library）是华为昇腾（Ascend）计算平台上的集合通信库，对标业界 MPI 集合通信接口，提供 AllReduce、AllGather、ReduceScatter、AlltoAll 等集合通信原语。

HCCL 采用 **Client-Server 异步架构**：
- **Client 端**运行在 AICore（AI 核）上，通过 `Hccl` 模板类下发通信任务（Prepare），通知服务端执行（Commit），并等待完成（Wait）。
- **Server 端**运行在 AICPU 上，轮询共享内存中的消息队列，执行算法选择、资源编排和实际数据传输。

本文档基于以下源码目录分析生成：

| 源码路径 | 说明 |
|----------|------|
| `include/adv_api/hccl/` | 公共头文件：API 声明、类型定义、Tiling 配置 |
| `impl/adv_api/detail/hccl/impl/` | 设备侧实现：按平台（V220/V310）分发 |
| `impl/adv_api/detail/hccl/common/` | 设备侧公共实现：AICPU 消息收发逻辑 |
| `impl/adv_api/detail/hccl/ccu/` | 设备侧 CCU（通信计算单元）实现 |
| `impl/adv_api/detail/hccl/cc/src/ops/` | Host 侧算法层：算子入口、选择器、执行器、模板 |
| `impl/adv_api/detail/hccl/cc/src/aicpu_kfc/` | AICPU 服务端框架 |

## 支持的集合通信算子

| 算子 | HcclCMDType 枚举值 | 说明 |
|------|---------------------|------|
| AllReduce | `HCCL_CMD_ALLREDUCE` | 对所有 rank 的数据进行规约，结果复制到每个 rank |
| AllGather | `HCCL_CMD_ALLGATHER` | 收集所有 rank 的数据到每个 rank |
| ReduceScatter | `HCCL_CMD_REDUCE_SCATTER` | 规约后分片散布到各 rank |
| AlltoAll | `HCCL_CMD_ALLTOALL` | 全互连数据交换 |
| AlltoAllV | `HCCL_CMD_ALLTOALLV` | 变长全互连数据交换 |
| BatchWrite | `HCCL_CMD_BATCH_WRITE` | 批量写入指定 rank 内存 |
| AlltoAllvWrite | — | 批量写入远端窗口 |

> 枚举定义见 `include/adv_api/hccl/hccl_common.h:22-44`（`HcclCMDType`）。

## 典型使用流程

```
1. 创建 Hccl 对象并初始化        → Hccl::InitV2()
2. 设置 Tiling 数据              → Hccl::SetCcTilingV2()
3. 异步下发通信任务（返回 handle） → Hccl::AllReduce<commit>()
4. 通知服务端执行任务             → Hccl::Commit(handle)
5. 等待任务完成                   → Hccl::Wait(handle)
6. 通知服务端退出                 → Hccl::Finalize()
```

> 参见 `include/adv_api/hccl/hccl.h:34-51` 中 `Hccl` 类的典型使用说明。

## 支持的服务端类型

| serverType | 枚举值 | 说明 |
|------------|--------|------|
| `HCCL_SERVER_TYPE_AICPU` | 0 | AICPU 服务端（主要支持） |
| `HCCL_SERVER_TYPE_CCU` | 5 | CCU 通信计算单元服务端 |

> 定义见 `include/adv_api/hccl/hccl_common.h:65`。

## 支持的平台

通过 `__NPU_ARCH__` 宏在编译期分发：

| `__NPU_ARCH__` | 平台 | 实现文件 |
|----------------|------|----------|
| `2201` | Atlas A2 训练/推理系列 | `impl/hccl_v220_impl.h` → `impl/platform_v220/` |
| `3510` | Ascend 910_93 | `impl/hccl_v310_impl.h` → `impl/platform_v310/` |

> 分发逻辑见 `impl/adv_api/detail/hccl/impl/hccl_impl_def.h:20-26`。
