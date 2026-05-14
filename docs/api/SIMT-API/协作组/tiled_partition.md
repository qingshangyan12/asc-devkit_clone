# tiled_partition

## 功能说明

`tiled_partition`API用于将一个线程组划分为多个更小、固定大小的子组，以便线程在以更精细的粒度上进行协作。提供模板和非模板两个版本的接口，分别用于编译时确定划分大小以及运行时确定划分大小的场景。模板版本的接口传入的父组类型只能为`thread_block`或`thread_block_tile`，非模板版本的接口传入的父组类型支持`thread_block`和`coalesced_group`。

## 函数原型

```C++
template <unsigned int Size, typename ParentT>
thread_block_tile<Size, ParentT> tiled_partition(const ParentT& g);

thread_group tiled_partition(const thread_group& parent, unsigned int tilesz);
thread_group tiled_partition(const thread_block& parent, unsigned int tilesz);
coalesced_group tiled_partition(const coalesced_group& parent, unsigned int tilesz);
```

## 参数说明

## 返回值说明

返回划分出的子组对象

## 约束说明

- Size 必须为$2^n$，且小于32，且小于父组的线程数。
- 对于模板版本的接口，父类中的线程数必须能被Size整除。

## 调用示例

### SIMT编程场景

```c++
using namespace cooperative_groups;
__global__ void simt_kernel(...)
{
    ...
    thread_block block = this_thread_block();
    auto tile4 = tiled_partition<4>(block);
    ...
}
```

### SIMD与SIMT混合编程场景

```c++
using namespace cooperative_groups;
__simt_vf__ inline void simt_kernel(...)
{
    ...
    thread_block block = this_thread_block();
    auto tile4 = tiled_partition<4>(block);
    ...
}
```