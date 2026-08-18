# CCE vcvt与builtin验证结果

验证环境：DAV 3510，CANN 9.2.0，`bisheng`，`-O2`。

| 检查项 | `vcvt`路径 | `__builtin_cce_vcvtff_f322f16_x`路径 | 结果 |
| --- | --- | --- | --- |
| 编译 | `-DUSE_VCVT_BUILTIN=0` | `-DUSE_VCVT_BUILTIN=1` | 均成功 |
| DAV 3510实机运行 | 64个float元素 | 64个float元素 | 均在30秒内完成 |
| 精度 | 与Host IEEE 754 RN-even `f32 -> f16 -> f32` golden逐位比较 | 同左 | 均为64/64通过 |
| `.aicore_binary`大小 | 8816字节 | 8816字节 | 相同 |
| `.aicore_binary` SHA-256 | `71b89d66f833344c4d39b5822d7f81c93c4d5937f1422d75e54f3bffa7ef041a` | `71b89d66f833344c4d39b5822d7f81c93c4d5937f1422d75e54f3bffa7ef041a` | 相同 |
| `.aicore_binary`逐字节比较 | `cmp -s` | `cmp -s` | 相同 |

结论：在本样例的DAV 3510、`-O2`配置下，`vcvt`与直接调用该builtin生成完全相同的设备二进制，未观察到设备侧优化或数值行为差异。完整宿主可执行文件相差32字节，是两种构建分别打印`"vcvt"`和`"__builtin_cce_vcvtff_f322f16_x"`路径名称导致的Host字符串差异，不是NPU代码差异。

直接builtin属于编译器内建接口，缺少`vcvt`封装的架构分支和参数约束；生产代码仍应优先使用`vcvt`或C API封装。本样例仅用于验证当前工具链的降级路径和优化结果。
