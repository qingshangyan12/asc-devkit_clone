# 与核函数（Kernel）直调工程的调用差异

本文对照aclnn工程化算子开发与核函数（Kernel）直调工程的调用方式、启动配置和框架职责，供开发者在两种方式之间迁移算子时参考。

本文中的核函数（Kernel）直调工程指Host代码通过`<<<>>>`调用核函数，未额外接入算子原型、Host Tiling回调和任务生成框架的工程。aclnn工程化算子开发以单算子API调用为主；涉及GE图执行或任务生成的内容单独说明。具体接口的产品支持范围和版本约束以对应API文档为准。

## 调用流程<a id="invocation-flow"></a>

- **aclnn工程化算子开发**：编写算子原型、Host Tiling和核函数（Kernel），编译部署算子包；应用程序调用第一段aclnn接口获取workspace大小和executor，申请workspace，再通过第二段接口传入workspace、executor和stream执行算子。框架负责衔接Tiling结果、核函数（Kernel）选择和启动参数。
- **核函数（Kernel）直调工程**：Host代码准备输入输出、切分参数和临时内存，选择核函数（Kernel）或模板实例，通过`<<<>>>`指定执行配置并传入核函数参数。

两种方式都可以在Host侧计算数据切分参数。TilingContext的创建、数据填充及结果消费由接入的框架流程决定，`<<<>>>`调用语法本身不提供这些流程。

## 核函数入口与分支选择<a id="kernel-entry-and-selection"></a>

| 对照项 | aclnn工程化算子开发 | 核函数（Kernel）直调工程 |
|---|---|---|
| Host调用入口 | 使用生成的两段式aclnn接口。 | 使用`kernel_name<<<...>>>(...)`。 |
| 核函数入口参数 | 按算子原型中的输入、输出顺序组织，随后追加workspace和tiling参数。 | Host调用与核函数声明保持一致，开发者自行组织参数；遵循编译器的参数类型约束。 |
| 默认Kernel Type | 在核函数入口中使用`KERNEL_TASK_TYPE_DEFAULT`设置。 | 使用`__vector__`、`__cube__`、`__mix__`等执行空间限定符；`KERNEL_TASK_TYPE_DEFAULT`也支持直调工程，具体约束见API说明。 |
| 按TilingKey设置Kernel Type | 使用`KERNEL_TASK_TYPE(key, value)`。 | 该接口仅支持自定义算子工程。直调侧分别配置所调用核函数的类型。 |
| TilingKey分支选择 | Host Tiling通过`SetTilingKey`选择分支，核函数侧使用`TILING_KEY_IS`配合编译工具组织分支。 | `TILING_KEY_IS`不支持直调工程。Host代码可自行选择核函数、模板实例，或通过普通参数控制计算分支。 |
| 模板实例选择 | 通过`ASCENDC_TPL_ARGS_DECL`、`ASCENDC_TPL_SEL`声明组合，Host Tiling使用`ASCENDC_TPL_SEL_PARAM`选择组合。 | Host调用时指定模板实参，例如`add_kernel<float><<<...>>>(...)`。 |
| 模板组合的Kernel Type | 在`ASCENDC_TPL_ARGS_SEL`中使用`ASCENDC_TPL_KERNEL_TYPE_SEL`配置。显式配置时，同一`ASCENDC_TPL_SEL`下每个组合均需设置。 | 按直调核函数的类型配置方式设置，Host调用不自动执行aclnn的模板组合选择流程。 |

## 启动配置<a id="launch-configuration"></a>

| 配置项 | aclnn工程化算子开发 | 核函数（Kernel）直调工程 |
|---|---|---|
| SIMD逻辑Block数量 | Host Tiling调用`SetSimdNumBlocks`。 | SIMD调用的`<<<numBlocks, dynUBufSize, stream>>>`中设置`numBlocks`。 |
| SIMT Grid维度 | Host Tiling调用`SetSimtGridDim`。 | SIMT调用的`blocks_per_grid`参数。 |
| SIMT Block内线程维度 | Host Tiling调用`SetSimtBlockDim`。 | SIMT调用的`threads_per_block`参数。 |
| 动态UB大小 | Host Tiling调用`SetDynUBufSize`。 | SIMD调用的`dynUBufSize`或SIMT调用的`dyn_ubuf_size`参数。 |
| 调度模式 | Host Tiling调用`SetScheduleMode`。 | 通过`__schedmode__(mode)`设置。 |

## 相关文档

- [核函数（Kernel）侧算子实现](../design_and_implementation/kernel_operator_implementation.md)
- [Host侧Tiling实现](../design_and_implementation/host_tiling_implementation.md)
- [多分支策略](../design_and_implementation/multi_branch_strategy.md)
- [算子工程编译与部署](../compilation_and_deployment/basic_process.md)
- [单算子API调用](../invocation/single_operator_api_call.md)
