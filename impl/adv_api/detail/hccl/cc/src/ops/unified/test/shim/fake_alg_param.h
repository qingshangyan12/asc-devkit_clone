// 伪头：仅在脱离 devkit 编译环境时由 -I 优先命中，提供 unified 链路消费的最小 devkit 类型面。
// 正式件编译时 mc2_client 的真实 include 路径（ops/op_common/inc）先于此目录命中真实 alg_param.h。
// 声明形态与真实头一致：HcclDataType/HcclCMDType 为 typedef enum（非 enum class），
// DATATYPE_SIZE_TABLE 为 constexpr 数组（真实头在 alg_param.h:48）。
#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <map>

using u32 = unsigned int;
using u64 = unsigned long long;

typedef enum {
    HCCL_DATA_TYPE_INT8 = 0, HCCL_DATA_TYPE_INT16 = 1, HCCL_DATA_TYPE_INT32 = 2,
    HCCL_DATA_TYPE_FP16 = 3, HCCL_DATA_TYPE_FP32 = 4, HCCL_DATA_TYPE_INT64 = 5,
    HCCL_DATA_TYPE_UINT64 = 6, HCCL_DATA_TYPE_UINT8 = 7, HCCL_DATA_TYPE_UINT16 = 8,
    HCCL_DATA_TYPE_UINT32 = 9, HCCL_DATA_TYPE_FP64 = 10, HCCL_DATA_TYPE_BFP16 = 11,
    HCCL_DATA_TYPE_RESERVED = 29
} HcclDataType;

constexpr u32 DATATYPE_SIZE_TABLE[HCCL_DATA_TYPE_RESERVED] = {
    1, 2, 4, 2, 4, 8, 8, 1, 2, 4, 8, 2, 0, 0, 0, 1, 1, 1, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0};

typedef enum {
    HCCL_CMD_INVALID = 0, HCCL_CMD_BROADCAST = 1, HCCL_CMD_ALLREDUCE, HCCL_CMD_REDUCE,
    HCCL_CMD_SEND, HCCL_CMD_RECEIVE, HCCL_CMD_ALLGATHER, HCCL_CMD_REDUCE_SCATTER,
    HCCL_CMD_ALLTOALLV, HCCL_CMD_ALLTOALLVC, HCCL_CMD_ALLTOALL
} HcclCMDType;

// 通道级 notify 槽位（真实 alg_param.h:81-85——shim 镜像，值逐项一致。
// R6 起 hcomm_verb_sink.h 同源消费，此处是唯一登记点）
constexpr u32 LOCAL_NOTIFY_IDX_ZERO = 0;
constexpr u32 NOTIFY_IDX_ACK = 0;
constexpr u32 NOTIFY_IDX_DATA_SIGNAL = 1;
constexpr u32 NOTIFY_IDX_FIN_ACK = 2;
constexpr u32 CUSTOM_TIMEOUT = 1836;

// HcclResult（真实定义 hccl_types.h:22-35；取值与名称逐项一致）
typedef enum {
    HCCL_SUCCESS = 0,
    HCCL_E_PARA = 1,
    HCCL_E_PTR = 2,
    HCCL_E_MEMORY = 3,
    HCCL_E_INTERNAL = 4,
    HCCL_E_NOT_SUPPORT = 5,
    HCCL_E_NOT_FOUND = 6,
    HCCL_E_UNAVAIL = 7
} HcclResult;

enum class CommEngine { COMM_ENGINE_AICPU = 0, COMM_ENGINE_CCU, COMM_ENGINE_RESERVED };

enum class Level0Shape { CLOS = 0, MESH_1D = 1, MESH_1D_CLOS = 2 };

struct OpParam {
    int opType = 0;
    int reduceType = 0;
    struct { u64 count = 0; HcclDataType dataType = HCCL_DATA_TYPE_FP16;
             HcclDataType outputType = HCCL_DATA_TYPE_FP16; u64 strideCount = 0; } DataDes;
    bool supportSymmetricMemory = false;
    void* inputPtr = nullptr;      // §10.2 B8：token 计算源（真实 alg_param.h:632 同名）
    void* outputPtr = nullptr;
    u32 userRank = 0;
    CommEngine engine = CommEngine::COMM_ENGINE_AICPU;
};

// 对齐真实头继承关系（alg_param.h:209 TopoInfo 基类持 userRank/userRankSize——
// 均为无默认值 POD，MINOR-2 三轮审查：默认值掩盖未初始化消费）
struct TopoInfo {
    u32 userRank;
    u32 userRankSize;
};
struct TopoInfoWithNetLayerDetails : TopoInfo {
    u32 topoLevelNums = 1;
    Level0Shape level0Topo = Level0Shape::MESH_1D;
};

// ---------------------------------------------------------------------------
// R6 · WP-A1：hcomm_verb_sink.h 消费的最小引擎类型面（真实头在
// hcomm_primitives.h / alg_param.h:444 ChannelInfo——shim 声明形态与真实一致：
// ThreadHandle/ChannelHandle 为 uint64_t typedef，原语为 extern "C" 函数声明）。
// 真实链路（devkit 树内编译）时本 shim 不命中，无副作用。
// ---------------------------------------------------------------------------
#ifndef CHANNEL_HANDLE_DEFINED
#define CHANNEL_HANDLE_DEFINED
typedef uint64_t ChannelHandle;
#endif
#ifndef THREAD_HANDLE_DEFINED
#define THREAD_HANDLE_DEFINED
typedef uint64_t ThreadHandle;
#endif

typedef enum {
    HCOMM_REDUCE_SUM = 0, HCOMM_REDUCE_PROD = 1, HCOMM_REDUCE_MAX = 2,
    HCOMM_REDUCE_MIN = 3, HCOMM_REDUCE_RESERVED = 255
} HcommReduceOp;
typedef enum {
    HCOMM_DATA_TYPE_INT8 = 0, HCOMM_DATA_TYPE_INT16 = 1, HCOMM_DATA_TYPE_INT32 = 2,
    HCOMM_DATA_TYPE_FP16 = 3, HCOMM_DATA_TYPE_FP32 = 4, HCOMM_DATA_TYPE_INT64 = 5,
    HCOMM_DATA_TYPE_UINT64 = 6, HCOMM_DATA_TYPE_UINT8 = 7, HCOMM_DATA_TYPE_UINT16 = 8,
    HCOMM_DATA_TYPE_UINT32 = 9, HCOMM_DATA_TYPE_FP64 = 10, HCOMM_DATA_TYPE_BFP16 = 11,
    HCOMM_DATA_TYPE_RESERVED = 255
} HcommDataType;

typedef enum { HCCL_MEM_TYPE_DEVICE = 0, HCCL_MEM_TYPE_HOST = 1 } HcclMemType;
struct HcclMem {
    HcclMemType type = HCCL_MEM_TYPE_DEVICE;
    void* addr = nullptr;
    uint64_t size = 0;
};
struct ChannelInfo {
    bool isValid = false;
    u32 remoteRank = 0xFFFFFFFFu;
    u32 notifyNum = 0;
    u32 portGroupSize = 1;
    ChannelHandle handle = 0;
    HcclMem remoteCclMem;
};

// 真原语（真实链路 hcomm_primitives.h:87/116/126/226/334/355 同签名；shim 下由
// 测试提供桩实现或仅声明——本 sink 单测在 shim 进程里用自定义桩链接）
extern "C" {
int32_t HcommLocalCopyOnThread(ThreadHandle thread, void* dst, const void* src, uint64_t len);
int32_t HcommReadOnThread(ThreadHandle thread, ChannelHandle channel, void* dst, const void* src, uint64_t len);
int32_t HcommThreadNotifyRecordOnThread(ThreadHandle thread, ThreadHandle dstThread, uint32_t dstNotifyIdx);
int32_t HcommThreadNotifyWaitOnThread(ThreadHandle thread, uint32_t notifyIdx, uint32_t timeOut);
int32_t HcommChannelNotifyRecordOnThread(ThreadHandle thread, ChannelHandle channel, uint32_t remoteNotifyIdx);
int32_t HcommChannelNotifyWaitOnThread(ThreadHandle thread, ChannelHandle channel, uint32_t localNotifyIdx, uint32_t timeOut);
}

// R6 · WP-A2：MakeHcommVerbRes 消费的最小 resCtx 类型面（真实头 alg_param.h:500
// AlgResourceCtxSerializable——shim 只补该函数消费的两字段：threads/channels[0]，
// 其余序列化字段不在 shim 面出现）
// 真头位置：mc2_ops_hccl 域内（alg_param.h:500 namespace mc2_ops_hccl 内）——
# // shim 对齐（此前全局域仅因 verb_sink 的 using 提升；MakeHcommVerbRes 取域内名）
namespace mc2_ops_hccl {
struct AlgHierarchyInfoForAllLevel {
    // 真头 :494-497：infos[level][group] = rank 列表（三维）
    std::vector<std::vector<std::vector<u32>>> infos;
};
struct AlgResourceCtxSerializable {
    std::vector<ThreadHandle> threads;
    std::vector<std::vector<ChannelInfo>> channels;
    HcclMem cclMem;
    AlgHierarchyInfoForAllLevel algHierarchyInfo;
};
} // namespace mc2_ops_hccl
