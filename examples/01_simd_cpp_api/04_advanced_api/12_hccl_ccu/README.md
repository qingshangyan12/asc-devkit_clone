# HCCL CCU通信样例

简体中文 | [English](./README_en.md)

## 概述

本样例集介绍如何使用MC2 builtin接口（`Mc2GetCcArgs`/`Mc2SetCc*`/`Mc2AcquireCcResCtx`/`Mc2CcKernelLaunch`）
准备CCU通信资源并启动CCU Server，再由AICore kernel通过HCCL高阶API（`Hccl::InitV2`/`Hccl::AllGather`等）
提交Client通信任务，实现集合通信。

样例会根据当前环境中的NPU数量创建通信域，每个Device对应一个rank。

## 样例列表

| 目录名称 | 功能描述 |
| ---- | ---- |
| [ccu_builtin/01_allgather](./ccu_builtin/01_allgather) | 通过MC2 builtin接口准备CCU资源，AICore侧提交AllGather通信任务 |
| [ccu_builtin/02_allgather_add](./ccu_builtin/02_allgather_add) | 在AllGather通信结果上执行Add计算的融合样例 |
| [ccu_builtin/03_allreduce](./ccu_builtin/03_allreduce) | 通过MC2 builtin接口准备CCU资源，AICore侧提交AllReduce(SUM)通信任务 |
| [ccu_builtin/04_check_res_errcode](./ccu_builtin/04_check_res_errcode) | 验证CheckOpResSufficient(ccArgs版)各错误码返回行为的用例集 |

## 支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
| ---- | ---- |
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |
