# OOM Storage-Shape Magic Number Update

## 需求

将 OOM 非连续场景 storage-shape 扩展头的 magic number 从 `1` 修改为 `0x4F`，并增加协议值回归测试。

## 背景简述

- `DEVKIT_PATH`: `/data/chu/01_OOM/asc-devkit-3`
- `CANN_PATH`: `/data/chu/Ascend/cann`
- `SOC_ARCH`: 不涉及

最近两次提交引入并调整了 OOM storage-shape 描述符。当前协议 magic 定义为
`g_oomStorageShapeMagic = 1U`，Device 端通过该常量识别扩展头。现有测试也使用该常量构造数据，
因此不能独立约束协议规定的具体字节值。

## 涉及领域

AscendC Basic API、OOM storage-shape 描述符协议、C++ 单元测试。

## 方案详述

1. 将 `impl/basic_api/utils/kernel_utils_macros.h` 中的 `g_oomStorageShapeMagic` 修改为 `0x4FU`。
2. 在 `test_dma_oom.cpp` 的 storage-shape 头测试中显式断言 magic 常量等于 `0x4F`。
3. 增加旧 magic `1` 被 `OOMHasStorageShapeHeader` 拒绝的检查，防止兼容性行为被意外放宽。
4. 不修改 storage-shape version、tensor-view version、descriptor type、扩展头尺寸或 Python 编译链。
5. 先运行新增断言并确认其在旧实现上按预期失败，再修改生产常量并运行目标 C++ UT；最后执行相关静态检查和可用的回归测试。

成功标准：扩展头首字节为 `0x4F` 时通过校验，首字节为旧值 `1` 时校验失败，现有 tensor 与 tensor-list 描述符测试保持通过。
