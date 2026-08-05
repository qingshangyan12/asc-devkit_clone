# SIMD VF Assert递进测试

## 概述

本目录将SIMD VF `printf`与`assert`组合成16个独立用例。每个用例单独编译和运行，因此一个编译器错误或Vector Core异常不会遮蔽其他用例。宿主端自动判断流同步结果，Python验证器自动检查打印值、断言信息和预期现象。

## 用例

| Case | 覆盖内容 | 预期结果 |
| --- | --- | --- |
| 0 | 单站点通过，assert前后打印 | 正常完成 |
| 1 | 单站点普通assert失败 | Vector Core异常 |
| 2 | 多条格式化打印后，带消息assert失败 | Vector Core异常 |
| 3 | 同一SIMD VF内两个assert站点，第一处通过、第二处失败 | Vector Core异常 |
| 4 | 多个SIMD VF与AICore打印交错，最后一个VF失败 | Vector Core异常 |
| 5 | 单个SIMD VF先输出超过2KB的数据，再触发assert | 完整打印后Vector Core异常 |
| 6 | SIMD callee分层调用，深层检查失败 | Vector Core异常 |
| 7 | `ascendc_assert(expr)`普通形式通过 | 正常完成 |
| 8 | `ascendc_assert(expr)`普通形式失败，校验失败后代码不执行 | Vector Core异常 |
| 9 | `ascendc_assert`格式化失败，混合有符号、无符号、十六进制、浮点和字符串 | Vector Core异常 |
| 10 | 同一个SIMD VF函数重复调用，第一次通过、第二次失败 | Vector Core异常 |
| 11 | 循环中的同一断言站点多次通过后失败 | Vector Core异常 |
| 12 | 多个SIMD VF中的断言全部通过，AICore打印交错 | 正常完成 |
| 13 | `ASCENDC_DUMP=0`时失败条件和打印均关闭 | 正常完成且无设备调测输出 |
| 14 | `NDEBUG`时断言关闭但普通打印保留 | 正常完成且保留设备打印 |
| 15 | `assert(expr, fmt, ...)`格式化形式通过 | 正常完成并执行断言后打印 |

所有用例都在`asc_assert.h`之前显式包含`<cassert>`，同时覆盖文档要求的头文件包含顺序。case 3、4、6、12覆盖同一个Kernel中的多个静态断言站点；这些用例也用于捕获当前BiSheng多断言编译问题。

## 覆盖边界

本测试覆盖SIMD VF用户常见的基本组合：`assert`与`ascendc_assert`两个宏别名、普通与格式化形式、条件通过与失败、失败后终止、单站点与多站点、重复VF调用、多个VF、SIMD callee、循环调用、打印交错、大于2KB的打印、混合格式参数、`ASCENDC_DUMP=0`、`NDEBUG`以及头文件包含顺序。

CPU调试域、SIMT编程模型、普通AICore中的assert和人为制造host应答超时不属于本SIMD VF样例的基本组合，需要由各自的专项测试覆盖。

## 运行

环境中已配置`CANN_PATH`、`ASCEND_HOME_PATH`或`ASCEND_TOOLKIT_HOME`时，直接执行：

```bash
./run.sh
```

脚本也会自动识别`/usr/local/Ascend/cann`。它会尝试全部16个用例，最后统一返回成功或失败，不需要人工选择或确认。

## 日志

每次运行会重新生成`logs/`：

- `configure.log`：CMake配置日志。
- `case_N_build.log`：各用例编译日志。
- `case_N_run.log`：各用例完整设备与宿主端日志，供人工复核。
- `case_N_verify.log`：各用例自动校验结果。
- `summary.log`：全部用例汇总。
