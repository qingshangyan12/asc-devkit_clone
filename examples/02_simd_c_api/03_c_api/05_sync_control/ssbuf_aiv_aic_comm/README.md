# SSBuffer AIV/AIC通信C API样例

## 概述

本样例参考DAV3510 KFC的消息状态设计，但不包含或调用内部KFC接口。Device代码使用Ascend C C API的`asc_init`、`asc_sync_intra_arrive`、`asc_sync_intra_wait`和`asc_get_sub_block_id`，在一个`__mix__(1, 2)`任务中实现两条AIV到AIC的SSBuffer环形消息队列。

## 支持范围

| 产品 | NPU架构 |
| --- | --- |
| Ascend 950PR/Ascend 950DT | dav-3510 |

仅支持NPU和仿真运行模式。该样例独占SSBuffer前2KB，不能和内部KFC路径、Matmul高阶API或其他SSBuffer协议占用重叠空间。公开的KFC资源分组接口使用GM上的`KfcWorkspace`，不提供SSBuffer基地址。

## 实现说明

- 每个AIV使用一条深度为8的环形消息队列，每条消息128B；每个AIV发送12条消息，以验证队列回绕。
- AIC先清消息头，再通过模式4核间同步分别通知AIV0和AIV1开始发送。
- AIV写完消息后发送消息准备完成通知；AIC消费后清消息头并返回ACK通知。第9条消息开始，AIV必须收到ACK后才能复用队列开头的消息空间。
- 启动、消息准备完成和ACK使用不同的`sync_id`。AIC侧ID 0～15映射到AIV0，16～31映射到AIV1。
- 跨核共享字段使用`volatile`，样例不使用自定义编译器屏障。`asc_sync_intra_arrive`和`asc_sync_intra_wait`属于核间同步；`asc_sync_pipe`和`asc_sync_data_barrier`属于核内同步，不能代替这段SSBuffer消息交接。
- C API当前没有与`GetSsbufBaseAddr`同名的接口。DAV3510 NPU中`__ssbuf__`地址空间的零地址就是SSBuffer基址，因此样例以`reinterpret_cast<__ssbuf__ SsbufWorkspace*>(0)`取得基址。这是语言地址空间语义，不是KFC接口。
- 当前编译器不支持直接声明带`__ssbuf__`限定的数组。样例把二维消息队列声明为`SsbufWorkspace`结构体的成员，再通过`__ssbuf__ SsbufWorkspace*`访问。
- Host代码启动一个Mix block，读取AIC写入GM的24条结果并逐条校验。

队列回绕是指消息序号超过队列深度后，新的消息重新使用队列开头的存储位置。当前深度为8，序号0～7使用位置0～7，序号8～11再次使用位置0～3。回绕前等待ACK，保证旧消息已经处理完成，因此不会覆盖尚未消费的数据。

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
