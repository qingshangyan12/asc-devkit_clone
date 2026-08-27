# sync_flag_id样例

## 概述

本样例验证核间同步接口统一使用`uint16_t flag_id`后的编译和运行行为。调用方使用普通`int64_t`局部变量传入超出`uint16_t`范围的值，每对`arrive/wait`使用完全相同的变量。不同模式使用不同的低位标识，避免硬件同步计数器相互影响：block传入65537并映射为1，subblock传入65538并映射为2，inter传入65544并映射为8；在`dav-3510`上，intra传入65539并映射为3。

样例启动2个AI Core。每种同步模式由独立进程执行，参与同步的AIC或AIV在同步完成后向GM写入完成标记；Host侧检查内核在限定时间内返回且至少存在一个完成标记。独立进程可以避免前一个模式的硬件同步计数器影响后一个模式，也便于超时后定位未配对的`flag_id`。

## 产品支持情况

| NPU架构 | 覆盖接口 | 当前验证状态 |
| --- | --- | --- |
| `dav-2201` | block、subblock、inter | 编译及NPU运行通过 |
| `dav-3510` | block、subblock、inter、intra | 已提供代码和执行脚本，待3510环境验证 |

## 目录结构

```text
├── CMakeLists.txt
├── README.md
├── run.sh
└── sync_flag_id.asc
```

## 编译运行

设置CANN安装路径后执行脚本：

```bash
export ASCEND_HOME_PATH=/path/to/cann
bash run.sh dav-2201
```

3510环境使用以下命令：

```bash
export ASCEND_HOME_PATH=/path/to/cann
bash run.sh dav-3510
```

脚本会临时将CANN包中的`asc/include/c_api`和`asc/impl/c_api`替换为指向当前仓库的软链接，并在退出时恢复原路径。脚本为每种同步模式启动独立进程，并使用`timeout`保护；默认超时时间为60秒，可通过`SYNC_TEST_TIMEOUT`调整。在非3510环境仅编译3510代码时，可设置`SYNC_BUILD_ONLY=1`。

## 合入标准及依据

合入标准如下：

- 2201架构编译并在NPU上运行通过，证明当前环境中的公开声明、架构实现和调用代码一致。
- UT中的函数指针类型断言通过，证明八个公开接口仅保留`void(pipe_t, uint16_t)`原型。
- NPU样例在限定时间内返回且写入完成标记，证明调用方传入`int64_t`越界值时，每对`arrive/wait`均映射到预期的低位硬件标识且同步等待已解除。
- 2201结果由当前环境验证；3510专属接口的最终运行结果由3510环境补充。

当前是否满足合入标准：**满足当前环境的合入标准**。2201的block、subblock、inter样例已分别使用`int64_t`越界值65537、65538、65544在NPU上运行通过；按照验证约定，3510编译和NPU运行不在当前2201环境执行，后续使用本目录脚本在3510环境补充结果。
