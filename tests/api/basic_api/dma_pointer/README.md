# DMA 指针接口测试

[English](README_en.md)

编译驱动使用 ASC 编译器的真实 GM、UB、L1、L0C、Bias 和 Fixpipe 地址空间实例化公共 DMA API。
编译检查不执行 kernel；下文的独立运行程序验证部分设备场景。

## 编译检查

在仓库根目录激活 CANN 环境后执行：

```bash
python3 tests/api/basic_api/dma_pointer/check_dma_pointer.py \
    --compiler bisheng --extras --output /tmp/dma-pointer-results
```

输出目录必须不存在或为空。每例分别保存生成的编译单元、含头文件追踪的编译日志，
`results.json` 保存准确命令和源码 hash。运行前后检查 Basic API 头文件与驱动的 hash，
发现源码变化会使汇总结果失效。可用 `--rows 21,31 --forms TT,PP` 选择诊断子集；
`--jobs 4` 启用并发。默认单编译进程，各例目录独立。

行号 1 至 44 按官方 DMA 清单排列：DataCacheCleanAndInvalid（1-3）、
DataCachePreload（4）、DataCopy（5-31）、DataCopyL1ToUB（32-33）、DataCopyPad（34-44）。
TT 为 Tensor/Tensor，PP 为 Pointer/Pointer，TP、PT 分别表示目标/源混合形式；Cache 用 T/P。
第 34 行使用 TensorTrait/primitive padding，检查 TT/TP/PT；纯指针擦除了 Trait 身份，
使用普通第 35 行入口。附加 `LP-C05-explicit-trait-pointer` 用例显式指定
`<TensorTrait<float>, float>`，检查双指针桥接。

历史行 5、19、39-42 选择 2201，其余选择 3510。2201 编译结果不证明 950 支持。
特别是行 6-11 的 Enhanced 路由仍不受 3510 实现支持，其 3510 编译检查仅证明签名绑定。
每行选择一种路由和类型对，只表示声明覆盖，不是全部 dtype、布局、参数边界或设备路由覆盖。

附加用例检查数组退化、自动类型推导、const 源、const 目标拒绝、错误地址空间、普通 Host 指针、
混合元素类型、SmallC0 模板选项和非法 Preload offset 类型。负例必须出现约束/重载诊断，
且正向对照通过；头文件加载失败不能当作负例通过。

每例包含对应的独立公共 DMA 头文件，使用支持的 `ASCENDC_DUMP=0`，
并核验 `-H` 输出中的 worktree 路径，拒绝已安装 SDK 的 Basic API 头文件。
`--umbrella` 可检查完整 `kernel_operator.h` 包含图。
默认检查不能替代官方 Header Checker、启用调试的构建或仓库完整测试分组。

CPU 测试 `test_dma_pointer_cache_compat.cpp` 由现有 950 AIV_BASIC 源码 glob
（`basic_test_four`）发现。它使用 mockcpp 检查旧 Tensor Preload 包装的 offset 类型和指针转发，
包含索引 Tensor 和 const 句柄。CPU Debug 擦除 Host 地址空间，因而排除 Pointer 重载；
CPU mock 不验证 NPU cache 效果。

## 搬运运行测试

确保当前环境可以找到 CANN ASC CMake 包，执行：

```bash
cmake -S tests/api/basic_api/dma_pointer -B /tmp/dma-pointer-runtime-build
cmake --build /tmp/dma-pointer-runtime-build -j2
/tmp/dma-pointer-runtime-build/dma_pointer_runtime --device DEVICE_ID
/tmp/dma-pointer-runtime-build/dma_pointer_cache --device DEVICE_ID
/tmp/dma-pointer-runtime-build/dma_pointer_l1toub --device DEVICE_ID
```

将 `DEVICE_ID` 替换为明确选定的空闲设备。若 CMake 无法定位 ASC，按实际安装位置传入
`-DASC_DIR=<CANN package>/lib64/cmake`。目标默认 `dav-3510`，使用独立公共 DMA/同步头文件，
通过 `-iquote` 优先使用当前 worktree 头文件。`ASCENDC_DUMP=0` 不验证调试构建，
也不替代完整 Header Checker。

对 `dav-2201` 设备须显式指定架构并使用独立构建目录：

```bash
cmake -S tests/api/basic_api/dma_pointer -B /tmp/dma-pointer-runtime-2201 \
    -DCMAKE_ASC_ARCHITECTURES=dav-2201
cmake --build /tmp/dma-pointer-runtime-2201 -j2
/tmp/dma-pointer-runtime-2201/dma_pointer_runtime --device DEVICE_ID
```

所选 CANN 编译器必须支持当前仓库基线和目标架构。2201 结果不证明 950 行为；
每次运行记录真实设备、编译器及可执行文件 hash。运行场景仅用 Normal padding，
不依赖 950 专属 Compact 模式。

搬运程序执行 240 例：长度 1、5、8、16、20、31、32、59、1024、2048，
三轮变化的确定性输入，count 和非对齐 Pad 搬运，TT/PP/TP/PT 四种形式。
输出 uint32 与 Host oracle 及对应 Tensor 结果逐位精确比较，同时检查 UB/GM 保护区和已定义 padding。
ACL 初始化、分配、复制、同步或清理出错均以非零状态退出。
kernel 显式初始化 SoC 状态，用 MTE2 barrier 串行化目标重叠的保护区初始化和输入 DMA，
并在局部数组生命周期结束前完成待处理流水；API 适配器自身保留异步语义。

无设备时可尝试 950 simulator，使用 `dav-3510` 构建且设备号为 0：

```bash
cannsim record /tmp/dma-pointer-runtime-build/dma_pointer_runtime \
    -s Ascend950 -o /tmp/dma-pointer-simulator -u '--device 0'
```

独立运行使用全新构建/证据目录。构建成功不证明运行正确；搬运通过必须同时满足退出码 0 和
`ALL_PASS cases=240 comparison=exact_u32_with_canaries_and_tensor_diff`。
中断或不完整的 simulator 输出不能计为通过；本测试不证明全部 44 重载设备覆盖或性能。

## Cache 运行测试

`dma_pointer_cache` 使用显式 Vector kernel，在三轮变化输入下执行 48 例。
覆盖 GM 三模板参数和二模板参数 DCCI、UB DCCI、int16/int64 offset 0/64 的 Preload，
以及适用的 Tensor/Pointer 和 SINGLE/ENTIRE 模式。
标量写入在观察前刷出，Host 检查 uint64 内容、前后保护区及 Tensor/Pointer 一致性。
通过必须满足退出码 0 和 `ALL_PASS cases=48 comparison=exact_u64_cache_visibility`。

UB 场景先用不同内容初始化后备存储，防止前一例残留数据掩盖写回缺失。
该测试检查指定 cache 可见性/使用场景，不测命中率或性能。

## L1ToUB 运行测试

`dma_pointer_l1toub` 运行 Mix1:2 kernel，使用文档定义的 AIC MTE1 producer 和
mode2 跨核完成通知。两个 AIV 都消费通知，仅所选 AIV 将其 UB 内容写回。
共 48 例：三轮输入、subBlockId 0/1、count/带 stride 的 Params、TT/PP/TP/PT。

每次待测搬运前，旧 Tensor 路径填充所选 peer UB 的保护值并完成 MTE1。
随后观察完整 UB 内容（包括 stride 空隙）、GM 保护区和未选 peer 的输出槽。
通过必须满足退出码 0 和 `L1TOUB_RUNTIME_PASS cases=48`。

Cache 和 L1ToUB 运行目标仅为 dav-3510 构建，不替代完整 DMA 路由/dtype/布局矩阵或性能测试。
