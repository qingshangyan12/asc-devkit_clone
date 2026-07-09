# asc_dci

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT：支持
<!-- end id1 -->

## 功能说明

- 接口声明文件：`"c_api/cache_ctrl/cache_ctrl.h"`
- 核心功能：数据缓存失效，使整个数据缓存无效化。该指令不会对处于"脏"状态的缓存行执行写回操作，所有缓存行将被直接标记为无效，其修改过但尚未同步至主存的数据将被丢弃。
- 实现原理/公式：Invalid操作将指定地址范围的Cache Line标记为"无效"，使其从缓存中移除，保证下一次访问这些内存地址时从GM重新加载数据。与Clean操作不同，Invalid操作不检查缓存行是否为"脏"（dirty，表示该数据已被修改但尚未写回到GM），若存在脏数据将被直接丢弃。

## 函数原型

<!-- npu="950" id2 -->
针对Ascend 950PR/Ascend 950DT：

```cpp
__aicore__ inline void asc_dci()
```

<!-- end id2 -->

## 参数说明

无

## 数据类型支持

该接口不涉及数据类型选择，无参数。

## 返回值说明

无

## 流水类型

PIPE_S

## 资源占用情况

- 临时空间占用：不占用临时空间。
- 同步资源占用：不占用同步资源。
- 资源占用与参数/数据量关系：无。

## 约束说明

- 在调用asc_dci之前，需调用asc_sync_data_barrier插入DSB_ALL指令，等待所有内存访问指令执行结束。

## 调用示例

<!-- npu="950" id3 -->
针对Ascend 950PR/Ascend 950DT：

```cpp
asc_sync_data_barrier(mem_dsb_t::DSB_ALL);
asc_dci();
```

- 完整样例链接：`<TODO>`

<!-- end id3 -->
