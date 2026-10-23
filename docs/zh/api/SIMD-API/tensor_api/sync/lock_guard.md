# lock_guard

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR&950DT系列产品：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3系列产品：不支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2系列产品：不支持
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2推理产品：不支持
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas推理系列产品AI Core：不支持
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas推理系列产品Vector Core：不支持
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas训练系列产品：不支持
<!-- end id7 -->

## 功能说明

头文件路径为：`tensor_api/tensor.h`。

`lock_guard`是作用域锁保护对象，持有mutex引用。构造时获取锁，离开作用域并析构时释放锁。

`lock_guard`通常与[`mutex<Pipe>`](mutex.md)配合使用，底层通过mutex的`lock`和`unlock`完成锁资源获取与释放。

## 函数原型

```cpp
template <typename Mutex>
class lock_guard {
public:
    __aicore__ inline explicit lock_guard(Mutex& mutex)
    __aicore__ inline ~lock_guard()

    lock_guard(const lock_guard&) = delete;
    lock_guard& operator=(const lock_guard&) = delete;
    lock_guard(lock_guard&&) = delete;
    lock_guard& operator=(lock_guard&&) = delete;
};
```

## 参数说明

**表1**  模板参数说明

| 参数名 | 输入/输出 | 描述 |
| :--- | :--- | :--- |
| Mutex | - | 编译期模板参数，被管理的mutex类型。 |

**表2**  接口参数说明

| 参数名 | 输入/输出 | 描述 |
| :--- | :--- | :--- |
| mutex | 输入 | 被管理的mutex对象引用。 |

## 返回值说明

无

## 约束说明

- 构造`lock_guard`前，被引用的mutex必须已完成构造。
- 被引用的mutex必须比`lock_guard`活得更久。
- 不要在`lock_guard`生命周期内手动调用所引用mutex的`unlock`。
- lock_guard禁止拷贝和移动。
- 对于同一mutex对应的`lock`与`unlock`，不得嵌套使用；同一流水内多条指令的顺序控制应使用[sync_pipe](sync_pipe.md)。

## 调用示例

```cpp
#include "tensor_api/tensor.h"

using namespace asc::te;

constexpr mutex_id buffer_lock_id = 0;

__aicore__ inline void lock_guard_example()
{
    mutex<pipe_t::PIPE_V> compute_mutex(buffer_lock_id);

    {
        lock_guard guard(compute_mutex);
        // 提交矢量计算指令，作用域结束时自动释放锁。
    }
}
```
