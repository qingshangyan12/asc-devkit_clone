# PR 6264 OOM Storage-Shape Adaptation Design

## 需求

将 GitCode PR 6264 的 OOM storage-shape descriptor 能力适配到
`/data/chu/asc-devkit-5` 的 `master` 分支，并保留目标分支已有的后续提交。

交付内容包括：

- OOM storage-shape 元数据解析与地址范围注册能力。
- 动态 shape 编译代码生成和 JSON 版本信息。
- 旧版 OOM 元数据的兼容回退路径。
- 对应的 Ascend910B1 Basic API UT 和 Python UT。

## 背景简述

- `DEVKIT_PATH`: `/data/chu/asc-devkit-5`
- `CANN_PATH`: `/data/chu/Ascend/cann-9.2.0`
- `SOC_ARCH`: Ascend910B1 / `dav-2201`
- PR head: `3c5e9d6f60cb19c4d2b0aa29d7ad61330d009e35`
- PR 的上游父提交: `3577641c0feec70af534dd61f2e11d7f116b3edf`
- 目标分支当前提交: `4f3a01a62`

目标分支与 PR 在 `3577641c0` 后分叉。目标分支新增的两个提交只修改
Tensor API 和 integer fast division 示例，不涉及 PR 修改的 12 个文件。
PR head 是 merge commit，其最终状态已经包含早期功能提交与上游重构之间的
冲突解法。

## 涉及领域

- AscendC Basic API 内部 OOM 地址范围注册。
- storage-shape descriptor 的小端字段解析。
- 动态 Tensor 和 TensorList 的 storage size 计算。
- AscendC 编译工程中的动态 shape 代码生成、DFX 大小和 JSON 元数据。
- Ascend910B1 AIV 单元测试及 Python 编译工具单元测试。

## 方案详述

### 移植方式

在干净的目标 `master` 上执行：

```bash
git cherry-pick -m 2 3c5e9d6f6
```

选择第二父提交是为了仅提取 PR 相对上游 `3577641c0` 的最终净改动。
这会复用 PR 已完成的冲突解法，同时避免合入 PR 第一父分支的额外历史。

如果 cherry-pick 出现意外冲突，仅在 PR 涉及的 12 个文件中按 PR head 的
最终语义解决；不改动目标分支的 Tensor API 和 integer fast division 变更。

### 功能行为

1. OOM 元数据末尾增加 2 字节 storage-shape 扩展头。
2. 扩展头 magic 和 version 有效时，按 descriptor 注册 Tensor 或 TensorList
   的实际 storage size。
3. 扩展头缺失、版本不匹配或 descriptor 无效时，继续使用原有长度信息，
   保持旧调用方兼容。
4. 检查整数溢出、descriptor block size、TensorList 数量与动态参数布局，
   无效输入不得推进 descriptor cursor。
5. 编译 JSON 增加 OOM 和 tensor descriptor 版本信息，`opParaSize` 同步增加
   扩展头长度。

### 代码范围

仅移植 PR 相对 `3577641c0` 的 12 个文件：

- `impl/basic_api/utils/kernel_utils_ceil_oom_que.h`
- `impl/basic_api/utils/kernel_utils_macros.h`
- `tests/api/basic_api/ascendc_case_ascend910b1/ascendc_case_ascend910b1_aiv/test_dma_oom.cpp`
- 两套 `tests/python/*/test_compile_op.py`
- 两套 `ascendc_compile_dfx.py`
- 两套 `ascendc_compile_gen_code.py`
- 两套 `compile_op.py`
- `tools/build/asc_op_compile_base/asc_op_compiler/cce_runtime.py`

不进行无关重构或格式化。

### 验证

1. 确认无未解决冲突和冲突标记。
2. 对修改的 Python 文件执行 `py_compile`。
3. 在 CANN 9.2.0 环境下运行两套 `test_compile_op.py` 中的 OOM
   storage-shape 定向用例。
4. 构建或复用 Ascend910B1 AIV Basic API UT，运行 `TestDMAOom` 的
   storage-shape 用例。
5. 执行针对本次 12 个文件的 `git diff --check`。
6. 检查最终工作区干净，并确认提交只包含设计文档和 PR 适配改动。

若完整 C++ UT 因本机构建依赖不可用而无法执行，必须记录具体阻塞原因，
不能将语法检查或旧构建产物表述为完整验证通过。
