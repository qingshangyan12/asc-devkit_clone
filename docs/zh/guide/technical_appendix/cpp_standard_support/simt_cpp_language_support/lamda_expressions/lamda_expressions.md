# Lambda表达式<a name="ZH-CN_TOPIC_000000253823119003"></a>

本文介绍Lambda表达式在Ascend C中的使用方式。SIMT Device侧支持C++11 Lambda表达式的完整形态——值捕获、引用捕获、mutable、嵌套、立即调用（IIFE）、无捕获lambda转函数指针、捕获this、作为函数模板实参等均可直接使用。不同编程场景下Lambda的编写要求不同，分为纯SIMT编程与SIMD/SIMT混合编程两种情况。

## 纯SIMT编程场景<a name="section_lambda_pure_simt"></a>

纯SIMT编程（编译时带`--enable-simt`）场景下，核函数（Kernel）内定义的Lambda表达式无需额外标记，直接使用即可：

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

完整形态验证（mutable、嵌套、立即调用、捕获this、作为模板实参等）请参考[Lambda与字面量特性样例](../../../../../../../examples/03_simt_api/02_features/02_language_features/03_cpp11_lambda_literals/README.md)。

## SIMD/SIMT混合编程场景<a name="section_lambda_hybrid"></a>

SIMD/SIMT混合编程场景下，SIMD VF函数与SIMT VF函数（`__simd_vf__`/`__simt_vf__`标记的函数）体内定义的Lambda表达式**必须显式标记函数属性**，否则编译报错。可用的标记包括：

-   `__aicore__`：标记函数在Device侧执行。
-   `__simd_callee__`：SIMD VF函数内部调用的Lambda。
-   `__simt_callee__`：SIMT VF函数内部调用的Lambda。

标记位置在Lambda的参数列表之后，示例如下：

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

完整可运行样例（含UB与GM之间的数据搬运、流水同步）请参考[混合编程lambda与标准库样例](../../../../../../../examples/03_simt_api/02_features/02_language_features/07_cpp11_hybrid_stdlib/README.md)。

混合编程的`__simt_vf__`函数本身的参数与调用约束（标量参数不允许指针和引用、指针形参需显式地址空间限定等）与Lambda无关，请参考[SIMT VF函数限制](../../syntax_restrictions/functions.md#section_simt_vf_constraint)。

## 使用约束<a name="section_lambda_constraints"></a>

-   **lambda的类型不可跨执行空间使用**：标记`__simt_callee__`的Lambda只能在对应的SIMT VF函数内定义和使用；纯SIMT核函数内直接编写的Lambda无需（也不能）携带callee标记。
-   **泛型Lambda与init-capture为C++14特性**：C++11语法范围内的Lambda（本节所述形态）在Device侧完整支持；`auto`参数与初始化捕获等C++14扩展见C++14相关章节。
-   **Lambda与AscendC::Std**：混合编程场景下，Lambda可正常操作`AscendC::Std`标准库设施（如tuple），但注意tuple模板实参不能携带UB地址空间限定类型，需先取值为普通标量局部变量再聚合，详见[混合编程样例](../../../../../../../examples/03_simt_api/02_features/02_language_features/07_cpp11_hybrid_stdlib/README.md)说明。
