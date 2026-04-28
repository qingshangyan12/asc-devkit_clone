# Cooperative Groups 协作组测试样例

## 概述

本目录包含 cooperative_groups.h 中定义的所有API的测试用例，验证协作组功能的正确性。

## 样例列表

| 目录名称 | 功能描述 |
| -------- | -------- |
| [thread_block](./thread_block/) | thread_block 类测试：this_thread_block(), sync(), thread_rank(), group_index(), thread_index(), dim_threads(), num_threads(), size(), group_dim() |
| [thread_block_tile](./thread_block_tile/) | thread_block_tile 类测试：sync(), num_threads(), thread_rank(), size(), meta_group_rank(), meta_group_size() |
| [coalesced_group](./coalesced_group/) | coalesced_group 类测试：coalesced_threads(), sync(), num_threads(), thread_rank(), size(), meta_group_size(), meta_group_rank() |
| [tiled_partition](./tiled_partition/) | tiled_partition() 函数测试：对 thread_block 和 coalesced_group 进行分区 |
| [shuffle_functions](./shuffle_functions/) | Warp Shuffle 函数测试：shfl(), shfl_up(), shfl_down(), shfl_xor() |
| [vote_functions](./vote_functions/) | Warp Vote 函数测试：any(), all(), ballot() |
| [fusion_compute_tests](./fusion_compute_tests/) | SIMD与SIMT混合编程测试：在混合编程模式下验证所有协作组API的正确性 |

## 背景知识

Ascend C SIMT 对标 CUDA cooperative_groups，提供了协作组API，所有API位于命名空间 `cooperative_groups` 下。

### 内置变量
- `gridDim`：网格维度
- `blockDim`：线程块维度
- `blockIdx`：线程块索引
- `threadIdx`：线程索引
- `warpSize`：warp大小（32）

### 主要类
- `thread_group`：所有协作组类的基类
- `thread_block`：线程块组
- `thread_block_tile<Size, ParentT>`：模板化的分块组
- `coalesced_group`：合并组