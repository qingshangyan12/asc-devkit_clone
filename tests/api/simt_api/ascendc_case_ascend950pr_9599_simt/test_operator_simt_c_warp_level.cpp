/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/
#include <gtest/gtest.h>
#include "simt_compiler_stub.h"
#include "simt_api/asc_simt.h"

using namespace std;
using namespace AscendC;
using namespace AscendC::Simt;

int THREAD_DIM=128;

// ================================ Test int32_t start ================================
struct WarpLevelcComputeParams_int32 {
    int32_t mode = 0;

    // for Warp vote function
    int32_t predicate = 0;

    // for Warp reduce and shuffle function
    int32_t laneOp = 0;
    int32_t width = 0;
};

class WarpLevelcComputeTestsuite_int32 : public testing::Test, public testing::WithParamInterface<WarpLevelcComputeParams_int32> {
protected:
    void SetUp() {}
    void TearDown() {}
};

INSTANTIATE_TEST_CASE_P(WarpLevelcComputeTestCase_int32, WarpLevelcComputeTestsuite_int32,
    ::testing::Values(WarpLevelcComputeParams_int32 {.mode = 0, .predicate = -1},
        WarpLevelcComputeParams_int32 {.mode = 0, .predicate = 0},
        WarpLevelcComputeParams_int32 {.mode = 0, .predicate = 1},

        WarpLevelcComputeParams_int32 {.mode = 1, .predicate = -1},
        WarpLevelcComputeParams_int32 {.mode = 1, .predicate = 0},
        WarpLevelcComputeParams_int32 {.mode = 1, .predicate = 1},

        WarpLevelcComputeParams_int32 {.mode = 2, .predicate = -1},
        WarpLevelcComputeParams_int32 {.mode = 2, .predicate = 0},
        WarpLevelcComputeParams_int32 {.mode = 2, .predicate = 1},

        WarpLevelcComputeParams_int32 {.mode = 3},

        WarpLevelcComputeParams_int32 {.mode = 4, .laneOp = 1, .width = 16},
        WarpLevelcComputeParams_int32 {.mode = 4, .laneOp = 1, .width = 32},

        WarpLevelcComputeParams_int32 {.mode = 5, .laneOp = 1, .width = 16},
        WarpLevelcComputeParams_int32 {.mode = 5, .laneOp = 1, .width = 32},

        WarpLevelcComputeParams_int32 {.mode = 6, .laneOp = 1, .width = 16},
        WarpLevelcComputeParams_int32 {.mode = 6, .laneOp = 1, .width = 32},

        WarpLevelcComputeParams_int32 {.mode = 7, .laneOp = 1, .width = 16},
        WarpLevelcComputeParams_int32 {.mode = 7, .laneOp = 1, .width = 32},

        WarpLevelcComputeParams_int32 {.mode = 8},

        WarpLevelcComputeParams_int32 {.mode = 9},

        WarpLevelcComputeParams_int32 {.mode = 10}
    ));

TEST_P(WarpLevelcComputeTestsuite_int32, WarpLevelcComputeTestCase_int32)
{
    auto param = GetParam();

    int typeSize = 4;
    int shapeSize = THREAD_DIM;
    uint8_t dstGM[shapeSize * typeSize] = {0};

    for (int i = 0; i < 256; i += 2) {
        if (i % 16 == 0 && i != 0) {
            printf("\n");
        }
        printf("%02x%02x ", dstGM[i], dstGM[i+1]);
    }

    int32_t expectValues[shapeSize] = {0};
    switch (param.mode) {
        case 0:
            for (int i = 0; i < shapeSize; i += 1) {
                if (param.predicate == -1 || param.predicate == 0) {
                    expectValues[i] = 0;
                }
                if (param.predicate > 0) {
                    expectValues[i] = 1;
                }
            }
            break;
        case 1:
            for (int i = 0; i < shapeSize; i += 1) {
                if (param.predicate == 0) {
                    expectValues[i] = 0;
                } else {
                    expectValues[i] = 1;
                }
            }
            break;
        case 2:
            for (int i = 0; i < shapeSize; i += 1) {
                if (param.predicate == -1) {
                    expectValues[i] = 0xfffffffe;
                }
                if (param.predicate == 0) {
                    expectValues[i] = 0;
                }
                if (param.predicate > 0) {
                    expectValues[i] = 0xffffffff;
                }
            }
            break;
        case 3:
            for (int i = 0; i < shapeSize; i += 1) {
                expectValues[i] = 0xffffffff;
            }
            break;
        case 4:
            for (int i = 0; i < shapeSize; i += 1) {
                int32_t laneId = i % 32;
                expectValues[i] = laneId / param.width * param.width + param.laneOp % param.width;
            }
            break;
        case 5:
            for (int i = 0; i < shapeSize; i += 1) {
                int32_t laneId = i % 32;
                int32_t groupStart = laneId / param.width * param.width;
                if (laneId - param.laneOp >= groupStart) {
                    expectValues[i] = laneId - param.laneOp;
                } else {
                    expectValues[i] = laneId;
                }
            }
            break;
        case 6:
            for (int i = 0; i < shapeSize; i += 1) {
                int32_t laneId = i % 32;
                int32_t groupEnd = laneId / param.width * param.width + param.width;
                if (laneId + param.laneOp < groupEnd) {
                    expectValues[i] = laneId + param.laneOp;
                } else {
                    expectValues[i] = laneId;
                }
            }
            break;
        case 7:
            for (int i = 0; i < shapeSize; i += 1) {
                int32_t laneId = i % 32;
                expectValues[i] = laneId ^ param.laneOp;
            }
            break;
        case 8:
            for (int i = 0; i < shapeSize; i += 1) {
                expectValues[i] = 496;
            }
            break;
        case 9:
            for (int i = 0; i < shapeSize; i += 1) {
                expectValues[i] = 31;
            }
            break;
        case 10:
            //  expect value is 0
            break;
        default:
            break;
    }
}
// ================================ Test int32_t end ================================

// ================================ Test uint32_t start ================================
struct WarpLevelcComputeParams_uint32 {
    int32_t mode = 0;

    // for Warp vote function
    int32_t predicate = 0;

    // for Warp reduce and shuffle function
    int32_t laneOp = 0;
    int32_t width = 0;
};

class WarpLevelcComputeTestsuite_uint32 : public testing::Test, public testing::WithParamInterface<WarpLevelcComputeParams_uint32> {
protected:
    void SetUp() {}
    void TearDown() {}
};

INSTANTIATE_TEST_CASE_P(WarpLevelcComputeTestCase_uint32, WarpLevelcComputeTestsuite_uint32,
    ::testing::Values(WarpLevelcComputeParams_uint32 {.mode = 0, .predicate = -1},
        WarpLevelcComputeParams_uint32 {.mode = 0, .predicate = 0},
        WarpLevelcComputeParams_uint32 {.mode = 0, .predicate = 1},

        WarpLevelcComputeParams_uint32 {.mode = 1, .predicate = -1},
        WarpLevelcComputeParams_uint32 {.mode = 1, .predicate = 0},
        WarpLevelcComputeParams_uint32 {.mode = 1, .predicate = 1},

        WarpLevelcComputeParams_uint32 {.mode = 2, .predicate = -1},
        WarpLevelcComputeParams_uint32 {.mode = 2, .predicate = 0},
        WarpLevelcComputeParams_uint32 {.mode = 2, .predicate = 1},

        WarpLevelcComputeParams_uint32 {.mode = 3},

        WarpLevelcComputeParams_uint32 {.mode = 4, .laneOp = 1, .width = 16},
        WarpLevelcComputeParams_uint32 {.mode = 4, .laneOp = 1, .width = 32},

        WarpLevelcComputeParams_uint32 {.mode = 5, .laneOp = 1, .width = 16},
        WarpLevelcComputeParams_uint32 {.mode = 5, .laneOp = 1, .width = 32},

        WarpLevelcComputeParams_uint32 {.mode = 6, .laneOp = 1, .width = 16},
        WarpLevelcComputeParams_uint32 {.mode = 6, .laneOp = 1, .width = 32},

        WarpLevelcComputeParams_uint32 {.mode = 7, .laneOp = 1, .width = 16},
        WarpLevelcComputeParams_uint32 {.mode = 7, .laneOp = 1, .width = 32},

        WarpLevelcComputeParams_uint32 {.mode = 8},

        WarpLevelcComputeParams_uint32 {.mode = 9},

        WarpLevelcComputeParams_uint32 {.mode = 10}
    ));

TEST_P(WarpLevelcComputeTestsuite_uint32, WarpLevelcComputeTestCase_uint32)
{
    auto param = GetParam();

    int typeSize = 4;
    int shapeSize = THREAD_DIM;
    uint8_t dstGM[shapeSize * typeSize] = {0};

    for (int i = 0; i < 256; i += 2) {
        if (i % 16 == 0 && i != 0) {
            printf("\n");
        }
        printf("%02x%02x ", dstGM[i], dstGM[i+1]);
    }

    uint32_t expectValues[shapeSize] = {0};
    switch (param.mode) {
        case 0:
            for (int i = 0; i < shapeSize; i += 1) {
                if (param.predicate == -1 || param.predicate == 0) {
                    expectValues[i] = 0;
                }
                if (param.predicate > 0) {
                    expectValues[i] = 1;
                }
            }
            break;
        case 1:
            for (int i = 0; i < shapeSize; i += 1) {
                if (param.predicate == 0) {
                    expectValues[i] = 0;
                } else {
                    expectValues[i] = 1;
                }
            }
            break;
        case 2:
            for (int i = 0; i < shapeSize; i += 1) {
                if (param.predicate == -1) {
                    expectValues[i] = 0xfffffffe;
                }
                if (param.predicate == 0) {
                    expectValues[i] = 0;
                }
                if (param.predicate > 0) {
                    expectValues[i] = 0xffffffff;
                }
            }
            break;
        case 3:
            for (int i = 0; i < shapeSize; i += 1) {
                expectValues[i] = 0xffffffff;
            }
            break;
        case 4:
            for (int i = 0; i < shapeSize; i += 1) {
                int32_t laneId = i % 32;
                expectValues[i] = laneId / param.width * param.width + param.laneOp % param.width;
            }
            break;
        case 5:
            for (int i = 0; i < shapeSize; i += 1) {
                int32_t laneId = i % 32;
                int32_t groupStart = laneId / param.width * param.width;
                if (laneId - param.laneOp >= groupStart) {
                    expectValues[i] = laneId - param.laneOp;
                } else {
                    expectValues[i] = laneId;
                }
            }
            break;
        case 6:
            for (int i = 0; i < shapeSize; i += 1) {
                int32_t laneId = i % 32;
                int32_t groupEnd = laneId / param.width * param.width + param.width;
                if (laneId + param.laneOp < groupEnd) {
                    expectValues[i] = laneId + param.laneOp;
                } else {
                    expectValues[i] = laneId;
                }
            }
            break;
        case 7:
            for (int i = 0; i < shapeSize; i += 1) {
                int32_t laneId = i % 32;
                expectValues[i] = laneId ^ param.laneOp;
            }
            break;
        case 8:
            for (int i = 0; i < shapeSize; i += 1) {
                expectValues[i] = 496;
            }
            break;
        case 9:
            for (int i = 0; i < shapeSize; i += 1) {
                expectValues[i] = 31;
            }
            break;
        case 10:
            //  expect value is 0
            break;
        default:
            break;
    }
}
// ================================ Test uint32_t end ================================

// ================================ Test float start ================================
struct WarpLevelcComputeParams_float {
    int32_t mode = 0;

    // for Warp vote function
    int32_t predicate = 0;

    // for Warp reduce and shuffle function
    int32_t laneOp = 0;
    int32_t width = 0;
};

class WarpLevelcComputeTestsuite_float : public testing::Test, public testing::WithParamInterface<WarpLevelcComputeParams_float> {
protected:
    void SetUp() {}
    void TearDown() {}
};

INSTANTIATE_TEST_CASE_P(WarpLevelcComputeTestCase_float, WarpLevelcComputeTestsuite_float,
    ::testing::Values(WarpLevelcComputeParams_float {.mode = 0, .predicate = -1},
        WarpLevelcComputeParams_float {.mode = 0, .predicate = 0},
        WarpLevelcComputeParams_float {.mode = 0, .predicate = 1},

        WarpLevelcComputeParams_float {.mode = 1, .predicate = -1},
        WarpLevelcComputeParams_float {.mode = 1, .predicate = 0},
        WarpLevelcComputeParams_float {.mode = 1, .predicate = 1},

        WarpLevelcComputeParams_float {.mode = 2, .predicate = -1},
        WarpLevelcComputeParams_float {.mode = 2, .predicate = 0},
        WarpLevelcComputeParams_float {.mode = 2, .predicate = 1},

        WarpLevelcComputeParams_float {.mode = 3},

        WarpLevelcComputeParams_float {.mode = 4, .laneOp = 1, .width = 16},
        WarpLevelcComputeParams_float {.mode = 4, .laneOp = 1, .width = 32},

        WarpLevelcComputeParams_float {.mode = 5, .laneOp = 1, .width = 16},
        WarpLevelcComputeParams_float {.mode = 5, .laneOp = 1, .width = 32},

        WarpLevelcComputeParams_float {.mode = 6, .laneOp = 1, .width = 16},
        WarpLevelcComputeParams_float {.mode = 6, .laneOp = 1, .width = 32},

        WarpLevelcComputeParams_float {.mode = 7, .laneOp = 1, .width = 16},
        WarpLevelcComputeParams_float {.mode = 7, .laneOp = 1, .width = 32},

        WarpLevelcComputeParams_float {.mode = 8},

        WarpLevelcComputeParams_float {.mode = 9},

        WarpLevelcComputeParams_float {.mode = 10}
    ));

TEST_P(WarpLevelcComputeTestsuite_float, WarpLevelcComputeTestCase_float)
{
    auto param = GetParam();

    int typeSize = 4;
    int shapeSize = THREAD_DIM;
    uint8_t dstGM[shapeSize * typeSize] = {0};

    for (int i = 0; i < 256; i += 2) {
        if (i % 16 == 0 && i != 0) {
            printf("\n");
        }
        printf("%02x%02x ", dstGM[i], dstGM[i+1]);
    }

    float expectValues[shapeSize] = {0};
    switch (param.mode) {
        case 0:
            for (int i = 0; i < shapeSize; i += 1) {
                if (param.predicate == -1 || param.predicate == 0) {
                    expectValues[i] = 0;
                }
                if (param.predicate > 0) {
                    expectValues[i] = 1;
                }
            }
            break;
        case 1:
            for (int i = 0; i < shapeSize; i += 1) {
                if (param.predicate == 0) {
                    expectValues[i] = 0;
                } else {
                    expectValues[i] = 1;
                }
            }
            break;
        case 2:
            for (int i = 0; i < shapeSize; i += 1) {
                if (param.predicate == -1) {
                    expectValues[i] = 0xfffffffe;
                }
                if (param.predicate == 0) {
                    expectValues[i] = 0;
                }
                if (param.predicate > 0) {
                    expectValues[i] = 0xffffffff;
                }
            }
            break;
        case 3:
            for (int i = 0; i < shapeSize; i += 1) {
                expectValues[i] = 0xffffffff;
            }
            break;
        case 4:
            for (int i = 0; i < shapeSize; i += 1) {
                int32_t laneId = i % 32;
                expectValues[i] = laneId / param.width * param.width + param.laneOp % param.width;
            }
            break;
        case 5:
            for (int i = 0; i < shapeSize; i += 1) {
                int32_t laneId = i % 32;
                int32_t groupStart = laneId / param.width * param.width;
                if (laneId - param.laneOp >= groupStart) {
                    expectValues[i] = laneId - param.laneOp;
                } else {
                    expectValues[i] = laneId;
                }
            }
            break;
        case 6:
            for (int i = 0; i < shapeSize; i += 1) {
                int32_t laneId = i % 32;
                int32_t groupEnd = laneId / param.width * param.width + param.width;
                if (laneId + param.laneOp < groupEnd) {
                    expectValues[i] = laneId + param.laneOp;
                } else {
                    expectValues[i] = laneId;
                }
            }
            break;
        case 7:
            for (int i = 0; i < shapeSize; i += 1) {
                int32_t laneId = i % 32;
                expectValues[i] = laneId ^ param.laneOp;
            }
            break;
        case 8:
            for (int i = 0; i < shapeSize; i += 1) {
                expectValues[i] = 496;
            }
            break;
        case 9:
            for (int i = 0; i < shapeSize; i += 1) {
                expectValues[i] = 31;
            }
            break;
        case 10:
            //  expect value is 0
            break;
        default:
            break;
    }
}
// ================================ Test float end ================================

// ================================ Test int64_t start ================================
struct WarpLevelcComputeParams_int64t {
    int32_t mode = 0;

    // for Warp vote function
    int32_t predicate = 0;

    // for Warp reduce and shuffle function
    int32_t laneOp = 0;
    int32_t width = 0;
};

class WarpLevelcComputeTestsuite_int64t : public testing::Test, public testing::WithParamInterface<WarpLevelcComputeParams_int64t> {
protected:
    void SetUp() {}
    void TearDown() {}
};

INSTANTIATE_TEST_CASE_P(WarpLevelcComputeTestCase_int64t, WarpLevelcComputeTestsuite_int64t,
    ::testing::Values(WarpLevelcComputeParams_int64t {.mode = 0, .predicate = -1},
        WarpLevelcComputeParams_int64t {.mode = 0, .predicate = 0},
        WarpLevelcComputeParams_int64t {.mode = 0, .predicate = 1},

        WarpLevelcComputeParams_int64t {.mode = 1, .predicate = -1},
        WarpLevelcComputeParams_int64t {.mode = 1, .predicate = 0},
        WarpLevelcComputeParams_int64t {.mode = 1, .predicate = 1},

        WarpLevelcComputeParams_int64t {.mode = 2, .predicate = -1},
        WarpLevelcComputeParams_int64t {.mode = 2, .predicate = 0},
        WarpLevelcComputeParams_int64t {.mode = 2, .predicate = 1},

        WarpLevelcComputeParams_int64t {.mode = 3},

        WarpLevelcComputeParams_int64t {.mode = 4, .laneOp = 1, .width = 16},
        WarpLevelcComputeParams_int64t {.mode = 4, .laneOp = 1, .width = 32},

        WarpLevelcComputeParams_int64t {.mode = 5, .laneOp = 1, .width = 16},
        WarpLevelcComputeParams_int64t {.mode = 5, .laneOp = 1, .width = 32},

        WarpLevelcComputeParams_int64t {.mode = 6, .laneOp = 1, .width = 16},
        WarpLevelcComputeParams_int64t {.mode = 6, .laneOp = 1, .width = 32},

        WarpLevelcComputeParams_int64t {.mode = 7, .laneOp = 1, .width = 16},
        WarpLevelcComputeParams_int64t {.mode = 7, .laneOp = 1, .width = 32},

        WarpLevelcComputeParams_int64t {.mode = 8},

        WarpLevelcComputeParams_int64t {.mode = 9},

        WarpLevelcComputeParams_int64t {.mode = 10}
    ));

TEST_P(WarpLevelcComputeTestsuite_int64t, WarpLevelcComputeTestCase_int64t)
{
    auto param = GetParam();

    int typeSize = 8;
    int shapeSize = THREAD_DIM;
    uint8_t dstGM[shapeSize * typeSize] = {0};

    for (int i = 0; i < 256; i += 2) {
        if (i % 16 == 0 && i != 0) {
            printf("\n");
        }
        printf("%02x%02x ", dstGM[i], dstGM[i+1]);
    }

    int64_t expectValues[shapeSize] = {0};
    switch (param.mode) {
        case 0:
            for (int i = 0; i < shapeSize; i += 1) {
                if (param.predicate == -1 || param.predicate == 0) {
                    expectValues[i] = 0;
                }
                if (param.predicate > 0) {
                    expectValues[i] = 1;
                }
            }
            break;
        case 1:
            for (int i = 0; i < shapeSize; i += 1) {
                if (param.predicate == 0) {
                    expectValues[i] = 0;
                } else {
                    expectValues[i] = 1;
                }
            }
            break;
        case 2:
            for (int i = 0; i < shapeSize; i += 1) {
                if (param.predicate == -1) {
                    expectValues[i] = 0xfffffffe;
                }
                if (param.predicate == 0) {
                    expectValues[i] = 0;
                }
                if (param.predicate > 0) {
                    expectValues[i] = 0xffffffff;
                }
            }
            break;
        case 3:
            for (int i = 0; i < shapeSize; i += 1) {
                expectValues[i] = 0xffffffff;
            }
            break;
        case 4:
            for (int i = 0; i < shapeSize; i += 1) {
                int32_t laneId = i % 32;
                expectValues[i] = laneId / param.width * param.width + param.laneOp % param.width;
            }
            break;
        case 5:
            for (int i = 0; i < shapeSize; i += 1) {
                int32_t laneId = i % 32;
                int32_t groupStart = laneId / param.width * param.width;
                if (laneId - param.laneOp >= groupStart) {
                    expectValues[i] = laneId - param.laneOp;
                } else {
                    expectValues[i] = laneId;
                }
            }
            break;
        case 6:
            for (int i = 0; i < shapeSize; i += 1) {
                int32_t laneId = i % 32;
                int32_t groupEnd = laneId / param.width * param.width + param.width;
                if (laneId + param.laneOp < groupEnd) {
                    expectValues[i] = laneId + param.laneOp;
                } else {
                    expectValues[i] = laneId;
                }
            }
            break;
        case 7:
            for (int i = 0; i < shapeSize; i += 1) {
                int32_t laneId = i % 32;
                expectValues[i] = laneId ^ param.laneOp;
            }
            break;
        case 8:
            for (int i = 0; i < shapeSize; i += 1) {
                expectValues[i] = 496;
            }
            break;
        case 9:
            for (int i = 0; i < shapeSize; i += 1) {
                expectValues[i] = 31;
            }
            break;
        case 10:
            //  expect value is 0
            break;
        default:
            break;
    }
}
// ================================ Test int64_t end ================================

// ================================ Test uint64_t start ================================
struct WarpLevelcComputeParams_uint64t {
    int32_t mode = 0;

    // for Warp vote function
    int32_t predicate = 0;

    // for Warp reduce and shuffle function
    int32_t laneOp = 0;
    int32_t width = 0;
};

class WarpLevelcComputeTestsuite_uint64t : public testing::Test, public testing::WithParamInterface<WarpLevelcComputeParams_uint64t> {
protected:
    void SetUp() {}
    void TearDown() {}
};

INSTANTIATE_TEST_CASE_P(WarpLevelcComputeTestCase_uint64t, WarpLevelcComputeTestsuite_uint64t,
    ::testing::Values(WarpLevelcComputeParams_uint64t {.mode = 0, .predicate = -1},
        WarpLevelcComputeParams_uint64t {.mode = 0, .predicate = 0},
        WarpLevelcComputeParams_uint64t {.mode = 0, .predicate = 1},

        WarpLevelcComputeParams_uint64t {.mode = 1, .predicate = -1},
        WarpLevelcComputeParams_uint64t {.mode = 1, .predicate = 0},
        WarpLevelcComputeParams_uint64t {.mode = 1, .predicate = 1},

        WarpLevelcComputeParams_uint64t {.mode = 2, .predicate = -1},
        WarpLevelcComputeParams_uint64t {.mode = 2, .predicate = 0},
        WarpLevelcComputeParams_uint64t {.mode = 2, .predicate = 1},

        WarpLevelcComputeParams_uint64t {.mode = 3},

        WarpLevelcComputeParams_uint64t {.mode = 4, .laneOp = 1, .width = 16},
        WarpLevelcComputeParams_uint64t {.mode = 4, .laneOp = 1, .width = 32},

        WarpLevelcComputeParams_uint64t {.mode = 5, .laneOp = 1, .width = 16},
        WarpLevelcComputeParams_uint64t {.mode = 5, .laneOp = 1, .width = 32},

        WarpLevelcComputeParams_uint64t {.mode = 6, .laneOp = 1, .width = 16},
        WarpLevelcComputeParams_uint64t {.mode = 6, .laneOp = 1, .width = 32},

        WarpLevelcComputeParams_uint64t {.mode = 7, .laneOp = 1, .width = 16},
        WarpLevelcComputeParams_uint64t {.mode = 7, .laneOp = 1, .width = 32}
    ));

TEST_P(WarpLevelcComputeTestsuite_uint64t, WarpLevelcComputeTestCase_uint64t)
{
    auto param = GetParam();

    int typeSize = 8;
    int shapeSize = THREAD_DIM;
    uint8_t dstGM[shapeSize * typeSize] = {0};

    for (int i = 0; i < 256; i += 2) {
        if (i % 16 == 0 && i != 0) {
            printf("\n");
        }
        printf("%02x%02x ", dstGM[i], dstGM[i+1]);
    }

    uint64_t expectValues[shapeSize] = {0};
    switch (param.mode) {
        case 0:
            for (int i = 0; i < shapeSize; i += 1) {
                if (param.predicate == -1 || param.predicate == 0) {
                    expectValues[i] = 0;
                }
                if (param.predicate > 0) {
                    expectValues[i] = 1;
                }
            }
            break;
        case 1:
            for (int i = 0; i < shapeSize; i += 1) {
                if (param.predicate == 0) {
                    expectValues[i] = 0;
                } else {
                    expectValues[i] = 1;
                }
            }
            break;
        case 2:
            for (int i = 0; i < shapeSize; i += 1) {
                if (param.predicate == -1) {
                    expectValues[i] = 0xfffffffe;
                }
                if (param.predicate == 0) {
                    expectValues[i] = 0;
                }
                if (param.predicate > 0) {
                    expectValues[i] = 0xffffffff;
                }
            }
            break;
        case 3:
            for (int i = 0; i < shapeSize; i += 1) {
                expectValues[i] = 0xffffffff;
            }
            break;
        case 4:
            for (int i = 0; i < shapeSize; i += 1) {
                int32_t laneId = i % 32;
                expectValues[i] = laneId / param.width * param.width + param.laneOp % param.width;
            }
            break;
        case 5:
            for (int i = 0; i < shapeSize; i += 1) {
                int32_t laneId = i % 32;
                int32_t groupStart = laneId / param.width * param.width;
                if (laneId - param.laneOp >= groupStart) {
                    expectValues[i] = laneId - param.laneOp;
                } else {
                    expectValues[i] = laneId;
                }
            }
            break;
        case 6:
            for (int i = 0; i < shapeSize; i += 1) {
                int32_t laneId = i % 32;
                int32_t groupEnd = laneId / param.width * param.width + param.width;
                if (laneId + param.laneOp < groupEnd) {
                    expectValues[i] = laneId + param.laneOp;
                } else {
                    expectValues[i] = laneId;
                }
            }
            break;
        case 7:
            for (int i = 0; i < shapeSize; i += 1) {
                int32_t laneId = i % 32;
                expectValues[i] = laneId ^ param.laneOp;
            }
            break;
        case 8:
            for (int i = 0; i < shapeSize; i += 1) {
                expectValues[i] = 496;
            }
            break;
        case 9:
            for (int i = 0; i < shapeSize; i += 1) {
                expectValues[i] = 31;
            }
            break;
        case 10:
            //  expect value is 0
            break;
        default:
            break;
    }
}
// ================================ Test uint64_t end ================================