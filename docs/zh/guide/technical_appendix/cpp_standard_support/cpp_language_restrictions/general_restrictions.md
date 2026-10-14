# C/C++通用语法限制

本文介绍Ascend C Device侧的通用语法限制，包含不支持的语法特性，以及指针与内存地址、变量、函数、类、模板等语法使用限制。

## 执行空间差异总览

**表1**  各执行空间及本文约定

| 执行空间 | 所在位置 | 本文简称 |
| --- | --- | --- |
| SIMT编程核函数 | `__global__ void`（`--enable-simt`）及其调用的`__aicore__`函数 | **SIMT** |
| 混合编程SIMT VF | `__simt_vf__`函数及其`__simt_callee__`链 | **SIMT VF** |
| 混合编程SIMD VF | `__simd_vf__`函数及其`__simd_callee__`链 | **SIMD VF** |
| 混合编程核函数体 | `__global__ __vector__`核函数的scalar侧代码（VF之外的核函数体） | **MainScalar** |

**表2**  通用语法限制四执行空间实测矩阵（CANN 9.3.0，Ascend 950PR真实芯片）

| 特性 | SIMT | SIMT VF | SIMD VF | MainScalar |
| --- | --- | --- | --- | --- |
| 运行期递归 | ✅ | ❌ `recursive aicore function call is not allowed` | ❌ 同左 | ❌ 同左 |
| 函数指针取地址/间接调用 | ✅ | ❌ callee标记使函数失去指针转换 | ❌ 同左 | ✅ |
| static局部变量 | ✅ | ❌ 后端报错（寄存器宽度） | ❌ 编译器崩溃 | ✅ |
| 全局const直接用/引用/取地址 | ✅ | ✅ | ✅ | ✅（引用/取地址可用） |
| namespace-scope全局变量读写 | ❌ `reference to __host__ variable` | ❌ `global variable ... is not allowed in aicore function` | ❌ 同左 | ❌ 同左 |
| 虚函数/运行期多态（非虚析构） | ✅ | ✅ | ✅ | ✅ |
| 函数模板/模板模板参数 | ✅ | ✅ | ✅ | ✅ |
| typeid（RTTI） | ❌ | ❌ | ❌ | ❌ |

> 说明：表中"callee标记"指混合编程中`__simt_callee__`/`__simd_callee__`/`__aicore__`函数属性标记。混合编程各执行空间内的辅助函数与Lambda需携带对应callee标记（标记规则见[Lambda表达式](../cpp_language_feature/lambda_expressions.md)），部分语言能力（函数指针转换、static局部变量等）会因标记后的代码生成路径受限而不可用。



## 不支持的特性

-   不支持虚函数
-   不支持虚继承
-   不支持运行时递归
-   不支持动态malloc、new/free
-   不支持STL
-   不支持运行时typeid
-   不支持文件系统IO
-   不支持标准库下的tuple及算法类运算（相关库函数调用需要标记aicore）

## 指针与内存地址

指针解引用（`*pointer`、`pointer->member`、`pointer[0]`）仅允许在目标内存所在的执行空间进行。Device侧的GM/UB指针在Host侧解引用、Host栈/全局地址在Device侧解引用均无有效语义（核函数指针形参实际指向Global Memory，Host侧地址在Device侧不可见），此类跨空间访问在编程模型层面禁止。

### 取Device函数的地址

- 允许取`__aicore__`函数的地址存入函数指针并通过函数指针调用，编译通过且运行结果正确：

    ```cpp
    __aicore__ inline float scale2(float v) { return v * 2.f; }

    __global__ __launch_bounds__(128) void probe(float* output, uint32_t count)
    {
        float (*fn)(float) = scale2;          // 取__aicore__函数地址：可用
        output[threadIdx.x] = fn(static_cast<float>(threadIdx.x));
    }
    ```

- SIMT VF/SIMD VF内的callee函数不支持以函数指针间接调用，因为`__simt_callee__`/`__simd_callee__`标记使函数类型带上函数属性，失去向普通函数指针的转换：

    ```cpp
    __simt_callee__ float scale2(float v) { return v * 2.f; }

    __simt_vf__ __launch_bounds__(128) inline void vf_func(__ubuf__ float* out, uint32_t count)
    {
        float (*fn)(float) = scale2;
        // error: cannot initialize a variable of type 'float (*)(float)' with an lvalue of type
        //        '__simt_callee__ float (float)'
    }
    ```

- SIMT VF/SIMD VF函数不支持以函数指针间接调用，只支持使用`asc_vf_call()`接口进行调用。

- 不允许引用Host函数的地址：核函数内引用Host函数（含直接调用与函数指针）编译报错：

    ```cpp
    float host_fn(float v) { return v + 100.f; }

    __global__ __launch_bounds__(128) void probe(float* output, uint32_t count)
    {
        float (*fp)(float) = host_fn;         // ERROR: reference to __host__ function 'host_fn' in __global__ function
        output[threadIdx.x] = fp(1.f);
    }
    ```
- Device代码在编译期随核函数静态链接，不支持运行时使用`dlopen`动态加载Device函数的机制。

### 核函数的地址

`__global__`核函数之间不允许互相调用，也不允许在核函数内引用核函数的地址。核函数只能由Host侧通过`<<<>>>`启动：

```cpp
__global__ void kernel_a(float* output) { ... }

__global__ __launch_bounds__(128) void probe(float* output, uint32_t count)
{
    void* p = reinterpret_cast<void*>(&kernel_a);  // ERROR: reference to __global__ function in __global__ function
    output[threadIdx.x] = p == nullptr ? 0.f : 1.f;
}
```

### 指针运算与字节重解释

禁止不同地址空间的指针大小比较和`reinterpret_cast`字节重解释，只支持同一地址空间内（如GM同一数组）的指针大小比较、`reinterpret_cast`字节重解释，使用示例如下：

```cpp
__global__ __launch_bounds__(128) void probe(float* output, uint32_t count)
{
    float* p1 = output + threadIdx.x;
    float* p2 = output + (count - 1 - threadIdx.x);
    bool cmp = p1 < p2;                                    // GM指针比较：可用
    int32_t* pi = reinterpret_cast<int32_t*>(p1);          // 字节重解释：可用
    pi[0] = static_cast<int32_t>(cmp ? 1 : 0);
}
```

## 变量

### static局部变量

核函数与`__aicore__`函数内允许声明static局部变量，但多核并行使static变量的读写具有不确定性，需要多核/多线程共享数据时，请通过UB/GM显式分配，不要依赖static局部变量：

```cpp
__global__  void static_life(float* output, uint32_t count)
{
    if (threadIdx.x >= count) {
        return;
    }
    static int32_t counter = 0;
    counter += 1;
    output[threadIdx.x] = static_cast<float>(counter); // output结果不确定
}
```
另外，static局部变量不允许动态初始化，初始化表达式须为编译期常量：

```cpp
__global__ void probe(float* output, uint32_t count)
{
    static int32_t v1 = 11;           // CORRECT：常量初始化
    // static int32_t v2 = count;     // ERROR：dynamic initialization is not supported
    output[threadIdx.x] = static_cast<float>(v1);
}
```

SIMT VF和SIMD VF内不支持使用static局部变量：

```cpp
__simt_vf__ __launch_bounds__(128) inline void vf_func(__ubuf__ float* out, uint32_t count)
{
    static int32_t counter = 0;    // 编译报错
    counter += 1;
    out[threadIdx.x] = static_cast<float>(counter);
}
```

VF函数内需要共享数据时，使用UB显式分配或经由`__gm__`指针传入状态区。

### 全局变量与namespace-scope变量

Device侧不支持可写的全局变量：包括namespace-scope的`static`变量和普通全局变量，其中普通全局变量是指非constexpr、非const常量初始化，见[const限定符变量](#const限定符变量)。在核函数内读写全局变量会触发编译报错。

SIMT编程场景报错示例如下：

```cpp
static int32_t g_counter = 0;         // namespace-scope变量

__global__ __launch_bounds__(128) void probe(float* output, uint32_t count)
{
    g_counter += 1;                   // ERROR: reference to __host__ variable 'g_counter' in __global__ function
    output[threadIdx.x] = static_cast<float>(g_counter);
}
```

混合编程场景报错示例如下：

```cpp
int g_counter = 0;                    // namespace-scope变量

__simt_vf__ __launch_bounds__(128) inline void vf_func(__ubuf__ float* out, uint32_t count)
{
    g_counter += 1;                   // error: global variable 'g_counter' is not allowed in aicore function
    out[threadIdx.x] = static_cast<float>(g_counter);
}
```

### const限定符变量

全局、namespace或类作用域声明的const限定变量（无Device侧内存空间标注）默认属于Host变量。Ascend C Device侧可直接使用、引用与取地址常量表达式初始化的const全局变量：

```cpp
const int ConstVar = 10;                  // 常量表达式初始化的全局const
const float ConstFloatVar = 5.0f;

__global__ __launch_bounds__(128) void probe(float* output, uint32_t count)
{
    int arr[ConstVar];                    // CORRECT：内建整型直接用作数组维度
    float v = ConstFloatVar;              // CORRECT：浮点const直接使用
    const int& ref = ConstVar;            // CORRECT：引用全局const变量
    const int* p = &ConstVar;             // CORRECT：取全局const变量地址
    output[threadIdx.x] = static_cast<float>(arr[ConstVar - 1]) + v
                        + static_cast<float>(ref) * 0.8f + static_cast<float>(*p) * 0.6f;
}
```

extern声明且未在本编译单元内以常量表达式初始化的const变量不可在核函数内使用：

```cpp
extern const int ExternVar;               // 本单元内无常量初始化式

__global__ void probe(float* output, uint32_t count)
{
    output[threadIdx.x] = static_cast<float>(ExternVar);
    // ERROR: reference to __host__ variable 'ExternVar' in __global__ function
}
```

> [!NOTE]说明
> 从C++14起，推荐使用constexpr（或C++17的inline constexpr）变量替代const限定变量。constexpr变量不受类型限制，且语义上明确为编译期常量，Device侧使用行为与const一致。

### volatile限定符变量

volatile关键字为兼容ISO C++而支持，volatile读写不提供线程间同步保证，也不保证硬件执行的记忆体访问次数与指令数一致，编译器不合并、不省略volatile访问，但多个线程对同一volatile对象的访问无顺序与可见性保证。

```cpp
__global__ __launch_bounds__(128) void probe(float* output, uint32_t count)
{
    // volatile写：每次赋值生成一次真实的GM写指令（不被优化合并）
    volatile float* vout = output;
    vout[threadIdx.x] = static_cast<float>(threadIdx.x) * 2.f;

    // volatile读：从内存重新读取（不使用寄存器缓存值）
    output[threadIdx.x] = vout[threadIdx.x];
}
```

volatile**不适用于**线程间同步与消息传递：

- 跨线程的消息传递应使用原子操作（`asc_atomic_add`/`asc_atomic_cas`）配合内存栅栏（`asc_threadfence`/`asc_threadfence_block`），volatile不提供任何内存序保证；
- 访存次数敏感的内存操作不应依赖volatile实现。

以生产者-消费者标志位为例，SIMT编程场景的线程同步典型示例如下：

```cpp
__global__ __launch_bounds__(128) void sync_example(__gm__ int* flag, __gm__ int* data)
{
    if (threadIdx.x == 0) {
        // 消费者：原子读标志位，置位后读数据
        while (asc_atomic_add(flag, 0) == 0) { }     // 原子读
        asc_threadfence();                            // 栅栏保证数据可见性
        // 读取data...
    } else if (threadIdx.x == 1) {
        // 生产者：写数据后置位标志
        data[0] = 42;
        asc_threadfence();
        asc_atomic_add(flag, 1);                      // 原子置位
    }
}
```

### constexpr全局只读表

编译期初始化的POD constexpr全局数组可以在核函数内运行期索引，取其地址作为指针使用同样可用：

```cpp
constexpr uint32_t TABLE[4] = {1u, 2u, 4u, 8u};

__global__ __launch_bounds__(128) void probe(float* output, uint32_t count)
{
    float acc = 0.f;
    for (uint32_t i = 0; i < 4u; ++i) {
        acc += static_cast<float>(TABLE[i]);   // 运行期索引constexpr表：可用
    }
    const uint32_t* tp = &TABLE[0];            // 取constexpr表地址：可用
    output[threadIdx.x] = acc + static_cast<float>(tp[threadIdx.x % 4u]);
}
```

### thread_local存储

不支持`thread_local`存储：SIMT编程场景核函数的局部变量本身即线程私有，通常无需thread_local，详细见[C++ 11语法约束限制](./cpp11_restrictions.md)相关说明。

## 函数

### 核函数（Kernel）参数列表限制<a name="section_kernel_func_param_restriction"></a>

核函数（Kernel）的参数列表（argument list）仅支持以下类型：

- 基础数据类型，如int32\_t、float等。
- 基础数据类型组成的结构体，支持嵌套结构体形式，但必须为POD（Plain Old Data）类型。
- 基础数据类型的指针类型，如int32\_t\*、float\*等，实际上这些指针指向的是Global Memory内存。

### SIMT VF函数限制

\_\_simt\_vf\_\_标记的SIMT VF函数需要遵循以下约束和限制：

-   标量参数不允许使用指针和引用。
-   不允许将栈数组作为参数传入SIMT VF函数；SIMT VF函数的指针形参必须显式使用`__gm__`或`__ubuf__`地址空间限定符。以下错误示例会在编译期报错。

    ```cpp
    __simt_vf__ __launch_bounds__(1024) inline void foo(__gm__ int* a, __gm__ int* b, __gm__ int* c, int* array) {
        int idx = blockIdx.x * blockDim.x + threadIdx.x;
        a[idx] = b[idx] + c[idx] + array[0];
    }
    __global__ __aicore__ void foo(__gm__ int* a, __gm__ int* b, __gm__ int* c) {
        int array[5] = {0,1,2,3,4};
        asc_vf_call<foo>(dim3{256}, a, b, c, array); // error: cannot initialize a parameter of type '__gm__ int *' with an lvalue of type 'int[5]'
    }
    ```

-   如果传参中出现多级指针，不允许使用内层栈地址指针访问，以下为错误示例。

    ```cpp
    __simt_vf__ __launch_bounds__(1024) inline void foo(__gm__ int* a, __gm__ int* b, __gm__ int* c, __ubuf__ uint64_t* s) {
        int idx = blockIdx.x * blockDim.x + threadIdx.x;
        int* stack = (int*)(s[0]);
        // error: *stack表示从多级指针中读取，不允许使用
        a[idx] = b[idx] + c[idx] + *stack;
    }
    __global__ __aicore__ void foo(__gm__ int* a, __gm__ int* b, __gm__ int* c) {
        int stack = 0;
        __ubuf__ uint64_t* s = ...;
        s[0] = &stack;
        asc_vf_call<foo>(dim3{256}, a, b, c, s);
    }
    ```

-   不支持通过函数指针进行间接调用，被调用的\_\_simt\_vf\_\_函数需要在编译期确定。
-   函数的inline行为由编译器决定，添加的always\_inline或noinline将被忽略。
-   不允许使用结构体作为参数，以下为错误示例。

    ```cpp
    __simt_vf__ __launch_bounds__(1024) inline void foo(__gm__ int* a, __gm__ int* b, __gm__ int* c, struct S s) {
        // error: s表示结构体参数，不允许使用
    }
    ```

### 递归

支持编译期递归（constexpr求值），运行期递归仅在SIMT编程场景可用，混合编程的SIMT VF、SIMD VF与MainScalar中，`__simt_callee__`/`__simd_callee__`/`__aicore__`函数的递归调用编译报错：

```cpp
// 混合编程SIMT VF内：ERROR
__simt_callee__ uint32_t rec_sum(uint32_t n)
{
    return n == 0u ? 0u : n + rec_sum(n - 1u);
    // error: recursive aicore function call is not allowed
}
```

### Host函数与Device函数的调用边界

Device侧代码不可以调用或引用Host函数，编译报错`reference to __host__ function`；`__global__`函数不能调用`__global__`函数，`__aicore__`函数也不能回调`__global__`函数——核函数只能由Host侧`<<<>>>`启动（详细见[指针与内存地址](#核函数的地址)）。

### 可变参数

`__aicore__`函数允许C风格可变参数（`...`）签名，编译与运行实测可用；但变参在Device侧的解引用需按ABI自行处理，不建议在Device代码中使用：

```cpp
__aicore__ int32_t sum_all(int32_t count, ...)   // 签名可编译运行，不建议使用
{
    return count;
}
```

### inline行为

函数的inline行为由编译器决定，inline标记是否生效由编译器确定，不支持用户强制指定inline行为。

## 结构体与类

### SIMD与SIMT混合编程场景结构体使用限制

SIMD与SIMT混合编程场景下，当使用位于Global Memory或Unified Buffer（UB）中的结构体或类拷贝构造新结构体或类时，需要开发者为结构体或类手动实现拷贝构造函数，编译器不会默认隐式生成。

```cpp
struct TestS {
    int a;
    float b;
    // 手动实现Global Memory内存空间的拷贝构造函数
    __callee__ TestS(const __gm__ TestS& other) {
        a = other.a;
        b = other.b;
    }
    // 手动实现UB内存空间的拷贝构造函数
    __callee__ TestS(const __ubuf__ TestS& other) {
        a = other.a;
        b = other.b;
    }
};

__simt_vf__ void simt_kernel(__gm__ TestS *data1, __ubuf__ TestS *data2)
{
    uint32_t idx = threadIdx.x;
    TestS t1 = data1[idx];   // 调用Global Memory拷贝构造函数
    TestS t2 = data2[idx];   // 调用UB拷贝构造函数
    ...
}
```

在异构编译场景中，Host和Device可共用结构体定义代码，但Host侧无法识别`__callee__`、`__gm__`等Device侧关键字，因此无法根据这些关键字实现函数重载。当需要定义多个地址空间重载函数时，需通过宏`__NPU_ARCH__`将相关Device代码隔离，以避免函数定义冲突。`__NPU_ARCH__`是由编译预定义的宏，可用于区分Device侧代码，具体可参考[《毕昇编译器》](https://www.hiascend.com/document/redirect/CannCommunityBiSheng)中的“基本编程指导 > AI Core编程指导 > 预定义宏和内建变量”章节。

此外，SIMD与SIMT混合编程场景下不支持直接向Global Memory或UB地址空间写入结构体。

```cpp
struct TestS {
    int a;
    float b;
    //device 侧代码隔离
#ifdef __NPU_ARCH__
    __callee__ TestS(const __gm__ TestS& other) {
        a = other.a;
        b = other.b;
    }
    __callee__ TestS(const __ubuf__ TestS& other) {
        a = other.a;
        b = other.b;
    }
#endif
    // host 侧代码
    TestS(const TestS& other) {
        a = other.a;
        b = other.b;
    }
};

__simt_vf__ void simt_kernel(__gm__ TestS *data1, __ubuf__ TestS *data2)
{
    uint32_t idx = threadIdx.x;
    TestS t = data1[idx];      // 调用Global Memory拷贝构造函数
    data2[0] = t;               // 不支持，编译报错
    ...
}

__global__ __vector__ void kernel_func(__gm__ TestS *data1, __gm__ TestS *data2)
{
    TestS t = data1[0];         // 调用Global Memory拷贝构造函数
    data2[0] = t;               // 不支持，编译报错
    ...
}
```

### 成员函数的执行空间标记

类的成员函数（含静态成员函数）在Device侧被调用时需标记`__aicore__`，未标记的成员函数属于Host函数，在核函数内调用报错：

```cpp
struct Holder {
    float value = 3.f;
    __aicore__ float read() const { return value; }   // 标记__aicore__：Device侧可用
};

__global__ __launch_bounds__(128) void probe(float* output, uint32_t count)
{
    Holder h;
    output[threadIdx.x] = h.read() + static_cast<float>(threadIdx.x) * 0.f; // 编译运行正常
}
```

```cpp
struct Holder {
    float value = 3.f;
    float read() const { return value; }              // 未标记：Host函数
};

__global__ __launch_bounds__(128) void probe(float* output, uint32_t count)
{
    Holder h;
    output[threadIdx.x] = h.read();   // 编译报错: reference to __host__ function 'read' in __global__ function
}
```

friend声明的访问权限与C++保持一致，但friend成员函数同样需标记`__aicore__`。

### 虚函数与运行期多态

虚函数与运行期多态实测可用：虚成员函数（标记`__aicore__ virtual`）经基类引用或指针调用，虚表分发正确；显式限定名调用（`d.Base::get()`）走静态调用：

```cpp
struct Base {
    float v = 1.f;
    __aicore__ virtual float get() const { return v; }
    __aicore__ ~Base() = default;                 // 非虚析构
};

struct Derived : public Base {
    __aicore__ float get() const override { return v * 10.f; }
};

__global__ __launch_bounds__(128) void probe(float* output, uint32_t count)
{
    Derived d;
    d.v = 2.f;
    Base& ref = d;
    Base* p = &d;
    float a = ref.get();       // 虚调用：实测返回20
    float b = p->get();        // 虚调用：实测返回20
    float c = d.Base::get();   // 静态调用：返回2
    output[threadIdx.x] = a + b + c;   // 实测输出42
}
```

**注意**：virtual析构函数会引入`operator delete`依赖，导致链接失败（`undefined symbol: dpp_dealloc`），Device侧类应避免虚析构。

**执行空间差异**：虚函数与运行期多态在四种执行空间均实测可用（虚表分发正确），差异仅在成员函数标记：纯SIMT用`__aicore__`，SIMT VF内用`__simt_callee__`，SIMD VF内用`__simd_callee__`，MainScalar内用`__aicore__`。虚析构的链接限制四种空间一致。

### 成员函数指针

成员函数指针在Device侧实测可用（指向的成员函数需标记`__aicore__`）：

```cpp
struct Holder {
    float value = 3.f;
    __aicore__ float twice() const { return value * 2.f; }
};

__global__ __launch_bounds__(128) void probe(float* output, uint32_t count)
{
    Holder h;
    float (Holder::*mp)() const = &Holder::twice;   // 成员函数指针：可用
    output[threadIdx.x] = (h.*mp)();                // 实测返回6
}
```

### 运行时类型识别（RTTI）

`dynamic_cast`与运行时typeid不可用：

```cpp
struct PolyBase { __aicore__ virtual ~PolyBase() = default; };
struct PolyDerived : public PolyBase { float w = 9.f; };

__global__ void probe(float* output, uint32_t count)
{
    PolyDerived pd;
    PolyBase* pb = &pd;
    PolyDerived* down = dynamic_cast<PolyDerived*>(pb);   // 链接失败：undefined symbol: __dynamic_cast
    output[threadIdx.x] = down != nullptr ? down->w : -1.f;
}
```

### 动态分配相关的类能力

类的成员函数不支持引入`operator new/delete`依赖，Device侧不支持动态new/delete、malloc/free（见[不支持的特性](#不支持的特性)）。

## 模板

### 模板在Device侧的使用

支持函数模板、类模板、变参模板：`__aicore__`函数模板实例化、核函数内类模板对象构造、`sizeof...`参数包计数、模板模板参数（`template <template <typename> class C>`）实例化：

```cpp
template <typename T>
__aicore__ inline T twice(T v) { return v + v; }

template <typename... Args>
__aicore__ inline uint32_t count_args(Args...)
{
    return sizeof...(Args);
}

__global__ __launch_bounds__(128) void probe(float* output, uint32_t count)
{
    float f = twice(5.5f);                    // 函数模板：11.0
    uint32_t c = count_args(1, 2.f, 'a');     // 变参模板：3
    output[threadIdx.x] = f + static_cast<float>(c);   // 输出14
}
```

模板函数标记随空间选择，如`__aicore__`、`__simt_callee__`、`__simd_callee__`等。

### 模板核函数

核函数本身可以是函数模板，融合编译场景，在`<<<>>>`调用时显式指定模板实参：

```cpp
template <typename T>
__global__ __launch_bounds__(128) void tpl_kernel(T* output, uint32_t count)
{
    if (threadIdx.x >= count) {
        return;
    }
    output[threadIdx.x] = static_cast<T>(threadIdx.x) + static_cast<T>(1);
}

// Host侧：
tpl_kernel<float><<<1, 128, 0, stream>>>(dev, 128);
```

### 显式实例化

混合编程VF模板函数无需显式实例化声明，模板经`asc_vf_call()`调用时显式指定实参即可完成实例化，示例代码如下：
