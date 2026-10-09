---
name: tensor-api-div-3510
description: 接续、实现、评审或验证本项目 __NPU_ARCH__ == 3510 的实验性 Tensor API 寄存器向量高精度除法；当任务涉及 div_algorithm、div_precise、FTZ true/false、0/1 ULP、subnormal 软仿、Reg::Div 适配及其测试时使用。
---

# Tensor API 向量除法（3510）

本 skill 面向仓内 `include/tensor_api/experimental/arch/vector` 的模式化除法工作。目标是提供与 3510 基础 API 除法模式一致的 Tensor API，而不是擅自宣称当前实现已经过硬件数值验证。

## 开始接续

1. 从仓库根目录开始，先读 [references/handoff.md](references/handoff.md)：它记录本次工作状态、相关路径、模式表及未完成的验证。
2. 运行 `git status --short` 并检查相关文件内容。此前修改**尚未提交**；若跨主机只带走了 skill 而没有同步工作区，先对照 handoff 找回或重建源码改动，不要假定代码已经存在。
3. 检查仓内适用的 `AGENTS.md` 和当前 SoC、编译器、`--cce-ftz` 条件。若可用，按仓内技能 `.agent/skills/asc-ar-dev/SKILL.md` 及 `.agent/skills/asc-api-ut-gen/SKILL.md` 的约束补充构建与测试。

## 代码定位和事实来源

- 基础 API 定义：`include/basic_api/reg_compute/kernel_reg_compute_utils.h` 中的 `DivAlgo` 与 `Reg::DivSpecificMode`。
- 3510 路由：`impl/basic_api/reg_compute/dav_3510/kernel_reg_compute_vec_binary_impl.h` 中的 `DivImpl`。分别追踪 `DivPrecisionImpl`、`DivIEEE754FloatImpl`、`DivIEEE754HalfImpl`；不要推断 FTZ 名字等价于运行时开关。
- Tensor API 声明/实现：`include/tensor_api/experimental/arch/vector/basic_arithmetic.h` 与 `impl/tensor_api/experimental/arch/vector/basic_arithmetic_impl.h`。
- 枚举：`include/tensor_api/experimental/utils/reg_enum.h`；测试：`tests/api/tensor_api/npu_arch_3510/aiv/test_tensor_api_basic_arithmetic.cpp`；文档：`docs/zh/api/SIMD-API/tensor_api/reg_vector_compute/basic_arithmetic/`。

## 继续完成的步骤

1. 先审查当前代码的模式映射与基础 API 原始 `DivImpl` 是否完全一致，特别是 `PrecisionMode=true`、`__ASC_FTZ__` 开关和 `half` 类型限制。
2. 审查 `Reg::RegTensor<T>::reg[0]` 到 `reg_tensor<T>::reg` 的适配能否同时通过 host stub、3510 device 编译，确认基础 API 头文件的包含顺序和对实验 Tensor API 的依赖影响。
3. 编译相关 3510 AIV 测试；新增真实数值/位级断言，覆盖普通值、接近舍入边界、subnormal、±0、Inf/NaN、mask 非全开及两个 FTZ 编译配置。现有测试主要检查类型与 mask，不能证明 0/1 ULP 精度。
4. 如必须避免基础 API 适配层而独立软仿，实现时对照原 `DivIEEE754FloatImpl` 和 `DivIEEE754HalfImpl` 全套规格化、指数恢复、sticky 舍入及异常分支；不可仅用差值补偿代替 FTZ_FALSE 路径。
5. 更新模式文档并做针对性差异检查。没有编译器或设备时如实标明未验证；不得把推测的精度当成测试结论。

## 边界

- 保留现有 `/` 的行为，除非用户明确要求更改；新 `div<...>` 与 `div_precise` 是独立入口。
- 不处理其他架构或无关 tensor/copy/mmad API。
- 不提交代码，除非用户明确要求。跨主机需要额外同步未提交改动（提交、补丁或完整工作树由用户决定）。
