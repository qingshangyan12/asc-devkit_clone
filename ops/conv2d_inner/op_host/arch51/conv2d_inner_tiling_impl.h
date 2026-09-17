/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2026-2026. All rights reserved.
 * Description:
 */

#ifndef ASCENDC_CONV2D_INNER_TILING_H
#define ASCENDC_CONV2D_INNER_TILING_H

#include <nlohmann/json.hpp>
#include <fstream>
#include "tiling/tiling_base.h"
#include "tiling/tiling_templates_registry.h"
#include "ascendc/exe_graph/runtime/tiling_context.h"
#include "ascendc_tiling_multicore_policy.h"
#include "optiling/math_utils.h"
#include "optiling/type_utils.h"
#include "utils/type_utils.h"
#include "npucl_profile.h"
#include "conv2d_inner_tilingdata.h"
#include "../../op_kernel/arch51/conv2d_inner_tilingkey.h"

namespace optiling {
class Conv2DInnerTiling : virtual public TilingBaseClass {
public:
    explicit Conv2DInnerTiling(gert::TilingContext *context) : TilingBaseClass(context)
    {
    }
    ~Conv2DInnerTiling() override = default;

protected:
    bool IsCapable() override;

    // 1、获取平台信息比如CoreNum、UB/L1/L0C资源大小
    uint32_t GetPlatformInfo() override;

    // 2、获取INPUT/OUTPUT/ATTR信息
    uint32_t GetShapeAttrsInfo() override;

    // 3、计算数据切分TilingData
    /**
     * @brief 算子层级多核 block tiling：
     *        coreNum表示多核数量
     *        shape表示维度信息
     *        tileNum_xx表示block tiling块数，注意是块数
     *        切分后整块大小为factor=CeilDiv(shape.xx, tileNum_xx)(切分cout需要考虑cout0完整性)
     *        切分后尾块大小为tailFactor=shape.xx - (tileNum_xx-1) * factor
     *        CUBE规格 = cube.m0 * cube.k0 * cube.n0
     *        1、生成所有可能的block切分
     *        blockTiles = []
     *        for tileNum_n in range(1, shape.co1 + 1):
     *            for tileNum_ho in range(1, shape.ho + 1):
     *                for tileNum_batch in range(1, shape.batch + 1):
     *                     if tileNum_n * tileNum_ho * tileNum_batch > coreNum:
     *                        break
     *                    blockTiles.append([tileNum_n, tileNum_ho, tileNum_batch])
     *                    if tileNum_n * tileNum_ho * tileNum_batch > coreNum:
     *                        break
     *                 if tileNum_n * tileNum_ho > coreNum:
     *                    break
     *            if tileNum_n > coreNum:
     *                break
     *        2、初步排序blockTiles
     *        1）最终多核切分块数<=coreNum的所有切分方案blockTiles，按照最终多核切分块数从大到小排序
     *        2）最终多核切分块数相同的，按照越高轴切分块数越多的优先级越高排序blockTiles
     *        3、从blockTiles选择最优的切分方案
     *        直接选择blockTiles[0]，简单高效，但存在选择的tiling不是最优的风险。
     *
     * @return 算子层级tiling状态
     */
    uint32_t DoOpTiling() override;

    // 4、计算高阶API的TilingData
    /**
     * @brief 高阶api层级负责核内tiling
     *
     * @return 核内tiling状态
     */
    uint32_t DoLibApiTiling() override;

    // 5、计算TilingKey
    ge::graphStatus GetTplTilingKey() override;
    uint64_t GetTilingKey() const override;

    // 6、计算Workspace 大小
    uint32_t GetWorkspaceSize() override;

    // 7、保存Tiling数据
    uint32_t PostTiling() override;

    // 8、dump Tiling数据
    void DumpTilingInfo() override;

    // 待补充实现
    uint8_t FmapTiling = 0;
    uint8_t WeightTiling = 0;
    uint8_t L1PingPong = 0;
    uint8_t L0PingPong = 0;
    uint8_t OutputOrder = 0;
    uint8_t IterOrder = 0;
    uint8_t Bl1ByPass = 0;
    uint8_t GroupType = 0;

protected:
    void DumpTilingInfoApi(nlohmann::json &apiTiling);
    void DumpTilingInfoOp(nlohmann::json &opTiling);

protected:
    Conv2DInnerTilingData conv2DInnerTilingData_;
};

} // optiling

#endif // ASCENDC_CONV2D_INNER_TILING_H
