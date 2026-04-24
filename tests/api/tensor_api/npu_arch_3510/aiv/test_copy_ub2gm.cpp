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

template <typename CopyOp, typename Trait, typename DstTensor, typename SrcTensor>
void RunCopyWithPaths(const DstTensor& dst, const SrcTensor& src)
{
    using namespace AscendC::Te;

    auto atom = CopyAtom<CopyTraits<CopyOp, Trait>>{}.with();
    atom.Call(dst, src);
    Copy(atom, dst, src);
}

} // namespace

TEST_F(Tensor_Api_Copy_3510, CopyUB2GMRoutesToVectorArchCopy)
{
    using namespace AscendC::Te;

    constexpr uint32_t m = 64;
    constexpr uint32_t n = 64;
    __ubuf__ int8_t src[m * n] = {0};
    __gm__ int8_t dst[m * n] = {0};

    auto ubTensor = MakeTensorAt<Location::UB>(src, MakeFrameLayout<NDExtLayoutPtn, LayoutTraitDefault<int8_t>>(m, n));
    auto gmTensor = MakeTensorAt<Location::GM>(dst, MakeFrameLayout<NDExtLayoutPtn, LayoutTraitDefault<int8_t>>(m, n));

    RunCopyCallPaths<CopyUB2GM, CopyUB2GMTraitDefault>(gmTensor, ubTensor);
    RunCopyWithPaths<CopyUB2GM, CopyUB2GMTraitDefault>(gmTensor, ubTensor);

    EXPECT_EQ(dst[0], 0);
}
