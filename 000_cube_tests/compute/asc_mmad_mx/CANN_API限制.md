# API 测试限制说明

## API: asc_mmad_mx
该 API 在 CANN 9.0.0-beta.2 (dav-3510) 下存在以下编译/链接限制：

- **原因**: CCE 编译器不支持 struct 替代类型创建地址空间数组 / impl 函数不支持目标特性 / 所需下游 API 缺失
- **状态**: 无法在当前 CANN 版本编译通过
- **建议**: 升级到 CANN 9.1.0-beta.1 或更新版本后重试
