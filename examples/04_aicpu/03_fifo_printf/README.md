# AICPU FIFO printf 样例

## 概述

本组样例演示 AICPU FIFO-based printf 的使用场景，包括多线程并发 printf 和大量 printf 导致 ringbuf 回绕的场景。

## 前置条件

FIFO printf 依赖 rts 侧提供 `AicpuGetDfxInfo` weak symbol 及 ringbuf 内存管理。需等待相关模块完成后联合调试才能正常运行。

## 样例列表

| 目录名称 | 功能描述 | 支持的产品 |
| ------- | -------- | --- |
| [01_multithread_printf](./01_multithread_printf) | 演示 AICPU 内核中多线程并发调用 printf，验证多线程写入 ringbuf 的正确性 | Ascend 950PR/Ascend 950DT<br>Atlas A3 训练系列产品/Atlas A3 推理系列产品<br>Atlas A2 训练系列产品/Atlas A2 推理系列产品 |
| [02_wraparound_printf](./02_wraparound_printf) | 演示大量 printf 填满 32KB ringbuf 并触发回绕（split-write），验证回绕写入的正确性 | Ascend 950PR/Ascend 950DT<br>Atlas A3 训练系列产品/Atlas A3 推理系列产品<br>Atlas A2 训练系列产品/Atlas A2 推理系列产品 |
| [03_multithread_print](./03_multithread_print) | 演示 host 侧多线程同时下发同一个 AICPU 打印算子到不同 stream，验证并发执行时 printf 信息是否正常 | Ascend 950PR/Ascend 950DT<br>Atlas A3 训练系列产品/Atlas A3 推理系列产品<br>Atlas A2 训练系列产品/Atlas A2 推理系列产品 |
| [04_multiprocess_print](./04_multiprocess_print) | 演示 host 侧 fork 多个子进程，每个进程独立注册并执行 AICPU 算子，验证多进程场景下 printf 信息是否正常隔离 | Ascend 950PR/Ascend 950DT<br>Atlas A3 训练系列产品/Atlas A3 推理系列产品<br>Atlas A2 训练系列产品/Atlas A2 推理系列产品 |
