# ELU 算子设计文档

## 概述

ELU（Exponential Linear Unit）激活函数，数学公式：
```
ELU(x) = scale * x,                     if x > 0
ELU(x) = alpha * scale * (exp(x * inputScale) - 1),  if x <= 0
```

## 实现方案

### 编程范式
C API 寄存器指令（asc_* 系列 API），Vector 核心 SIMD 向量化计算。

### 计算流程
1. **数据搬运**：`asc_copy_gm2ub_align` 将输入从 GM → UB
2. **向量计算**：`elu_vf` / `elu_vf_fp16` 在寄存器中完成 ELU 运算
   - `asc_mul_scalar` 计算正半轴
   - `asc_exp` + `asc_mul_scalar` + `asc_add_scalar` 计算负半轴
   - `asc_gt_scalar` + `asc_select` 选择输出
3. **数据搬出**：`asc_copy_ub2gm_align` 将结果从 UB → GM

### 精度
- fp32：精度阈值 1e-4
- fp16：精度阈值 1e-2

## 目录结构
```
elu/
├── op_kernel/elu.asc    — 核函数实现
├── scripts/gen_data.py   — 数据生成脚本
├── screenshots/          — 测试截图
├── CMakeLists.txt        — 编译文件
├── README.md             — 样例介绍
├── design.md             — 本文档
├── 测试报告.txt           — 自动测试报告
└── 自测报告.md            — 自验证报告
```

## 支持的产品

| 产品 | CANN 软件版本 |
|------|--------------|
| Ascend 950PR / Ascend 950DT | >= CANN 9.1.0 |