# OKF 用户使用指南

写给算子开发者：写 Ascend C 算子要用到 C API 时，不用再翻目录，把需求说给 AI，帮你找到该看哪些接口文档。

> 本文件只讲"怎么用"。想了解这个系统是怎么建设和维护的，看 [README.md](README.md)。

## 1. 这是干什么用的

C API 文档有几百篇，凭记忆找接口很慢。OKF 把这些文档整理成一个能检索的库：你说清楚想干什么，它返回候选接口文档的路径和用途。

可以这样问：

- "把 GM 上的数据搬到 UB，用哪个接口？"
- "想把 half 转成 fp8，有哪些接口？"
- "asc_xxx 这个接口要搭配什么接口一起用？"
- "9.1.0 起可用的拷贝接口有哪些？"
- "round_mode 这个参数哪些接口在用？"

边界要清楚：

- 当前只覆盖 C API 文档（`docs/zh/api/SIMD-API/c_api/`），其他 API 层级暂时不在范围里；
- 只帮你找文档，不生成算子代码，也不替你写 kernel。

## 2. 如何快速使用

开始前确认三件事：

- 本地有 asc-devkit 仓库；
- 装好了 CANN（脚本能自动读到版本；读不到时也可手动指定）；
- 有 Python 3，不用额外装包。

### 方式一：让 AI 帮你查（推荐）

把你的需求直接说给 AI 助手。如果你的 AI 工具支持自动加载 skill，就把 `okf-index`、`okf-retrieve`
两个目录接到它的 skill 目录里（例如 opencode 的 `.opencode/skills/`）；接的时候建议用软链接、不要复制，
脚本要靠仓库目录结构找文档。没接 skill 也没关系，在对话里补一句：

```text
按 okf_skills/okf-retrieve/SKILL.md 的流程，帮我找能做非对齐拷贝的接口。
```

### 方式二：自己跑命令

两条命令都在仓库根目录执行：

```bash
# 第一步：确保索引可用。第一次会自动建索引；以后只做检查，
#         发现换了芯片架构或 CANN 版本，会自动重建。
python3 okf_skills/okf-index/scripts/ensure_index.py --json

# 第二步：查询。条件可以组合，下面这条是"搬入阶段 + 拷贝操作"：
python3 okf_skills/okf-index/scripts/query.py --stage in --operation copy
```

查询结果是 `文档路径 | API 名 | 命中原因`。再给几个常用例子：

```bash
# 类型转换：half 转 int4
python3 okf_skills/okf-index/scripts/query.py --convert half int4

# 按关键词找：非对齐、填充
python3 okf_skills/okf-index/scripts/query.py --kw 非对齐,填充

# 找 9.1.0 起可用的接口
python3 okf_skills/okf-index/scripts/query.py --version 9.1.0-
```

注意：索引只装与本机 CANN 版本、芯片架构都匹配的接口。查不到时先看
`ensure_index.py --json` 的输出是不是正常，别急着下"仓库不支持"的结论。

### 2.1 API 文档顶部的 yaml 头是什么

在用户侧拿到的 API 文档里，每篇最上方都有一段用 `---` 包起来的 YAML，长这样：

```yaml
---
okftype: reference.api
description: "对 Global Memory 中 address 指向的单个元素执行原子加操作，把旧值与输入标量之和写回该地址"
tag:
  - roles: [compute]
  - operations: [add]
  - traits: [atomic]
dependencies:
  - path: docs/zh/api/SIMD-API/c_api/cache_ctrl/asc_dcci.md
    kind: requires
version: 9.1.0-
---
```

这是文档的"档案卡"：**你不用写，也不用改**，它是随文档一起发的，检索系统靠它干活。
每段什么意思：

| 字段 | 说人话 |
| --- | --- |
| `okftype` | 这是哪类页面。`reference.api` 表示"某个接口的参考页" |
| `description` | 一句话说明这个接口是干什么的；搜关键词时也会匹配它 |
| `tag` | 三组标签：`roles` 是它在流程里的角色（如 compute 计算）、`operations` 是它能做的操作（如 add 加法）、`traits` 是稳定特性（如 atomic 原子操作） |
| `dependencies` | 用这个接口还要看哪些文档、是什么关系。`kind` 常见取值：`requires` 要配合它、`configures` 用它配置、`constrained_by` 受它约束、`related` 相关、`replaced_by` 被它取代 |
| `version` | 从哪个 CANN 版本开始能用。`9.1.0-` 表示 9.1.0 及以后都行；写成 `9.0.0-9.2.0` 就是只有这个区间能用 |

它有什么用：你问的"搬入还是计算""要配什么接口""哪个版本能用"，都是靠这些字段对上具体接口的。
没有它，就只能靠文档正文硬搜。

## 3. 示例展示

下面拿 `asc_add` 这个接口走一遍真实流程：先看它的三种头长什么样，再实际建一次索引、查一次。

### 3.1 三种头长什么样

一篇 API 的检索信息分散在三处，靠 `override_id`（重载编号）串起来。

**文档头**：写在 API 文档最上方，给的是语义信息。`docs/zh/api/SIMD-API/c_api/vector_compute/vector_arith/asc_add.md`：

```yaml
---
okftype: reference.api
description: "对两个源矢量执行逐元素加法并将结果写入目的矢量，支持前n个数据计算与高维切分计算两种原型，目的与源操作数起始地址需32字节对齐。"
tag:
  - roles: [compute]
  - operations: [add]
  - traits: [elementwise, high_dimensional]
dependencies:
  - path: docs/zh/api/SIMD-API/c_api/general_description_and_constraints.md
    kind: constrained_by
version: 9.0.0-
---
```

它交代了这个接口的"身份"：哪类页面、做什么、属于什么操作、受哪个通用约束页约束、从哪个版本开始能用。

**include 头**：写在头文件里每个函数声明上方，给的是结构信息。`include/c_api/vector_compute/compute/vector_arith.h`：

```cpp
/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_add__PUf32_PUf32_PUf32_u32
 * @archs 2201
 * @param [out] dst 目的操作数。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_add(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count);
```

它标出了：这个重载的编号、适用架构（2201，即 A3/A2）、每个参数的方向和含义。asc_add 一共 8 个这样的重载块，这里只截了 float 前n个计算这一个。

**样例头**：写在样例调用点上下，把样例和具体重载对上号。`examples/02_simd_c_api/00_introduction/01_add/c_api_async_add/c_api_add.asc`：

```cpp
//! [asc_add__PUf32_PUf32_PUf32_u32]
asc_add(z_local, x_local, y_local, block_length);
//! [asc_add__PUf32_PUf32_PUf32_u32]
```

上下两行 marker 里的编号，和 include 头里那个重载的 `@override_id` 完全相同，这样就能知道这个样例调用的到底是哪个重载。

### 3.2 建索引 + 查询

提问：**在 A3 上想把两个 float 向量逐元素相加，用哪个接口？**

第一步，确保索引就绪。asc_add 只支持 A3/A2，所以这里指定 `--arch A3`：

```bash
python3 okf_skills/okf-index/scripts/ensure_index.py --arch A3
```

真实输出：

```text
[ensure] ready surface=c_api cann=9.2.0 arch=A3(2201) docs=1 scope=vector_compute index=/mnt/workspace/asc-devkit/okf_skills/okf-index/index/c_api; rebuilt: index.json missing
```

读法：本机 CANN 版本 9.2.0、目标架构 A3（编号 2201）的索引已经准备好；`rebuilt: index.json missing` 说明这是第一次建，以后同样条件再跑只做检查、不会重复建。（这个示例仓库目前只给 `asc_add` 加了三种头，所以只有 1 篇；完整的用户侧包里是全量文档。）

第二步，把提问翻译成查询条件：相加 = `--operation add`，float = `--dtype float`，两个条件要同时满足：

```bash
python3 okf_skills/okf-index/scripts/query.py --operation add --dtype float
```

真实输出：

```text
docs/zh/api/SIMD-API/c_api/vector_compute/vector_arith/asc_add.md | asc_add | operation:add,dtype:float
  对两个源矢量执行逐元素加法并将结果写入目的矢量，支持前n个数据计算与高维切分计算两种原型，目的与源操作数起始地址需32字……
```

（描述这一行默认只显示前 60 个字，完整内容在文档里。）

怎么读结果：第一行是文档路径、接口名和命中原因，第二行是这个接口的一句话说明。顺着路径打开文档，再结合文档头的 `version`（9.0.0 起可用）和 `dependencies`（配套的通用约束页），就能判断它是不是你要的接口。

---

想了解 yaml 头每个字段的完整规则：[字段与推导规范.md](字段与推导规范.md)；
想了解检索流程和结果格式：[okf-retrieve/SKILL.md](okf-retrieve/SKILL.md)；
想了解全部查询条件：[okf-index/SKILL.md](okf-index/SKILL.md)。
