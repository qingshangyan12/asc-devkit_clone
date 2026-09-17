# 调查记录（历史归档）

本目录是 \>\>\>\>算子 Exception Dump 外挂能力的调查验证原型（ifa_exception_dump_kit）的四份文档归档，
**反映的是调查时点的状态**，其中部分设计已在入仓时按需求评审结论调整：

- 原生三段式链路（`RegisterBySymbol` + 算子侧 symbol 导出补丁 + D8–D10/E1 用例）已从样例中**完整移除**，
  样例仅保留全局引导链路（首次异常回调内自动挂载 per-binary），算子零改动；
- 异常回调注册接口由已废弃的 `aclrtSetExceptionInfoCallback` 更换为
  `aclrtExceptionInfoCallbackRegister` / `aclrtExceptionInfoCallbackUnregister`（支持多回调共存）；
- 实测基线为 CANN 9.1.T103 / Ascend 910C（docker ops_dev_sw），入仓样例的实测基线为 CANN 9.2.0 / Ascend 910B4。

| 文件 | 内容 |
|------|------|
| ifa_exception_dump_kit_README.md | 原型执行手册（含当时的两条链路与 D1–D10 用例基线） |
| test_plan.md | 原型测试方案（含 G2/D8–D10/E1，入仓样例已删除对应项） |
| test_report.md | 原型实测结果报告（D1–D10 全 PASS 的证据链，含 md5 基线值） |
| incre_flash_attention_exception_dump_report.md | 技术主报告：机制原理、mangled 名匹配、args 布局校准、tiling 解析、五步接入法 |

现行样例见 `examples/01_simd_cpp_api/01_utilities/02_dump/python_model_exception_dump/`，
其 README 中的"新算子接入指南"承接了主报告的接入方法论。本目录是否随 PR 提交由评审决定。
