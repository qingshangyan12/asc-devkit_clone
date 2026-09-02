# SSBuffer边界场景样例

## 概述

本样例在一个`__mix__(1, 2)`任务中验证SSBuffer的安全边界用法，不调用KFC或Matmul高阶API。AIV0写入偏移0处的首个32B记录，AIV1写入偏移3040处的最后一个32B记录；AIC消费并写回GM供host校验。

## 覆盖场景

- 3KB共享SSBuffer中偏移0的32B对齐访问。
- 3KB共享SSBuffer中偏移3040的32B对齐访问；该记录结束于偏移3072，不越界。
- AIV0/AIV1使用不重叠的首尾32B分区，验证AIC可分别观察两个发布者。
- host连续启动16次同一kernel，验证AIC每轮显式清理ready状态后可复用相同位置。

样例不会执行越界、非32B对齐或不同任务同时访问SSBuffer的操作。这些场景违反接口约束，不能作为正常功能验证。

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
