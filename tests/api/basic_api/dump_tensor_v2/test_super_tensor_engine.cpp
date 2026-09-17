// Copyright (c) 2026 Huawei Technologies Co., Ltd.
// Licensed under the CANN Open Software License Agreement Version 2.0.
#include <cassert>
#include <cstdint>
#include <cstring>

#define __aicore__
#define __gm__
#include "impl/utils/debug/asc_dump_super_tensor_engine.h"

struct FakeTransport {
    uint8_t storage[512] = {};
    uint32_t cursor = 0;
    uint32_t commits = 0;
    uint32_t fail_offset = UINT32_MAX;
    uint32_t committed_lengths[4] = {};

    uint32_t Capacity(uint32_t) const { return 32; }

    uint8_t* Reserve(uint32_t header_size, uint32_t payload_size)
    {
        const uint32_t record_length = (header_size + payload_size + 31) / 32 * 32;
        assert(cursor + record_length <= sizeof(storage));
        std::memset(storage + cursor, 0xCD, record_length);
        return storage + cursor;
    }

    bool Copy(uint8_t* record, uint32_t header_size, uint64_t offset, uint32_t payload_size)
    {
        if (offset == fail_offset) {
            return false;
        }
        for (uint32_t i = 0; i < payload_size; ++i) {
            record[header_size + i] = static_cast<uint8_t>((offset + i) & 0xFF);
        }
        return true;
    }

    void Commit(uint8_t* record, uint32_t record_length)
    {
        committed_lengths[commits++] = record_length;
        cursor = static_cast<uint32_t>(record + record_length - storage);
    }
};

static void CheckSuccessfulSplit()
{
    FakeTransport transport;
    const uint32_t shape[] = {20};
    __asc_aicore::dump_super_tensor(
        transport, 80, 0x1234, 4, 0, 7, 9, shape, 1);
    assert(transport.commits == 3);
    assert(transport.committed_lengths[0] == 128);
    assert(transport.committed_lengths[1] == 96);
    assert(transport.committed_lengths[2] == 64);

    const auto* first = reinterpret_cast<const __asc_aicore::DumpTensorInfo*>(transport.storage);
    assert(first->dumpType == 11);
    assert(first->dumpLength == 120);
    assert(first->tensorLength == 80);
    assert(first->tensorOffset == 0);
    assert(first->dumpSize == 32);
    assert(first->shape[0] == 20);
    for (uint32_t i = 120; i < 128; ++i) {
        assert(transport.storage[i] == 0);
    }

    const auto* body = reinterpret_cast<const __asc_aicore::DumpTensorBodyInfo*>(transport.storage + 128);
    assert(body->dumpType == 12);
    assert(body->dumpLength == 88);
    assert(body->tensorLength == 80);
    assert(body->tensorOffset == 32);
    assert(body->dumpSize == 32);
}

static void CheckCopyFailureDoesNotCommit()
{
    FakeTransport transport;
    transport.fail_offset = 32;
    __asc_aicore::dump_super_tensor(transport, 80, 0, 4, 0, 0, 1, nullptr, 0);
    assert(transport.commits == 1);
}

int main()
{
    CheckSuccessfulSplit();
    CheckCopyFailureDoesNotCommit();
    return 0;
}
