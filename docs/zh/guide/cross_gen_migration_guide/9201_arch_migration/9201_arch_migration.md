# 9201架构迁移指导

本章介绍3510到9201的架构变更，以及相应的Ascend C算子迁移方法。后续章节使用`__NPU_ARCH__`的取值3510和9201区分两个架构。

- [3510到9201架构变更](./3510_to_9201_arch_changes.md)
- [3510迁移9201指导](./3510_to_9201_guide/3510_to_9201_guide.md)
  - [基础API迁移指导](./3510_to_9201_guide/basic_api_migration.md)
  - [Regbase API迁移指导](./3510_to_9201_guide/reg_api_migration.md)
  - [C API迁移指导](./3510_to_9201_guide/c_api_migration.md)
  - [高阶API及Tensor API迁移指导](./3510_to_9201_guide/advanced_api_migration.md)
  - [算子编译迁移指导](./3510_to_9201_guide/op_compilation_migration.md)

> [!NOTE]
> 本章覆盖3510到9201已识别的兼容性变更，不构成全部API的支持列表。硬件指令删除不等于同名API删除；接口也可能通过软件仿真实现兼容。软件仿真、无符号移位量重载及新版本接口的可用性，应以目标产品配套SDK的公开头文件和接口文档为准。本文中的API链接用于说明接口语义，其中仅描述3510的产品支持、容量或示例参数不能直接作为9201的配置依据。
