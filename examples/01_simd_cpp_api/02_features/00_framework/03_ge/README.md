# GE样例介绍

## 概述

本目录介绍Ascend C自定义算子接入GE原生构图执行的方法。

## 样例列表

| 目录名称 | 功能描述 |
| --- | --- |
| [triple_chevron_notation](./triple_chevron_notation) | 样例介绍如何在GE `EagerExecuteOp::Execute`中直接通过`<<<>>>`调用Ascend C kernel，并通过GE在线构图执行`AddCustom` |
