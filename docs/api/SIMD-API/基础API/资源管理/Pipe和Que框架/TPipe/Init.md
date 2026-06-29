# Init<a name="ZH-CN_TOPIC_0000001717287036"></a>

## 产品支持情况<a name="section1550532418810"></a>

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
- Atlas 训练系列产品：支持
<!-- end id7 -->
<!-- npu="x90" id8 -->
- Kirin X90：支持
<!-- end id8 -->
<!-- npu="9030" id9 -->
- Kirin 9030：支持
<!-- end id9 -->

## 功能说明<a name="section618mcpsimp"></a>

头文件路径为：`"basic_api/kernel_tpipe.h"`。

用于内存和同步流水事件EventID的初始化。

## 函数原型<a name="section620mcpsimp"></a>

```
__aicore__ inline void Init()
```

## 约束说明<a name="section633mcpsimp"></a>

重复申请释放tpipe，要与[Destroy](Destroy.md)接口成对使用，tpipe如果要重复申请需要先Destroy释放后再Init。

## 返回值说明<a name="section640mcpsimp"></a>

无

## 调用示例<a name="section642mcpsimp"></a>

完整算子样例参考：[TPipeReuse](https://gitcode.com/cann/asc-devkit/tree/master/examples/01_simd_cpp_api/03_basic_api/07_tpipe_tque/tpipe_reuse)。

```
// 实例化一个浮点型的自定义算子对象
KernelTPipeInit<float> op;
uint32_t srcSize = 128;

// 第一次执行流程：使用pipeIn管道
AscendC::TPipe pipeIn;
pipeIn.Init();  // 初始化pipeIn管道资源
op.Init(x, z, srcSize, &pipeIn);
op.Process();
pipeIn.Destroy();  // 销毁pipeIn管道资源

// 第二次执行流程：复用算子对象，但更换为pipeCast管道
AscendC::TPipe pipeCast;
pipeCast.Init();  // 初始化pipeCast管道资源
op.Init(x, z, srcSize, &pipeCast);
op.Process();
pipeCast.Destroy();  // 销毁pipeCast管道资源
```
