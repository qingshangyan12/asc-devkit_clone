/*
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <cstdint>
#include <cstring>
#include <cerrno>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sys/stat.h>
#include <sys/types.h>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
constexpr uint16_t MAGIC_NUM = 0xAE86U;
constexpr uint32_t PRINT_ARG_LEN = 8U;
constexpr uint32_t RESV_LEN_SIMT = 40U;
constexpr uint32_t SIMT_PRINT_MSG_LEN_OFFSET = 44U;

enum class DumpType : uint32_t {
    DUMP_DEFAULT = 0,
    DUMP_SCALAR,
    DUMP_TENSOR,
    DUMP_SHAPE,
    DUMP_ASSERT,
    DUMP_META,
    DUMP_TIMESTAMP,
    DUMP_SIMT,
    DUMP_BUFI,
    DUMP_BUFO,
    DUMP_SKIP,
    DUMP_SIMT_ASSERT = 0xF0E00F0E,
    DUMP_SIMT_PRINTF = 0xF0F00F0F,
    DUMP_WAIT = 0xF0A55A0F,
};

#pragma pack(push, 1)
struct BlockRingBufInfo {
    uint32_t length;
    uint32_t coreId;
    uint32_t blockNum;
    uint32_t ringBufLen;
    uint16_t magic;
    uint16_t flag;
    uint32_t rsv;
    uint64_t ringBufAddr;
    uint64_t dbgAddr;
    uint32_t resv[4];
};

struct BlockReadInfo {
    DumpType dumpType;
    uint32_t length;
    uint64_t readIdx;
    uint64_t resv;
};

struct BlockWriteInfo {
    DumpType dumpType;
    uint32_t length;
    uint64_t writeIdx;
    uint64_t packIdx;
};

struct PrintTlvInfoHead {
    DumpType type;
    uint32_t length;
    uint32_t blockIdx[3];
    uint32_t threadIdx[3];
    uint32_t resv[4];
    uint64_t fmtOffset;
};

struct DumpInfoHead {
    DumpType type;
    uint32_t infoLen;
    uint8_t infoMsg[0];
};
#pragma pack(pop)

static_assert(sizeof(BlockRingBufInfo) == 56U);
static_assert(sizeof(BlockReadInfo) == 24U);
static_assert(sizeof(BlockWriteInfo) == 24U);
static_assert(sizeof(PrintTlvInfoHead) == 56U);
static_assert(sizeof(DumpInfoHead) == 8U);

struct Options {
    std::vector<std::string> files;
    bool parseAll = true;
    bool parseLastSnapshot = false;
    bool hasSnapshotIndex = false;
    uint64_t snapshotIndex = 0U;
};

struct FixedTlvFormat {
    uint64_t tlvLen = 0U;
    uint32_t infoLen = 0U;
};

struct DecodeResult {
    bool ok = false;
    std::string text;
    std::string error;
};

std::string DumpTypeName(DumpType type)
{
    switch (type) {
        case DumpType::DUMP_BUFI:
            return "DUMP_BUFI";
        case DumpType::DUMP_BUFO:
            return "DUMP_BUFO";
        case DumpType::DUMP_SIMT_ASSERT:
            return "DUMP_SIMT_ASSERT";
        case DumpType::DUMP_SIMT_PRINTF:
            return "DUMP_SIMT_PRINTF";
        case DumpType::DUMP_WAIT:
            return "DUMP_WAIT";
        default: {
            std::ostringstream os;
            os << "UNKNOWN_0x" << std::hex << std::setw(8) << std::setfill('0')
               << static_cast<uint32_t>(type);
            return os.str();
        }
    }
}

std::vector<uint8_t> ReadFile(const std::string &path)
{
    std::ifstream in(path, std::ios::binary);
    if (!in) {
        throw std::runtime_error("failed to open " + path);
    }
    in.seekg(0, std::ios::end);
    const auto size = in.tellg();
    if (size < 0) {
        throw std::runtime_error("failed to get size for " + path);
    }
    in.seekg(0, std::ios::beg);
    std::vector<uint8_t> data(static_cast<size_t>(size));
    in.read(reinterpret_cast<char *>(data.data()), static_cast<std::streamsize>(data.size()));
    if (static_cast<size_t>(in.gcount()) != data.size()) {
        throw std::runtime_error("short read for " + path);
    }
    return data;
}

uint64_t ParseUint64(const std::string &value)
{
    size_t pos = 0U;
    const uint64_t parsed = std::stoull(value, &pos, 0);
    if (pos != value.size()) {
        throw std::invalid_argument("invalid integer: " + value);
    }
    return parsed;
}

void PrintUsage(const char *argv0)
{
    std::cerr << "Usage: " << argv0
              << " [--all | --last-snapshot | --last-2mb | --snapshot INDEX] <raw.bin> [more.bin...]\n"
              << "\n"
              << "Parse raw printf data saved as: BlockRingBufInfo | ring buffer 0 | ring buffer 1 | ...\n"
              << "The ring buffer snapshot length is read from BlockRingBufInfo::ringBufLen.\n";
}

Options ParseArgs(int argc, char **argv)
{
    Options opts;
    for (int i = 1; i < argc; ++i) {
        const std::string arg = argv[i];
        if (arg == "-h" || arg == "--help") {
            PrintUsage(argv[0]);
            std::exit(0);
        }
        if (arg == "--all") {
            opts.parseAll = true;
            opts.parseLastSnapshot = false;
            opts.hasSnapshotIndex = false;
            continue;
        }
        if (arg == "--last-snapshot" || arg == "--last-2mb" || arg == "--snapshot=last" ||
            arg == "--snapshot=last-2mb") {
            opts.parseAll = false;
            opts.parseLastSnapshot = true;
            opts.hasSnapshotIndex = false;
            continue;
        }
        if (arg == "--snapshot") {
            if (i + 1 >= argc) {
                throw std::invalid_argument("--snapshot needs an index");
            }
            const std::string value = argv[++i];
            if (value == "all") {
                opts.parseAll = true;
                opts.parseLastSnapshot = false;
                opts.hasSnapshotIndex = false;
            } else if (value == "last" || value == "last-2mb" || value == "last-snapshot") {
                opts.parseAll = false;
                opts.parseLastSnapshot = true;
                opts.hasSnapshotIndex = false;
            } else {
                opts.parseAll = false;
                opts.parseLastSnapshot = false;
                opts.hasSnapshotIndex = true;
                opts.snapshotIndex = ParseUint64(value);
            }
            continue;
        }
        const std::string snapshotPrefix = "--snapshot=";
        if (arg.rfind(snapshotPrefix, 0) == 0) {
            const std::string value = arg.substr(snapshotPrefix.size());
            if (value == "all") {
                opts.parseAll = true;
                opts.parseLastSnapshot = false;
                opts.hasSnapshotIndex = false;
            } else if (value == "last" || value == "last-2mb" || value == "last-snapshot") {
                opts.parseAll = false;
                opts.parseLastSnapshot = true;
                opts.hasSnapshotIndex = false;
            } else {
                opts.parseAll = false;
                opts.parseLastSnapshot = false;
                opts.hasSnapshotIndex = true;
                opts.snapshotIndex = ParseUint64(value);
            }
            continue;
        }
        if (!arg.empty() && arg[0] == '-') {
            throw std::invalid_argument("unknown option: " + arg);
        }
        opts.files.push_back(arg);
    }
    if (opts.files.empty()) {
        throw std::invalid_argument("no input file");
    }
    return opts;
}

template <typename T>
const T *At(const std::vector<uint8_t> &data, uint64_t offset)
{
    if (offset > data.size() || sizeof(T) > data.size() - offset) {
        return nullptr;
    }
    return reinterpret_cast<const T *>(data.data() + offset);
}

template <typename T>
T LoadParam(const uint8_t *paramBegin, uint32_t paramIndex)
{
    T value {};
    std::memcpy(&value, paramBegin + static_cast<size_t>(paramIndex) * PRINT_ARG_LEN, sizeof(T));
    return value;
}

std::string HexBytes(const uint8_t *data, size_t size)
{
    std::ostringstream os;
    for (size_t i = 0; i < size; ++i) {
        if (i != 0U) {
            os << ' ';
        }
        os << std::hex << std::setw(2) << std::setfill('0') << static_cast<uint32_t>(data[i]);
    }
    return os.str();
}

std::string ToHex(uint64_t value, bool upper)
{
    std::ostringstream os;
    if (upper) {
        os << std::uppercase;
    }
    os << std::hex << value;
    return os.str();
}

std::string ParseFormatToken(const char *format)
{
    std::string token;
    if (*format == 'l') {
        token += *format++;
        if (*format == 'l') {
            token += *format++;
        }
    }
    if (*format != '\0') {
        token += *format;
    }
    return token;
}

void AppendFormattedParam(const std::string &token, const uint8_t *paramBegin, uint32_t paramIndex,
    std::string &out)
{
    const char spec = token.empty() ? '\0' : token.back();
    switch (spec) {
        case 'd':
        case 'i':
            out += std::to_string(LoadParam<int64_t>(paramBegin, paramIndex));
            return;
        case 'u':
            out += std::to_string(LoadParam<uint64_t>(paramBegin, paramIndex));
            return;
        case 'f':
        case 'F':
            out += std::to_string(LoadParam<float>(paramBegin, paramIndex));
            return;
        case 'p': {
            std::ostringstream os;
            os << "0x" << std::hex << LoadParam<uint64_t>(paramBegin, paramIndex);
            out += os.str();
            return;
        }
        case 'x':
            out += ToHex(LoadParam<uint64_t>(paramBegin, paramIndex), false);
            return;
        case 'X':
            out += ToHex(LoadParam<uint64_t>(paramBegin, paramIndex), true);
            return;
        case 's': {
            const uint64_t offset = LoadParam<uint64_t>(paramBegin, paramIndex);
            const char *offsetAddr = reinterpret_cast<const char *>(paramBegin + static_cast<size_t>(paramIndex) * PRINT_ARG_LEN);
            const char *str = offsetAddr + offset;
            const uint32_t maxLen = LoadParam<uint32_t>(paramBegin - SIMT_PRINT_MSG_LEN_OFFSET, 0);
            out += std::string(str, strnlen(str, maxLen));
            return;
        }
        default:
            out += "%" + token;
            return;
    }
}

DecodeResult DecodeSimtPrintf(const DumpInfoHead *dumpHead)
{
    if (dumpHead->infoLen < RESV_LEN_SIMT + PRINT_ARG_LEN) {
        return {false, "", "bad-infoLen"};
    }
    const uint8_t *paramBegin = dumpHead->infoMsg + RESV_LEN_SIMT;
    const uint64_t strOffset = LoadParam<uint64_t>(paramBegin, 0);
    if (strOffset < PRINT_ARG_LEN || strOffset > dumpHead->infoLen - RESV_LEN_SIMT) {
        return {false, "", "bad-format-offset"};
    }
    const uint32_t argsNum = static_cast<uint32_t>(strOffset / PRINT_ARG_LEN) - 1U;
    const char *format = reinterpret_cast<const char *>(paramBegin + strOffset);
    const uint64_t formatMaxLen = static_cast<uint64_t>(dumpHead->infoLen) - RESV_LEN_SIMT - strOffset;

    uint32_t paramIndex = 0U;
    std::string out;
    uint64_t formatPos = 0U;
    while (formatPos < formatMaxLen && format[formatPos] != '\0') {
        const char *cur = format + formatPos;
        if (*cur != '%') {
            out += *cur;
            formatPos++;
            continue;
        }
        formatPos++;
        if (formatPos >= formatMaxLen) {
            return {false, out, "truncated-format-token"};
        }
        cur = format + formatPos;
        const std::string token = ParseFormatToken(cur);
        if (token == "%") {
            out += '%';
            formatPos++;
            continue;
        }
        paramIndex++;
        if (paramIndex > argsNum) {
            return {false, out, "missing-param"};
        }
        try {
            AppendFormattedParam(token, paramBegin, paramIndex, out);
        } catch (const std::exception &e) {
            return {false, out, std::string("format-param-error:") + e.what()};
        }
        formatPos += token.size();
    }
    if (formatPos >= formatMaxLen) {
        return {false, out, "unterminated-format"};
    }
    return {true, out, ""};
}

void PrintBlock(std::ostream &os, const BlockRingBufInfo &block)
{
    os << "BlockRingBufInfo: length=" << block.length
       << " coreId=" << block.coreId
       << " blockNum=" << block.blockNum
       << " ringBufLen=" << block.ringBufLen
       << " remainLen=" << block.ringBufLen
       << " magic=0x" << std::hex << block.magic << std::dec
       << " flag=" << block.flag
       << " ringBufAddr=0x" << std::hex << block.ringBufAddr
       << " dbgAddr=0x" << block.dbgAddr << std::dec << "\n";
}

void PrintReadWriteUnavailable(std::ostream &os)
{
    os << "RingBufWriteInfo: <unavailable>\n";
    os << "RingBufReadInfo: <unavailable>\n";
}

void PrintFirstHead(std::ostream &os, const uint8_t *ring, size_t ringSize, uint64_t offset)
{
    if (offset > ringSize || sizeof(PrintTlvInfoHead) > ringSize - offset) {
        os << "FirstPrintTlvInfoHeadAfterRead: <out-of-range offset=" << offset << ">\n";
        return;
    }
    const auto *head = reinterpret_cast<const PrintTlvInfoHead *>(ring + offset);
    os << "FirstPrintTlvInfoHeadAfterRead: offset=" << offset
       << " type=" << DumpTypeName(head->type) << "(" << static_cast<uint32_t>(head->type) << ")"
       << " length=" << head->length
       << " blockIdx=[" << head->blockIdx[0] << "," << head->blockIdx[1] << "," << head->blockIdx[2] << "]"
       << " threadIdx=[" << head->threadIdx[0] << "," << head->threadIdx[1] << "," << head->threadIdx[2] << "]"
       << " resv=[" << head->resv[0] << "," << head->resv[1] << "," << head->resv[2] << "," << head->resv[3] << "]"
       << " fmtOffset=0x" << std::hex << head->fmtOffset << std::dec << "\n";
    os << "FirstPrintTlvInfoHeadRawHex: "
       << HexBytes(reinterpret_cast<const uint8_t *>(head), sizeof(PrintTlvInfoHead)) << "\n";
}

void PrintTlvInfoHeadFields(std::ostream &os, const PrintTlvInfoHead &head, const std::string &prefix)
{
    os << prefix
       << "type=" << DumpTypeName(head.type) << "(" << static_cast<uint32_t>(head.type) << ")"
       << " length=" << head.length
       << " blockIdx=[" << head.blockIdx[0] << "," << head.blockIdx[1] << "," << head.blockIdx[2] << "]"
       << " threadIdx=[" << head.threadIdx[0] << "," << head.threadIdx[1] << "," << head.threadIdx[2] << "]"
       << " resv=[" << head.resv[0] << "," << head.resv[1] << "," << head.resv[2] << "," << head.resv[3] << "]"
       << " fmtOffset=0x" << std::hex << head.fmtOffset << std::dec << "\n";
}

FixedTlvFormat FixedTlvFormatFromFirstRingBuffer(const uint8_t *ring, uint64_t ringSize)
{
    if (ring == nullptr || ringSize < sizeof(PrintTlvInfoHead)) {
        return {};
    }
    const auto *head = reinterpret_cast<const PrintTlvInfoHead *>(ring);
    const uint64_t tlvLen = sizeof(DumpInfoHead) + static_cast<uint64_t>(head->length);
    if (tlvLen < sizeof(PrintTlvInfoHead) || tlvLen > ringSize) {
        return {};
    }
    return {tlvLen, head->length};
}

void ParseTlvsByFixedFormat(std::ostream &os, const std::vector<uint8_t> &window, const FixedTlvFormat &format)
{
    if (format.tlvLen == 0U) {
        os << "FixedTlvFormat: <unavailable>\n";
        os << "ParsedTlvCount=0 invalidTlvCount=0 processedBytes=0 remainingBytes=" << window.size() << "\n";
        return;
    }

    os << "FixedTlvFormat: source=first-ring-buffer-first-tlv"
       << " tlvLen=" << format.tlvLen
       << " infoLen=" << format.infoLen << "\n";

    uint32_t index = 0U;
    uint32_t invalidCount = 0U;
    uint64_t offset = 0U;
    while (offset + format.tlvLen <= window.size()) {
        const auto *dump = reinterpret_cast<const DumpInfoHead *>(window.data() + offset);
        const auto *head = reinterpret_cast<const PrintTlvInfoHead *>(dump);
        const uint64_t actualTlvLen = sizeof(DumpInfoHead) + static_cast<uint64_t>(dump->infoLen);
        const bool validType = dump->type == DumpType::DUMP_SIMT_PRINTF || dump->type == DumpType::DUMP_SIMT_ASSERT;
        const bool validLen = actualTlvLen == format.tlvLen;
        os << "PrintTlv[" << index << "]: ringBufferOffset=" << offset;
        if (!validType || !validLen) {
            os << " <invalid-header>"
               << " type=" << DumpTypeName(dump->type) << "(" << static_cast<uint32_t>(dump->type) << ")"
               << " actualTlvLen=" << actualTlvLen
               << " fixedTlvLen=" << format.tlvLen
               << "\n";
            PrintTlvInfoHeadFields(os, *head, "  PrintTlvInfoHead: ");
            os << "  PrintTlvInfoHeadRawHex: "
               << HexBytes(window.data() + offset,
                    static_cast<size_t>(std::min<uint64_t>(format.tlvLen, sizeof(PrintTlvInfoHead))))
               << "\n";
            invalidCount++;
            offset += format.tlvLen;
            index++;
            continue;
        }
        os << " <valid-header>\n";
        PrintTlvInfoHeadFields(os, *head, "  PrintTlvInfoHead: ");
        const DecodeResult decoded = DecodeSimtPrintf(dump);
        if (decoded.ok) {
            os << "  Decode: ok\n";
            os << decoded.text;
        } else {
            os << "  Decode: failed reason=" << decoded.error << "\n";
            if (!decoded.text.empty()) {
                os << "  DecodePartial: " << decoded.text << "\n";
            }
            invalidCount++;
        }
        offset += format.tlvLen;
        index++;
    }
    os << "ParsedTlvCount=" << index
       << " invalidTlvCount=" << invalidCount
       << " processedBytes=" << offset
       << " remainingBytes=" << (window.size() - offset) << "\n";
}

std::vector<uint64_t> SelectSnapshots(uint64_t count, const Options &opts)
{
    if (count == 0U) {
        return {};
    }
    if (opts.parseLastSnapshot) {
        return {count - 1U};
    }
    if (opts.hasSnapshotIndex) {
        if (opts.snapshotIndex >= count) {
            throw std::out_of_range("snapshot index " + std::to_string(opts.snapshotIndex) +
                " out of range 0.." + std::to_string(count - 1U));
        }
        return {opts.snapshotIndex};
    }
    std::vector<uint64_t> indices;
    indices.reserve(static_cast<size_t>(count));
    for (uint64_t i = 0U; i < count; ++i) {
        indices.push_back(i);
    }
    return indices;
}

bool IsValidBlockRingBufInfo(const BlockRingBufInfo &block)
{
    return block.magic == MAGIC_NUM &&
        block.length >= sizeof(BlockRingBufInfo) &&
        block.ringBufLen >= sizeof(PrintTlvInfoHead);
}

void EnsureOutputDir(const std::string &dir)
{
    if (mkdir(dir.c_str(), 0755) != 0 && errno != EEXIST) {
        throw std::runtime_error("failed to create output directory: " + dir);
    }
}

std::string BaseName(const std::string &path)
{
    const size_t pos = path.find_last_of("/\\");
    if (pos == std::string::npos) {
        return path;
    }
    return path.substr(pos + 1U);
}

std::string OutputPathForSnapshot(const std::string &inputPath, uint64_t index)
{
    std::ostringstream os;
    os << "parse_output/" << BaseName(inputPath) << ".ring_" << std::setw(6) << std::setfill('0')
       << index << ".txt";
    return os.str();
}

void ParseFile(const std::string &path, const Options &opts)
{
    const auto data = ReadFile(path);
    const auto *block = At<BlockRingBufInfo>(data, 0);
    if (block == nullptr) {
        throw std::runtime_error("raw file is too small for BlockRingBufInfo");
    }
    if (!IsValidBlockRingBufInfo(*block)) {
        throw std::runtime_error("invalid BlockRingBufInfo: magic/ringBufLen is unexpected");
    }

    const uint64_t dataOffset = sizeof(BlockRingBufInfo);
    const uint64_t ringSize = block->ringBufLen;
    const uint64_t payloadSize = data.size() > dataOffset ? data.size() - dataOffset : 0U;
    const uint64_t snapshotCount = ringSize == 0U ? 0U : payloadSize / ringSize;
    const uint64_t trailingBytes = ringSize == 0U ? payloadSize : payloadSize % ringSize;
    const auto snapshots = SelectSnapshots(snapshotCount, opts);
    FixedTlvFormat fixedFormat {};
    if (snapshotCount > 0U && dataOffset + ringSize <= data.size()) {
        fixedFormat = FixedTlvFormatFromFirstRingBuffer(data.data() + dataOffset, ringSize);
    }

    std::cout << "file: " << path << "\n";
    std::cout << "size: " << data.size() << "\n";
    std::cout << "layout: BlockRingBufInfo | ring buffer snapshots\n";
    PrintBlock(std::cout, *block);
    std::cout << "snapshots: count=" << snapshotCount
              << " ringBufLen=" << ringSize
              << " dataOffset=" << dataOffset
              << " trailingBytes=" << trailingBytes << "\n";
    std::cout << "globalFixedTlvFormat: source=snapshot[0] offset=0";
    if (fixedFormat.tlvLen == 0U) {
        std::cout << " <unavailable>\n";
    } else {
        std::cout << " tlvLen=" << fixedFormat.tlvLen
                  << " infoLen=" << fixedFormat.infoLen << "\n";
    }
    EnsureOutputDir("parse_output");

    uint64_t parsedRingBuffers = 0U;
    const uint64_t selectedRingBuffers = snapshots.size();
    for (const uint64_t index : snapshots) {
        const uint64_t offset = dataOffset + index * ringSize;
        if (offset > data.size() || ringSize > data.size() - offset) {
            parsedRingBuffers++;
            std::cout << "[PROGRESS] parsedRingBuffers=" << parsedRingBuffers << "/" << selectedRingBuffers
                      << " ringIndex=" << index
                      << " totalRingBuffers=" << snapshotCount
                      << " status=truncated"
                      << " offset=" << offset << "\n";
            continue;
        }
        const uint8_t *ring = data.data() + offset;
        std::vector<uint8_t> window(ring, ring + ringSize);
        const std::string outputPath = OutputPathForSnapshot(path, index);
        std::ofstream out(outputPath, std::ios::trunc);
        if (!out) {
            throw std::runtime_error("failed to open output file: " + outputPath);
        }
        out << "file: " << path << "\n";
        out << "snapshot[" << index << "]: fileOffset=" << offset
            << " ringBufLen=" << ringSize << "\n";
        PrintBlock(out, *block);
        PrintReadWriteUnavailable(out);
        out << "RingBufferOffsets: ringBufferStartOffset=0 ringBufferSize=" << ringSize
            << " readIdxMod=<unavailable> writeIdxMod=<unavailable>\n";
        PrintFirstHead(out, ring, static_cast<size_t>(ringSize), 0U);
        ParseTlvsByFixedFormat(out, window, fixedFormat);
        parsedRingBuffers++;
        std::cout << "[PROGRESS] parsedRingBuffers=" << parsedRingBuffers << "/" << selectedRingBuffers
                  << " ringIndex=" << index
                  << " totalRingBuffers=" << snapshotCount
                  << " status=parsed"
                  << " output=" << outputPath << "\n";
    }
    std::cout << "\n";
}
} // namespace

int main(int argc, char **argv)
{
    try {
        const Options opts = ParseArgs(argc, argv);
        for (const auto &file : opts.files) {
            ParseFile(file, opts);
        }
    } catch (const std::exception &e) {
        std::cerr << "error: " << e.what() << "\n";
        PrintUsage(argv[0]);
        return 1;
    }
    return 0;
}
