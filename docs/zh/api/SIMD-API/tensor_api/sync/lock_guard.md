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

- `lock_guard`只管理构造时传入的同一个mutex对象。进入`lock_guard`作用域时调用该mutex的`lock`，离开作用域时调用其`unlock`。
- `lock_guard`生存期间，开发者不得手动调用该mutex的`lock`或`unlock`，否则会破坏成对使用约束。
- `lock_guard`使用的mutex仍须满足[mutex](mutex.md)中的`Pipe`取值、`mutex_id`管理、成对使用和禁止嵌套约束。
- 同一流水内多条指令的顺序控制不应依赖`lock_guard`，应使用[sync_pipe](sync_pipe.md)。

## 调用示例

以下示例在UB到GM搬出阶段使用lock_guard管理mutex：进入作用域时自动获取锁，离开作用域时自动释放锁。

```cpp
#include "tensor_api/tensor.h"

constexpr uint32_t total_length = 64;
constexpr uint8_t buffer_lock_id = 0;

__global__ __vector__ void lock_guard_kernel(__gm__ float* dst)
{
    using namespace asc::te;
    __ubuf__ float buffer[total_length];

    auto layout = make_layout(make_shape(total_length));
    auto dst_gm = make_tensor(make_mem_ptr<location::gm>(dst), layout);
    auto ub_tensor = make_tensor(make_mem_ptr(buffer), layout);

    mutex<pipe_t::PIPE_MTE3> copy_out_mutex(buffer_lock_id);
    {
        lock_guard guard(copy_out_mutex);
        copy(make_copy(copy_ub_to_gm{}), dst_gm, ub_tensor);
    }
}
```
