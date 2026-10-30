# Introduction to Cooperative Groups

<!-- md-trans-meta sourceCommit=e552eb8ded44e154ba5812983f5bf0383b80631c translatedAt=2026-10-16T11:33:55.202Z pushedAt=2026-10-20T03:02:40.206Z -->

Cooperative groups are an extension of the Ascend C SIMT programming model for organizing cooperative thread groups. Cooperative groups enable finer-grained thread management, thereby achieving more efficient thread-level parallel collaboration.

The currently supported cooperative group types are as follows:

- [grid_group](grid_group/grid_group_intro.md): An abstraction of all SIMT threads launched by a kernel, providing cross-thread-block query and synchronization interfaces.
- [thread_block](thread_block/thread_block_intro.md): An abstraction of a thread block, providing unified thread management interfaces.
- [thread_block_tile](thread_block_tile/thread_block_tile_intro.md): A thread subgroup manually partitioned.
- [coalesced_group](coalesced_group/coalesced_group_intro.md): The set of active threads in a warp.
- [thread_group](thread_group/thread_group_intro.md): The base class of all cooperative groups, providing common cooperative group interfaces.

Based on the cooperative group types above, the following partition operations are provided to support partitioning a cooperative group into multiple subgroups:

- [tiled_partition](tiled_partition.md): Partitions a parent group into subgroups of a specified size.
- [binary_partition](binary_partition.md): Divides the parent group into two subgroups based on custom 0 and 1 labels.

Based on the cooperative group types above, the following common operations are provided:

- [sync](sync.md): Synchronizes all threads in the specified cooperative group, which is functionally equivalent to calling the member function `sync()` of the cooperative group object.

> [!NOTE] Note
> All cooperative group APIs are under the `cooperative_groups` namespace and must be called with the namespace specified.  
> `grid_group` supports only the SIMT programming scenario and does not support the mixed SIMD and SIMT programming scenario.
