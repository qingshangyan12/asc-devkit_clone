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

- `lock()`用于获取锁资源，`unlock()`用于释放锁资源。
- AIC支持的`Pipe`取值为`PIPE_S`、`PIPE_M`、`PIPE_MTE1`、`PIPE_MTE2`、`PIPE_FIX`；AIV支持的`Pipe`取值为`PIPE_S`、`PIPE_MTE2`、`PIPE_MTE3`、`PIPE_V`。
- 构造函数只保存ID，不分配或初始化硬件锁资源。
- 析构函数不会自动释放锁资源。
- mutex禁止拷贝和移动。
- `mutex_id`由开发者自行管理。同一组同步依赖需要使用相同的`mutex_id`，不同同步依赖建议使用不同的`mutex_id`，否则实际同步行为可能与预期不符。
- `lock`与`unlock`必须严格成对使用，并使用相同的`Pipe`和`mutex_id`。对应的`unlock`必须始终写在`lock`之后，否则属于未定义行为。
- 对于`mutex_id`相同的`lock`与`unlock`组合，无论`Pipe`是否相同，都不得嵌套使用，否则属于未定义行为。

## 调用示例

```cpp
#include "tensor_api/tensor.h"

using namespace asc::te;

constexpr mutex_id buffer_lock_id = 0;

__aicore__ inline void mutex_example()
{
    mutex<pipe_t::PIPE_MTE2> copy_in_mutex(buffer_lock_id);

    copy_in_mutex.lock();
    // 提交GM到UB搬运指令。
    copy_in_mutex.unlock();
}
```
