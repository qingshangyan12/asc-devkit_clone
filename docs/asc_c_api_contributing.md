# C API璐＄尞鎸囧崡

## 姒傝堪

C API 鏄疉scend C缂栫▼妗嗘灦涓殑鎸囦护灞侫PI锛岀洿鎺ュ搴旀槆鑵続I澶勭悊鍣ㄧ殑纭欢鎸囦护闆嗐€侰 API閲囩敤C璇█椋庢牸鐨勫嚱鏁版帴鍙ｏ紝涓虹畻瀛愬紑鍙戞彁渚涙渶鎺ヨ繎纭欢鐨勭紪绋嬭兘鍔涖€?

C API鐨勪富瑕佺壒鐐癸細

- **鎸囦护绾ф槧灏?*锛氭瘡涓狝PI鐩存帴鏄犲皠鍒颁竴鏉℃垨澶氭潯纭欢鎸囦护銆?
- **鎸囬拡寮忕紪绋?*锛氱洿鎺ユ搷浣滃唴瀛樺湴鍧€锛屾棤鎶借薄灞傚紑閿€銆?
- **楂樻€ц兘**锛氶浂寮€閿€灏佽锛屾渶澶у寲纭欢鎬ц兘銆?
- **绮剧粏鎺у埗**锛氭敮鎸佹祦姘寸嚎銆乵ask銆乻tride绛夊簳灞傛帶鍒躲€?

鏈枃妗ｄ粙缁嶅浣曞熀浜嶢scend C妗嗘灦寮€鍙戞柊鐨凜 API锛屽寘鎷紪绋嬪熀纭€銆佺洰褰曡鍒掋€佹灦鏋勮璁″拰鍥炲綊楠岃瘉瑕佹眰銆?

---

## 寮€鍙戞祦绋?

C API鐨勫紑鍙戞祦绋嬩富瑕佸寘鎷互涓嬫楠わ細

- **闇€姹傚垎鏋?*锛氭槑纭瓵PI鍔熻兘銆?
- **鎺ュ彛璁捐**锛氬畾涔塁椋庢牸鐨勫嚱鏁版帴鍙ｃ€?
- **瀹炵幇寮€鍙?*锛氱紪鍐欐寚浠ゅ皝瑁呭拰鍙傛暟杞崲浠ｇ爜銆?
- **娴嬭瘯楠岃瘉**锛氬畬鎴怣ock娴嬭瘯鍜屽姛鑳芥祴璇曘€?
- **鏂囨。缂栧啓**锛氬畬鎴怉PI鏂囨。鍜岀害鏉熻鏄庛€?

---

## API浠嬬粛

### 纭欢鎸囦护鍩虹

C API鐩存帴灏佽纭欢鎸囦护锛岄渶瑕佺悊瑙ｏ細

#### 鎸囦护绫诲瀷

- **鐭㈤噺鎸囦护**锛歏ADD銆乂MUL銆乂CONV銆乂REDUCE绛?
- **鏍囬噺鎸囦护**锛歋ADD銆丼MUL绛?
- **鏁版嵁鎼繍鎸囦护**锛歏MOV銆丏COPY绛?
- **鎺у埗鎸囦护**锛歋ETMASK銆丳IPEBARRIER绛?

#### 鎸囦护灞炴€?

- **娴佹按绾跨被鍨?*锛歏銆丮銆丼銆丮TE1/2/3绛?
- **鏁版嵁绫诲瀷**锛歨alf銆乫loat銆乮nt16銆乮nt32绛?
- **鎿嶄綔鏁颁釜鏁?*锛氫竴鍏冦€佷簩鍏冦€佷笁鍏冭繍绠?
- **鍚屾鐗规€?*锛氬紓姝?鍚屾鎵ц

### 缂栫▼妯″瀷

C API閲囩敤**鎸囬拡寮忕紪绋嬫ā鍨?*锛?

```cpp
// C API缂栫▼绀轰緥
constexpr uint32_t count = 64;
__ubuf__ half dst[count];   // Unified Buffer鍐呭瓨
__ubuf__ half src0[count];
__ubuf__ half src1[count];


// 鐩存帴璋冪敤C API
asc_add(dst, src0, src1, count);
```

#### 鏁版嵁娴佹ā寮?

C API鏀寔涓夌鏁版嵁娴佹ā寮忥細

妯″紡1锛氬墠n涓暟鎹绠?

```cpp
asc_add(dst, src0, src1, count);
```

- 杩炵画鍐呭瓨鍧楄绠?
- 鑷姩澶勭悊mask
- 绠€鍗曟槗鐢?

妯″紡2锛氶珮缁村垏鍒嗚绠?

```cpp
asc_add(dst, src0, src1, repeat,
        dst_block_stride, src0_block_stride, src1_block_stride,
        dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
```

- 闈炶繛缁唴瀛樿闂?
- 鏀寔澶氱淮鏁版嵁
- 鐏垫椿鎺у埗

妯″紡3锛氬悓姝ヨ绠?

```cpp
asc_add_sync(dst, src0, src1, count);
```

- 鑷姩鍚屾娴佹按绾?
- 淇濊瘉鏁版嵁涓€鑷存€?
- 鐢ㄤ簬鍏抽敭璺緞

### 鍙傛暟绾﹀畾

#### 閫氱敤鍙傛暟

| 鍙傛暟鍚?| 绫诲瀷 | 璇存槑 |
| -------- | ------ | ------ |
| dst | `__ubuf__ T*` | 杈撳嚭鍦板潃 |
| src0, src1 | `__ubuf__ T*` | 杈撳叆鍦板潃 |
| count | `uint32_t` | 鍏冪礌涓暟 |
| repeat | `uint8_t` | 杩唬娆℃暟 |
| block_stride | `uint8_t` | Block鍐呭湴鍧€姝ラ暱 |
| repeat_stride | `uint8_t` | Repeat闂村湴鍧€姝ラ暱 |
| mask0, mask1 | `uint64_t` | 璁＄畻mask |

#### Mask璁＄畻

- **鍗婄簿搴?*锛氭瘡repeat澶勭悊256瀛楄妭锛?28涓猦alf锛?
  - mask0瀵瑰簲浣?4涓厓绱狅紝mask1瀵瑰簲楂?4涓厓绱?
  - count=64鏃讹紝mask0=0xFFFFFFFFFFFFFFFF, mask1=0

- **鍗曠簿搴?*锛氭瘡repeat澶勭悊256瀛楄妭锛?4涓猣loat锛?
  - mask0瀵瑰簲鍏ㄩ儴64涓厓绱?
  - count=32鏃讹紝mask0=0x00000000FFFFFFFF

#### Stride璁＄畻

- **Block Stride**锛氬崟娆¤凯浠ｅ唴涓嶅悓DataBlock鐨勫湴鍧€鍋忕Щ
  - 閫氬父涓?锛堣繛缁闂級
  - 鍙敤浜庤浆缃€佸垎鍧楄闂?

- **Repeat Stride**锛氱浉閭昏凯浠ｉ棿鐩稿悓DataBlock鐨勫湴鍧€鍋忕Щ
  - 鍗婄簿搴︼細8锛?56瀛楄妭瀵归綈锛?
  - 鍗曠簿搴︼細4锛?56瀛楄妭瀵归綈锛?

---

## 鐩綍瑙勫垝

### 鐩綍缁撴瀯

```text
asc-devkit/
鈹溾攢鈹€ include/
鈹?  鈹斺攢鈹€ c_api/                    # C API澶存枃浠?
鈹?      鈹溾攢鈹€ vector_compute/       # 鐭㈤噺璁＄畻
鈹?      鈹?  鈹斺攢鈹€ vector_compute.h
鈹?      鈹溾攢鈹€ scalar_compute/       # 鏍囬噺璁＄畻
鈹?      鈹?  鈹斺攢鈹€ scalar_compute.h
鈹?      鈹溾攢鈹€ vector_datamove/      # 鐭㈤噺鏁版嵁鎼繍
鈹?      鈹?  鈹斺攢鈹€ vector_datamove.h
鈹?      鈹溾攢鈹€ cube_compute/         # Cube璁＄畻
鈹?      鈹?  鈹斺攢鈹€ cube_compute.h
鈹?      鈹溾攢鈹€ cube_datamove/        # Cube鏁版嵁鎼繍
鈹?      鈹?  鈹斺攢鈹€ cube_datamove.h
鈹?      鈹溾攢鈹€ sync/                 # 鍚屾鎺у埗
鈹?      鈹?  鈹斺攢鈹€ sync.h
鈹?      鈹溾攢鈹€ cache_ctrl/           # 缂撳瓨鎺у埗
鈹?      鈹?  鈹斺攢鈹€ cache_ctrl.h
鈹?      鈹溾攢鈹€ atomic/               # 鍘熷瓙鎿嶄綔
鈹?      鈹?  鈹斺攢鈹€ atomic.h
鈹?      鈹溾攢鈹€ sys_var/              # 绯荤粺鍙橀噺
鈹?      鈹?  鈹斺攢鈹€ sys_var.h
鈹?      鈹溾攢鈹€ misc/                 # 鏉傞」
鈹?      鈹?  鈹斺攢鈹€ misc.h
鈹?      鈹溾攢鈹€ utils/                # 宸ュ叿鍜屽父閲?
鈹?      鈹?  鈹溾攢鈹€ debug/
鈹?      鈹?  鈹?  鈹溾攢鈹€ asc_dump.h
鈹?      鈹?  鈹?  鈹溾攢鈹€ asc_printf.h
鈹?      鈹?  鈹?  鈹斺攢鈹€ asc_assert.h
鈹?      鈹?  鈹溾攢鈹€ c_api_constants.h
鈹?      鈹?  鈹溾攢鈹€ c_api_union.h
鈹?      鈹?  鈹斺攢鈹€ c_api_utils_intf.h
鈹?      鈹溾攢鈹€ asc_simd.h            # 鎬诲叆鍙?
鈹?      鈹斺攢鈹€ c_api_utils_intf.h    # 宸ュ叿鎺ュ彛
鈹?
鈹溾攢鈹€ impl/
鈹?  鈹斺攢鈹€ c_api/                    # C API瀹炵幇
鈹?      鈹溾攢鈹€ instr_impl/           # 鎸囦护瀹炵幇
鈹?      鈹?  鈹溾攢鈹€ npu_arch_2201/    # NPU ARCH 220X鏋舵瀯
鈹?      鈹?  鈹?  鈹溾攢鈹€ vector_compute_impl/
鈹?      鈹?  鈹?  鈹?  鈹溾攢鈹€ asc_abs_impl.h
鈹?      鈹?  鈹?  鈹?  鈹斺攢鈹€ ...
鈹?      鈹?  鈹?  鈹溾攢鈹€ vector_datamove_impl/
鈹?      鈹?  鈹?  鈹?  鈹溾攢鈹€ asc_copy_gm2ub_align_impl.h
鈹?      鈹?  鈹?  鈹?  鈹斺攢鈹€ ...
鈹?      鈹?  鈹?  鈹溾攢鈹€ vector_compute_impl.h
鈹?      鈹?  鈹?  鈹溾攢鈹€ vector_datamove_impl.h
鈹?      鈹?  鈹?  鈹斺攢鈹€ ...
鈹?      鈹?  鈹斺攢鈹€ npu_arch_3510/    # NPU ARCH 351X鏋舵瀯
鈹?      鈹?      鈹溾攢鈹€ vector_compute_impl/
鈹?      鈹?      鈹?  鈹溾攢鈹€ asc_abs_impl.h
鈹?      鈹?      鈹?  鈹斺攢鈹€ ...
鈹?      鈹?      鈹溾攢鈹€ vector_compute_impl.h
鈹?      鈹?      鈹斺攢鈹€ ...
鈹?      鈹溾攢鈹€ utils/
鈹?      鈹?  鈹斺攢鈹€ c_api_utils_impl.cpp
鈹?      鈹斺攢鈹€ stub/                 # Stub
鈹?          鈹斺攢鈹€ cce_stub.h
鈹?
鈹溾攢鈹€ tests/
鈹?  鈹斺攢鈹€ api/
鈹?      鈹斺攢鈹€ c_api/                # C API娴嬭瘯
鈹?          鈹溾攢鈹€ npu_arch_2201/    # NPU ARCH 220X鏋舵瀯
鈹?          鈹?  鈹溾攢鈹€ vector_compute/
鈹?          鈹?  鈹?  鈹溾攢鈹€ test_add_instr.cpp
鈹?          鈹?  鈹?  鈹溾攢鈹€ test_axpy_instr.cpp
鈹?          鈹?  鈹?  鈹斺攢鈹€ ...
鈹?          鈹?  鈹溾攢鈹€ scalar_compute/
鈹?          鈹?  鈹斺攢鈹€ ...
鈹?          鈹溾攢鈹€ npu_arch_3510/    # NPU ARCH 351X鏋舵瀯
鈹?          鈹?  鈹斺攢鈹€ ...
鈹?          鈹斺攢鈹€ common/           # 閫氱敤娴嬭瘯
鈹?              鈹斺攢鈹€ test_utils.cpp
鈹?
鈹斺攢鈹€ docs/
    鈹斺攢鈹€ api/
        鈹斺攢鈹€ context/
            鈹斺攢鈹€ c_api/            # C API鏂囨。
                鈹溾攢鈹€ vector_compute/
                鈹?  鈹溾攢鈹€ asc_add.md
                鈹?  鈹溾攢鈹€ asc_muls.md
                鈹?  鈹斺攢鈹€ ...
                鈹溾攢鈹€ scalar_compute/
                鈹溾攢鈹€ vector_datamove/
                鈹溾攢鈹€ general_instruction.md  # 閫氱敤璇存槑
                鈹斺攢鈹€ c_api_list.md           # API鍒楄〃
```

### 鏂囦欢缁勭粐鍘熷垯

1. **鎸夊姛鑳藉垎绫?*锛氱煝閲忚绠椼€佹爣閲忚绠椼€佹暟鎹惉杩愮瓑
2. **鎸夋灦鏋勯殧绂?*锛氫笉鍚孨PU鏋舵瀯鐨勫疄鐜板垎寮€缁勭粐
3. **鎺ュ彛涓庡疄鐜板垎绂?*锛歩nclude/瀛樻斁澹版槑锛宨mpl/瀛樻斁瀹炵幇
4. **娴嬭瘯鎸夋灦鏋勭粍缁?*锛氫究浜庡鏋舵瀯娴嬭瘯鍜岀淮鎶?

### 鏂囦欢鍛藉悕瑙勮寖

- **澶存枃浠?*锛歚<category>.h`锛屽 `vector_compute.h`
- **瀹炵幇鏂囦欢**锛歚<category>_c_api_impl.h`锛屽 `vector_compute_c_api_impl.h`
- **娴嬭瘯鏂囦欢**锛歚test_<api>_instr.cpp`锛屽 `test_add_instr.cpp`
- **鏂囨。鏂囦欢**锛歚<api_name>.md`锛屽 `asc_add.md`

---

## 寮€鍙戠ず渚嬶細瀹炵幇asc_axpy C API

### 闇€姹傚垎鏋?

瀹炵幇鐭㈤噺涔樺姞C API锛歚dst = src * scalar + dst`

- 鏀寔鏁版嵁绫诲瀷锛歨alf, float
- 鏀寔涓夌璋冪敤妯″紡
- 鐩存帴鏄犲皠纭欢鎸囦护

### 鎺ュ彛璁捐

鍦?`include/c_api/vector_compute/vector_compute.h` 涓坊鍔狅細

```cpp
// ==========asc_axpy(half/float)==========
// 杩炵画妯″紡
__aicore__ inline void asc_axpy(__ubuf__ half* dst, __ubuf__ half* src, half scalar, uint32_t count);

__aicore__ inline void asc_axpy(__ubuf__ float* dst, __ubuf__ float* src, float scalar, uint32_t count);

// 閰嶇疆妯″紡
__aicore__ inline void asc_axpy(__ubuf__ half* dst, __ubuf__ half* src, half value, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_axpy(__ubuf__ float* dst, __ubuf__ float* src, float value, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

// 鍚屾妯″紡
__aicore__ inline void asc_axpy_sync(__ubuf__ half* dst, __ubuf__ half* src, half scalar, uint32_t count);

__aicore__ inline void asc_axpy_sync(__ubuf__ float* dst, __ubuf__ float* src, float scalar, uint32_t count);
```

### 瀹炵幇浠ｇ爜

鍙傝€冨叾浠栨帴鍙ｅ疄鐜?

---

## 娴嬭瘯楠岃瘉瑕佹眰

### 娴嬭瘯妗嗘灦

C API娴嬭瘯浣跨敤 **gTest + MockCPP** 妗嗘灦锛?

```cpp
#include <gtest/gtest.h>
#include <mockcpp/mockcpp.hpp>
#include "c_api/stub/cce_stub.h"
#include "c_api/asc_simd.h"
```

### 娴嬭瘯绫诲瀷

#### Mock娴嬭瘯锛堟寚浠ゆ槧灏勯獙璇侊級

楠岃瘉C API鏄惁姝ｇ‘璋冪敤搴曞眰鎸囦护銆?

#### 鍔熻兘娴嬭瘯锛堟纭€ч獙璇侊級

楠岃瘉API璁＄畻缁撴灉姝ｇ‘銆?

---

## 浠ｇ爜瑙勮寖

### 鍛藉悕瑙勮寖

```cpp
// 鍑芥暟鍚嶏細灏忓啓瀛楁瘝+涓嬪垝绾匡紝asc_<operation>
void asc_add(...);
void asc_axpy(...);

// 鍙傛暟鍚嶏細灏忛┘宄版垨涓嬪垝绾?
__ubuf__ half* dstTensor;
uint32_t elementCount;

// 瀹忓畾涔夛細鍏ㄥぇ鍐?涓嬪垝绾?
#define ASC_CAPI_MAX_COUNT 4096

// 绫诲瀷鍚嶏細澶ч┘宄?
struct AscUnaryConfig;
```

### 浠ｇ爜椋庢牸

```cpp
// 1. 缂╄繘锛?绌烘牸
// 2. 澶ф嫭鍙凤細K&R椋庢牸
// 3. 绌烘牸锛氭搷浣滅鍓嶅悗绌烘牸锛岄€楀彿鍚庣┖鏍?
// 4. 娉ㄩ噴锛欴oxygen椋庢牸

/**
 * @brief 鐭㈤噺涔樺姞杩愮畻
 * @param dst 鐩爣鍦板潃
 * @param src 婧愬湴鍧€
 * @param scalar 鏍囬噺鍊?
 * @param tmp 涓存椂buffer
 * @param count 鍏冪礌涓暟
 */
__aicore__ inline void asc_axpy(__ubuf__ half* dst, __ubuf__ half* src, half scalar, uint32_t count)
{
    // 鍙傛暟楠岃瘉
    ASCENDC_ASSERT(count > 0, "count must be positive");
    ASCENDC_ASSERT(dst != nullptr && src != nullptr && tmp != nullptr,
                   "pointers cannot be null");

    // 璁＄畻閫昏緫
    uint8_t repeat = (count + 127) / 128;
    uint64_t mask0 = static_cast<uint64_t>(-1);
    uint64_t mask1 = static_cast<uint64_t>(-1);

    // 璁剧疆mask
    set_vector_mask(mask1, mask0);

    // 鎵ц鎸囦护
    vmuls(tmp, src, scalar, repeat, 1, 1, 8, 8);
    pipe_barrier(PIPE_V);

    vadd(dst, dst, tmp, repeat, 1, 1, 1, 8, 8, 8);
    pipe_barrier(PIPE_V);
}
```

### 閿欒澶勭悊

```cpp
// 1. 鍙傛暟楠岃瘉锛圖ebug妯″紡锛?
ASCENDC_ASSERT(count > 0, "count must be greater than 0");
ASCENDC_ASSERT(dst != nullptr, "dst cannot be nullptr");
ASCENDC_ASSERT(src != nullptr, "src cannot be nullptr");
ASCENDC_ASSERT(tmp != nullptr, "tmp cannot be nullptr");

// 2. 鍦板潃瀵归綈妫€鏌?
ASCENDC_ASSERT((reinterpret_cast<uintptr_t>(dst) % 32) == 0,
               "dst must be 32-byte aligned");
ASCENDC_ASSERT((reinterpret_cast<uintptr_t>(src) % 32) == 0,
               "src must be 32-byte aligned");

// 3. 鑼冨洿妫€鏌?
ASCENDC_ASSERT(count <= ASC_CAPI_MAX_COUNT,
               "count exceeds maximum supported value");

// 4. 鏋舵瀯妫€鏌?
#if !defined(__NPU_ARCH__) || (__NPU_ARCH__ != 2201 && __NPU_ARCH__ != 3510)
    #error "Unsupported NPU architecture"
#endif
```

### 瀹忎繚鎶?

```cpp
// 澶存枃浠朵繚鎶?
#ifndef INCLUDE_C_API_VECTOR_COMPUTE_VECTOR_COMPUTE_H
#define INCLUDE_C_API_VECTOR_COMPUTE_VECTOR_COMPUTE_H

// 鏉′欢缂栬瘧
#if defined(__NPU_ARCH__) && __NPU_ARCH__ == 2201
    // NPU ARCH 2201鏋舵瀯鐗瑰畾浠ｇ爜
#elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
    // NPU ARCH 3510鏋舵瀯鐗瑰畾浠ｇ爜
#endif

#endif
```

---
