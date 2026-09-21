"""Execute the production FIFO sender with host memory/transport substitutions.

Device compilation is checked separately. Only transport and DMA intrinsics are
substituted here; packet selection, bounds, serialization and publication use
functions extracted verbatim from the production headers.
"""
from pathlib import Path
import subprocess

ROOT = Path(__file__).resolve().parents[4]


def function(text, name):
    import re
    match = re.search(r'(?m)^(?:template <[^\n]+>\n)?__aicore__[^\n]*\b' + name + r'\(', text)
    assert match, name
    begin = text.index('{', match.start())
    depth = 1
    end = begin + 1
    while depth:
        depth += (text[end] == '{') - (text[end] == '}')
        end += 1
    return text[match.start():end]


def test_sender(tmp_path):
    utils = (ROOT/'impl/utils/debug/asc_debug_utils.h').read_text()
    dump = (ROOT/'impl/utils/debug/asc_aicore_dump_impl.h').read_text()
    functions = [function(utils, name) for name in ['set_super_tensor_header']]
    functions += [function(dump, name) for name in [
        'set_dump_tlv_info', 'set_dump_shape_info', 'set_dump_tlv_data',
        'copy_super_tensor_data', 'asc_dump_super_tensor', 'check_super_tensor_local_range', 'asc_dump_impl']]
    code = r'''
#include <cassert>
#include <cstdint>
#include <cstring>
#include <vector>
#include <algorithm>
#include <fstream>
#define __aicore__
#define __gm__
#define __ubuf__
#define __NPU_ARCH__ 3510
#include "impl/utils/debug/asc_debug_types.h"
namespace AscendC { enum class Hardware { GM, UB, L1, L0A, L0B, L0C, BIAS, FIXBUF }; }
namespace __asc_aicore {
constexpr uint32_t K_MAX_SHAPE_DIM=8, ASC_ONE_DATABLOCK_SIZE=32;
DebugBlockHeadInfo block;
DebugBlockWriteInfo writer;
std::vector<uint8_t> ring;
std::vector<std::vector<uint8_t>> packets;
const uint8_t* sourceBegin;
const uint8_t* sourceEnd;
bool reject=false;
uint32_t align_up(uint32_t n,uint32_t a) { return (n+a-1)/a*a; }
void sync_all() {}
void asc_entire_dcci(uint64_t*) {}
uint32_t asc_debug_get_block_idx() { return 3; }
template<class T> constexpr DumpTensorDataType get_dump_datatype() { return DumpTensorDataType::ACL_UINT8; }
DebugBlockHeadInfo* get_block_info() { return &block; }
DebugBlockWriteInfo* get_block_write_info(DebugBlockHeadInfo*) { return &writer; }
uint8_t* get_ringbuf_tlv_addr(DebugBlockHeadInfo*) { return ring.data()+writer.bufOffset; }
bool check_ringbuf_space(DebugBlockHeadInfo*,const uint32_t& size) {
    if (reject || size>block.ringBufLen) return false;
    if (writer.bufOffset+size>block.ringBufLen) writer.bufOffset=0;
    return true;
}
void update_write_info(DebugBlockWriteInfo*,const uint32_t& span) {
    assert(writer.bufOffset+span<=block.ringBufLen);
    packets.emplace_back(ring.begin()+writer.bufOffset,ring.begin()+writer.bufOffset+span);
    writer.bufOffset+=span;
}
uint32_t mem_copy_gm_to_gm(uint8_t* dst,const uint8_t* src,const uint32_t& n) {
    std::memcpy(dst,src,n); return 0;
}
template<class D,class S> uint32_t dma(D* dst,S* src,uint32_t bytes) {
    assert(reinterpret_cast<const uint8_t*>(src)>=sourceBegin);
    assert(reinterpret_cast<const uint8_t*>(src)+bytes<=sourceEnd);
    std::memcpy(dst,src,bytes); return 0;
}
template<class D,class S> uint32_t mem_copy_ub_to_gm_impl(D* dst,S* src,uint32_t blocks) { return dma(dst,src,blocks*32); }
#define COPY_STUB(name) template<class D,class S> uint32_t name(D* dst,S* src,uint32_t bytes) { return dma(dst,src,bytes); }
COPY_STUB(mem_copy_l1buf_to_gm_impl)
COPY_STUB(mem_copy_abuf_to_gm_impl)
COPY_STUB(mem_copy_bbuf_to_gm_impl)
COPY_STUB(mem_copy_cbuf_to_gm_impl)
COPY_STUB(mem_copy_biasbuf_to_gm_impl)
COPY_STUB(mem_copy_fbuf_to_gm_impl)
'''+ '\n'.join(functions) + r'''
}
using namespace __asc_aicore;
void setup(uint32_t capacity) {
    block.ringBufLen=capacity; ring.assign(capacity,0xDA); block.ringBufAddr=reinterpret_cast<uintptr_t>(ring.data()); writer.bufOffset=0; packets.clear(); reject=false;
}
void verify(const std::vector<uint8_t>& input,bool super) {
    size_t offset=0;
    for (const auto& packet: packets) {
        uint32_t tag; std::memcpy(&tag,packet.data(),4);
        uint32_t size,head;
        if (!super) {
            auto h=reinterpret_cast<const DumpTensorTlv*>(packet.data()); assert(tag==2);
            size=h->dumpSize; head=sizeof(*h);
        } else if (!offset) {
            auto h=reinterpret_cast<const DumpSuperTensorTlv*>(packet.data()); assert(tag==11);
            assert(h->tensorOffset==0 && h->tensorLength==input.size() && h->blockIdx==3);
            assert(h->dim==2 && h->shape[0]==2 && h->shape[1]==4 && h->shape[2]==0);
            size=h->dumpSize; head=sizeof(*h);
        } else {
            auto h=reinterpret_cast<const DumpSuperTensorBodyTlv*>(packet.data()); assert(tag==12);
            assert(h->tensorOffset==offset && h->tensorLength==input.size());
            size=h->dumpSize; head=sizeof(*h);
        }
        assert(size && offset+size<=input.size());
        assert(std::equal(input.begin()+offset,input.begin()+offset+size,packet.begin()+head));
        offset+=size;
    }
    assert(offset==input.size());
}
int main(int argc,char** argv) {
    uint32_t shape[]={2,4};
    for (uint32_t capacity: {256,512,1024,4096}) {
        uint32_t limit=(capacity-sizeof(DumpTensorTlv))/32*32;
        for (uint32_t size: {0U,1U,limit,limit+1,capacity*9+17}) {
            setup(capacity); std::vector<uint8_t> input(size);
            for(size_t i=0;i<size;++i) input[i]=i%251;
            sourceBegin=input.data(); sourceEnd=input.data()+size;
            asc_dump_impl<AscendC::Hardware::GM,uint8_t>(input.data(),7,size,shape,2);
            verify(input,size>limit);
            if (size>limit && capacity>=512) {
                setup(capacity);
                assert(check_super_tensor_local_range(AscendC::Hardware::UB,size,size));
                assert(!check_super_tensor_local_range(AscendC::Hardware::UB,size,size-1));
                asc_dump_impl<AscendC::Hardware::UB,uint8_t>(input.data(),7,size,shape,2);
                verify(input,true);
            }
        }
    }
    setup(4096); std::vector<uint8_t> data(32785,19); sourceBegin=data.data();sourceEnd=data.data()+data.size();
    if (check_super_tensor_local_range(AscendC::Hardware::L1,data.size(),data.size())) {
        asc_dump_impl<AscendC::Hardware::L1,uint8_t>(data.data(),7,data.size(),shape,2);
    }
    assert(packets.empty()); // reject unprovable aligned local read before publishing the first packet
    for (auto position: {AscendC::Hardware::L1, AscendC::Hardware::L0C}) {
        setup(4096);
        const uint32_t alignment = position == AscendC::Hardware::L0C ? 1024 : 32;
        const uint32_t capacity = align_up(data.size(),alignment);
        assert(!check_super_tensor_local_range(position,data.size(),capacity-1));
        assert(check_super_tensor_local_range(position,data.size(),capacity));
        std::vector<uint8_t> storage(capacity,19);
        sourceBegin=storage.data(); sourceEnd=storage.data()+storage.size();
        if (position == AscendC::Hardware::L1) {
            asc_dump_impl<AscendC::Hardware::L1,uint8_t>(storage.data(),7,data.size(),shape,2);
        } else {
            asc_dump_impl<AscendC::Hardware::L0C,uint8_t>(storage.data(),7,data.size(),shape,2);
        }
        verify(data,true);
    }
    setup(4096);
    // Ordinary requests retain the existing behavior; this guard only handles oversized requests.
    assert(check_super_tensor_local_range(AscendC::Hardware::L1,16,16));
    sourceBegin=data.data();sourceEnd=data.data()+data.size();
    reject=true;
    asc_dump_impl<AscendC::Hardware::GM,uint8_t>(data.data(),7,data.size(),shape,2);
    assert(packets.empty()); // failed reservation must not advance/publish a fragment
    setup(4096);
    asc_dump_impl<AscendC::Hardware::GM,uint8_t>(data.data(),7,data.size(),shape,2);
    if (argc>1) {
        std::ofstream output(argv[1],std::ios::binary);
        uint8_t blockHeader[56]={}; uint16_t magic=0xAE86;std::memcpy(blockHeader+16,&magic,2);
        output.write(reinterpret_cast<char*>(blockHeader),56);
        for(auto& packet:packets) output.write(reinterpret_cast<char*>(packet.data()),packet.size());
    }
}
'''
    source=tmp_path/'sender.cpp';source.write_text(code)
    exe=tmp_path/'sender'
    subprocess.run(['g++','-std=c++17','-O1','-g','-fsanitize=address,undefined','-I',str(ROOT),str(source),'-o',str(exe)],check=True)
    subprocess.run([str(exe),str(tmp_path/'sender.bin')],check=True)


def test_wire_layout(tmp_path):
    source = Path(__file__).with_name('test_layout.cpp')
    subprocess.run(['g++', '-std=c++17', '-I', str(ROOT), str(source), '-o', str(tmp_path/'layout')], check=True)
    subprocess.run([str(tmp_path/'layout')], check=True)
