# GetICachePreloadStatus\(ISASI\)<a name="ZH-CN_TOPIC_0000001788032978"></a>

## 产品支持情况<a name="section1550532418810"></a>

| 产品 | 是否支持  |
| :----------------------- | :------: |
|<!-- npu="950" id100 -->Ascend 950PR/Ascend 950DT | √<!-- end id100 -->|
|<!-- npu="A3" id101 -->Atlas A3 训练系列产品/Atlas A3 推理系列产品 | √<!-- end id101 -->|
|<!-- npu="910b" id102 -->Atlas A2 训练系列产品/Atlas A2 推理系列产品 | √<!-- end id102 -->|
|<!-- npu="310b" id103 -->Atlas 200I/500 A2 推理产品 | x<!-- end id103 -->|
|<!-- npu="310p" id104 -->Atlas 推理系列产品AI Core | x<!-- end id104 -->|
|<!-- npu="310p" id105 -->Atlas 推理系列产品Vector Core | x<!-- end id105 -->|
|<!-- npu="910" id106 -->Atlas 训练系列产品 | x<!-- end id106 -->|

## 功能说明<a name="section618mcpsimp"></a>

头文件路径为：`"basic_api/kernel_operator_cache_intf.h"`。

GetICachePreloadStatus为调试接口，在ICachePreLoad后调用，用于获取ICache的PreLoad的状态：当返回值为0时，说明ICache的PreLoad已完成；当返回值为1时，说明ICache的PreLoad未完成。

## 函数原型<a name="section620mcpsimp"></a>

```cpp
__aicore__ inline int64_t GetICachePreloadStatus()
```

## 参数说明<a name="section622mcpsimp"></a>

无

## 返回值说明<a name="section640mcpsimp"></a>

int64\_t类型，0表示ICache的PreLoad已完成，1表示ICache的PreLoad未完成。

## 约束说明<a name="section633mcpsimp"></a>

无

## 调用示例<a name="section837496171220"></a>

```cpp
// 获取ICache预加载状态，0表示已完成，1表示未完成。
int64_t cachePreloadStatus = AscendC::GetICachePreloadStatus();
```
