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

**表1**  参数说明

| 参数名 | 输入/输出 | 描述 |
| :--- | :--- | :--- |
| Pipe | 输入 | 编译期流水参数，类型为`pipe_t`。 |
| id | 输入 | 硬件锁ID，类型为`mutex_id`，即`uint8_t`。 |

## 返回值说明

构造函数无返回值。`lock`和`unlock`无返回值。

## 约束说明

- `lock()`用于获取锁资源，`unlock()`用于释放锁资源。
- 构造函数只保存ID，不分配或初始化硬件锁资源。
- 析构函数不会自动释放锁资源。
- mutex禁止拷贝和移动。
- 锁ID范围、pipe支持范围、配对使用和嵌套约束以底层硬件约束为准。
- 同一ID的资源管理由调用方负责。

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
