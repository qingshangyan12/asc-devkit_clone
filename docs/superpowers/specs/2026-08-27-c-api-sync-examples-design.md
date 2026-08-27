# C API同步接口可执行示例设计

## 需求

参考`asc_sync_inter_arrive.md`的调用示例，为`docs/zh/api/SIMD-API/c_api/sync`目录中缺少完整直接可执行示例的接口补齐示例，并在`dav-3510`NPU环境中逐个编译运行通过。新增中文与英文之间不添加空格。

## 范围

补齐以下11个接口文档：

- `asc_lock.md`、`asc_unlock.md`
- `asc_sync_block_wait.md`、`asc_sync_inter_wait.md`、`asc_sync_intra_wait.md`、`asc_sync_subblock_wait.md`
- `asc_sync_data_barrier.md`
- `asc_sync_mte2.md`、`asc_sync_mte3.md`、`asc_sync_vec.md`、`asc_sync_wait.md`

`asc_lock.md`现有多重循环和双buffer示例过于复杂，将整体替换为单轮矢量加法示例。

## 示例结构

每个接口页的调用示例包含：

1. 示例功能说明。
2. 将代码保存为`example.asc`的说明。
3. `dav-3510`编译运行命令。
4. 完整头文件、Kernel、ACL Host调用和结果校验。
5. 成功返回0，失败返回1，并输出唯一的成功或失败信息。

配对接口的示例同时调用两端接口，但文案、关键注释和结果输出突出当前文档接口。`asc_lock`和`asc_unlock`使用相同的简洁同步流程；4个`wait`接口复用对应`arrive`页已验证的同步场景，并调整当前接口的说明和输出。

## 验证

使用`/usr/local/Ascend/cann`指向的CANN9.2环境，在`/dev/davinci0`对应的Ascend950设备上执行：

1. 扫描11个目标页，确认修改前缺少`main`或`bisheng`命令，形成失败基线。
2. 从每个目标页的调用示例中提取第一个`cpp`代码块为独立`example.asc`。
3. 使用`bisheng example.asc -o main --npu-arch=dav-3510`编译。
4. 在NPU上运行并要求进程返回0且输出成功标志。
5. 对sync目录全部18个接口页执行相同的结构检查，确保每页均有完整直接可执行示例。
6. 执行Markdown结构、代码块、术语、中英文空格和Git差异检查。

## 不在范围内

- 不修改接口声明、实现或产品支持范围。
- 不修改概述类文档。
- 不新增外置examples工程。
- 不保留`asc_lock.md`现有复杂流水线优化示例。
