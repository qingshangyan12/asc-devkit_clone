// Copyright (c) 2026 Huawei Technologies Co., Ltd.
// Licensed under the CANN Open Software License Agreement Version 2.0.
#ifndef ASCENDC_DAV_M200_KERNEL_OPERATOR_DUMP_TENSOR_TRANSPORT_H
#define ASCENDC_DAV_M200_KERNEL_OPERATOR_DUMP_TENSOR_TRANSPORT_H

// Independent FIFO transport for dav_m200. It reserves the private backup
// area together with the visible TLV, but commits only the visible record.
__aicore__ inline bool CheckDumpRingBufSpace(__gm__ BlockRingBufInfo* info, uint32_t length)
{
    if (info == nullptr || length >= info->ringBufLen) {
        return false;
    }
    volatile __gm__ RingBufReadInfo* read = GetRingBufReadInfo(info);
    volatile __gm__ RingBufWriteInfo* write = GetRingBufWriteInfo(info);
    if (read->bufOffset > info->ringBufLen || write->bufOffset > info->ringBufLen) {
        return false;
    }
    uint32_t counter = 0;
    while (write->bufOffset < read->bufOffset && length >= read->bufOffset - write->bufOffset) {
        if (counter++ >= 15) {
            return false;
        }
        RingBufferWaitRtsSync();
        dcci((__gm__ uint64_t*)read, cache_line_t::ENTIRE_DATA_CACHE);
    }
    return CheckAndWaitRingBufSpace(info, length);
}

template <template <typename> class Tensor, typename T>
class IndependentDumpTransport {
public:
    __aicore__ inline IndependentDumpTransport(
        const Tensor<T>& src, __gm__ BlockRingBufInfo* info, Hardware position)
        : src_(src), info_(info), position_(position)
    {}

    __aicore__ inline uint32_t Capacity(uint32_t headerSize) const
    {
        const uint32_t alignedHeader = AlignUp(headerSize, ONE_BLK_SIZE);
        if (info_ == nullptr || info_->ringBufLen <= alignedHeader + 32) {
            return 0;
        }
        if constexpr (IsSameType<Tensor<T>, GlobalTensor<T>>::value) {
            return ((info_->ringBufLen - alignedHeader - 1) / ONE_BLK_SIZE) * ONE_BLK_SIZE;
        }
        const uint32_t copies = position_ == Hardware::UB ? 2 : 3;
        uint32_t capacity = ((info_->ringBufLen - alignedHeader - 32) / copies / ONE_BLK_SIZE) * ONE_BLK_SIZE;
        capacity = capacity < 65535U * ONE_BLK_SIZE ? capacity : 65535U * ONE_BLK_SIZE;
        if (position_ != Hardware::UB) {
            capacity = capacity < ONE_DUMP_BACKUP_SIZE ? capacity : ONE_DUMP_BACKUP_SIZE;
        }
        if (position_ == Hardware::L0C) {
            return capacity >= 1024 ? 1024 : 0;
        }
        return capacity;
    }

    __aicore__ inline __gm__ uint8_t* Reserve(uint32_t headerSize, uint32_t payloadBytes)
    {
        if (payloadBytes == 0 || payloadBytes > Capacity(headerSize)) {
            return nullptr;
        }
        const uint32_t alignedBytes = AlignUp(payloadBytes, ONE_BLK_SIZE);
        const uint32_t recordLength = AlignUp(headerSize + payloadBytes, ONE_BLK_SIZE);
        uint32_t reserveLength = recordLength;
        if constexpr (IsSameType<Tensor<T>, LocalTensor<T>>::value) {
            const uint32_t scratchBytes = position_ == Hardware::L0C ? 2048 :
                (position_ == Hardware::L1 ? alignedBytes * 2 : alignedBytes);
            reserveLength += 31 + scratchBytes;
        }
        return CheckDumpRingBufSpace(info_, reserveLength) ? GetRingBufTlv(info_) : nullptr;
    }

    __aicore__ inline bool Copy(__gm__ uint8_t* record, uint32_t headerSize,
        uint64_t offset, uint32_t payloadBytes)
    {
        PipeBarrier<PIPE_ALL>();
        const uint32_t alignedBytes = AlignUp(payloadBytes, ONE_BLK_SIZE);
        const uint32_t recordLength = AlignUp(headerSize + payloadBytes, ONE_BLK_SIZE);
        if constexpr (IsSameType<Tensor<T>, GlobalTensor<T>>::value) {
            const uintptr_t address = reinterpret_cast<uintptr_t>(src_.GetPhyAddr());
            if (address == 0 || offset > UINTPTR_MAX - address || payloadBytes > UINTPTR_MAX - address - offset) {
                return false;
            }
            MemCopyGm2Gm(record + headerSize,
                reinterpret_cast<__gm__ const uint8_t*>(src_.GetPhyAddr()) + offset, payloadBytes);
        } else {
            if (offset % sizeof(T) != 0 || offset / sizeof(T) > UINT32_MAX) {
                return false;
            }
            const uintptr_t stagingAddr =
                (reinterpret_cast<uintptr_t>(record + recordLength) + 31U) & ~static_cast<uintptr_t>(31U);
            __gm__ uint8_t* staging = reinterpret_cast<__gm__ uint8_t*>(stagingAddr);
            const auto chunk = src_[static_cast<uint32_t>(offset / sizeof(T))];
            if (position_ == Hardware::UB) {
                DataCopyParams params = {1, static_cast<uint16_t>(alignedBytes / ONE_BLK_SIZE), 0, 0};
                DataCopyUB2GMImpl<uint8_t>(staging,
                    reinterpret_cast<__ubuf__ uint8_t*>(chunk.GetPhyAddr()), params);
            } else if (position_ == Hardware::L1) {
                SetDumpDataL12GM(staging, chunk, alignedBytes);
            } else if (position_ == Hardware::L0C) {
                SetDumpDataL0C2GM(staging, chunk, alignedBytes);
            } else {
                return false;
            }
            PipeBarrier<PIPE_ALL>();
            MemCopyGm2Gm(record + headerSize, staging, payloadBytes);
        }
        return true;
    }

    __aicore__ inline void Commit(__gm__ uint8_t*, uint32_t recordLength)
    {
        PipeBarrier<PIPE_ALL>();
        dcci((__gm__ uint64_t*)GetRingBufTlv(info_), cache_line_t::ENTIRE_DATA_CACHE);
        UpdateWriteInfo(GetRingBufWriteInfo(info_), recordLength);
    }

private:
    const Tensor<T>& src_;
    __gm__ BlockRingBufInfo* info_;
    Hardware position_;
};

#endif // ASCENDC_DAV_M200_KERNEL_OPERATOR_DUMP_TENSOR_TRANSPORT_H
