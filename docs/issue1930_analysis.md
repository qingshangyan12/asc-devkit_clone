# Issue 1930分析报告

## Issue信息

- Issue：[1930](https://gitcode.com/cann/asc-devkit/issues/1930)
- 标题：`[Bug-Report|缺陷反馈]`。
- 状态：待办。
- 问题描述：反馈`cross_core_set_wait_flag.h:398-453`中使用`GetBlockIdx()/2`分区，导致两个核写入同一个`atomicResultGm`，并建议在跨核写入点增加`CrossCoreSetFlag/CrossCoreWaitFlag`。
- 关联Issue：[1670](https://gitcode.com/cann/asc-devkit/issues/1670)。Issue 1670已关闭，修复内容是为多核普通GM写入按核切分GM空间。
- Issue 1930当前无评论。

## 结论

当前问题不需要修复，建议关闭Issue 1930。代码中的同址写入是该样例用于演示原子累加和核间同步的设计，不是未同步的普通写冲突；`GetBlockIdx()/2`也不用于`atomicResultGm`的地址计算。

## 代码核查

### `GetBlockIdx()/2`的使用位置

在[cross_core_set_wait_flag.h第103-106行](../examples/01_simd_cpp_api/03_basic_api/05_sync_control/cross_core_set_wait_flag/cross_core_set_wait_flag.h#L103)中，`GetBlockIdx()/2`只用于`aGM`、`bGM`、`AVectorGM`和`BVectorGM`的输入地址分区。两个AIV共享同一个AI Core对应的输入分片，这是Cube与Vector融合场景中AIV0/AIV1协作处理同一分片的预期行为。

`CVectorGM`在[第107行](../examples/01_simd_cpp_api/03_basic_api/05_sync_control/cross_core_set_wait_flag/cross_core_set_wait_flag.h#L107)使用完整的`GetBlockIdx()`计算偏移，因此融合场景中不同AIV的最终C矩阵写回区域互不重叠。这与下述纯Vector场景中的`atomicResultGm`是不同的写入路径；后者确实存在多个核向同一地址写入。

### `atomicResultGm`的同址写入

`atomicResultGm`在[第376-377行](../examples/01_simd_cpp_api/03_basic_api/05_sync_control/cross_core_set_wait_flag/cross_core_set_wait_flag.h#L373)被所有参与同步的AIV映射到同一段GM空间。这是模式0和模式1的归约目标：

- 模式0中，所有AIV执行[第407-409行](../examples/01_simd_cpp_api/03_basic_api/05_sync_control/cross_core_set_wait_flag/cross_core_set_wait_flag.h#L406)的`SetAtomicAdd<float>()`和`DataCopy`，将各核结果原子累加到同一目标。
- 模式1中，BlockIdx为2和3的两个AIV执行[第443-444行](../examples/01_simd_cpp_api/03_basic_api/05_sync_control/cross_core_set_wait_flag/cross_core_set_wait_flag.h#L443)的原子累加，两个核共享目标是模式1的预期计算逻辑。
- 该接口文档说明`SetAtomicAdd`会将待拷贝内容与GM已有内容求和后写回GM，并要求在累加完成后关闭原子累加。样例分别在[第415行](../examples/01_simd_cpp_api/03_basic_api/05_sync_control/cross_core_set_wait_flag/cross_core_set_wait_flag.h#L415)和[第447行](../examples/01_simd_cpp_api/03_basic_api/05_sync_control/cross_core_set_wait_flag/cross_core_set_wait_flag.h#L447)关闭原子累加。

### 核间同步已经存在

每次原子写入之后，代码均配对使用核间通知和等待：

- 模式0：[第411-413行](../examples/01_simd_cpp_api/03_basic_api/05_sync_control/cross_core_set_wait_flag/cross_core_set_wait_flag.h#L411)。
- 模式1：[第445-446行](../examples/01_simd_cpp_api/03_basic_api/05_sync_control/cross_core_set_wait_flag/cross_core_set_wait_flag.h#L445)。

等待解除后，代码才关闭原子累加，并由指定核读取归约结果。该顺序与样例说明中“等待所有参与AIV完成PIPE_MTE3搬运后再读取结果”的描述一致。

## 与Issue 1670的区别

Issue 1670针对的是多个核以普通`DataCopy`写入同一GM地址，且没有原子累加，属于真实的WAW竞争。Issue 1930对应代码使用了`SetAtomicAdd<float>()`，同址写入本身是原子归约操作；同时已有`CrossCoreSetFlag/CrossCoreWaitFlag`用于保证归约完成。因此不能直接套用Issue 1670的“按核扩展输出地址”方案，否则会破坏样例要演示的归约语义。

## 判定

不需要修改代码，也不需要额外增加`CrossCoreSetFlag/CrossCoreWaitFlag`。增加额外同步不会解决Issue所述问题，因为当前写入已经是原子累加，且写入后的核间同步已经存在。

## 建议回复内容

> 经核查，模式0/模式1中确实存在多个AIV向同一段`atomicResultGm`地址写入。这是样例有意设计的原子累加归约：各核在`DataCopy`前调用`SetAtomicAdd<float>()`，将本核结果累加到共享GM，而不是以普通写入覆盖其他核的结果。
>
> 原子操作保证共享目标上的累加更新；代码中已有的`CrossCoreSetFlag/CrossCoreWaitFlag`用于等待参与核完成搬运，再由指定核读取归约结果。两者分别承担原子更新和完成同步的作用，该场景与Issue 1670中的普通GM同址覆盖写不同。
>
> 另外，`GetBlockIdx()/2`出现在`cross_core_set_wait_flag.h`第103-106行的融合场景输入分片代码中；`atomicResultGm`的地址设置位于第377行，未使用`GetBlockIdx()/2`，不能将这两段代码视为同一条写入路径。
>
> 因此当前代码不存在Issue 1930描述的未同步WAW问题，无需修改，建议关闭该Issue。

## Issue1930回复

感谢您反馈Issue1930并关注样例中的跨核GM写入问题。

经核查，当前代码不存在Issue1930描述的未同步WAW问题，无需新增代码修改，建议关闭本Issue。

原因如下：

1. Issue1930中提到的`GetBlockIdx()/2`并不用于`atomicResultGm`的地址计算。在融合场景中，`GetBlockIdx()/2`仅用于划分`aGM`、`bGM`、`AVectorGM`和`BVectorGM`的输入数据；`atomicResultGm`在初始化时使用完整的共享地址，没有通过`GetBlockIdx()/2`计算写入偏移。
2. `atomicResultGm`被多个AIV写入是样例演示原子累加归约的预期设计。写入前调用`SetAtomicAdd<float>()`，使`DataCopy`搬运到同一GM地址时执行原子加法，而不是普通写入覆盖其他核的数据。模式0由参与计算的AIV共同累加，模式1由BlockIdx为2和3的两个AIV共同累加。
3. 原子写入完成后，代码使用`CrossCoreSetFlag`通知其他AIV，并使用`CrossCoreWaitFlag`等待参与核完成`PIPE_MTE3`上的搬运，再关闭原子累加并读取归约结果。现有核间同步用于保证归约完成后的执行顺序，不需要在同一写入点额外添加一组同步接口。

本Issue与Issue1670的场景不同。Issue1670中的多个核使用普通`DataCopy`写入同一GM地址区间，没有原子累加，属于真实的跨核WAW竞争，因此需要按核划分GM空间，使各核写入互不重叠。Issue1930中的多个核则通过`SetAtomicAdd<float>()`向共享目标执行原子累加，同址写入本身就是样例要演示的归约语义。因此，Issue1670需要修复普通搬运的地址竞争，Issue1930不需要修改原子操作场景。


