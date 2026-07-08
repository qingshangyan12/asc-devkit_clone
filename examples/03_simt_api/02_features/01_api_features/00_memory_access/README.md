# Memory Access样例介绍

## 概述

基于Ascend C SIMT的内存访问样例，展示SIMT编程中Global Memory离散访存和跨核数据同步相关API的使用方法。

## 样例列表

| 目录名称 | 功能描述 |
| --- | --- |
| [insert_hash_table](./insert_hash_table) | 样例基于Ascend C SIMT编程方式实现InsertHashTable算子，展示通过`asc_atomic_cas()`原子操作和`asc_threadfence()`内存栅栏接口实现多线程哈希表插入的复杂业务逻辑能力。 |
