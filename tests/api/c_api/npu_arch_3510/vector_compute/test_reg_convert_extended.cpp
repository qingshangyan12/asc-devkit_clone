#include <gtest/gtest.h>
#include <mockcpp/mockcpp.hpp>
#include "tests/api/c_api/stub/cce_stub.h"
#include "tests/api/c_api/npu_arch_3510/utils/reg_convert_extended_signatures.h"

namespace {

void CheckFloatToHalfRdSat(
    vector_half& dst, vector_float src, vector_bool mask, Literal round, Literal saturation,
    Literal position, Literal mode)
{
    EXPECT_EQ(round, ROUND_F);
    EXPECT_EQ(saturation, RS_ENABLE);
    EXPECT_EQ(position, PART_ODD);
    EXPECT_EQ(mode, MODE_ZEROING);
}

void CheckHalfToInt4RnSat(
    vector_int4x2_t& dst, vector_half src, vector_bool mask, int32_t round, int32_t saturation,
    int32_t position, int32_t mode)
{
    EXPECT_EQ(round, ROUND_R);
    EXPECT_EQ(saturation, RS_ENABLE);
    EXPECT_EQ(position, PART_P3);
    EXPECT_EQ(mode, MODE_ZEROING);
}

half CheckScalarFloatToHalf(float value)
{
    EXPECT_FLOAT_EQ(value, 1.25f);
    return half(1.25f);
}

}

TEST(RegConvertExtended, CastForwardsRoundingSaturationAndPosition)
{
    vector_half dst;
    vector_float src;
    vector_bool mask;
    MOCKER_CPP(vcvt, void(vector_half&, vector_float, vector_bool, Literal, Literal, Literal, Literal))
        .times(1)
        .will(invoke(CheckFloatToHalfRdSat));

    asc_cast(dst, src, mask, ASC_CAST_RD, ASC_CAST_SAT, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

TEST(RegConvertExtended, ReturningOverloadForwardsToPositionOverload)
{
    vector_float src;
    vector_bool mask;
    MOCKER_CPP(vcvt, void(vector_half&, vector_float, vector_bool, Literal, Literal, Literal, Literal))
        .times(1)
        .will(invoke(CheckFloatToHalfRdSat));

    asc_float2half_rd_sat(src, mask, ASC_POSITION_ODD);
    GlobalMockObject::verify();
}

TEST(RegConvertExtended, Int4PositionAndReturningOverloadsSelectFourthQuarter)
{
    vector_int4x2_t dst;
    vector_half src;
    vector_bool mask;
    MOCKER_CPP(vcvt_f162s4, void(vector_int4x2_t&, vector_half, vector_bool, int32_t, int32_t, int32_t, int32_t))
        .times(2)
        .will(invoke(CheckHalfToInt4RnSat));

    asc_half2int4x2_rn_sat(dst, src, mask, ASC_DISPERSE_FOURTH_QUARTER);
    asc_half2int4x2_rn_sat(src, mask, ASC_DISPERSE_FOURTH_QUARTER);
    GlobalMockObject::verify();
}

TEST(RegConvertExtended, ScalarFloatToHalfForwardsInputAndReturnsResult)
{
    MOCKER_CPP(conv_f322f16o, half(float)).times(1).will(invoke(CheckScalarFloatToHalf));

    EXPECT_FLOAT_EQ(static_cast<float>(asc_float2half_ro(1.25f)), 1.25f);
    GlobalMockObject::verify();
}
