# mutex

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

`mutex`是对象式流水同步句柄。模板参数`Pipe`在编译期指定流水，构造函数保存运行期硬件锁ID，`lock`和`unlock`分别用于获取和释放锁资源。

`lock`根据`mutex_id`获取Mutex。若Mutex已被锁定，将阻塞指定流水上后续指令的执行，直到所有流水中具有相同`mutex_id`的Mutex均已被释放。`unlock`在指定流水的前序指令执行完成后，根据`mutex_id`释放对应Mutex。

mutex用于核内异步流水之间的资源交接，不是CPU线程锁，也不是跨核互斥锁。

## 函数原型

```cpp
using mutex_id = uint8_t;

template <pipe_t Pipe>
class mutex {
public:
    __aicore__ inline explicit mutex(mutex_id id)
    __aicore__ inline void lock()
    __aicore__ inline void unlock()

    mutex(const mutex&) = delete;
    mutex& operator=(const mutex&) = delete;
    mutex(mutex&&) = delete;
    mutex& operator=(mutex&&) = delete;
};
```

## 参数说明

**表1**  模板参数说明

| 参数名 | 输入/输出 | 描述 |
| :--- | :--- | :--- |
| Pipe | - | 编译期模板参数，类型为`pipe_t`，指定与Mutex关联的流水。 |

**表2**  接口参数说明

| 参数名 | 输入/输出 | 描述 |
| :--- | :--- | :--- |
| id | 输入 | 构造函数参数，硬件锁ID，类型为`mutex_id`，即`uint8_t`。 |

## 返回值说明

构造函数无返回值。`lock`和`unlock`无返回值。

## 约束说明

- AIC支持的`Pipe`取值为`PIPE_S`、`PIPE_M`、`PIPE_MTE1`、`PIPE_MTE2`、`PIPE_FIX`；AIV支持的`Pipe`取值为`PIPE_S`、`PIPE_MTE2`、`PIPE_MTE3`、`PIPE_V`。
- `mutex_id`由开发者自行管理。同一组同步依赖需要使用相同的`mutex_id`，不同同步依赖建议使用不同的`mutex_id`，否则实际同步行为可能与预期不符。
- `lock`与`unlock`必须严格成对使用，并使用相同的`Pipe`和`mutex_id`。对应的`unlock`必须始终位于`lock`之后，否则属于未定义行为。
- 对于`mutex_id`相同的`lock`与`unlock`组合，无论`Pipe`是否相同，都不得嵌套使用，否则属于未定义行为。
- 当具有相同`mutex_id`与`Pipe`的两对`lock`与`unlock`连续调用时，第二对`lock`不能再次阻塞同一流水。因此该写法不能实现同一流水内多条指令之间的顺序控制，单流水内多条指令的顺序控制应使用[sync_pipe](sync_pipe.md)。

## 调用示例

以下示例使用mutex保护GM到UB搬入、UB内计算和UB到GM搬出三个流水阶段，保证各阶段按依赖顺序执行。

```cpp
#include "tensor_api/tensor.h"

constexpr uint32_t total_length = 64;
constexpr uint8_t buffer_lock_id = 0;

__global__ __vector__ void mutex_kernel(__gm__ float* src, __gm__ float* dst)
{
    using namespace asc::te;
    __ubuf__ float buffer[total_length];

    auto layout = make_layout(make_shape(total_length));
    auto src_gm = make_tensor(make_mem_ptr<location::gm>(src), layout);
    auto dst_gm = make_tensor(make_mem_ptr<location::gm>(dst), layout);
    auto ub_tensor = make_tensor(make_mem_ptr(buffer), layout);

    mutex<pipe_t::PIPE_MTE2> copy_in_mutex(buffer_lock_id);
    copy_in_mutex.lock();
    copy(make_copy(copy_gm_to_ub{}), ub_tensor, src_gm);
    copy_in_mutex.unlock();

    mutex<pipe_t::PIPE_MTE3> copy_out_mutex(buffer_lock_id);
    copy_out_mutex.lock();
    copy(make_copy(copy_ub_to_gm{}), dst_gm, ub_tensor);
    copy_out_mutex.unlock();
}
```
