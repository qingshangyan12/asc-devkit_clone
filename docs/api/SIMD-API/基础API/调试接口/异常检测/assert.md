# assert<a name="ZH-CN_TOPIC_0000001920717221"></a>

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：支持
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2 推理产品：支持
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas 推理系列产品AI Core：支持
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas 推理系列产品Vector Core：不支持
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas 训练系列产品：不支持
<!-- end id7 -->
<!-- npu="x90" id8 -->
- Kirin X90：支持
<!-- end id8 -->
<!-- npu="9030" id9 -->
- Kirin 9030：支持
<!-- end id9 -->

## 功能说明<a name="section259105813316"></a>

头文件路径为：`"utils/debug/asc_assert.h"`。

该接口实现CPU/NPU域assert断言功能。算子执行中，如果assert内部条件判断不为真，则输出assert条件并将输入的信息格式化打印在屏幕上。

在算子Kernel侧实现代码中需要增加断言的地方使用assert检查代码，并格式化输出一些调测信息。示例如下：

```cpp
int assertFlag = 10;

assert(assertFlag == 10);
```

> [!CAUTION]注意 
> 该接口主要用于调试分析，开启后会对算子性能产生一定影响，通常在调试阶段使用，生产环境建议关闭。<br>
> 默认情况下，调用该接口就会打印相关内容，开发者可以参考[关闭ASCENDC_DUMP说明](../关闭ASCENDC_DUMP说明.md)，按需关闭该接口功能。

## 函数原型<a name="section2067518173415"></a>

```cpp
define assert(...)
```

## 参数说明<a name="section158061867342"></a>

**表1**  参数说明

| 参数名称 | 输入/输出 | 描述 |
| ------ | ------ | ------ |
| ... | 输入 | assert断言是否终止程序的条件。为true则程序继续执行，为false则终止程序。|

## 数据类型

无

## 返回值说明<a name="section640mcpsimp"></a>	 

无

## 约束说明<a name="section43265506459"></a>

- 单次调用本接口打印的数据总量不可超过30KB。使用时应注意，如果超出这个限制，则数据不会被打印。
- 在cmake工程的release模式下，默认使能NDEBUG宏定义，assert打印信息失效。

## 调用示例<a name="section82241477610"></a>

```cpp
int assertFlag = 10;
// 断言条件
assert(assertFlag != 10);
```

程序运行时会触发assert，打印效果如下：

```plain
[ASSERT] /home/.../add_custom.cpp:44: Assertion `assertFlag != 10' 
```
