# GetResGroupBarrierWorkSpaceSize<a name="ZH-CN_TOPIC_0000002078653646"></a>

<!-- md-trans-meta sourceCommit=ec5513f20d06e4751ee8b926efe401753e00bd6b translatedAt=2026-10-23T03:09:22.134Z pushedAt=2026-10-23T07:01:40.542Z nl=2 -->

## Description<a name="zh-cn_topic_0000001969805546_zh-cn_topic_0000001391767420_section36583473819"></a>

Obtains the workspace size required by [GroupBarrier](../../../SIMD-API/basic_api/cube_group_mgmt_ISASI/GroupBarrier/GroupBarrier.md).

## Prototype<a name="zh-cn_topic_0000001969805546_zh-cn_topic_0000001391767420_section13230182415108"></a>

```
uint32_t GetResGroupBarrierWorkSpaceSize(void) const
```

## Parameters<a name="zh-cn_topic_0000001969805546_zh-cn_topic_0000001391767420_section189014013619"></a>

None

## Return Value<a name="zh-cn_topic_0000001969805546_zh-cn_topic_0000001391767420_section25791320141317"></a>

The workspace size required by the current GroupBarrier. The API returns the workspace size on success. If `std::numeric_limits<uint32_t>::max()` (that is, `UINT32_MAX`) is returned, the API has failed.

## Constraints<a name="zh-cn_topic_0000001969805546_zh-cn_topic_0000001391767420_section19165124931511"></a>

None

## Examples<a name="zh-cn_topic_0000001969805546_zh-cn_topic_0000001391767420_section320753512363"></a>

### Custom Operator Project

```cpp
// User-defined tiling function.
static ge::graphStatus TilingFunc(gert::TilingContext* context)
{
    AddApiTiling tiling;
    ...
    // To use the system workspace, call GetLibApiWorkSpaceSize to obtain the system workspace size.
    auto ascendcPlatform = platform_ascendc:: PlatformAscendC(context->GetPlatformInfo());
    uint32_t sysWorkspaceSize = ascendcPlatform.GetLibApiWorkSpaceSize();
    if (sysWorkspaceSize == std::numeric_limits<uint32_t>::max()) {
        return ge::GRAPH_FAILED;
    }
    // Set the sum of the workspace required by the user and the size required by GroupBarrier as the total usrWorkspace size.
    uint32_t groupBarrierWorkspaceSize = ascendcPlatform.GetResGroupBarrierWorkSpaceSize();
    if (groupBarrierWorkspaceSize == std::numeric_limits<uint32_t>::max()) {
        return ge::GRAPH_FAILED;
    }
    size_t usrSize = 256 + groupBarrierWorkspaceSize; // Set the workspace size required by the user.
    size_t *currentWorkspace = context->GetWorkspaceSizes(1); // Obtain the workspace pointer through the framework. The input of GetWorkspaceSizes is the number of workspace blocks required. Currently, only one block is supported.
    currentWorkspace[0] = usrSize + sysWorkspaceSize; // Set the total workspace size. The total workspace space is applied for and managed by the framework.
    ...
}
```

### Kernel Direct Call

```cpp
uint32_t GetGroupBarrierWorkspaceSize()
{
    auto* ascendcPlatform = platform_ascendc::PlatformAscendCManager::GetInstance();
    if (ascendcPlatform == nullptr) {
        return std::numeric_limits<uint32_t>::max();
    }
    return ascendcPlatform->GetResGroupBarrierWorkSpaceSize();
}
```
