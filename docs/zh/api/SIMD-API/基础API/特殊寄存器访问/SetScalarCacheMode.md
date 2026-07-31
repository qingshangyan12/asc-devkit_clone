# SetScalarCacheMode

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：不支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：不支持
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2 推理产品：不支持
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas 推理系列产品AI Core：不支持
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas 推理系列产品Vector Core：不支持
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas 训练系列产品：不支持
<!-- end id7 -->
<!-- npu="x90" id8 -->
- Kirin X90：不支持
<!-- end id8 -->
<!-- npu="9030" id9 -->
- Kirin 9030：不支持
<!-- end id9 -->

## 功能说明

头文件路径为：`"basic_api/kernel_operator_common_intf.h"`。

设置L2 Cache的读写缓存模式。

## 函数原型

```cpp
template <CacheRwMode rwMode, CacheMode cacheMode>
__aicore__ inline void SetScalarCacheMode()
```

## 参数说明

**表1** 模板参数说明

| 参数名 | 描述 |
|--------|------|
| rwMode | L2 Cache读写模式，类型为CacheRwMode枚举类，定义如下：<br><pre>enum class CacheRwMode {<br>    READ = 1,<br>    WRITE = 2,<br>    RW = 3<br>};<br></pre>注意：本接口仅支持READ或WRITE，不支持RW。 |
| cacheMode | 用户指定的L2 Cache模式。<br><pre>enum class CacheMode {<br>    CACHE_MODE_DISABLE = 0, // 不启用L2 Cache<br>    CACHE_MODE_NORMAL = 1, // 启用L2 Cache，并且将分配的Cache Line标记为高替换优先级。<br>    CACHE_MODE_LAST = 2, // 启用L2 Cache，并且将分配的Cache Line标记为低替换优先级。<br>    CACHE_MODE_PERSISTENT = 4 // 启用L2 Cache驻留模式<br>};<br></pre>如果不调用该接口，默认为CacheMode::CACHE_MODE_NORMAL。|

## 数据类型

本接口为寄存器访问接口，不涉及张量数据类型操作。

## 返回值说明

无

## 约束说明

模板参数CacheRwMode仅支持READ或WRITE，不支持RW。

## 调用示例

如下示例中分别设置读缓存模式为LAST、写缓存模式为PERSISTENT。

```cpp
AscendC::SetScalarCacheMode<AscendC::CacheRwMode::READ, AscendC::CacheMode::CACHE_MODE_LAST>();
AscendC::SetScalarCacheMode<AscendC::CacheRwMode::WRITE, AscendC::CacheMode::CACHE_MODE_PERSISTENT>();
```
