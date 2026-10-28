# 矩阵计算优化样例介绍

## 概述

本目录包含基于Cube矩阵计算流水实现的优化样例。

## 样例列表

| 目录名称 | 功能描述 |
| --- | --- |
| [00_mmad_double_buffer](./00_mmad_double_buffer) | Matmul C-API高性能样例，对比单缓冲基线与L1双缓冲场景，展示MTE2、MTE1和Cube的跨K轮流水重叠。 |
| [01_mmad_direction](./01_mmad_direction) | Mmad计算方向优化样例，展示在开启UnitFlag的场景下，Mmad计算方向对性能的影响，样例对比了多个场景在不同方向配置下的性能表现。 |
| [03_matmul_mx](./03_matmul_mx) | 本样例介绍如何基于C API，通过 L1/L0 双缓冲机制、大包搬运、细粒度流水同步等多种优化手段，实现一个高性能的 MxFP8 Matmul kernel。 |
