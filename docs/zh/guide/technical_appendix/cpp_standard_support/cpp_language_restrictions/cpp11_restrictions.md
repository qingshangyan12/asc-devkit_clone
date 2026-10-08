# C++11限制<a name="ZH-CN_TOPIC_000000253823119002"></a>

本文介绍C++11特性在Ascend C SIMT Device侧的限制。受限特性分为并发类与运行时类两类：并发类特性的标准形式不可用，对应能力由SIMT API提供；运行时类特性因Device侧无对应运行时机制而不支持。每个"不支持"结论均有编译期报错作为证据，可在样例中编写同形态最小用例并编译复现。

## 并发类特性限制<a name="section_cpp11_concurrency_restrictions"></a>

C++11并发特性的标准形式在Device侧均不可用，报错形态见[限制根因分类](#section_cpp11_restriction_root_causes)。对应的同步、原子、栅栏能力由SIMT API提供，替代方案见[并发能力替代方案](#section_cpp11_concurrency_alternatives)。

**表1**  并发类特性限制

| C++11特性 | 支持情况 | Device侧表现 |
| --- | --- | --- |
| 顺序点（Sequence points，C++11起为sequenced-before关系） | ⚠️ | 语义按C++11 sequenced-before规则执行，未序列化修改等场景的诊断策略与Host侧clang一致（告警而非硬错误）。 |
| 原子操作（std::atomic） | ❌ | `std::atomic`的成员函数被Host标准库头文件标记为`__host__`，Device侧调用报"call to \_\_host\_\_ function"。 |
| 强比较交换（Strong Compare and Exchange） | ❌ | 同上，`compare_exchange_strong`不可用。 |
| 双向栅栏（Bidirectional Fences） | ❌ | `std::atomic_thread_fence`被标记为`__host__`。 |
| 内存模型（Memory model） | ⚠️ | Device侧无C++标准内存序（memory\_order）描述；访存可见性由SIMT执行模型加栅栏API约束。 |
| 数据依赖排序（Data-dependency ordering: atomics and memory model） | ⚠️ | 同上，跨线程数据依赖场景需显式使用栅栏API。 |
| 异常传播（Propagating exceptions） | ❌ | `try`/`throw`在`__global__`函数中直接报编译错误："cannot use 'try' in \_\_global\_\_ function"、"cannot use 'throw' in \_\_global\_\_ function"。 |
| 信号处理函数中使用原子操作（Allow atomics use in signal handlers） | ❌ | Device侧无POSIX信号概念；`<csignal>`的宏常量可编译但无运行时意义。 |
| 线程局部存储（Thread-local storage） | ❌ | 编译器后端不支持TLS，报后端错误"Cannot select: GlobalTLSAddress"。核函数的局部变量本身即线程私有，通常无需thread\_local。 |
| 并发环境动态初始化（Dynamic initialization and destruction with concurrency） | ❌ | Device侧全局/静态变量禁止动态初始化，报"dynamic initialization is not supported for \_\_device\_\_ ... variables"。 |

## 运行时类特性限制<a name="section_cpp11_runtime_restrictions"></a>

**表2**  运行时类特性限制

| 事项 | 支持情况 | 说明 |
| --- | --- | --- |
| 动态内存分配（new/delete/malloc/free） | ❌ | 编译通过但链接失败，Device堆管理符号`dpp_alloc`/`dpp_dealloc`未定义。 |
| 运行时typeid | ❌ | `typeid`本身可取，但`type_info::name()`/`operator==`被标记为`__host__`，无法调用。 |
| STL（标准模板库） | ❌ | 纯SIMT模式下`utils/std/*.h`均带`--enable-simt`编译保护；`std::tuple`构造、`std::move`等被标记为`__host__`。SIMD/SIMT混合编程场景可使用`AscendC::Std`命名空间下的标准库设施，见[Lambda表达式](../cpp_language_feature/lambda_expressions.md)与[C++标准库](../../../../api/Utils-API/cpp_stdlib/cpp_stdlib.md)。 |

## 限制根因分类<a name="section_cpp11_restriction_root_causes"></a>

Device侧的"不支持"按根因分为三类，排查编译报错时可按此定位：

**表3**  限制根因分类

| 根因类别 | 涉及特性 | 典型报错 |
| --- | --- | --- |
| 语言层面直接拒绝 | try/catch、throw | cannot use 'try' in \_\_global\_\_ function |
| Host标准库标记\_\_host\_\_ | std::atomic成员、std::atomic\_thread\_fence、std::initializer\_list::begin/end/size、type\_info::name/operator==、std::move/std::forward、std::tuple构造、GC接口（declare\_no\_pointers等） | call to \_\_host\_\_ function / reference to \_\_host\_\_ function |
| 后端或运行时不支持 | thread\_local（LLVM后端无法选择GlobalTLSAddress节点）、动态new/delete（Device堆符号未定义）、Device全局动态初始化 | error in backend: Cannot select: GlobalTLSAddress / undefined symbol: dpp\_alloc / dynamic initialization is not supported |

> [!NOTE]说明
> 第二类（Host标准库标记`__host__`）是最主要的报错形态。其本质是Host标准库头文件中的函数带有`__host__`标记（多数源自`noexcept`函数属性），并非该语言特性本身缺失——语言层语法（如右值引用）在Device侧完全可用，仅标准库包装不可调用。

## 并发能力替代方案<a name="section_cpp11_concurrency_alternatives"></a>

标准C++11并发形式不可用时，按语义对应关系使用以下SIMT API：

**表4**  C++11并发形式与SIMT API对应关系

| C++11标准形式 | SIMT API替代 | 说明 |
| --- | --- | --- |
| `std::atomic<T>::fetch_add` | `asc_atomic_add`（另有sub/max/min/exch等） | 支持Unified Buffer（`__ubuf__`）与Global Memory（`__gm__`）两种地址空间，int32\_t/uint32\_t/float等类型。 |
| `std::atomic<T>::compare_exchange_strong` | `asc_atomic_cas` | 原子比较赋值，返回目标地址上的初始数据。 |
| `std::atomic_thread_fence` | `asc_threadfence`（设备级）、`asc_threadfence_block`（块级） | 双向内存栅栏。 |
| （C++11无线程块概念） | `asc_syncthreads` | 线程块内同步。 |

API详细说明请参考[原子操作](../../../../api/SIMT-API/atomic_operations/atomic_operations.md)与[同步与内存栅栏](../../../../api/SIMT-API/sync_and_memory_fence/sync_and_memory_fence.md)。
