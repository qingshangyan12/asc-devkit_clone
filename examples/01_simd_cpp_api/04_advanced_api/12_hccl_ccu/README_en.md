# HCCL CCU Communication Examples

English | [简体中文](./README.md)

## Overview

These examples demonstrate how to use the MC2 builtin APIs (`Mc2GetCcArgs`/`Mc2SetCc*`/`Mc2AcquireCcResCtx`/
`Mc2CcKernelLaunch`) to prepare CCU communication resources and launch the CCU Server, and then submit
collective-communication client tasks from the AICore kernel through HCCL high-level APIs
(`Hccl::InitV2`/`Hccl::AllGather`, etc.).

Each example creates a communication domain based on the number of NPUs in the current environment,
with one rank per Device.

## Example List

| Directory | Description |
| ---- | ---- |
| [ccu_builtin/01_allgather](./ccu_builtin/01_allgather) | Prepare CCU resources via MC2 builtin APIs and submit an AllGather client task on the AICore side |
| [ccu_builtin/02_allgather_add](./ccu_builtin/02_allgather_add) | Fused example that performs Add computation on the AllGather result |
| [ccu_builtin/03_allreduce](./ccu_builtin/03_allreduce) | Prepare CCU resources via MC2 builtin APIs and submit an AllReduce (SUM) client task on the AICore side |

## Supported Products and CANN Versions

| Product | CANN Version |
| ---- | ---- |
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |
