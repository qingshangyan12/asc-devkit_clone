# 跨目录引用追踪规则（强制）

本文件定义跨目录引用追踪的完整执行流程。阶段 1/2/3/5 必须在代码分析前执行此流程。

## 为什么需要追踪

CANN 算子代码中，一个 aclnn 接口可能调用多个底层算子目录的代码。跨目录引用有两种机制：

1. **`#include "..."` 头文件引用** — 通过相对路径引用外部共享模块（共享 kernel 模板、公共宏、基类、tiling 策略等）
2. **`OP_TYPE_REGISTER(OpName)` 算子注册调用** — aclnn 接口的实现文件中通过注册宏调用其他底层算子，这些算子有各自独立的算子目录，其中的代码（op_host/op_kernel/op_tiling 等）也包含关键的约束、校验和计算逻辑

若不追踪这些外部引用，会导致：
- 遗漏关键约束（dtype/format/shape 校验在被调用算子的代码中）
- 遗漏核心计算逻辑（kernel 实现在被调用算子或共享模板中）
- 遗漏芯片差异分支（SocVersion/NpuArch 判断在被调用算子或公共工具函数中）

## 执行流程（5 步，必须按序执行）

### 步骤 1：扫描 #include 指令

对算子目录下 `op_api/`、`op_host/`、`op_kernel/`、`op_tiling/` 中的**每一个** `.h`/`.hpp`/`.cpp` 文件，提取所有 `#include "..."` 指令（忽略 `#include <...>` 系统头文件）。

**扫描命令**（使用 Grep 工具）：
```
pattern: #include\s+"([^"]+)"
path: <算子目录>
glob: *.{h,hpp,cpp}
```

### 步骤 2：扫描 OP_TYPE_REGISTER 调用

在算子目录下所有 `.cpp` 文件中查找 `OP_TYPE_REGISTER(OpName)` 宏调用。

**扫描命令**（使用 Grep 工具）：
```
pattern: OP_TYPE_REGISTER\((\w+)\)
path: <算子目录>
glob: *.cpp
```

**典型示例**（抽象示例）：
```
aclnn_foo.cpp 中 #include "foo_dispatch.h" → 找到 foo_dispatch.h 及其对应的 foo_dispatch.cpp
foo_dispatch.cpp 中：
  OP_TYPE_REGISTER(BarOpV2);    → 对应算子目录 bar_op_v2（找到则纳入分析）
  OP_TYPE_REGISTER(BazOp);     → 无对应算子目录（跳过，在清单中标注）
```

### 步骤 3：定位外部引用的实际路径

#### 3a. #include 路径解析

对每条 `#include "path"` 指令，按以下规则解析实际文件路径：

1. **相对路径**：以包含该 `#include` 的文件所在目录为基准解析
   - 例：`op_kernel/foo.cpp` 中的 `#include "../common/bar.h"` → 解析为 `common/bar.h`（算子目录的同级或上级）
   - 例：`op_host/op_api/aclnn_convolution.cpp` 中的 `#include "../../../../matmul/common/op_host/op_api/matmul_util.h"` → 解析为 matmul 目录下的文件
2. **同目录引用**：`#include "utils.h"` → 同目录下的 `utils.h`（属于内部引用，不需追踪）
3. **子目录引用**：`#include "sub/impl.h"` → 当前目录下的子目录（通常属于内部引用）

**判断是否为外部引用**：
- 解析后的实际路径**超出**当前算子根目录 → **外部引用，必须追踪**
- 解析后的路径仍在算子目录内 → 内部引用，正常读取即可

**常见外部引用模式**（CANN 代码库高频出现）：

| 引用模式 | 典型路径 | 包含的关键内容 |
|---------|---------|--------------|
| 共享 kernel 模板 | `../common/`、`../../shared/`、`../xxx_common/` | 核心计算实现、模板函数 |
| 公共工具宏/函数 | `../../utils/`、`../common_utils/` | CHECK 宏、dtype 校验工具 |
| 基类头文件 | `../base/`、`../../base_op/` | 基类定义、虚函数接口 |
| 同级算子复用 | `../另一个算子名/` | 被复用的计算逻辑 |
| 共享 tiling | `../common_tiling/`、`../../tiling_utils/` | tiling 策略、workspace 计算 |
| 跨类别算子引用 | `../../../../matmul/common/...` | 跨算子类别的共享工具 |

#### 3b. OP_TYPE_REGISTER 算子目录定位

对每个 `OP_TYPE_REGISTER(OpName)` 调用，按以下规则定位对应的算子目录：

**命名转换规则**（PascalCase → snake_case）：
1. 在每个大写字母前插入 `_`（首字母除外）
2. 全部转为小写
3. 连续大写+数字视为整体（如 `2D` → `2d`、`V2` → `_v2`）

**转换示例**（抽象示例）：
| OP_TYPE_REGISTER 名称 | 转换后的目录名 |
|----------------------|--------------|
| `FooBarV2` | `foo_bar_v2` |
| `FooBaz3D` | `foo_baz3d` |
| `MyOpTransposeV3` | `my_op_transpose_v3` |
| `SimpleOp` | `simple_op` |

**定位方法**：
1. 将 OpName 按上述规则转为 snake_case 目录名
2. 在**当前算子的同级目录**下查找该目录（使用 Glob 或 ls）
3. 若同级目录找不到，在**上一级目录**下查找
4. **若找不到对应目录，跳过该算子，在清单中标注"无对应算子目录"**
5. 找到目录后，该目录下的 `op_host/`、`op_kernel/`、`op_tiling/` 等代码纳入分析范围

### 步骤 4：定位被引用函数并精准读取

外部文件通常规模庞大，读取整个文件会引入大量无关代码。正确做法是：**先从调用方代码找到实际引用的函数名，再在外部文件中定位并只读取那些函数的实现**。

#### 4a. #include 引入的外部文件

**第一步：读取头文件，提取导出的函数/类名**

先读取 `.h` 头文件（通常远小于对应的 `.cpp`），从函数声明和类定义中整理出该文件导出的所有符号名（函数名、类名、命名空间前缀等）。例如读取 `cube_util.h` 后得到导出函数列表：`CheckDtypeAndFormat`、`CalcWorkspaceSize`、`ValidateShape` 等。

**第二步：在调用方 .cpp 中 Grep 这些符号名，确认实际被调用的函数**

用提取到的函数名列表逐一 Grep 调用方 `.cpp`，命中的即为实际被调用的函数。Grep 模式示例：

```
pattern: CheckDtypeAndFormat\s*\(
path: <调用方.cpp>
```

若导出符号属于某个命名空间（如 `cube_util::`）或类（如 `CubeUtil::`），可直接 Grep 命名空间前缀一次性找到所有调用：

```
pattern: CubeUtil::|cube_util::
path: <调用方.cpp>
```

这样可以一次得到所有实际引用的函数名，而无需逐一搜索。

**第三步：查找同名实现文件**

若引入的是 `.h`/`.hpp` 头文件，在同目录下查找对应的 `.cpp` 实现文件：
- `foo.h` → 同目录下 `foo.cpp`
- `foo_utils.h` → 同目录下 `foo_utils.cpp`

**第四步：在外部文件中定位并只读取被引用函数的实现**

用 Grep 在 `.h` 和对应 `.cpp` 中搜索第一步得到的函数名，找到函数定义的位置（行号），然后用 Read 的 `offset`/`limit` 参数只读取该函数体（从函数签名到对应的闭合 `}`，通常 20–80 行）。**禁止整文件读取**——外部工具文件往往有数百乃至数千行，绝大多数与当前算子无关。

**第五步：在已读取的函数体内搜索芯片相关模式**

对读取到的函数体，逐一搜索：
- `AddConfig` 声明
- `SocVersion::`、`NpuArch::`、`GetCurNpuArch()` 分支
- `__NPU_ARCH__`、`__DAV_xxx__`、`IsRegBase` 编译期分支
- `CheckDtype`、`CheckFormat` 等校验调用

**第六步：递归追踪**

若被读取的函数体内还有 `#include "..."` 外部引用或调用了其他外部函数，按相同方式继续追踪（最大深度：3 层）。

#### 4b. OP_TYPE_REGISTER 对应的算子目录

**第一步：确定调用入口**

在调用方代码中找到触发该 `OP_TYPE_REGISTER` 的函数调用链，确认实际传入底层算子的参数和调用的接口名。

**第二步：列出目录结构**

对定位到的算子目录执行 Glob，列出 `op_host/`、`op_kernel/`、`op_tiling/`、`op_api/` 下的所有 `.h`/`.hpp`/`.cpp` 文件，识别入口文件（通常是与算子同名的 `.cpp`）。

**第三步：从入口文件出发，只读取实际执行路径上的函数**

- 读取 `op_host/*_def.cpp` 的 AddConfig/DataType 声明部分（通常在文件头部，可只读前 100–200 行）
- 从调用方传入的接口名出发，在 `op_kernel/` 和 `op_tiling/` 中 Grep 该接口名，找到入口函数，只读取入口函数体及其直接调用的关键子函数
- **不要把整个 op_kernel/ 目录的所有文件全部读取**——只沿调用链向下读取实际执行的函数

**第四步：重点关注函数体内的以下模式**

AddConfig 声明（芯片支持）、CheckDtype/CheckFormat（约束）、SocVersion/NpuArch 分支（芯片差异）、核心计算路径、shape 校验、Contiguous/ViewCopy 调用。

**第五步：对算子目录内代码同样执行 #include 外部引用追踪**（最大深度：2 层）。

**【常见错误】**：
- 读取整个工具文件（如 `cube_util.cpp` 有 2000 行），而非只读取被调用的函数
- 仅读取 `*_def.cpp` 而跳过 `op_kernel/` 和 `op_tiling/`
- 没有从调用方代码出发确认实际引用的函数名，导致在外部文件中漫无目的地搜索

**递归深度限制**：#include 最多追踪 3 层；OP_TYPE_REGISTER 引入的算子目录内的 #include 最多追踪 2 层。

### 步骤 5：将外部文件纳入分析范围

追踪到的所有外部文件**必须与算子目录内的文件同等对待**——在后续的芯片差异分析、参数提取、公式推导、文档生成中，外部文件中的代码逻辑必须被纳入分析。

## 代码分析清单格式要求

每个阶段报告末尾的 `## 代码分析清单` 必须包含追踪到的外部文件，并在"分析原因"列中注明引用链：

| 文件路径 | 分析原因 |
|---------|---------|
| /abs/path/算子目录/op_host/op_api/aclnn_foo.cpp | aclnn 接口主实现文件 |
| /abs/path/算子目录/op_host/op_api/foo_dispatch.cpp | aclnn 接口调用的算子调度文件 |
| /abs/path/common/shared_kernel.h | **由 op_kernel/main.cpp #include "../common/shared_kernel.h" 引入**，包含核心计算模板 |
| /abs/path/common/utils.h | **由 common/shared_kernel.h #include "utils.h" 引入**（第2层追踪），包含 dtype 校验宏 |
| /abs/path/bar_op_v2/op_host/xxx.cpp | **由 foo_dispatch.cpp OP_TYPE_REGISTER(BarOpV2) 引入**，包含 BarOpV2 算子的校验逻辑 |
| /abs/path/bar_op_v2/op_kernel/xxx.cpp | **由 OP_TYPE_REGISTER(BarOpV2) 引入**，包含 BarOpV2 算子的 kernel 实现 |
| BazOp | **OP_TYPE_REGISTER(BazOp)，无对应算子目录**，跳过 |

**强制要求**：
- #include 引入的外部文件，"分析原因"以加粗的"**由 xxx #include 引入**"开头，注明引入链路和层数
- OP_TYPE_REGISTER 引入的算子目录，"分析原因"以加粗的"**由 xxx OP_TYPE_REGISTER(OpName) 引入**"开头
- 找不到对应目录的 OP_TYPE_REGISTER，仍需记录并标注"无对应算子目录"

## 验证清单（阶段输出前必须逐条检查）

在写入阶段输出文件前，必须对照以下清单验证：

- [ ] 已扫描算子目录下所有 `.h`/`.hpp`/`.cpp` 文件的 `#include "..."` 指令
- [ ] 已扫描算子目录下所有 `.cpp` 文件的 `OP_TYPE_REGISTER(...)` 调用
- [ ] 已识别出所有指向算子目录外部的 #include 引用
- [ ] 已对每个 OP_TYPE_REGISTER 的算子名去除下划线后与同级目录名比对，尝试定位对应目录
- [ ] 已从调用方代码中提取实际引用的外部函数/类名，而非整文件读取
- [ ] 对每个外部 `.h`/`.hpp`，已查找同目录对应的 `.cpp`，并只读取被引用函数的实现（用 Grep 定位行号 + Read offset/limit 精准读取）
- [ ] 已在读取到的函数体内搜索 AddConfig、SocVersion、NpuArch、`__NPU_ARCH__`、CheckDtype/CheckFormat 等芯片相关和校验相关代码模式
- [ ] OP_TYPE_REGISTER 目录：从调用入口出发沿调用链读取，未整目录全量读取
- [ ] 已对外部文件递归追踪 `#include`（#include 最多 3 层，OP_TYPE_REGISTER 目录内最多 2 层）
- [ ] 代码分析清单中包含所有追踪到的外部文件和算子目录，并注明引用链
- [ ] 分析结论中已纳入外部文件和被调用算子目录中的关键逻辑（校验、计算、分支等）

若某条未通过，必须补充执行后再输出。
