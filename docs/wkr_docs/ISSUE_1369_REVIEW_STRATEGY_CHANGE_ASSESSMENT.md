# Issue1369评审策略消化与修改评估

## 文档目的

本文记录Issue1369后续评审提出的修改策略，并结合当前仓库源码评估推荐的落地方式、兼容性风险、头文件包含时机和验证范围。

- Issue：https://gitcode.com/cann/asc-devkit/issues/1369
- 评估仓库：`/data/wkr/asc-devkit`
- 评估基线：`4a436a3ea7d48b5814700cfa7715a61fdf8b6635`
- 评估日期：2026-08-18
- 本文范围：方案消化、修改评估及第一阶段公共宏归位实现。

## 评审原文

以下内容按收到的评审意见原样保留，不对命名、签名、大小写和表述做修正。

```text
几个修改参考策略：1.likely()/unlikely()接口优选编译器内置进built-in，如果内置不了，放到工具类函数utils/base/sys_macros.h中并对外开放，之前引用的文件引用”utils/base/sys_macros.h“
2.针对AscendC::Ceil(uint32_t,uint32_t) & AscendC::CeilDivision(int32_t,int32_t):提供统一的CeilDiv函数到utils/base/sys_math.h文件中, 并通知算子使用该接口定义：

template <typename T, typename V>

__callee__ inline void CeilDiv(T a, V b) {

    实现参考CUDA cmath内的实现，防止溢出问题（原始公式 (a + b - 1) / b 中，当 a 很大时 a + b - 1 可能溢出。转 unsigned 后溢出是良定义的（wrap around），不会 UB。）

    // 判断有无符号

    if constexpr (::cuda::std::is_signed_v<_Prom>) {

        return static_cast<_Common>((__a1 + __b1 - 1) / __b1);

    } else {

...

    }

}
参考cuda：
标准库: std::ceil(value);

CUDAC:

_CCCL_TEMPLATE(class _Tp, class _Up)

_CCCL_REQUIRES(::cuda::std::is_integral_v<_Tp> _CCCL_AND ::cuda::std::is_integral_v<_Up>)

[[nodiscard]] _CCCL_API constexpr ::cuda::std::common_type_t<_Tp, _Up> ceil_div(const _Tp __a, const _Up __b) noexcept

cccl/libcudacxx/include/cuda/__cmath/ceil_div.h at main · NVIDIA/cccl · GitHub

// 公共的头文件,

namespace AscendC {

     template <typename T, typename V>

     __host__ __callee__ inline void ceil_div(T a, V b);

}

编译器，正常规划函数， 对impl内部函数的保护。
3.REGISTER_TILING_DEFAULT、KERNEL_TASK_TYPE_DEFAULT、PRELOAD作为当前在用的接口能力，对外开放，统一放到"utils/base/sys_macros.h"对外开放
以上是评审的修改策略，主要是把这几个宏从impl头文件移到include下。中间要注意关注下宏位置变化的影响，以及头文件包含关系，会不会使得这个宏被定义的时机过迟。
先把以上消化消化后连带原文存到md文档里，再评估怎么修改
```

## 消化后的目标

评审策略可以归纳为以下三项目标：

1. 按接口归属迁移宏：跨模块使用的`likely`、`unlikely`进入`include/utils/base/sys_macros.h`；仅属于Basic API的REGISTER、TilingKey、Kernel Task Type和`PRELOAD`进入`include/basic_api/kernel_utils_macros.h`。
2. 将整数上取整除法收敛为一个公共`CeilDiv`接口，放入新公共头`include/utils/base/sys_math.h`，解决现有实现重复及大数溢出问题。
3. `likely`、`unlikely`优先由编译器提供built-in能力；编译器暂时无法提供时，在公共`sys_macros.h`中保留兼容定义。

这不是简单的文件移动。宏依赖内部支撑宏，整数除法已有新旧多套公共或内部实现；需要同时处理公共头自包含性、API语义、旧调用兼容和安装发布路径。

## 当前仓库现状

### 宏定义及依赖

**表 1**  宏定义及依赖

| 接口 | 当前唯一定义 | 主要依赖或条件 | 评审目标 |
| --- | --- | --- | --- |
| `likely`、`unlikely` | `impl/basic_api/utils/kernel_utils_macros.h`第29至34行 | `__builtin_expect`；已有`#ifndef`防重定义 | 编译器built-in优先，否则移至公共`sys_macros.h` |
| `KERNEL_TASK_TYPE_DEFAULT` | `impl/basic_api/utils/kernel_utils_macros.h`第374至383行 | `ENABLE_FEATURE_FOR_COMPILE`、`__NPU_ARCH__`、插件保护宏 | 连同依赖移至Basic API公共宏头 |
| `REGISTER_TILING_DEFAULT` | `impl/basic_api/utils/kernel_utils_macros.h`第385行 | `ENABLE_FEATURE_FOR_TILING`、`TILING_STRUCT_SECTION_INIT`、`__COUNTER__` | 连同依赖移至Basic API公共宏头 |
| `PRELOAD` | `impl/basic_api/utils/kernel_utils_ceil_oom_que.h`第25至37行 | `ASCENDC_CPU_DEBUG`、`uint64_t`、汇编和`preload` built-in能力 | 移至Basic API公共宏头 |
| `TILING_KEY_IS` | `impl/basic_api/utils/kernel_utils_macros.h`第436行 | `TILING_KEY_VAR`和`g_tilingKey` | 连同TilingKey宏族移至Basic API公共宏头 |

迁移前，公开`include/utils/base/sys_macros.h`仅包含一行有效逻辑：

```cpp
#include "impl/utils/sys_macros.h"
```

因此它在迁移前是内部实现头的公开转包含入口，还没有承载上述宏。`impl/basic_api/kernel_macros.h`也直接包含`impl/utils/sys_macros.h`，尚未统一走公共路径。

### 整数上取整除法现状

当前至少存在以下相关实现。

**表 2**  整数上取整除法实现

| 接口 | 位置 | 现状 |
| --- | --- | --- |
| `AscendC::Ceil(uint32_t, uint32_t)` | `impl/basic_api/utils/kernel_utils_ceil_oom_que.h` | 使用`(a + b - 1) / b`，不检查除数为0 |
| `AscendC::CeilDivision(int32_t, int32_t)` | 同上 | 除数为0返回0；部分架构Vector分支调用`get_repeat_ceiling`，其他分支仍使用易溢出公式 |
| `AscendC::Std::ceil_div(T, U)` | `include/utils/std/cmath.h`声明，`impl/utils/std/cmath/ceil_division.h`实现 | 2026-08-10已进入当前基线；提交说明为“临时仅作为CeilDivision替换”；支持Host和AI Core，但通用分支仍使用易溢出公式 |
| `AscendC::CeilDiv(uint16_t, uint16_t)` | `impl/adv_api/detail/matmul/utils/batch_matmul_utils.h` | Matmul内部同名函数，使用易溢出公式 |
| 其他`Ceil`、`CeilDiv` | Matmul、Conv及tiling内部文件 | 存在多个局部实现，部分位于`AscendC`命名空间，需逐一排查重载和冲突 |

仅统计`include/`和`impl/`，`CeilDivision(`当前有1067处文本命中，分布在168个文件中。直接一次性改名会形成较大回归面，应先建立兼容层，再逐步迁移算子。

### 已有公开文档

- `CeilDivision`已有基础API文档，约束为输入正数，除数为0时返回0。
- `AscendC::Std::ceil_div`已有Utils API文档，声明支持整型模板、Host和AI Core，除数为0时返回0。
- `TILING_KEY_IS`和`REGISTER_TILING_DEFAULT`已有Kernel Tiling API文档；`KERNEL_TASK_TYPE_DEFAULT`已在Kernel类型文档和示例中作为接口使用。
- 原基线未找到`PRELOAD`同名公开API文档。2026-08-18复核CANN 9.2安装包后，确认多个AscendC算子实际调用该宏；C API已有独立的`asc_icache_preload`，Basic API已有语义对应的`ICachePreLoad`。因此`PRELOAD`定位为Basic API兼容宏，进入Basic API公共宏头，并在缓存控制分类补充正式文档。

## 对评审原文的技术校正

### `CeilDiv`不能返回`void`

原文中的两个原型均写成`void`，但函数体返回计算结果。公共原型至少应返回两个输入类型运算后的公共整型。建议先按现有工程能力写成`auto`并明确返回类型规则；若实现`common_type`，则返回`common_type_t<T, V>`。

命名也需要统一：评审目标使用`CeilDiv`，示例公共声明又写成`ceil_div`。当前仓库已经有`AscendC::Std::ceil_div`，因此建议：

- 新公共基础API固定为`AscendC::CeilDiv`；
- 已发布的`AscendC::Std::ceil_div`保留，改为转发到同一个核心实现；
- 不再新增第二个顶层小写`AscendC::ceil_div`。

原文提到的`std::ceil(value)`是对单个数值做向上取整，不是整数上取整除法，不能作为`CeilDiv`的直接对应接口。此次算法参考应以CCCL的`cuda::ceil_div`为准。

### Unsigned溢出有定义不等于结果正确

将运算转为unsigned可以避免signed overflow UB，但如果unsigned的`(a + b - 1)`发生回绕，商仍可能错误。例如最大unsigned值除以2，回绕后的分子会变小。

NVIDIA CCCL的实现并不是对所有类型都直接使用该公式。它先按除法表达式确定提升类型，再转换为对应unsigned类型：

- 提升类型为signed时，在`a >= 0`、`b > 0`的前提下使用unsigned中间值计算`(a + b - 1) / b`；
- 提升类型为unsigned时，采用商和余数判定，或采用等价的Device侧优化形式，避免分子加法溢出。

AscendC实现应复用这一思路，并以CCCL的前置条件为边界，而不能只复制其中一个分支。参考：[NVIDIA CCCL `ceil_div.h`](https://github.com/NVIDIA/cccl/blob/main/libcudacxx/include/cuda/__cmath/ceil_div.h)。

### 输入语义必须先固定

CCCL要求`a >= 0`且`b > 0`。当前`CeilDivision`和`Std::ceil_div`却把`b == 0`定义为返回0。因此新API必须明确选择：

1. 为保持现有AscendC兼容性，`b == 0`继续返回0，文档明确只支持非负被除数和正除数；这是推荐方案。
2. 改为断言或未定义输入；这更接近CCCL，但会改变现有公开行为，不宜与头文件搬迁一起完成。

负数场景不应在本次扩展。C++整数除法向0截断，通用signed ceiling division还需处理符号组合和最小负数除以-1等边界，与当前“只支持正数”的API契约不同。

### `__callee__`不能直接替换现有修饰符

当前仓库中`__callee__`主要出现在SIMT数学函数声明；现有Utils公共数学接口统一使用`__host__ __aicore__ inline constexpr`。`__callee__`是否适用于普通AscendC Kernel函数、能否同时满足Host编译和CPU Debug，需要编译器Owner确认。

在该结论明确前，建议新API沿用已验证的`__host__ __aicore__ inline constexpr`形式，不应仅依据CUDA原型改成`__host__ __callee__`。

### 当前类型工具能力不足以原样移植CCCL

仓库自有`AscendC::Std` type traits当前包含`is_integral`，但未找到`common_type`、`is_signed`和`make_unsigned`。落地前需要做一个编译可行性验证，确认使用系统`<type_traits>`、补齐自有traits，还是由编译器提供等价built-in。不能在公共头中直接引用`::cuda::std`。

## 宏移动的包含时机评估

### 当前包含链

```text
kernel_operator.h
├── kernel_tpipe.h
│   ├── impl/basic_api/kernel_macros.h
│   │   └── impl/utils/sys_macros.h
│   └── impl/basic_api/utils/kernel_utils_ceil_oom_que.h
│       └── impl/basic_api/utils/kernel_utils_macros.h
└── kernel_tensor.h
    ├── impl/basic_api/kernel_macros.h
    └── impl/basic_api/utils/kernel_utils_macros.h
```

评审要求调整后，核心链路应变为：

```text
kernel_operator.h / kernel_tensor.h / kernel_tpipe.h / 内部使用方
└── basic_api/kernel_utils_macros.h
    └── utils/base/sys_macros.h
        └── impl/utils/sys_macros.h
```

### “定义得早”本身不是问题

宏体中的其他宏一般在调用点才继续展开。因此，`KERNEL_TASK_TYPE_DEFAULT`即使先于`ENABLE_FEATURE_FOR_COMPILE`定义，只要用户真正调用前后者已定义，预处理仍可成功。

真正的风险是公共头契约：如果用户只包含`basic_api/kernel_utils_macros.h`就调用公开宏，那么它依赖的`ENABLE_FEATURE_FOR_COMPILE`、`ENABLE_FEATURE_FOR_TILING`和`TILING_STRUCT_SECTION_INIT`也必须已经可用。仅移动三个外层宏，而把支撑宏留在稍后才被包含的内部头，会得到一个不自包含的公共头。

### 推荐处理原则

1. `likely`、`unlikely`是Basic API和Adv API均使用的通用分支提示宏，迁移到`utils/base/sys_macros.h`并保留防重定义保护。
2. `PRELOAD`仅作为Basic API兼容能力使用，迁移到`basic_api/kernel_utils_macros.h`，保留CPU Debug空操作和Device实现。
3. `KERNEL_TASK_TYPE_DEFAULT`应连同它所需的最小支撑逻辑一起迁移，保留3003/3113的空实现和`__PLUGIN__KERNEL_TASK_TYPE_DEFAULT_DEFINED__`插件保护。
4. `REGISTER_TILING_DEFAULT`应连同`ENABLE_FEATURE_FOR_TILING`、section初始化和`__CHECK_FEATURE_AT_PRECOMPILE`两种行为一起迁移，保证Basic API公共宏头自包含。
5. 内部`kernel_macros.h`改为包含`utils/base/sys_macros.h`；内部`kernel_utils_macros.h`改为包含`basic_api/kernel_utils_macros.h`，禁止继续从内部头取得公共宏主定义。
6. 宏移动后检查所有同名定义，保证每个公开宏只有一个主定义；必要的兼容头只负责包含公共头，不保留复制实现。
7. 公共宏头可能比当前内部定义更早进入包含链，需要保留`__PLUGIN__KERNEL_TASK_TYPE_DEFAULT_DEFINED__`和`__PLUGIN__KERNEL_META_TYPE_ENUME_DEFINED__`保护，确保插件预定义优先。

### `TILING_KEY_IS`纳入本阶段

Issue1369原始范围明确包含`TILING_KEY_IS`，且该宏已有正式API文档。为完整修复Issue，将`TILING_KEY_IS`、`TILING_KEY_LIST`、`TILING_KEY_VAR`和所需支撑宏一并迁入Basic API公共宏头。

## 推荐修改方案

### 总体方案

推荐分三阶段完成，不进行一次性全仓替换。

#### 公共宏归位

- 在`include/utils/base/sys_macros.h`中公开`likely`、`unlikely`。
- 新增`include/basic_api/kernel_utils_macros.h`，公开REGISTER、TilingKey、Kernel Task Type和`PRELOAD`，并同步迁移完整依赖闭包。
- 将`impl/basic_api/kernel_macros.h`改为包含通用公共宏头，将`impl/basic_api/utils/kernel_utils_macros.h`改为包含Basic API公共宏头。
- 从两个`impl/basic_api/utils`头中删除已迁移宏的主定义；内部头继续通过公共头获得能力。
- `kernel_operator.h`显式包含Basic API公共宏头，同时保留`kernel_tensor.h`、`kernel_tpipe.h`现有传递可见性。
- 为新公共头增加安装兼容软链接，并为`PRELOAD`新增缓存控制API文档。

#### 建立唯一的`CeilDiv`核心实现

- 新增`include/utils/base/sys_math.h`，公开`AscendC::CeilDiv`。
- 约束模板参数为整型，明确返回类型、`a >= 0`、`b > 0`以及`b == 0`的兼容行为。
- 按CCCL的类型提升及signed/unsigned分支思路实现，补齐当前`(a + b - 1) / b`的溢出缺陷。
- 将现有`AscendC::Std::ceil_div`改为转发到同一个核心实现，避免两套公开算法继续分叉。
- 将旧`AscendC::Ceil(uint32_t, uint32_t)`、`AscendC::CeilDivision(int32_t, int32_t)`暂时保留为兼容转发，不在同一提交中直接删除。
- 排查`AscendC`命名空间内已有`CeilDiv(uint16_t, uint16_t)`等重载，统一为转发或移除局部实现，避免同名重载继续保留不同语义。

#### 迁移调用和编译器能力

- 通知并推动算子从`Ceil`、`CeilDivision`迁移到`AscendC::CeilDiv`。
- 迁移完成并经过版本周期后，再评估旧接口废弃或删除。
- 若编译器提供`likely`、`unlikely` built-in，则公共头切换为编译器能力；在支持的最低编译器版本覆盖前保留受保护的fallback。

### 不推荐的方案

不建议在一个修改中直接删除旧接口并全仓替换1067处`CeilDivision`。这样会把公共头边界、数学语义变化、算子迁移和多架构回归绑在一起，出现问题时难以区分是include时机、重载解析、返回类型还是计算语义导致。

## 预计修改文件

**表 3**  预计修改文件

| 文件 | 建议修改 |
| --- | --- |
| `include/utils/base/sys_macros.h` | 承载`likely`、`unlikely`通用公共宏 |
| `include/basic_api/kernel_utils_macros.h` | 新增Basic API公共宏及必要的最小支撑逻辑 |
| `impl/basic_api/kernel_macros.h` | 改为包含 `utils/base/sys_macros.h` |
| `impl/basic_api/utils/kernel_utils_macros.h` | 删除已迁移的公开宏主定义，保留非公开内部宏 |
| `impl/basic_api/utils/kernel_utils_ceil_oom_que.h` | 第一阶段删除`PRELOAD`主定义；第二阶段将旧除法接口改为兼容转发 |
| `include/kernel_operator.h` | 显式包含Basic API公共宏头 |
| `include/utils/base/sys_math.h` | 新增 `AscendC::CeilDiv` 公共接口及唯一核心实现入口 |
| `include/basic_api/kernel_tpipe.h`等公共入口 | 在兼容路径中尽早包含`sys_math.h`，保证旧接口转发和新接口可见性 |
| `include/utils/std/cmath.h`、`impl/utils/std/cmath/ceil_division.h` | 让 `AscendC::Std::ceil_div` 复用新核心实现，并修复溢出路径 |
| `impl/adv_api/detail/matmul/utils/batch_matmul_utils.h` 等 | 处理 `AscendC::CeilDiv` 同名局部实现和潜在重载冲突 |
| `impl/basic_api/CMakeLists.txt` | 第一阶段增加`asc/include/interface/kernel_utils_macros.h`兼容软链接；第二阶段增加`sys_math.h`兼容软链接 |
| `docs/zh/api/...` | 第一阶段更新宏头路径并新增`PRELOAD`文档；后续新增或更新`CeilDiv`及旧接口兼容说明 |
| `tests/api/...` | 增加公共头自包含、宏行为、类型/边界值和旧接口兼容测试 |

## 验证矩阵

### 公共头和宏

- 单独包含`utils/base/sys_macros.h`，验证`likely`、`unlikely`可用且不重复定义。
- 单独包含`basic_api/kernel_utils_macros.h`，验证REGISTER、TilingKey、Kernel Task Type和`PRELOAD`依赖完整。
- 通过`kernel_operator.h`、`kernel_tensor.h`、`kernel_tpipe.h`三种现有入口编译，确认调用方式不变。
- 覆盖`__CHECK_FEATURE_AT_PRECOMPILE`开启和关闭两条路径。
- 覆盖`__PLUGIN__KERNEL_TASK_TYPE_DEFAULT_DEFINED__`预定义场景，确认不发生重定义。
- 覆盖`__NPU_ARCH__=3003/3113`的空宏分支及其他架构的正常分支。
- 覆盖`ASCENDC_CPU_DEBUG=1`和NPU编译，确认`PRELOAD`分别为空操作和Device实现。
- 对预处理结果检查宏定义位置、定义次数和展开结果，重点确认没有“定义过迟”或支撑宏缺失。
- 执行安装包检查，确认`asc/include/basic_api/kernel_utils_macros.h`和`asc/include/interface/kernel_utils_macros.h`两个发布入口均有目标头。

### `CeilDiv`

- 类型：8/16/32/64位signed、unsigned，以及不同宽度的混合输入。
- 返回类型：对每组模板参数做编译期检查，确认与公开契约一致。
- 数值：`a=0`、`b=1`、`a<b`、整除、有余数、各类型最大值附近。
- 溢出回归：至少覆盖`UINT32_MAX / 2`、`UINT32_MAX / UINT32_MAX`、`INT32_MAX / 2`、64位对应边界。
- 非法输入：按最终契约验证`b=0`；signed输入验证负数被明确拒绝或不在支持范围。
- 编译环境：Host、CPU Debug、AI Core，覆盖使用`get_repeat_ceiling`和通用实现的架构分支。
- 兼容性：旧`Ceil`、`CeilDivision`、`Std::ceil_div`与新`CeilDiv`在合法输入上结果一致。
- 重载：包含Matmul等已有同名`CeilDiv`的公共入口，验证没有二义性和静默选择旧实现。

## 实施前需要确认的决策

第一阶段决策已经闭环：

- `TILING_KEY_IS`及相关TilingKey宏族纳入本次迁移。
- `PRELOAD`定位为Basic API兼容宏，不进入C API或全局`sys_macros.h`。
- REGISTER、TilingKey、Kernel Task Type和`PRELOAD`统一由`basic_api/kernel_utils_macros.h`公开。
- `likely`、`unlikely`在编译器能力覆盖所有支持版本前，由`utils/base/sys_macros.h`提供受保护的fallback。

第二阶段仍需确认：

1. 新公共API名称是否最终确定为`AscendC::CeilDiv`。建议确定该名称，并保留`AscendC::Std::ceil_div`为兼容转发。
2. `b == 0`是否继续返回0。建议保持现有公开行为，本次不引入破坏性语义变化。
3. 是否采用`__host__ __aicore__`，还是编译器已确认支持`__host__ __callee__`。当前证据支持前者。
4. 类型traits由系统`<type_traits>`、AscendC自有实现还是编译器built-in提供，需要先做最小编译验证。

## 评估结论

评审方向合理，核心是把已经作为接口使用的能力从内部实现边界提升到公共边界。新评审结论对原方案的修正是：公共API仍需遵守模块归属，不能因为接口形式是宏就全部放入全局`sys_macros.h`。

- 通用的`likely`、`unlikely`进入全局公共工具头；仅属于Basic API的宏进入Basic API公共宏头。
- 宏不能只移动外层定义，必须保证Basic API公共宏头自包含，并完整保留预编译、插件和架构分支行为。
- `CeilDiv`不能另起一套重复实现。当前主线已经有临时`AscendC::Std::ceil_div`，应以新`sys_math.h`中的单一核心实现统一它和旧`Ceil`、`CeilDivision`，再通过兼容转发分阶段迁移算子。

第一阶段的归属决策已经闭环，可按本方案完成实现和验证；第二、第三阶段继续按“数学核心、调用迁移”两个独立修改推进。
