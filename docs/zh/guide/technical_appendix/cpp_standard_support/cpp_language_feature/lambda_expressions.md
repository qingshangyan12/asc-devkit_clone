# Lambda表达式<a name="ZH-CN_TOPIC_000000253823119003"></a>

本文介绍Lambda表达式在Ascend C中的使用方式。SIMT Device侧支持C++11 Lambda表达式的完整形态——值捕获、引用捕获、mutable、嵌套、立即调用（IIFE）、无捕获lambda转函数指针、捕获this、作为函数模板实参等均可直接使用；C++14泛型lambda与init-capture、C++17 constexpr lambda与`[*this]`按值捕获同样可用（见各标准章节）。

Ascend C的Lambda是**Device侧局部闭包**：核函数（Kernel）参数仅支持基础类型、POD结构体与指针（见[核函数（Kernel）参数列表限制](../cpp_language_restrictions/general_restrictions.md#section_kernel_function_argument_list_constraint)），不存在"Host侧lambda传入核函数"的用法（即无`--extended-lambda`机制的等价物，试图传递会报`lambda type cannot be used as template argument for kernel function`），因此也没有跨执行空间的Lambda限制与闭包类型traits。需要按参数定制计算时，将标量参数直接传入核函数，在核函数内定义Lambda消费参数。

SIMT编程与SIMD与SIMT混合编程场景场景下Lambda的使用约束略有不同。下文将会展开说明。

## SIMT编程场景<a name="section_lambda_pure_simt"></a>

SIMT编程场景下，核函数与`__aicore__`函数内定义的Lambda表达式无需额外标记，直接使用即可：

```cpp
__global__ __launch_bounds__(THREAD_COUNT) void kernel(float* output)
{
    float scale = 2.f;
    float bias = 1.f;
    // 核函数内lambda：值捕获，无需标记函数属性
    auto affine = [scale, bias](float v) { return v * scale + bias; };
    // 引用捕获：可修改外部变量
    float counter = 0.f;
    auto bump = [&counter]() { counter += 1.f; };
    bump();
    // 无捕获lambda可转函数指针
    float (*fn)(float, float) = [](float a, float b) { return a + b; };
    output[threadIdx.x] = affine(3.f) + counter + fn(10.f, 5.f);
}
```

SIMT场景的限制如下：
- lambda体内不可使用try/throw、动态分配、TLS等Device侧不支持的能力；
- namespace-scope的lambda对象受全局变量规则约束（非constexpr闭包报`global variable is not allowed in aicore function`，constexpr闭包可用）。

Lambda捕获在实际算子开发中的用法请参考[Lambda捕获实战样例](../../../../../../examples/03_simt_api/02_features/02_language_features/00_lambda_operator_capture/README.md)。

## SIMD与SIMT混合编程场景场景<a name="section_lambda_hybrid"></a>

SIMD与SIMT混合编程场景场景下，Device侧各执行上下文中定义的Lambda表达式**必须显式标记函数属性**，否则编译报错。标记的选择规则如下：

**表1**  混合编程Lambda标记选择规则

| Lambda定义位置 | 应使用的标记 | 说明 |
| --- | --- | --- |
| `__simt_vf__`函数体内 | `__simt_callee__` | 与所在VF函数的callee标记一致。 |
| `__simd_vf__`函数体内 | `__simd_callee__` | 同上。 |
| `__simt_callee__`/`__simd_callee__`/`__aicore__`辅助函数体内 | `__simt_callee__`/`__simd_callee__`/`__aicore__`（与宿主函数一致） | 混合编程中`__aicore__`辅助函数内的lambda需标记`__aicore__`。 |
| 混合编程核函数体内（`__global__`的SIMD侧scalar代码） | `__aicore__` | 未标记报`call to __host__ function from __global__ __aicore__ function`。 |
| `__callee__`函数体内 | `__callee__` | —— |

标记位置在Lambda的参数列表之后：

```cpp
__simt_vf__ __launch_bounds__(THREAD_COUNT) inline void simt_lambda_by_value(
    __ubuf__ float* in, __ubuf__ float* out, uint32_t count, float scale, float bias)
{
    if (threadIdx.x >= count) {
        return;
    }
    // __simt_callee__ 标识该lambda函数属性
    auto affine = [scale, bias](float v) __simt_callee__ {
        return v * scale + bias; };
    out[threadIdx.x] = affine(in[threadIdx.x]);
}
```

Host侧通过`asc_vf_call`启动上述SIMT VF函数：

```cpp
__global__ __vector__ void kernel(__gm__ float* input, __gm__ float* output)
{
    __ubuf__ float ub_in[THREAD_COUNT];
    __ubuf__ float ub_out[THREAD_COUNT];
    for (uint32_t i = 0; i < THREAD_COUNT; ++i) {
        ub_in[i] = input[i];
    }
    asc_vf_call<simt_lambda_by_value>(dim3(THREAD_COUNT), ub_in, ub_out, THREAD_COUNT, 2.f, 1.f);
    ...
}
```

混合编程下Lambda捕获的完整可运行样例（含UB与GM之间的数据搬运、流水同步）请参考[混合编程Lambda捕获实战样例](../../../../../../examples/05_simd_simt_hybrid/03_features/lambda_operator_hybrid/README.md)。


### VF函数内支持的Lambda能力<a name="section_lambda_hybrid_list"></a>

混合场景下，VF函数内支持的lambda能力有限制，如下表所示：

**表2**  混合编程VF函数内Lambda能力列表

| 能力 | 结论 | 写法/说明 |
| --- | --- | --- |
| 值捕获标量 | ✅ | `[scale, bias](float v) __simt_callee__ { ... }`。 |
| 引用捕获 | ✅ | `[&counter]`可修改VF函数内局部变量；与CUDA extended device lambda"只能值捕获"不同，Ascend C允许引用捕获。 |
| 捕获`__ubuf__`/`__gm__`指针 | ✅ | lambda体内可直接访存UB/GM；指针来源需为VF函数形参或合法局部指针。 |
| 捕获this / `[*this]` | ✅ | `__simt_callee__`成员函数内`[this]`或C++17 `[*this]`均可用。 |
| mutable | ✅ | 标记在`mutable`**之前**：`[m]() __simt_callee__ mutable { ... }`；写在`mutable`之后是语法错误（`expected body of lambda expression`）。 |
| 泛型lambda（C++14） | ✅ | `[](auto x) __simt_callee__ { ... }`。 |
| init-capture（C++14） | ✅ | `[sum = base * 2.f](float x) __simt_callee__ { ... }`。 |
| 嵌套lambda | ✅ | 外层返回内层时，**内外层均需携带标记**。 |
| 立即调用（IIFE） | ✅ | `[](float a) __simt_callee__ { ... }(2.f)`。 |
| 后置返回类型 | ✅ | 标记在`-> T`之前：`[](float x) __simt_callee__ -> float { ... }`。 |
| 作为函数模板实参 | ✅ | 可传给`__simt_callee__`/`__simd_callee__`函数模板（如`applyTwice(f, v)`）。 |
| callee-lambda调用callee-lambda | ✅ | 捕获另一闭包对象直接调用。 |
| `__simt_callee__`函数返回lambda | ✅ | `__simt_callee__ auto makeAdder(float bias)`返回闭包对象。 |
| constexpr lambda（C++17） | ⚠️ | `static_assert`与结果落`constexpr`变量后使用均可用；**不要**在VF内直接运行期调用constexpr闭包（实测个别版本结果错误，见下文注意事项）。 |
| SIMD VF内调用矢量intrinsics | ✅ | `__simd_callee__` lambda体内可使用`asc_duplicate_scalar`/`asc_mul_scalar`/`asc_storealign`等并写UB。 |
| 无参数列表形式 | ⚠️ | `[v] __simt_callee__ { ... }`可编译但触发C++2b扩展告警；C++17下应写显式`()`。 |
| callee标记lambda转函数指针 | ❌ | 标记使闭包类型失去向函数指针的转换，报`no viable conversion`；需要函数指针形态时改用普通callee函数。 |
| namespace-scope lambda对象 | ❌ | 闭包对象是全局变量，报`global variable ... is not allowed in aicore function`；`constexpr`闭包可用。 |

### 使用约束<a name="section_lambda_constraints"></a>

-   **constexpr lambda先落constexpr变量**：在VF函数内使用constexpr lambda时，将结果先赋给`constexpr`局部变量（`constexpr int32_t r64 = sq(8);`）再参与计算；不要在运行期路径直接调用constexpr闭包。
-   **lambda的类型不可跨执行空间使用**：标记callee的Lambda只能在对应VF函数及其callee链内定义和使用；纯SIMT核函数内直接编写的Lambda无需（也不能）携带callee标记。
-   **泛型Lambda与init-capture为C++14特性**：C++11语法范围内的Lambda（本节所述形态）在Device侧完整支持；`auto`参数与初始化捕获等C++14扩展见[C++14语言特性](./cpp14_language_feature.md)。
-   **Lambda与AscendC::Std**：混合编程场景下，Lambda可正常操作`AscendC::Std`标准库设施（如tuple，可在lambda体内`make_tuple`/`get`），但注意tuple模板实参不能携带UB地址空间限定类型，需先取值为普通标量局部变量再聚合，详见[混合编程Lambda捕获实战样例](../../../../../../examples/05_simd_simt_hybrid/03_features/lambda_operator_hybrid/README.md)说明。
