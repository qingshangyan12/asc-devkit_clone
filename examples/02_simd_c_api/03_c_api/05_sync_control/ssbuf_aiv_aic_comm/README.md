# SSBuffer AIV/AIC通信C API样例

## 概述

本样例参考DAV3510 KFC的消息状态机，但不包含或调用内部KFC接口。Device代码使用Ascend C C API的`asc_init`、`asc_sync_block_arrive`、`asc_sync_block_wait`和`asc_get_sub_block_id`，在一个`__mix__(1, 2)`任务中实现两条AIV到AIC的SSBuffer环形队列。

## 支持范围

| 产品 | NPU架构 |
| --- | --- |
| Ascend 950PR/Ascend 950DT | dav-3510 |

仅支持NPU和仿真运行模式。该样例独占SSBuffer前2KB，不能和KFC、Matmul高阶API或其他SSBuffer协议同时使用。

## 实现说明

- 每个AIV使用一条8槽环形队列，每槽128B；每个AIV发送12条消息，以验证槽位回绕。
- AIC先清消息头，再通过mode 2 block同步一次放行同一mix block中的两个AIV。
- AIV先写消息体，最后写valid位；AIC消费后清valid位作为ACK。
- C API当前没有与`GetSsbufBaseAddr`同名的接口。DAV3510 NPU中`__ssbuf__`地址空间的零地址就是SSBuffer基址，因此样例以`reinterpret_cast<__ssbuf__ SsbufWorkspace*>(0)`取得基址。这是语言地址空间语义，不是KFC接口。
- host代码启动一个mix block，读取AIC写入GM的24条结果并逐条校验。

## 编译运行

```bash
source ${install_path}/cann/set_env.sh
mkdir -p build && cd build
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DCMAKE_ASC_RUN_MODE=npu ..
make -j
./demo
```

成功输出：

```text
test pass!
```
