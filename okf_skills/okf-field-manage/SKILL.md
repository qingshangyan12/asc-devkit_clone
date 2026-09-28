---
name: okf-field-manage
description: 管理 OKF YAML 字段的注册、生成器、校验器和 Profile 一致性。新增、修改、废弃字段或检查字段能力闭包时使用。
---

# OKF 字段管理

本 skill 管理字段的完整生命周期。字段不能只修改注册表；必须同时具备注册、生成、校验和测试能力。

## 操作

当前脚本提供只读检查：

```bash
python3 scripts/field_manager.py check
```

新增、修改和废弃字段先按本文件的清单生成变更计划，完成实现和测试后再写入注册表或代码；自动修改命令将在变更计划稳定后补充。

## 强制检查

字段检查必须确认：

- `common_fields`、`common_required_fields`、`field_order`、`field_catalog` 互相一致；
- 每个 Profile 引用的字段已注册；
- 每个注册字段都有代码生成能力和校验能力；
- 选中文档可以分类到已注册的 `okftype`；
- 生成器不会产生注册表之外的字段。

缺少生成器或校验器时，`okf-yaml-generate prepare` 必须中止，不得创建可执行批次或写入文档。

## 新增字段要求

新增字段需要同步提交：字段语义、Profile 归属、必填性、输出顺序、生成器、校验器、证据来源和测试。
不引入独立的 `field_types` YAML；字段类型规则由现有校验器实现，字段能力由代码能力映射声明。
