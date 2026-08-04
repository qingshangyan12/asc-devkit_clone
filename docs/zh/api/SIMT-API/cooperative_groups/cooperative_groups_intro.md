# cooperative_groups_intro

cooperative_groups（cooperative_groups）是Ascend C SIMTprogramming_model的一种拓展，用于组织协作线程组。通过cooperative_groups能够更细粒度地管理线程，从而实现更高效的线程并行协作。

当前提供cooperative_groups类型有：

- [thread_block](thread_block/thread_block_intro.md)：对线程块的抽象，提供统一的线程管理接口。
- [coalesced_group](coalesced_group/coalesced_group_intro.md)：Warp中的活跃线程集合。
- [thread_block_tile](thread_block_tile/thread_block_tile_intro.md)：手动划分出的线程子组。

基于以上cooperative_groups类型，提供以下划分操作，支持将一个cooperative_groups划分为多个子组：

- [tiled_partition](tiled_partition.md)：将父组划分成指定大小的子组。
- [binary_partition](binary_partition.md)：根据自定义的0、1标签将父组划分为两个子组。

## 需包含的头文件

```cpp
#include "simt_api/cooperative_groups.h"
```

> [!NOTE]说明 
> 所有cooperative_groupsAPI均在`cooperative_groups`命名空间下，调用时需添加命名空间。
