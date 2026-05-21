# CANN 9.0.0 测试记录

> 测试时间: 2026-05-21
> 命令: `source /home/jinhu/Ascend/cann-9.0.0/set_env.sh`

## 编译测试

| 架构 | 结果 | 原因 |
|------|------|------|
| dav-3510 | ❌ cmake 失败 | CANN 9.0.0 的 cmake FindASC 找不到 `.asc` 文件扩展名，不支持 ASC 语言 |
| dav-2201 | ❌ 编译失败 | typed 2D 重载 (如 `asc_copy_gm2l1_sync` 的 m_start/k_start 版本) 不在 2201 段中；2201 使用不同的 API 签名 |

## 结论

CANN 9.0.0 **不支持**当前测试套件。当前唯一可用的版本是 CANN 9.0.0-beta.2（需 struct 规避类型缺失，10/11 PASS）。

测试套件在设计上是面向 CANN 9.0.0-beta.2 + dav-3510 的，与 CANN 9.0.0 的 C API 接口不兼容（API 签名、cmake 支持均不同）。
