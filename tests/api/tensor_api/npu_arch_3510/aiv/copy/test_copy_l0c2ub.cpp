#include <gtest/gtest.h>
#include "include/tensor_api/tensor.h"

class Tensor_Api_Vector_Copy_3510 : public testing::Test {
protected:
    static void SetUpTestCase() {}
    static void TearDownTestCase() {}

    void SetUp() override 
    {
        AscendC::SetGCoreType(1);
    }

    void TearDown() override 
    {
        AscendC::SetGCoreType(0);
    }
};

namespace {

template <typename LocationTag, typename Pointer, typename Layout>
auto MakeTensorAt(Pointer ptr, const Layout& layout)
{
    return AscendC::Te::MakeTensor(AscendC::Te::MakeMemPtr<LocationTag>(ptr), layout);
}

template <typename CopyOp, typename Trait, typename DstTensor, typename SrcTensor>
void RunCopyCallPaths(const DstTensor& dst, const SrcTensor& src)
{
    using namespace AscendC::Te;

    auto atom = MakeCopy(CopyOp{}, Trait{});
    atom.Call(dst, src);

    CopyAtom<CopyTraits<CopyOp, Trait>>{}.Call(dst, src);
    Copy(CopyAtom<CopyTraits<CopyOp, Trait>>{}, dst, src);
}

template <typename CopyOp, typename Trait, typename Param, typename DstTensor, typename SrcTensor>
void RunCopyWithParamPaths(const DstTensor& dst, const SrcTensor& src, const Param& param)
{
    using namespace AscendC::Te;

    auto atom = MakeCopy(CopyOp{}).with(param);
    atom.Call(dst, src);

    auto copiedAtom = CopyAtom<CopyTraits<CopyOp, Trait>>{}.with(param);
    copiedAtom.Call(dst, src);

    Copy(copiedAtom, dst, src);
    Copy(CopyAtom<CopyTraits<CopyOp, Trait>>{}, dst, src, param);
}

} // namespace

TEST_F(Tensor_Api_Copy_3510, CopyL0C2UBRoutesToVectorArchCopy)
{
    using namespace AscendC::Te;

    constexpr uint32_t m = 32;
    constexpr uint32_t n = 32;
    __cc__ float src[m * n] = {0};
    __ubuf__ float dst[m * n] = {0};

    auto l0cTensor = MakeTensorAt<Location::L0C>(src, MakeFrameLayout<NZLayoutPtn, LayoutTraitDefault<float, 16>>(m, n));
    auto ubTensor = MakeTensorAt<Location::UB>(dst, MakeFrameLayout<NDExtLayoutPtn, LayoutTraitDefault<float>>(m, n));

    RunCopyCallPaths<CopyL0C2UB, CopyL0C2UBTraitDefault>(ubTensor, l0cTensor);
    RunCopyWithParamPaths<CopyL0C2UB, CopyL0C2UBTraitDefault>(ubTensor, l0cTensor, FixpipeParams{});

    EXPECT_EQ(dst[0], 0);
}
