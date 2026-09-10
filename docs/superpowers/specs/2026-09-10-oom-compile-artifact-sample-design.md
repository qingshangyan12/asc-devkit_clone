# OOM 编译产物检测样例设计

## 需求

在 `examples/01_simd_cpp_api/02_features/04_compile` 下新增一个不依赖 NPU
设备和 SoC 的编译产物检测样例，用于验证最新两次提交实现的 OOM 非连续
Tensor storage-shape 支持，并证明原有 OOM 与非 OOM 编译场景保持兼容。

样例必须自动检查生成源码、单算子 JSON 能力字段和 `opParaSize`。任一检查
失败时脚本返回非零状态；全部检查通过时逐项输出 `PASS`。

## 背景简述

- `DEVKIT_PATH`: `/data/chu/01_OOM/asc-devkit-3`
- `CANN_PATH`: `/data/chu/Ascend/cann`
- `SOC_ARCH`: 不涉及；样例不执行设备编译或 kernel 启动。

提交 `526d3dbba` 为动态 shape OOM 参数增加可选 storage-shape 描述符，提交
`d7bb8779f` 将扩展头和描述符 ABI 修正为紧凑布局。动态 shape 的生成代码在
旧 OOM 参数之后探测扩展头；探测成功时按 storage shape 注册范围，否则继续
使用旧的 logical/view shape 长度。OOM 单算子 JSON 同时增加版本能力字段，
`opParaSize` 为可选扩展头增加 2 字节预留。

现有 `04_compile` 目录中的样例主要演示 ASC/C++ 构建模式，没有针对编译器
生成源码和 JSON 契约的自包含验证样例。

## 涉及领域

AscendC 编译工程、OOM 地址范围注册、动态 shape、TensorList、单算子 JSON、
生成源码兼容性、Python 样例与文档。

## 方案详述

### 1. 目录与文件

新增目录：

```text
examples/01_simd_cpp_api/02_features/04_compile/04_oom_compile_artifact_check/
  check_oom_artifacts.py
  README.md
  README_en.md
```

并在 `04_compile/README.md` 和 `04_compile/README_en.md` 的样例列表中加入入口。

### 2. 样例边界

`check_oom_artifacts.py` 直接加载仓内 `asc_op_compiler` 代码生成模块，以最小
fixture 构造代码生成所需的算子、tiling 和构建配置对象。它不调用 `opc`、
bisheng 或设备 runtime，因此不要求 `soc_version`、NPU 设备或 OPP 安装。

脚本从自身路径向上解析 devkit 根目录，允许通过命令行参数显式覆盖根目录。
仓内工具模块不可加载、生成接口发生异常或产物断言失败时，脚本打印具体失败
项并返回非零状态，不把环境或导入错误误报为验证通过。

### 3. 验证矩阵

#### 动态普通 Tensor + OOM

生成源码必须同时包含：

- `OOMHasStorageShapeHeader` 扩展头探测；
- 2 字节扩展头游标推进；
- `OOMTryRegisterTensorWithStorageShape` 新描述符路径；
- `OOMCheckAddrRange` 旧逻辑 shape 回退路径。

这证明新 binary 可以消费新 runtime 参数，同时仍能处理未附加 magic 的旧参数。

#### 动态 TensorList + OOM

生成源码必须同时包含：

- `OOMTryRegisterTensorListWithStorageShape` 新描述符路径；
- `OOMCheckTensorListRange` 旧二级指针布局回退路径。

fixture 同时包含普通 Tensor 与 TensorList 时，脚本还检查描述符消费顺序与参数
顺序一致，防止后续生成逻辑错配 cursor。

#### 静态 shape + OOM

生成源码必须继续使用编译期固定长度的 `OOMCheckAddrRange`，且不得出现
storage-shape header 探测或描述符注册调用。

#### 非 OOM

生成源码不得包含 OOM storage-shape 路径；单算子 JSON 不得包含 `oom` 能力
字段。这验证普通编译场景的产物没有变化。

#### OOM JSON 与参数大小

OOM 单算子 JSON 必须包含：

```json
"oom": {
  "version": 1,
  "tensor_version": 1
}
```

对同一个 tiling 与参数 fixture，脚本检查 OOM `opParaSize` 等于旧 OOM 固定
布局大小再加 2 字节扩展头。非 OOM 大小仍遵循原有计算公式。

### 4. 输出与退出码

每类契约使用具名检查。成功输出示例：

```text
PASS dynamic tensor keeps legacy fallback
PASS dynamic tensor emits storage-shape registration
PASS dynamic TensorList keeps legacy fallback
PASS dynamic TensorList emits storage-shape registration
PASS static shape keeps legacy OOM generation
PASS non-OOM artifacts remain unchanged
PASS OOM JSON capability and opParaSize
All OOM compile artifact checks passed.
```

失败时输出 `FAIL <check>: <actual mismatch>`，汇总失败数并返回 1；参数错误或
模块加载失败返回 2。

### 5. 文档与使用方式

中英文 README 说明该样例验证的是编译产物 ABI，不是设备运行时行为，并给出：

```bash
python3 check_oom_artifacts.py
```

文档列出五类验证场景、成功输出、依赖条件和常见失败原因。由于脚本直接使用
devkit 仓内模块，默认不依赖 `CANN_PATH`；只有仓内工具模块自身要求额外 Python
依赖时才报告实际缺失项。

### 6. 测试策略

实现采用测试先行：先建立样例契约测试，使其因样例尚不存在而失败；再添加最小
样例实现使测试通过。完成后执行：

1. 样例自身检查脚本；
2. 针对样例输出和退出码的自动化测试；
3. 最近提交已有的 OOM Python 聚焦测试；
4. `python3 -m py_compile` 检查新增 Python 文件；
5. `git diff --check` 检查格式问题。

设备侧 OOM UT 已覆盖描述符解析，本样例不重复模拟 GM 注册行为；它专注于从
编译器输入到生成源码和 JSON 的契约边界。
