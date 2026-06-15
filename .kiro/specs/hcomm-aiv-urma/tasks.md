# Implementation Plan: HcommImpl<COMM_PROTOCOL_UBC_CTP> AIV直驱URMA通信

## Overview

根据 requirements.md 和 design.md 中的内容，生成一份合并的、格式化的最终需求&设计文档（Markdown格式），输出到 `/home/yzz/asc-api/gen_doc/asc-devkit/docs/hcomm_aiv_urma_design_doc.md`。

## Tasks

- [x] 1. 生成最终合并文档
  - [x] 1.1 生成最终需求&设计文档
    - 在 `/home/yzz/asc-api/gen_doc/asc-devkit/docs/hcomm_aiv_urma_design_doc.md` 生成一份完整文档
    - 文档标题: `HcommImpl<COMM_PROTOCOL_UBC_CTP> AIV直驱URMA 需求&设计文档`
    - 文档必须包含以下6个章节（严格按此顺序），不要包含辅助函数描述，只描述对外接口：
    - **章节1: 需求及场景分析**
      - 从 requirements.md 的 Introduction 和典型场景表格中提取
      - 包含背景说明、5个典型场景表格、核心设计目标
    - **章节2: 接口描述**
      - 仅包含以下5个对外接口（不包含WriteWithNotifyNbi）：Init、WriteNbi、ReadNbi、Commit、Drain
      - 每个接口包含：函数签名、参数说明表格、功能描述、返回值说明
      - Init需要描述两个重载版本（raw pointer和LocalTensor）
      - 从 requirements.md 的 Requirement 1-5 和接口描述子节中提取
    - **章节3: 约束条件**
      - 从 requirements.md 的 Requirement 6-10 中提取
      - 包含：Buffer大小约束(≥512B)、地址对齐(32字节)、远端Buffer注册范围约束、SQ深度溢出保护、WQE默认配置约束
    - **章节4: 方案概述**
      - 从 requirements.md 的方案概述章节和 design.md 的 Architecture 章节中提取
      - 包含：「填充-提交-等待」三阶段模型说明
      - 包含：ASCII代码逻辑调用图（从requirements.md中的调用图复制）
      - 包含：关键数据流描述（WriteNbi流程、ReadNbi流程、Drain流程）
      - 注意：所有操作（包括Commit和Drain）都是数据面操作，不要出现"控制面"字样
    - **章节5: 可维可测**
      - 从 requirements.md 的可维可测章节中提取
      - 包含：错误提示场景一览表（6种错误场景+返回值+日志级别+日志内容）
      - 包含：正常流程日志表（KERNEL_INFO级别的9种场景）
      - 包含：WQE/CQE Dump能力说明
    - **章节6: 测试用例场景说明**
      - 从 requirements.md 的测试用例场景说明章节中提取
      - 包含：已有UT用例概览表（test_hcomm_urma_950.cpp中的7个用例）
      - 包含：按类别分组的测试场景描述（正向功能验证、异常/边界验证、接口兼容性验证、内部状态验证）
      - 注意：WriteWithNotifyNbi相关的用例在表中保留但标注"本文档不涉及该接口详细描述"
    - 参考文件:
      - `/home/yzz/asc-api/gen_doc/asc-devkit/.kiro/specs/hcomm-aiv-urma/requirements.md`
      - `/home/yzz/asc-api/gen_doc/asc-devkit/.kiro/specs/hcomm-aiv-urma/design.md`
      - `/home/yzz/asc-api/gen_doc/asc-devkit/impl/adv_api/detail/hcomm/impl/platform_v310/hcomm_aiv_urma.h`
      - `/home/yzz/asc-api/gen_doc/asc-devkit/impl/adv_api/detail/hcomm/impl/platform_v310/hcomm_aiv_urma_def.h`
      - `/home/yzz/asc-api/gen_doc/asc-devkit/tests/api/adv_api/hcomm/test_hcomm_urma_950.cpp`
    - _Requirements: 全部_

## Task Dependency Graph

```json
{
  "waves": [
    { "id": 0, "tasks": ["1.1"] }
  ]
}
```
