# Cooperative groups 协作组

Ascend C simt 对标CUDA提供协作组API，新头文件名：cooperative_groups.h，所有api 位于命名空间 cooperative_groups 下。

## 背景知识

Ascend C simt 对标CUDA，提供了相同的内置变量：`gridDim`、`blockDim`、`blockIdx`、`threadIdx` 和 `warpSize`，内置数据结构`dim3`，内置接口 `int32_t laneid()` 可直接使用

Ascend C simt 的接口声明需添加 `__SIMT_DEVICE_FUNCTIONS_DECL__` 标识，该标识定义在 `device_types.h` 头文件中。并且所有接口需添加 `inline` 关键字

## class thread_group

所有协作组类的基类（抽象类，接口）

### 成员变量：

```C++
uint32_t _type // 协作组类型

struct _tiled_info {
    bool is_tiled;
    unsigned int mask;
    unsigned int num_threads;
    unsigned int meta_group_rank;
    unsigned int meta_group_size;
} tiled_info;
```

### public 接口：

```C++
unsigned long long size() const;        // 组内线程数
unsigned long long num_threads() const; // 同size
unsigned long long thread_rank() const; // 当前线程在组内排名
void sync() const;                      // 组内线程同步
```

## class thread_block

线程块组，与 thread block 概念对应，隐式组

不提供默认构造函数，用户通过接口获取：

```C++
thread_block g = this_thread_block();
```

### public 成员函数

```C++
static inline void sync() {
    asc_syncthreads();
}

static inline unsigned int thread_rank() {
    return vec3_to_linear<unsigned int>(threadIdx, blockDim);
}

static inline dim3 group_index() {
    return dim3(blockIdx.x, blockIdx.y, blockIdx.z);
}

static inline dim3 thread_index() {
    return dim3(threadIdx.x, threadIdx.y, threadIdx.z);
}

static inline dim3 dim_threads() {
    return dim3(blockDim.x, blockDim.y, blockDim.z);
}

static inline unsigned int num_threads() {
    return static_cast<unsigned int>(blockDim.x * blockDim.y * blockDim.z);
}

static inline unsigned int size() {
    return num_threads();
}

static inline dim3 group_dim() {
    return dim_threads();
}
```

## class thread_block_tile

A templated version of a tiled group, where a template parameter is used to specify the size of the tile - with this known at compile time there is the potential for more optimal execution.

```C++
template <unsigned int Size, typename ParentT = void>
class thread_block_tile;
```

Constructed via:

```C++
template <unsigned int Size, typename ParentT>
thread_block_tile<Size, ParentT> tiled_partition(const ParentT& g)
```

Size must be a power of 2 and less than or equal to 32.
Ascend C simt 受限于硬件，只能将一个 warp 内的线程划分成更小的 thread_block_tile 组，即划分大小必须是2的幂并且要小于32

ParentT is the parent-type from which this group was partitioned. It is automatically inferred, but a value of void will store this information in the group handle rather than in the type.

### Public Member Functions:

`void sync() const`: 同步组内线程。warp内的线程同时执行相同的指令，因此天然就是同步的。此接口实现上只需调用 `asc_threadfence_block` 接口插入MEMBAR，避免编译器进行指令重排。

```C++
void sync() const
{
    asc_threadfence_block();
}
```

`unsigned long long num_threads() const`: Total number of threads in the group

`unsigned long long thread_rank() const`: Rank of the calling thread within [0, num_threads)

`unsigned long long meta_group_size() const`: Returns the number of groups created when the parent group was partitioned.

`unsigned long long meta_group_rank() const`: Linear rank of the group within the set of tiles partitioned from a parent group (bounded by meta_group_size)

`T shfl(T var, unsigned int src_rank) const`: Refer to Warp Shuffle Functions

```C++
T shfl(T var, unsigned int src_rank) const
{
    static_assert(SupportTypeSimtInternel<T, int32_t, uint32_t, int64_t, uint64_t, float, half, half2>, "Input type T only supports int32_t, uint32_t, int64_t, uint64_t, float, half, half2.");
    return asc_shfl(var, src_rank, numThreads);
}
```

`T shfl_up(T var, int delta) const`: Refer to Warp Shuffle Functions

```C++
T shfl_up(T var, int delta) const
{
    static_assert(SupportTypeSimtInternel<T, int32_t, uint32_t, int64_t, uint64_t, float, half, half2>, "Input type T only supports int32_t, uint32_t, int64_t, uint64_t, float, half, half2.");
    return asc_shfl_up(var, delta, numThreads);
}
```

`T shfl_down(T var, int delta) const`: Refer to Warp Shuffle Functions

```C++
T shfl_down(T var, int delta) const
{
    static_assert(SupportTypeSimtInternel<T, int32_t, uint32_t, int64_t, uint64_t, float, half, half2>, "Input type T only supports int32_t, uint32_t, int64_t, uint64_t, float, half, half2.");
    return asc_shfl_down(var, delta, numThreads);
}
```

`T shfl_xor(T var, int delta) const`: Refer to Warp Shuffle Functions

```C++
T shfl_xor(T var, int delta) const
{
    static_assert(SupportTypeSimtInternel<T, int32_t, uint32_t, int64_t, uint64_t, float, half, half2>, "Input type T only supports int32_t, uint32_t, int64_t, uint64_t, float, half, half2.");
    asc_shfl_xor(var, delta, numThreads);
}
```

`int any(int predicate) const`: Refer to Warp Vote Functions

```C++
int any(int predicate) const
{
    uint32_t lane_ballot = asc_ballot(predicate) & build_mask();
    return (lane_ballot != 0);
}
```

`int all(int predicate) const`: Refer to Warp Vote Functions

```C++
int all(int predicate) const
{
    uint32_t lane_ballot = asc_ballot(predicate) & build_mask();
    return (lane_ballot == build_mask());
}
```

`unsigned int ballot(int predicate) const`: Refer to Warp Vote Functions

```C++
unsigned int ballot(int predicate) const
{
    uint32_t lane_ballot = asc_ballot(predicate) & build_mask();
    return adjust_mask(build_mask(), lane_ballot); 
}
```

`unsigned long long size() const`: Total number of threads in the group (alias of num_threads())

## class coalesced_group

### Public Member Functions:

`void sync() const`: 同步组内线程。warp内的线程同时执行相同的指令，因此天然就是同步的。此接口实现上只需调用 `asc_threadfence_block` 接口插入MEMBAR，避免编译器进行指令重排。

```C++
void sync() const {
    asc_threadfence_block();
}
```

`unsigned long long num_threads() const`: Total number of threads in the group

```C++
unsigned long long num_threads() const
{
    return __popc(_mask);
}
```

`unsigned long long thread_rank() const`: Rank of the calling thread within [0, num_threads)

```C++
unsigned long long thread_rank() const {
    return __popc(_mask & lanemask_lt());
}
```

`unsigned long long meta_group_size() const`: Returns the number of groups created when the parent group was partitioned. If this group was created by querying the set of active threads, for example coalesced_threads() the value of meta_group_size() will be 1.

`unsigned long long meta_group_rank() const`: Linear rank of the group within the set of tiles partitioned from a parent group (bounded by meta_group_size). If this group was created by querying the set of active threads, e.g. coalesced_threads() the value of meta_group_rank() will always be 0.

`T shfl(T var, unsigned int src_rank) const`: Refer to Warp Shuffle Functions

```C++
template <class T>
T shfl(T var, unsigned int src_rank) const {
    src_rank = src_rank % static_cast<unsigned int>(num_threads());
    unsigned int lane = src_rank;
    if (num_threads() == warpSize) {
        lane = __fns(_mask, 0, src_rank + 1);
    }
    return asc_shfl(var, lane, warpSize);
}
```

T shfl_up(T var, int delta) const: Refer to Warp Shuffle Functions

使用 shfl 接口实现

```C++
template <class T>
T shfl_up(T var, int delta) const {
    if (num_threads() == warpSize) {
        asc_shfl_up(var, delta, warpSize);
    }

    int lane = __fns(_mask, laneid(), delta + 1);
    if (lane >= 32) {
        lane = laneid();
    }

    asc_shfl(var, lane, warpSize);
}
```

T shfl_down(T var, int delta) const: Refer to Warp Shuffle Functions
通过当前线程的lane id 和 delta 计算出所要取值的线程 lane id，通过 shfl 接口取值

```C++
template <class T>
T shfl_up(T var, int delta) const {
    if (num_threads() == warpSize) {
        asc_shfl_down(var, delta, warpSize);
    }

    int lane = __fns(_mask, laneid(), -(delta + 1));
    if (lane >= 32) {
        lane = laneid();
    }

    asc_shfl(var, lane, warpSize);
}
```

`int any(int predicate) const`: Refer to Warp Vote Functions

```C++
int any(int predicate) const {
    return (asc_ballot(predicate) & _mask) != 0;
}
```

`int all(int predicate) const`: Refer to Warp Vote Functions

```C++
int all(int predicate) const {
    return (asc_ballot(predicate) & _mask) == _mask;
}
```

`unsigned int ballot(int predicate) const`: Refer to Warp Vote Functions

```C++
unsigned int ballot(int predicate) const {
    if (num_threads() == warpSize) {
        return asc_ballot(predicate) & 0xFFFFFFFF;
    }
    unsigned int lane_ballot = asc_ballot(predicate) & _mask;
    return _packLanes(lane_ballot);
}
```

`unsigned long long size() const`: Total number of threads in the group (alias of num_threads())

## partition

### tiled_partition

```C++
template <unsigned int Size, typename ParentT>
thread_block_tile<Size, ParentT> tiled_partition(const ParentT& g);

template <unsigned int Size, typename ParentT>
thread_block_tile<Size, ParentT> tiled_partition(const ParentT& g)
{
    return tiled_partition_impl<Size, ParentT>(g);
}
```

```C++
template <unsigned int Size, typename ParentT>
struct tiled_partition_impl;

template <unsigned int Size>
struct tiled_partition_impl<Size, thread_block> : public thread_block_tile<Size, thread_block> {
    tiled_partition_impl(const thread_block& g) : thread_block_tile<Size, thread_block>(g) {}
}

template <unsigned int Size, unsigned int ParentSize, typename GrandParent>
struct tiled_partition_impl<Size, thread_block_tile<ParentSize, GrandParent>> :
    public thread_block_tile<Size, thread_block_tile<ParentSize, GrandParent>> {
    static_assert(Size < ParentSize, "Tile size bigger or equal to the parent group size");
    tiled_partition_impl(const thread_block_tile<ParentSize, GrandParent>& g) :
        thread_block_tile<Size, thread_block_tile<ParentSize, GrandParent>>(g);
}
```


```C++
thread_group tiled_partition(const thread_group& parent, unsigned int tilesz)
{
    if (parent.get_type() == coalesced_group_type) {
        const coalesced_group *cg = static_cast<const coalesced_group*>(&parent);
        return cg->create_tiled_group(tilesz);
    } else {
        const thread_block *tb = static_cast<const thread_block*>(&parent);
        return tb->create_tiled_group(tilesz);
    }
}

thread_group tiled_partition(const thread_block& parent, unsigned int tilesz)
{
    return parent.create_tiled_group(tilesz);
}

coalesced_group tiled_partition(const coalesced_group& parent, unsigned int tilesz)
{
    return parent.create_tiled_group(tilesz);
}
```

### binary_partition

```C++
coalesced_group binary_partition(const coalesced_group& g, bool pred)
{
    uint32_t mask = asc_ballot(pred) & g.get_mask();
    if (pred) {
        return coalesced_group(mask);
    } else {
        return coalesced_group(g.get_mask() ^ mask);
    }
}

```

```C++
template <unsigned int Size>
coalesced_group binary_partition(const thread_block_tile<Size>& g, bool pred);
```

```C++
template <typename Label>
```

## 工具函数

### __fns

获取从 mask 的 index=base 的 bit 开始算，第 offset 个为1的bit的index

```C++
__SIMT_DEVICE_FUNCTIONS_DECL__ inline unsigned int __fns(unsigned int mask, unsigned base, int offset)
{
    unsigned int temp_mask = mask;
    unsigned int temp_offset = offset;
    if (offset == 0) {
        temp_mask &= (1 << base);
        temp_offset = 1;
    } else if (offset < 0) {
        temp_mask = __brev(mask);
        base = 31 - base;
        temp_offset = -offset;
    }

    temp_mask = temp_mask & ((~0U) << base);
    if (__popc(temp_mask) < temp_offset) {
        return -1;
    }
    int total = 0;
    for (int i = 0x10; i> 0; i >>= 1) {
        unsigned int temp_mask_lo = temp_mask & ((1U << i) - 1);
        int pcnt = __popc(temp_mask_lo);
        if (pcnt < temp_offset) {
            temp_mask = temp_mask >> i;
            temp_offset -= pcnt;
            total += i;
        } else {
            temp_mask = temp_mask_lo;
        }
    }
    if (offset < 0) {
        return 31 - total;
    } else {
        return total;
    }
}
```
