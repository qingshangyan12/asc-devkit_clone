# 通信API样例介绍

## 概述

本目录提供Ascend C通信相关样例，演示通过HCCL Resource API查询通信链路、创建通信Channel并完成跨rank
数据搬运。

## 样例列表

| 目录名称 | 功能描述 | 支持的产品 |
| --- | --- | --- |
| [one_multi_path](./one_multi_path) | 查询UB_MEM链路，为每个peer创建one path/multi path Channel并验证远端数据搬运 | Ascend 950PR/Ascend 950DT |
