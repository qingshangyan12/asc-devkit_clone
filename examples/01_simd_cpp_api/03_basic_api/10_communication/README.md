# 通信样例

本目录提供Ascend C通信相关样例，演示通过HCCL Resource API查询拓扑链路、创建通信Channel并完成跨rank数据搬运。

## 样例列表

| 目录名称 | 功能描述 |
| --- | --- |
| [hccl_link_select](./hccl_link_select) | 查询MESH/CLOS拓扑下的UB_MEM链路，注册内存、创建Channel并验证远端数据搬运 |
