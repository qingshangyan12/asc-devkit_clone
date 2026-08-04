# SIMT语言扩展层C API<a name="ZH-CN_TOPIC_0000002509743873"></a>

SIMT编程基于AI Core的硬件能力实现，当前，SIMT语言扩展层支持的C API类别如下：

-   [sync_and_memory_fence](../../../api/SIMT-API/sync_and_memory_fence/sync_and_memory_fence.md)：提供sync_and_memory_fence接口，解决线程间可能存在的数据竞争以及线程的同步问题。
-   [atomic_operations](../../../api/SIMT-API/atomic_operations/atomic_operations.md)：提供对Unified Buffer或Global Memory上的数据与指定数据执行atomic_operations的一系列API接口。
-   [Warp_functions](../../../api/SIMT-API/Warp_functions/Warp_functions.md)：提供对单个Warp内32个线程数据进行处理的一系列API接口。
-   [math_functions](../../../api/SIMT-API/math_functions/math_functions.md)：提供处理数学运算的函数接口集合。
-   [memory_access_functions](../../../api/SIMT-API/memory_access_functions/memory_access_functions.md)：提供开启Cache Hints的Load/Store函数。
-   [address_space_predicate_functions](../../../api/SIMT-API/address_space_predicate_functions/address_space_predicate_functions.md)：提供判断输入指针所属地址空间的函数接口集合。
-   [address_space_conversion_functions](../../../api/SIMT-API/address_space_conversion_functions/address_space_conversion_functions.md)：提供不同地址空间地址值与指针之间相互转换的函数接口集合。
-   [cooperative_groups](../../../api/SIMT-API/cooperative_groups/cooperative_groups.md)：提供更细粒度的线程间协作的API接口。
-   [tuning_interface](../../../api/Utils-API/tuning_interface/tuning_interface.md)：SIMT调试场景下使用的相关接口。
