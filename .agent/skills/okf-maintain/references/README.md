# okf-maintain reference 导航

本目录保存存量头的漂移体检与修复流程文档。字段与写入规则不在本目录维护，
以建设侧 okf-build 的契约为准。

| 文档 | 内容边界 |
| --- | --- |
| [README.md](README.md) | 本 reference 导航 |
| [drift.md](drift.md) | 漂移分类与定级、状态账本格式、check/update/baseline 流程与语义修复路径 |

## 维护规则

- 漂移分类与报告口径只在 [drift.md](drift.md) 维护；脚本行为与其保持一致。
- 字段契约、证据规则、写入面引用 okf-build 文档，不复制。
- 新增漂移类别时同步 `scripts/maintain.py` 与 `tests/test_maintain.py`。
