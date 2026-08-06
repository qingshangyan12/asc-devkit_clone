# Vector Compute Practices样例介绍

## 概述

本目录提供基于Ascend C C API的Vector计算性能优化样例，重点介绍GM/UB数据搬运与Vector计算之间的流水优化方法。

## 样例列表

| 目录名称 | 功能描述 | 支持的产品 |
| --- | --- | --- |
| [add_high_performance](./add_high_performance) | Add高性能样例，通过相同计算规格下的单缓冲基线与Ping-Pong双缓冲场景，对比MTE2、Vector和MTE3流水重叠带来的性能变化。 | Ascend 950PR/Ascend 950DT |
