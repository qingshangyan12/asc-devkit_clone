struct HcclTeamTagWorld {};
struct HcclTeamTagRail {};

typedef struct {
    uint32_t nRanks;
    uint32_t rank;
    uint32_t stride;
    void *ranks;
} HcclTeam;

// extern AIN_DEVICE HcclTeam HcclTeamWorld(HcclDevComm const& devComm);
// extern AIN_DEVICE HcclTeam HcclTeamRail(HcclDevComm const& devComm);

struct HcclAinBarrierHandle {
    uint32_t signal0;
    uint32_t reserved;
};

typedef struct {
    // CommAbiHeader abiHeader;
    uint32_t rankSize;
    uint32_t rankId;
    void* AivRes;
    // 省略其他暂未用到的成员
} HcclDevComm;

typedef struct {
    ChannelEntity **entity;
    uint32_t* entityNumPerRank;
    int ainSignalCount;
    uint64_t* ainSignalShadows;
    HcclAinBarrierHandle ainWorldBarrier;
    HcclAinBarrierHandle ainRailBarrier;
    HcclAinBarrierHandle ainCustomBarrier;
    uint64_t* symMemPool;
    // 省略其他暂未用到的成员
} AivRes;

enum HcclResult {
    HCCL_SUCCESS = 0,
    HCCL_TIMEOUT = 1
};
