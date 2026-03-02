class CopyGmToCbufMultiDn2ZnBase : public CopyGmToCbufMultiNd2nzInstr {
public:
    template <const DataCopyTrait& trait, typename T, typename U, typename Coord>
    __aicore__ inline void Run(const T& dst, const U& src, const Coord& coord) {
        using type = typename U::elementType;
        auto dstLayout = dst.Layout();
        auto srcLayout = src.Layout();

        uint16_t ndNum = 1;
        uint16_t nValue = GetEleFromLayout<decltype(srcLayout), AttrInfo::SHAPE, AttrInfo::COLUMN, 1>(srcLayout);
        uint32_t dValue = GetEleFromLayout<decltype(srcLayout), AttrInfo::SHAPE, AttrInfo::ROW, 1>(srcLayout);
        uint64_t srcNdMatrixStride = 0;

        auto srcRowStride = GetEleFromLayout<decltype(srcLayout), AttrInfo::STRIDE, AttrInfo::COLUMN, 1>(srcLayout);
        uint64_t srcDValue = srcRowStride;
        uint16_t dstNzC0Stride = GetEleFromLayout<decltype(dstLayout), AttrInfo::STRIDE, AttrInfo::ROW, 1>(dstLayout) * sizeof(type) / C0_SIZE;
        uint16_t dstNzNStride = 1;
        uint32_t dstNzMatrixStride = 0;

        uint64_t loop1SrcStride = srcDValue * sizeof(type);
        uint64_t loop4SrcStride = srcNdMatrixStride * sizeof(type);

        uint16_t loop2DstStride = dstNzNStride;  // loop2_dst_stride = dst_nz_n_stride
        uint16_t loop3DstStride = dstNzC0Stride; // loop3_dst_stride = dst_nz_c0_Stride
        // loop4_dst_stride: dst_nz_matrix_stride * size_of_dst_type / C0_size
        uint16_t loop4DstStride = static_cast<uint16_t>(dstNzMatrixStride * sizeof(type) / C0_SIZE);

        uint8_t cacheMode = GetCacheModeFromTensor(src.Data().Get());

        DataCopy(ndNum, loop2DstStride, loop3DstStride, loop4DstStride, loop1SrcStride, cacheMode, nValue, dValue, loop4SrcStride, false);
    }
}