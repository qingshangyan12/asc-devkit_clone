# SSBuffer AIV/AIC通信样例

## 概述

本样例参考DAV3510 KFC的消息队列机制，但不包含或调用内部KFC头文件/API。样例在一个`__mix__(1, 2)`任务中，通过SSBuffer建立两条AIV到AIC的单向环形队列，并在host侧校验AIC收到的全部消息。

## 支持范围

| 产品 | NPU架构 |
| --- | --- |
| Ascend 950PR/Ascend 950DT | dav-3510 |

仅支持NPU和仿真运行模式。SSBuffer是任务内共享资源，不能把本协议与KFC、Matmul高阶API或其他占用SSBuffer的实现同时使用。

## 通信协议

- SSBuffer前2KB被划分为AIV0/AIV1各8个128B消息槽，槽大小和队列深度参考KFC实现。
- AIC首先清除全部消息头，再通过`CrossCoreSetFlag<2>`通知两个AIV开始运行，避免SSBuffer初始脏数据被误判为有效消息。
- AIV先写`sequence/value/checksum`，最后设置消息头的valid位；AIC轮询valid位后读取并校验消息。
- AIC消费完成后清除消息头。AIV把消息头恢复为0视为ACK，槽位随后可以复用。
- 每个AIV发送12条消息，超过8槽深度，能够覆盖队列回绕和发送端背压。

本样例显式调用`GetSsbufBaseAddr`并读写`__ssbuf__`地址，不需要设置`ENABLE_CV_COMM_VIA_SSBUF`。该宏用于选择框架内置路径，不是访问SSBuffer地址空间的前提。

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
