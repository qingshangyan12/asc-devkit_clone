# 贡献指南

本项目欢迎广大开发者体验并参与贡献，在参与社区贡献之前。请参见[cann-community](https://gitcode.com/cann/community)了解行为准则，进行CLA协议签署，了解源码仓的贡献流程。

开发者准备本地代码与提交PR时需要重点关注如下几点：

1. 提交PR时，请按照PR模板仔细填写本次PR的业务背景、目的、方案等信息。
2. 若您的修改不是简单的bug修复，而是涉及到新增特性、新增接口、新增配置参数或者修改代码流程等，请务必先通过Issue进行方案讨论，以避免您的代码被拒绝合入。若您不确定本次修改是否可被归为“简单的bug修复”，亦可通过提交Issue进行方案讨论。

## API开发指南

本项目支持三种类型的API开发，每类API都有专门的贡献指南：

### 1. 高阶API

高阶API基于单核对常见算法进行抽象和封装，实现了一些常用的计算算法，旨在提高编程开发效率。高阶API一般通过调用多种基础API实现。高阶API包数学计算、矩阵计算、激活函数等API。

- **适用场景**：常见算法的抽象和封装，提高算子开发效率
- **贡献指南**：[docs/asc_adv_api_contributing.md](./docs/asc_adv_api_contributing.md)

### 2. 基础API

基础API实现对硬件能力的抽象，开放芯片的能力，保证完备性和兼容性。

- **适用场景**：使用C++语言编程风格的场景
- **贡献指南**：[docs/asc_basic_api_contributing.md](./docs/asc_basic_api_contributing.md)

### 3. C API

C API提供纯C风格的接口，符合C语言算子开发习惯，开放芯片完备编程能力，支持数组分配内存，一般基于指针编程。提供与业界类似的编程体验。

- **适用场景**：使用C语言编程风格的场景
- **贡献指南**：[docs/asc_c_api_contributing.md](./docs/asc_c_api_contributing.md)

## 开发者贡献场景主要包括

- Bug修复

  如果您在本项目中发现了某些Bug，希望对其进行修复，欢迎您新建Issue进行反馈和跟踪处理。

  您可以按照[提交Issue/处理Issue任务](https://gitcode.com/cann/community#提交Issue处理Issue任务)指引新建 `Bug-Report|缺陷反馈` 类Issue对Bug进行描述，然后在评论框中输入“/assign”或“/assign @yourself”，将该Issue分配给您进行处理。
  
- 代码优化

  如果您对本项目中某些API实现有泛化性增强/性能优化思路，希望着手实现这些优化点，欢迎您对API进行优化贡献。

  您可以按照[提交Issue/处理Issue任务](https://gitcode.com/cann/community#提交Issue处理Issue任务)指引新建 `Requirement|需求建议` 类Issue对优化点进行说明，并提供您的设计方案，
  然后在评论框中输入“/assign”或“/assign @yourself”，将该Issue分配给您进行跟踪优化。

- 文档纠错

  如果您在本项目中发现某些文档描述错误，欢迎您新建Issue进行反馈和修复。

  您可以按照[提交Issue/处理Issue任务](https://gitcode.com/cann/community#提交Issue处理Issue任务)指引新建 `Documentation|文档反馈` 类Issue指出对应文档的问题，然后在评论框中输入“/assign”或“/assign @yourself”，将该Issue分配给您纠正对应文档描述。
  
- 帮助解决他人Issue

  如果社区中他人遇到的问题您有合适的解决方法，欢迎您在Issue中发表评论交流，帮助他人解决问题和痛点，共同优化易用性。

  如果对应Issue需要进行代码修改，您可以在Issue评论框中输入“/assign”或“/assign @yourself”，将该Issue分配给您，跟踪协助解决问题。

## 代码规范

本项目有完整的代码贡献规范体系，请在开发前阅读相关指南。完整索引见 [docs/contributing/README.md](./docs/contributing/README.md)。

| 文档 | 内容 |
|------|------|
| [code-style.md](./docs/contributing/code-style.md) | C++ 命名、include、header guard、namespace、宏、constexpr、格式化 |
| [api-design.md](./docs/contributing/api-design.md) | API 接口设计、模板参数、`__aicore__` 使用、Tiling 接口、弃用标注 |
| [directory-structure.md](./docs/contributing/directory-structure.md) | 目录结构、文件命名后缀、架构目录、新增 API 操作清单 |
| [examples.md](./docs/contributing/examples.md) | 示例目录结构、命名、文件组成、新增流程、npu-arch 编译选项 |
| [testing.md](./docs/contributing/testing.md) | 测试框架、文件命名、UT 编写模式、Header Checker |
| [documentation.md](./docs/contributing/documentation.md) | Doxygen 注释格式、参数标注、弃用标注模板 |
| [experimental.md](./docs/contributing/experimental.md) | Experimental 特性开发、标记方式、转正流程 |

## Experimental 特性

新开发的实验性特性请放置于 experimental 目录下（即 `include/tensor_api/` 和 `impl/tensor_api/`），并在代码中明确标注为 experimental。完整规范见 [experimental.md](./docs/contributing/experimental.md)。
