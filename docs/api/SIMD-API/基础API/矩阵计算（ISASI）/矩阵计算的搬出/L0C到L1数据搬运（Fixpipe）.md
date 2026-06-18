# L0C鍒癓1鏁版嵁鎼繍锛團ixpipe锛?a id="ZH-CN_TOPIC_0000002538231198"></a>

## 浜у搧鏀寔鎯呭喌<a id="zh-cn_topic_0000002511188540_section796754519912"></a>

| 浜у搧 | 鏄惁鏀寔 |
| ---------- | :----------: |
| <cann-filter npu-type = "950">Ascend 950PR/Ascend 950DT | 鏀寔鍖呭惈FixpipeParamsV220/FixpipeParamsArch3510鍙傛暟鐨勬帴鍙ｃ€?</cann-filter> |
| <cann-filter npu-type = "A3">Atlas A3 璁粌绯诲垪浜у搧/Atlas A3 鎺ㄧ悊绯诲垪浜у搧 | 浠呮敮鎸佸寘鍚獸ixpipeParamsV220鍙傛暟鐨勬帴鍙ｃ€?</cann-filter> |
| <cann-filter npu-type = "910b">Atlas A2 璁粌绯诲垪浜у搧/Atlas A2 鎺ㄧ悊绯诲垪浜у搧 | 浠呮敮鎸佸寘鍚獸ixpipeParamsV220鍙傛暟鐨勬帴鍙ｃ€?</cann-filter> |
| <cann-filter npu-type = "310b">Atlas 200I/500 A2 鎺ㄧ悊浜у搧 | x </cann-filter> |
| <cann-filter npu-type = "310p">Atlas 鎺ㄧ悊绯诲垪浜у搧AI Core | x </cann-filter> |
| <cann-filter npu-type = "310p">Atlas 鎺ㄧ悊绯诲垪浜у搧Vector Core | x </cann-filter> |
| <cann-filter npu-type = "910">Atlas 璁粌绯诲垪浜у搧 | x </cann-filter> |
| <cann-filter npu-type = "x90">Kirin X90 | 鈭?</cann-filter> |
| <cann-filter npu-type = "9030">Kirin 9030 | 鈭?</cann-filter> |

## 鍔熻兘璇存槑<a id="zh-cn_topic_0000002511188540_section106841136114319"></a>

澶存枃浠惰矾寰勪负锛歜asic_api/kernel_operator_fixpipe_intf.h銆乥asic_api/kernel_operator_data_copy_intf.h銆?

鐭╅樀璁＄畻鐨勭粨鏋滃瓨鏀惧湪L0C Buffer锛孎ixpipe鎺ュ彛鐢ㄤ簬灏嗙粨鏋滄惉杩愯嚦L1 Buffer涓紝骞朵笖鍦ㄦ惉杩愯繃绋嬩腑鏀寔闅忚矾鏍煎紡杞崲绛夋搷浣溿€?

<cann-filter npu-type = "A3,910b">

浠ュ涓嬩骇鍝佸瀷鍙蜂负渚嬶細

<cann-filter npu-type = "A3">

Atlas A3 璁粌绯诲垪浜у搧/Atlas A3 鎺ㄧ悊绯诲垪浜у搧锛?

</cann-filter>

<cann-filter npu-type = "910b">

Atlas A2 璁粌绯诲垪浜у搧/Atlas A2 鎺ㄧ悊绯诲垪浜у搧锛?

</cann-filter>

涓嬪浘灞曠ず浜嗛殢璺噺鍖栥€侀殢璺疪eLU銆侀殢璺€氶亾鍚堝苟鐨勬湁鏁堢粍鍚堛€佷腑闂存暟鎹被鍨嬪拰鏁版嵁璺緞銆備笅鍥句腑鐨凢32-\>F16涓嶧32-\>BF16涓洪潪閲忓寲妯″紡锛屼粎涓篊ast锛屽叾浣欎负闅忚矾scalar/tensor閲忓寲妯″紡銆?

**鍥?1** L0C2L1娴佺▼鍥?a id="zh-cn_topic_0000002511188540_fig8956371257"></a>  

![](../../../../figures/L0C2L1_Function_Combination.png)

</cann-filter>

## 鍑芥暟鍘熷瀷<a id="zh-cn_topic_0000002511188540_section82039854412"></a>

涓轰簡鏂逛究鐢ㄦ埛浣跨敤锛屽噺灏戝涔犵悊瑙ｆ垚鏈紝閽堝鐢ㄦ埛缁忓父浣跨敤鐨勫満鏅粍鍚堬紝鎻愪緵浜嗙煩闃垫惉鍑虹殑缁勫悎鎺ュ彛Fixpipe锛屾帴鍙ｅ唴鍖呭惈浜嗚缃瘎瀛樺櫒涓庢暟鎹惉杩愯兘鍔涳紝瀵瑰簲鐨勬帴鍙ｅ涓嬶細

**浼犲叆FixpipeParamsArch3510鍙傛暟鐨勬帴鍙ｏ細**

- 閫氳矾L0C Buffer(CO1)->L1 Buffer(C1)锛屼笉寮€鍚殢璺痆tensor閲忓寲](./鍏抽敭鐗规€ц鏄?闅忚矾閲忓寲.md)鍔熻兘锛?

    ```cpp
    template <typename T, typename U, const FixpipeConfig& config = CFG_ROW_MAJOR>
    __aicore__ inline void Fixpipe(const LocalTensor<T>& dst, const LocalTensor<U>& src, const FixpipeParamsArch3510<config.format>& intriParams)
    ```

- 閫氳矾L0C Buffer(CO1)->L1 Buffer(C1)锛屽紑鍚殢璺痆tensor閲忓寲](./鍏抽敭鐗规€ц鏄?闅忚矾閲忓寲.md)鍔熻兘锛?

    ```cpp
    template <typename T, typename U, const FixpipeConfig& config = CFG_ROW_MAJOR>
    __aicore__ inline void Fixpipe(const LocalTensor<T>& dst, const LocalTensor<U>& src, const LocalTensor<uint64_t>& cbufWorkspace, const FixpipeParamsArch3510<config.format>& intriParams)
    ```

**浼犲叆FixpipeParamsV220鍙傛暟鐨勬帴鍙ｏ細**

- 閫氳矾L0C Buffer(CO1)->L1 Buffer(C1)锛屼笉寮€鍚殢璺痆tensor閲忓寲](./鍏抽敭鐗规€ц鏄?闅忚矾閲忓寲.md)鍔熻兘锛?

    ```cpp
    template <typename T, typename U, const FixpipeConfig& config = CFG_ROW_MAJOR>
    __aicore__ inline void Fixpipe(const LocalTensor<T>& dst, const LocalTensor<U>& src, const FixpipeParamsV220& intriParams)
    ```

- 閫氳矾L0C Buffer(CO1)->L1 Buffer(C1)锛屽紑鍚殢璺痆tensor閲忓寲](./鍏抽敭鐗规€ц鏄?闅忚矾閲忓寲.md)鍔熻兘锛?

    ```cpp
    template <typename T, typename U, const FixpipeConfig& config = CFG_ROW_MAJOR, typename S = uint64_t, typename Std::enable_if<Std::is_same<PrimT<S>, uint64_t>::value, bool>::type = true>
    __aicore__ inline void Fixpipe(const LocalTensor<T>& dst, const LocalTensor<U>& src, const LocalTensor<S>& cbufWorkspace, const FixpipeParamsV220& intriParams)
    ```

## 鍙傛暟璇存槑<a id="zh-cn_topic_0000002511188540_section16128134420472"></a>

**琛?1** Fixpipe妯℃澘鍙傛暟璇存槑

| 鍙傛暟鍚?| 鎻忚堪 |
| ---------- | ---------- |
| T/U | 鐩殑鎿嶄綔鏁?婧愭搷浣滄暟鐨勬暟鎹被鍨嬨€傛敮鎸佺殑鏁版嵁绫诲瀷璇峰弬鑰僛鏁版嵁绫诲瀷](#zh-cn_topic_0000002511188540_section4219135304818)銆?|
| config | Fixpipe鐩稿叧閰嶇疆鍙傛暟锛岀被鍨嬩负FixpipeConfig銆傚彇鍊煎涓嬶細<br>&nbsp;&nbsp;&bull; **CFG_ROW_MAJOR锛堥粯璁ゅ彇鍊硷級**锛氬紑鍚疦Z2ND锛岃緭鍑烘暟鎹牸寮忎负ND鏍煎紡銆?cann-filter npu-type = "A3,910b">閽堝<cann-filter npu-type = "A3">Atlas A3 璁粌绯诲垪浜у搧/Atlas A3 鎺ㄧ悊绯诲垪浜у搧锛?/cann-filter><cann-filter npu-type = "910b">Atlas A2 璁粌绯诲垪浜у搧/Atlas A2 鎺ㄧ悊绯诲垪浜у搧锛?/cann-filter>鍦↙0C Buffer -> L1 Buffer閫氳矾涓嬩笉鐢熸晥銆?/cann-filter><br>&nbsp;&nbsp;&bull; CFG_NZ锛歂Z2NZ锛岃緭鍑烘暟鎹牸寮忎负NZ鏍煎紡銆?br><cann-filter npu-type = "950">&nbsp;&nbsp;&bull; CFG_COLUMN_MAJOR锛氶拡瀵笰scend 950PR/Ascend 950DT锛屽紑鍚疦Z2DN锛岃緭鍑烘暟鎹牸寮忎负DN鏍煎紡銆?br></cann-filter><pre>struct FixpipeConfig {<br>    CO2Layout format;<br>    bool isToUB; // 鐢ㄤ簬鐢ㄦ埛鎸囧畾鐩殑鍦板潃鐨勪綅缃槸鍚︽槸UB <br>};<br>enum class CO2Layout : uint8_t {<br>    NZ = 0, // 杈撳嚭鏁版嵁鏍煎紡浠嶄负NZ鏍煎紡銆?br>    ROW_MAJOR, // 寮€鍚疦Z2ND锛岃緭鍑烘暟鎹牸寮忎负ND鏍煎紡銆?br><cann-filter npu-type = "950">    COLUMN_MAJOR, // 浠匒scend 950PR/Ascend 950DT鏀寔锛屽紑鍚疦Z2DN锛岃緭鍑烘暟鎹牸寮忎负DN鏍煎紡銆?br></cann-filter>};<br>constexpr FixpipeConfig CFG_NZ = {CO2Layout::NZ};<br>constexpr FixpipeConfig CFG_ROW_MAJOR = {CO2Layout::ROW_MAJOR};<cann-filter npu-type = "950"><br>constexpr FixpipeConfig CFG_COLUMN_MAJOR = {CO2Layout::COLUMN_MAJOR}; // 浠匒scend 950PR/Ascend 950DT鏀寔</cann-filter></pre> |
| S | 鍙傛暟cbufWorkspace鐨勬暟鎹被鍨嬶紝鍗抽殢璺噺鍖栧弬鏁扮殑鏁版嵁绫诲瀷銆?br>&nbsp;&nbsp;&bull; 褰撶洰鐨勬搷浣滄暟銆佹簮鎿嶄綔鏁般€乧bufWorkspace浣跨敤鍩虹鏁版嵁绫诲瀷鏃讹紝妯℃澘鍙傛暟S蹇呴』涓簎int64_t绫诲瀷锛屽惁鍒欑紪璇戝け璐ャ€?br>&nbsp;&nbsp;&bull; 褰撶洰鐨勬搷浣滄暟銆佹簮鎿嶄綔鏁般€乧bufWorkspace浣跨敤TensorTrait绫诲瀷鏃讹紝妯℃澘鍙傛暟S鐨凩iteType蹇呴』涓簎int64_t绫诲瀷锛屽惁鍒欑紪璇戝け璐ャ€?br>妯℃澘鍙傛暟S鍚庝竴涓ā鏉垮弬鏁颁粎鐢ㄤ簬涓婅堪鏁版嵁绫诲瀷妫€鏌ワ紝鐢ㄦ埛鏃犻渶鍏虫敞銆?|

**琛?2** Fixpipe鍙傛暟璇存槑

| 鍙傛暟鍚嶇О | 杈撳叆/杈撳嚭 | 鍚箟 |
| ---------- | ---------- | ---------- |
| dst | 杈撳嚭 | 鐩殑鎿嶄綔鏁帮紝绫诲瀷涓篖ocalTensor銆傛暟鎹牸寮忎负NZ鏍煎紡銆侼Z鍦板潃闇€婊¤冻32瀛楄妭瀵归綈銆?cann-filter npu-type = "950"><br>閽堝Ascend 950PR/Ascend 950DT锛岃繕鏀寔鏁版嵁鏍煎紡涓篘D銆丏N锛屽湴鍧€闇€婊¤冻32瀛楄妭瀵归綈銆?/cann-filter> |
| src | 杈撳叆 | 婧愭搷浣滄暟锛岀被鍨嬩负LocalTensor锛屾敮鎸佺殑鐗╃悊鍦板潃涓篖0C Buffer锛圱Position涓篊O1锛夛紝涓篗mad鎺ュ彛璁＄畻鐨勭粨鏋溿€傛暟鎹牸寮忎负NZ鏍煎紡锛屽湴鍧€闇€瑕佹弧瓒?瀵归綈銆?|
| intriParams | 杈撳叆 | Fixpipe鎼繍鍙傛暟锛屽叿浣撳畾涔夎鍙傝€?"basic_api/interface/kernel_struct_fixpipe.h"銆?br>鍙傛暟璇存槑璇峰弬鑰僃ixpipe鎼繍鍙傛暟锛團ixpipeParamsArch3510銆丗ixpipeParamsV220锛夌粨鏋勪綋璇存槑銆?|
| cbufWorkspace | 杈撳叆 | 閲忓寲鍙傛暟锛岀被鍨嬩负`LocalTensor<uint64_t>`锛屾敮鎸佺殑鐗╃悊鍦板潃涓篖1 Buffer锛圱Position涓篊1锛夛紝鍦板潃闇€婊¤冻32瀛楄妭瀵归綈銆?br>&nbsp;&nbsp;&bull; 褰搎uantPre涓篤DEQF16銆乂QF322B8_PRE銆乂REQ8鏃舵敮鎸併€?br><cann-filter npu-type = "950">&nbsp;&nbsp;&bull; 閽堝Ascend 950PR/Ascend 950DT锛岄櫎涓婅堪澶栬繕鏈塚QF322FP8_PRE銆乂QF322HIF8_PRE銆乂QF322HIF8_PRE_HYBRID銆乂QS322BF16_PRE銆乂QF322F16_PRE銆乂QF322BF16_PRE銆乂QF322F32_PRE鏀寔銆?br></cann-filter>quantPre浠嬬粛璇峰弬鑰僃ixpipe鎼繍鍙傛暟缁撴瀯浣撲腑quantPre閮ㄥ垎銆?|

**琛?3** Fixpipe鎼繍鍙傛暟锛團ixpipeParamsArch3510锛夌粨鏋勪綋璇存槑

| 鍙傛暟鍚嶇О | 鏁版嵁绫诲瀷 | 鍚箟 |
| ---------- | ---------- | ---------- |
| nSize | 杈撳叆 | 婧怤Z鐭╅樀鍦∟鏂瑰悜涓婄殑澶у皬锛屽彇鍊艰寖鍥翠负nSize 鈭圼0, 4095]銆?br>&nbsp;&nbsp;&bull; 鑻ュ紑鍚痗hannelSplit鍔熻兘锛宯Size蹇呴』涓?鐨勫€嶆暟銆?br>&nbsp;&nbsp;&bull; 鑻ヤ笉寮€鍚痗hannelSplit鍔熻兘锛宯Size蹇呴』涓?6鐨勫€嶆暟銆?br>**娉細nSize=0琛ㄧず涓嶆墽琛屾惉杩愶紝璇ユ帴鍙ｅ皢琚涓篘OP锛堢┖鎿嶄綔锛夈€?* |
| mSize | 杈撳叆 | 婧怤Z鐭╅樀鍦∕鏂瑰悜涓婄殑澶у皬锛屽彇鍊艰寖鍥翠负mSize鈭圼0, 65535]銆?br>**娉細mSize=0琛ㄧず涓嶆墽琛屾惉杩愶紝璇ユ帴鍙ｅ皢琚涓篘OP锛堢┖鎿嶄綔锛夈€?* |
| srcStride | 杈撳叆 | 婧怤Z鐭╅樀涓浉閭籞鎺掑竷鐨勮捣濮嬪湴鍧€鍋忕Щ锛屽彇鍊艰寖鍥翠负srcStride鈭圼0, 65535]锛屽崟浣嶄负C0_Size锛?6*sizeof(T)锛孴涓簊rc鐨勬暟鎹被鍨嬶級銆?|
| dstStride | 杈撳叆 | &bull; 涓嶅紑鍚疦Z2ND鍔熻兘锛氱洰鐨凬Z鐭╅樀涓浉閭籞鎺掑竷鐨勮捣濮嬪湴鍧€鍋忕Щ锛屽彇鍊间笉涓?锛屽崟浣嶄负element銆?br>&bull; 寮€鍚疦Z2ND/NZ2DN鍔熻兘锛氱洰鐨凬D鐭╅樀姣忎竴琛屼腑鐨勫厓绱犱釜鏁帮紝鍙栧€间笉涓?锛屽崟浣嶄负element銆?|
| quantPre | 杈撳叆 | QuantMode_t鏄竴涓灇涓剧被鍨嬶紝鐢ㄤ簬鎺у埗閲忓寲妯″紡锛岄粯璁ゅ€间负QuantMode_t::NoQuant锛屽嵆涓嶅紑鍚噺鍖栧姛鑳姐€俀uantMode_t鍙栧€煎涓嬶細<br>&nbsp;&nbsp;&bull; NoQuant锛屼笉寮€鍚噺鍖栧姛鑳姐€?br>&nbsp;&nbsp;&bull; F322F16锛宖loat cast鎴恏alf锛宑ast mode涓篊AST_RINT妯″紡銆?br>&nbsp;&nbsp;&bull; F322BF16锛宖loat cast鎴恇float16_t锛宑ast mode涓篊AST_RINT妯″紡銆?br>&nbsp;&nbsp;&bull; DEQF16锛宨nt32_t閲忓寲鎴恏alf锛宻calar閲忓寲銆?br>&nbsp;&nbsp;&bull; VDEQF16锛宨nt32_t閲忓寲鎴恏alf锛宼ensor閲忓寲銆?br>&nbsp;&nbsp;&bull; QF322B8_PRE锛宖loat閲忓寲鎴恥int8_t/int8_t锛宻calar閲忓寲銆?br>&nbsp;&nbsp;&bull; VQF322B8_PRE锛宖loat閲忓寲鎴恥int8_t/int8_t锛宼ensor閲忓寲銆?br>&nbsp;&nbsp;&bull; REQ8锛宨nt32_t閲忓寲鎴恥int8_t/int8_t锛宻calar閲忓寲銆?br>&nbsp;&nbsp;&bull; VREQ8锛宨nt32_t閲忓寲鎴恥int8_t/int8_t锛宼ensor閲忓寲銆?br>&nbsp;&nbsp;&bull; QF322FP8_PRE锛宖loat閲忓寲鎴恌p8_e4m3fn_t锛宻calar閲忓寲銆?br>&nbsp;&nbsp;&bull; VQF322FP8_PRE锛宖loat閲忓寲鎴恌p8_e4m3fn_t锛宼ensor閲忓寲銆?br>&nbsp;&nbsp;&bull; QF322HIF8_PRE锛宖loat閲忓寲鎴恏ifloat8_t(Half to Away Round)锛宻calar閲忓寲銆?br>&nbsp;&nbsp;&bull; VQF322HIF8_PRE锛宖loat閲忓寲鎴恏ifloat8_t(Half to Away Round)锛宼ensor閲忓寲銆?br>&nbsp;&nbsp;&bull; QF322HIF8_PRE_HYBRID锛宖loat閲忓寲鎴恏ifloat8_t(Hybrid Round)锛宻calar閲忓寲銆?br>&nbsp;&nbsp;&bull; VQF322HIF8_PRE_HYBRID锛宖loat閲忓寲鎴恏ifloat8_t(Hybrid Round)锛宼ensor閲忓寲銆?br>&nbsp;&nbsp;&bull; QS322BF16_PRE锛宨nt32_t閲忓寲鎴恇float16_t锛宻calar閲忓寲銆?br>&nbsp;&nbsp;&bull; VQS322BF16_PRE锛宨nt32_t閲忓寲鎴恇float16_t锛宼ensor閲忓寲銆?br>&nbsp;&nbsp;&bull; QF322F16_PRE锛宖loat閲忓寲鎴恏alf锛宻calar閲忓寲銆?br>&nbsp;&nbsp;&bull; VQF322F16_PRE锛宖loat閲忓寲鎴恏alf锛宼ensor閲忓寲銆?br>&nbsp;&nbsp;&bull; QF322BF16_PRE锛宖loat閲忓寲鎴恇float16_t锛宻calar閲忓寲銆?br>&nbsp;&nbsp;&bull; VQF322BF16_PRE锛宖loat閲忓寲鎴恇float16_t锛宼ensor閲忓寲銆?br>&nbsp;&nbsp;&bull; QF322F32_PRE锛宖loat閲忓寲鎴恌loat锛宻calar閲忓寲锛岃閲忓寲妯″紡绮惧害鏃犳硶杈惧埌鍙屼竾鍒嗕箣涓€锛屽彲浠ヨ揪鍒板弻鍗冨垎涔嬩竴銆傚鏋滄湁鍙屼竾鍒嗕箣涓€鐨勭簿搴﹁姹傦紝寤鸿浣跨敤[AscendDeQuant](../../../楂橀樁API/閲忓寲鎿嶄綔/AscendDequant.md)楂橀樁API銆?br>&nbsp;&nbsp;&bull; VQF322F32_PRE锛宖loat閲忓寲鎴恌loat锛宼ensor閲忓寲锛岃閲忓寲妯″紡绮惧害鏃犳硶杈惧埌鍙屼竾鍒嗕箣涓€锛屽彲浠ヨ揪鍒板弻鍗冨垎涔嬩竴銆傚鏋滄湁鍙屼竾鍒嗕箣涓€鐨勭簿搴﹁姹傦紝寤鸿浣跨敤[AscendDeQuant](../../../楂橀樁API/閲忓寲鎿嶄綔/AscendDequant.md)楂橀樁API銆?|
| deqScalar | 杈撳叆 | scalar閲忓寲鍙傛暟锛岃〃绀哄崟涓猻cale鍊硷紝quantPre閲忓寲妯″紡涓簊calar閲忓寲鏃堕渶瑕佽缃鍙傛暟銆傛敮鎸佺殑鏁版嵁绫诲瀷涓簎int64_t銆?|
| reluEn | 杈撳叆 | 鏄惁寮€鍚疪eLU鐨勫紑鍏筹細<br>&nbsp;&nbsp;&bull; false锛氫笉寮€鍚疪eLU鍔熻兘锛?br>&nbsp;&nbsp;&bull; true锛氬紑鍚疪eLU鍔熻兘銆?|
| unitFlag | 杈撳叆 | unitFlag鏄竴绉峂mad鎸囦护鍜孎ixpipe鎸囦护缁嗙矑搴︾殑骞惰锛屽紑鍚鍔熻兘鍚庯紝纭欢姣忚绠楀畬涓€涓垎褰紝璁＄畻缁撴灉灏变細琚惉鍑恒€傚彇鍊艰鏄庡涓嬶細<br>&nbsp;&nbsp;&bull; 0锛?'b00锛夛細涓嶅紑鍚痷nitFlag锛?br>&nbsp;&nbsp;&bull; 1锛?'b01锛夛細淇濈暀鍊硷紱<br>&nbsp;&nbsp;&bull; 2锛?'b10锛夛細寮€鍚痷nitFlag锛岀‖浠舵墽琛屽畬鎸囦护涔嬪悗锛屼笉浼氳缃瘎瀛樺櫒锛?br>&nbsp;&nbsp;&bull; 3锛?'b11锛夛細寮€鍚痷nitFlag锛岀‖浠舵墽琛屽畬鎸囦护涔嬪悗锛屼細灏唘nitFlag鍏抽棴銆?|
| params | 杈撳叆 | params鏄被鍨嬩负TransformParams鐨勬垚鍛樺彉閲忥紝TransformParams缁撴瀯浣撴槸涓€涓熀浜庢ā鏉垮弬鏁扮殑绫诲瀷閫夋嫨鍣紝鐢ㄤ簬鍦ㄧ紪璇戞椂鏍规嵁涓嶅悓鐨凜O2Layout甯冨眬绫诲瀷锛岃嚜鍔ㄩ€夋嫨瀵瑰簲鐨勫弬鏁扮被鍨嬨€?br><pre>template \<CO2Layout format\><br>struct TransformParams {};<br>template \<\><br>struct TransformParams\<CO2Layout::NZ\> {<br>    \_\_aicore\_\_ inline TransformParams(){};<br>    using PARAMS = uint8_t;<br>};<br>template \<\><br>struct TransformParams\<CO2Layout::ROW_MAJOR\> {<br>    \_\_aicore\_\_ inline TransformParams(){};<br>    using PARAMS = Nz2NdParams;<br>};<br>template \<\><br>struct TransformParams\<CO2Layout::COLUMN_MAJOR\> {<br>    \_\_aicore\_\_ inline TransformParams(){};<br>    using PARAMS = Nz2DnParams;<br>};</pre><br>CO2Layout甯冨眬绫诲瀷涓篟OW_MAJOR鏃讹紝璇ユ寚浠よ瀹氫箟涓轰粠L0C Buffer鍒扮洰鏍囦綅缃殑鏁版嵁绉诲姩锛屽苟闄勫甫NZ2ND杞崲锛岀粨鏋勪綋鍙傛暟濡備笅锛?br><pre>struct Nz2NdParams {<br>    uint16_t ndNum = 1;<br>    uint16_t srcNdStride = 0;<br>    uint32_t dstNdStride = 0; <br>};</pre>&nbsp;&nbsp;&bull; ndNum锛氭簮NZ鐭╅樀鐨勬暟鐩紝涔熷氨鏄紶杈揘D鐭╅樀鐨勬暟鐩紝鍙栧€艰寖鍥翠负ndNum鈭圼0, 65535]銆?*娉細ndNum=0琛ㄧず涓嶆墽琛屾惉杩愶紝璇ユ帴鍙ｅ皢琚涓篘OP锛堢┖鎿嶄綔锛夈€?*<br>&nbsp;&nbsp;&bull; srcNdStride锛氫笉鍚孨Z鐭╅樀璧峰鍦板潃涔嬮棿鐨勯棿闅旓紝鍙栧€艰寖鍥翠负srcNdStride鈭圼0, 65535]锛屽崟浣嶄负C0_SIZE銆傚綋ndNum閰嶇疆涓?鏃讹紝srcNdStride閰嶇疆涓?鍗冲彲锛屼笉鐢熸晥銆?br>&nbsp;&nbsp;&bull; dstNdStride锛氱洰鐨勭浉閭籒D鐭╅樀璧峰鍦板潃涔嬮棿鐨勫亸绉伙紝鍙栧€艰寖鍥翠负dstNdStride鈭圼1, 2^32 -1]锛屽崟浣嶄负element銆傚綋ndNum閰嶇疆涓?鏃讹紝dstNdStride閰嶇疆涓?鍗冲彲锛屼笉鐢熸晥銆?br><br>CO2Layout甯冨眬绫诲瀷涓篊OLUMN_MAJOR鏃讹紝璇ユ寚浠よ瀹氫箟涓轰粠L0C Buffer鍒扮洰鏍囦綅缃殑鏁版嵁绉诲姩锛屽苟闄勫甫NZ2DN杞崲锛岀粨鏋勪綋鍙傛暟濡備笅锛?br><pre>struct Nz2DnParams {<br>    uint16_t dnNum = 1;<br>    uint16_t srcNzMatrixStride = 0;<br>    uint32_t dstDnMatrixStride = 0;<br>    uint16_t srcNzC0Stride = 0; <br>};</pre>&nbsp;&nbsp;&bull; dnNum锛氫紶杈揇N鐭╅樀鐨勬暟鐩紝鍙栧€艰寖鍥翠负dnNum鈭圼0, 65535]銆?*娉細dnNum=0琛ㄧず涓嶆墽琛屾惉杩愶紝璇ユ帴鍙ｅ皢琚涓篘OP锛堢┖鎿嶄綔锛夈€?*<br>&nbsp;&nbsp;&bull; srcNzMatrixStride锛氫笉鍚屾簮NZ鐭╅樀鐨勫亸绉伙紙澶翠笌澶达級锛屽崟浣嶄负C0_SIZE銆?br>&nbsp;&nbsp;&bull; dstDnMatrixStride锛氱洰鐨勭浉閭籇N鐭╅樀璧峰鍦板潃闂寸殑鍋忕Щ锛屽彇鍊艰寖鍥翠负dstDnMatrixStride鈭圼1, 2^32 -1]锛屽崟浣嶄负element銆?br>&nbsp;&nbsp;&bull; srcNzC0Stride锛氭簮鐭╅樀NZ鍒嗗舰涓浉閭昏鐨勫湴鍧€鍋忕Щ锛堝ご涓庡ご锛夛紝鍗曚綅涓篊0_SIZE銆?br><br>CO2Layout甯冨眬绫诲瀷涓篘Z鏃讹紝涓烘櫘閫氭惉杩怐MA妯″紡锛岃〃绀轰粠L0C Buffer鍒扮洰鏍囦綅缃殑姝ｅ父鏁版嵁绉诲姩銆?|
| dualDstCtrl | 杈撳叆 | 鍙岀洰鏍囨ā寮忔帶鍒躲€傚綋鍚敤鍙岀洰鏍囨ā寮忔帶鍒舵椂锛孡0C Buffer涓殑M脳N鐭╅樀灏嗚鍒嗘垚涓ゅ崐锛屽苟鍚屾椂鍐欏叆涓や釜瀛愬潡锛圫UB BLOCK锛夌殑UB涓紝鍏朵腑鍓嶅崐閮ㄥ垎鍐欏叆SUB BLOCK0锛屽悗鍗婇儴鍒嗗啓鍏UB BLOCK1銆?br>&nbsp;&nbsp;&bull; 2'b00锛氬崟鐩爣妯″紡锛屽皢鏁翠釜鐭╅樀鍐欏叆閫氳繃subBlockId鍙傛暟閰嶇疆鐨勭洰鏍嘦B銆?br>&nbsp;&nbsp;&bull; 2'b01锛氬弻鐩爣妯″紡锛屾寜M缁村害鎷嗗垎锛孧 / 2 \* N鍐欏叆姣忎釜UB锛孧蹇呴』涓?鐨勫€嶆暟銆?br>&nbsp;&nbsp;&bull; 2'b10锛氬弻鐩爣妯″紡锛屾寜N缁村害鎷嗗垎锛孧 \* N / 2鍐欏叆姣忎釜UB锛孨椤讳负32鐨勫€嶆暟銆?br>&nbsp;&nbsp;&bull; 2'b11锛氫繚鐣欍€?br>dualDstCtrl浠呮敮鎸佸湪鏅€氭惉杩愭ā寮忔垨NZ2ND鎼繍鍦烘櫙涓嬩娇鐢紝涓嶆敮鎸侀殢璺姛鑳藉満鏅€?|
| subBlockId | 杈撳叆 | 鍦ㄥ惎鐢ㄥ崟鐩爣妯″紡鏃舵寚绀虹洰鏍嘦B鐨勭紪鍙枫€?|
| isChannelSplit | 杈撳叆 | 鏄惁寮€鍚€氶亾鎷嗗垎鐨勫姛鑳姐€傞粯璁や负false锛屼笉寮€鍚鍔熻兘銆備粎鍦╯rc鍜宒st閮戒负float鏃舵墠鑳藉紑鍚€氶亾鎷嗗垎锛屼笖涓嶈兘鍚屾椂寮€鍚疌hannelSplit鍜孨Z2ND鍔熻兘銆?|

**琛?4** Fixpipe鎼繍鍙傛暟锛團ixpipeParamsV220锛夌粨鏋勪綋璇存槑

| 鍙傛暟鍚嶇О | 鏁版嵁绫诲瀷 | 鍚箟 |
| --- | --- | --- |
| nSize | 蹇呴€夎緭鍏?| 婧怤Z鐭╅樀鍦∟鏂瑰悜涓婄殑澶у皬銆傚彇鍊艰寖鍥磏Size鈭圼0, 4095]锛宯Size蹇呴』涓?6鐨勫€嶆暟銆?br>**娉細nSize=0琛ㄧず涓嶆墽琛屾惉杩愶紝璇ユ帴鍙ｅ皢琚涓篘OP锛堢┖鎿嶄綔锛夈€?* |
| mSize | 蹇呴€夎緭鍏?| 婧怤Z鐭╅樀鍦∕鏂瑰悜涓婄殑澶у皬銆傚彇鍊艰寖鍥翠负mSize鈭圼0, 65535]銆?br>**娉細mSize=0琛ㄧず涓嶆墽琛屾惉杩愶紝璇ユ帴鍙ｅ皢琚涓篘OP锛堢┖鎿嶄綔锛夈€?* |
| srcStride | 蹇呴€夎緭鍏?| 婧怤Z鐭╅樀涓浉閭籞鎺掑竷鐨勮捣濮嬪湴鍧€鍋忕Щ锛屽彇鍊艰寖鍥翠负srcStride鈭圼0, 65535]锛屽崟浣嶄负C0_Size锛?6*sizeof(T)锛夛紝T涓簊rc鐨勬暟鎹被鍨嬶紝鍏跺€煎簲濉垚mSize瀵?6鍚戜笂鍙栨暣銆?|
| dstStride | 蹇呴€夎緭鍏?| 鐩殑NZ鐭╅樀涓浉閭籞鎺掑竷鐨勮捣濮嬪湴鍧€鍋忕Щ锛屽彇鍊间笉涓?锛屽崟浣嶄负datablock锛?2瀛楄妭锛夈€?|
| [quantPre](./鍏抽敭鐗规€ц鏄?闅忚矾閲忓寲.md) | 鍙€夎緭鍏?| 鐢ㄤ簬鎺у埗閲忓寲妯″紡锛孮uantMode_t绫诲瀷锛岄粯璁ゅ€间负`QuantMode_t::NoQuant`锛屽叿浣撳畾涔夊涓嬶細<br>&nbsp;&nbsp;&bull; half/bfloat16_t杈撳嚭锛屾鍙傛暟闇€閰嶇疆涓篳QuantMode_t::F322F16`/`QuantMode_t::F322BF16`銆?br>娉細姝ゅ弬鏁伴渶瑕佺敤鎴锋墜鍔ㄩ厤缃紝涓嶄細鑷姩鎺ㄥ閰嶇疆瀵瑰簲閲忓寲妯″紡銆傛閫氳矾涓嶆敮鎸丯oQuant妯″紡銆?br><pre>enum QuantMode_t<br>{<br>    NoQuant,      // 涓嶅紑鍚噺鍖栧姛鑳斤紝姝ら€氳矾涓嶆敮鎸?br>    F322F16,      // Float32_2_Float16锛歠loat cast鎴恏alf锛宑ast mode涓篊AST_RINT妯″紡<br>    F322BF16,     // Float32_2_BFloat16锛歠loat cast鎴恇float16_t锛宑ast mode涓篊AST_RINT妯″紡<br>    DEQF16,       // DeQuant_Float16锛歩nt32_t閲忓寲鎴恏alf锛宻calar閲忓寲<br>    VDEQF16,      // Vector_DeQuant_Float16锛歩nt32_t閲忓寲鎴恏alf锛宼ensor閲忓寲<br>    QF322B8_PRE,  // Quant_Float32_2_B8锛歠loat閲忓寲鎴恑nt8_t/uint8_t锛宻calar閲忓寲<br>    VQF322B8_PRE, // Vector_Quant_Float32_2_B8锛歠loat閲忓寲鎴恑nt8_t/uint8_t锛宼ensor閲忓寲<br>    REQ8,         // ReQuant_int8锛歩nt32_t閲忓寲鎴恑nt8_t/uint8_t锛宻calar閲忓寲<br>    VREQ8,        // Vector_ReQuant_int8锛歩nt32_t閲忓寲鎴恑nt8_t/uint8_t锛宼ensor閲忓寲<br>};</pre> |
| deqScalar | 鍙€夎緭鍏?| scalar閲忓寲鍙傛暟锛岃〃绀哄崟涓猻cale鍊硷紝quantPre閲忓寲妯″紡涓篬闅忚矾閲忓寲](./鍏抽敭鐗规€ц鏄?闅忚矾閲忓寲.md)鏃堕渶瑕佽缃鍙傛暟銆傛敮鎸佺殑鏁版嵁绫诲瀷涓篳uint64_t`銆?|
| [reluEn](./鍏抽敭鐗规€ц鏄?闅忚矾relu.md) | 鍙€夎緭鍏?| 鏄惁寮€鍚疦ormReLU鐨勫紑鍏筹細<br>&nbsp;&nbsp;&bull; `false`锛氫笉寮€鍚疦ormReLU鍔熻兘锛岄粯璁や负`false`锛?br>&nbsp;&nbsp;&bull; `true`锛氬紑鍚疦ormReLU鍔熻兘銆?|
| unitFlag | 鍙€夎緭鍏?| unitFlag鏄竴绉峂mad鎸囦护鍜孎ixpipe鎸囦护缁嗙矑搴︾殑骞惰锛屽紑鍚鍔熻兘鍚庯紝纭欢姣忚绠楀畬涓€涓垎褰紝璁＄畻缁撴灉灏变細琚惉鍑恒€傚彇鍊艰鏄庡涓嬶細<br>&nbsp;&nbsp;&bull; 0锛?'b00锛夛細涓嶅紑鍚痷nitFlag锛?br>&nbsp;&nbsp;&bull; 1锛?'b01锛夛細淇濈暀鍊硷紱<br>&nbsp;&nbsp;&bull; 2锛?'b10锛夛細寮€鍚痷nitFlag锛岀‖浠舵墽琛屽畬鎸囦护涔嬪悗锛屼笉澶嶄綅鍗曞厓鏍囪浣嶏紱<br>&nbsp;&nbsp;&bull; 3锛?'b11锛夛細寮€鍚痷nitFlag锛岀‖浠舵墽琛屽畬鎸囦护涔嬪悗锛屽浣嶅崟鍏冩爣璁颁綅銆?br>寮€鍚鍔熻兘鏃讹紝椤诲皢Mmad鎸囦护鍜孎ixpipe鎸囦护鐨剈nitFlag鍊艰缃负2鎴?銆?br>鍙傛暟璁剧疆鏂规鍜岀壒鎬х粏鑺傚彲鍙傝€冿細Mmad璁＄畻涓叧閿壒鎬ц鏄庣殑[UnitFlag](../Mmad璁＄畻/鍏抽敭鐗规€ц鏄?UnitFlag.md)绔犺妭銆?|
| [channelSplit](./鍏抽敭鐗规€ц鏄?F32-Channel-Split.md) | 鍙€夎緭鍏?| 浠呭湪L0C Buffer锛圕O1锛?>GM閫氳矾鏃剁敓鏁堬紝姝ら€氳矾璁剧疆涓篺alse鍗冲彲銆?|
| ndNum | 鍙€夎緭鍏?| 浠呭湪L0C Buffer锛圕O1锛?>GM閫氳矾鏃剁敓鏁堬紝姝ら€氳矾鏃犻渶棰濆璁剧疆銆?|
| srcNdStride | 鍙€夎緭鍏?| 浠呭湪L0C Buffer锛圕O1锛?>GM閫氳矾鏃剁敓鏁堬紝姝ら€氳矾鏃犻渶棰濆璁剧疆銆?|
| dstNdStride | 鍙€夎緭鍏?| 浠呭湪L0C Buffer锛圕O1锛?>GM閫氳矾鏃剁敓鏁堬紝姝ら€氳矾鏃犻渶棰濆璁剧疆銆?|

## 鏁版嵁绫诲瀷<a id="zh-cn_topic_0000002511188540_section4219135304818"></a>

**婧愮煩闃典笌鐩殑鐭╅樀鏀寔鐨勬暟鎹被鍨嬬粍鍚?*

<cann-filter npu-type = "950">

閽堝Ascend 950PR/Ascend 950DT锛屾敮鎸佺殑鏁版嵁绫诲瀷缁勫悎濡備笅锛?

| 婧愮煩闃碉紙L0C Buffer锛?| 鐩殑鐭╅樀锛圠1 Buffer锛?|
| ---------- | ---------- |
| float | int8_t銆乽int8_t銆乭ifloat8_t銆乫p8_e4m3fn_t銆乭alf銆乥float16_t銆乫loat銆?|
| int32_t | int8_t銆乽int8_t銆乭alf銆乥float16_t銆乮nt32_t銆?|

</cann-filter>

<cann-filter npu-type = "A3">

閽堝Atlas A3 璁粌绯诲垪浜у搧/Atlas A3 鎺ㄧ悊绯诲垪浜у搧锛屾敮鎸佺殑鏁版嵁绫诲瀷缁勫悎濡備笅锛?

| 婧愮煩闃碉紙L0C Buffer锛?| 鐩殑鐭╅樀锛圠1 Buffer锛?|
| ---------- | ---------- |
| float | int8_t銆乽int8_t銆乭alf銆乥float16_t銆?|
| int32_t | int8_t銆乽int8_t銆乭alf銆?|

</cann-filter>

<cann-filter npu-type = "910b">

閽堝Atlas A2 璁粌绯诲垪浜у搧/Atlas A2 鎺ㄧ悊绯诲垪浜у搧锛屾敮鎸佺殑鏁版嵁绫诲瀷缁勫悎濡備笅锛?

| 婧愮煩闃碉紙L0C Buffer锛?| 鐩殑鐭╅樀锛圠1 Buffer锛?|
| ---------- | ---------- |
| float | int8_t銆乽int8_t銆乭alf銆乥float16_t銆?|
| int32_t | int8_t銆乽int8_t銆乭alf銆?|

</cann-filter>

<cann-filter npu-type = "x90">

閽堝Kirin X90锛屾敮鎸佺殑鏁版嵁绫诲瀷缁勫悎濡備笅锛?

| 婧愮煩闃碉紙L0C Buffer锛?| 鐩殑鐭╅樀锛圠1 Buffer锛?|
| ---------- | ---------- |
| float | int8_t銆乭alf銆?|
| int32_t | int8_t銆乭alf銆?|

</cann-filter>

<cann-filter npu-type = "9030">

閽堝Kirin 9030锛屾敮鎸佺殑鏁版嵁绫诲瀷缁勫悎濡備笅锛?

| 婧愮煩闃碉紙L0C Buffer锛?| 鐩殑鐭╅樀锛圠1 Buffer锛?|
| ---------- | ---------- |
| float | half |
| int32_t | half |

</cann-filter>

## 杩斿洖鍊艰鏄?

鏃?

## 绾︽潫璇存槑<a id="zh-cn_topic_0000002511188540_section2045914466492"></a>

- 瀵逛簬閲忓寲杈撳叆涓篺loat32鏁版嵁绫诲瀷鐨勮鏄庡涓嬶細
    - 鏍囧噯鐨処EEE 754 float32鏍煎紡涓猴細1bit绗﹀彿浣嶏紝8bits鎸囨暟浣嶏紝23bits灏炬暟浣嶏紱褰撳墠AI澶勭悊鍣ㄦ敮鎸佺殑float32鏍煎紡涓猴細1bit绗﹀彿浣嶏紝8bits鎸囨暟浣嶏紝10bits灏炬暟浣嶃€?
    - 濡傛灉鐢ㄦ埛鎻愪緵鐨勬槸鏍囧噯鐨処EEE 754 float32杈撳叆锛孉PI鍐呴儴浼氬鐞嗘垚澶勭悊鍣ㄦ敮鎸佺殑float32鏍煎紡杩涜璁＄畻锛屾鏃跺鏋済olden鏁版嵁鐢熸垚杩囩▼涓娇鐢ㄧ殑鏄爣鍑嗙殑IEEE 754 float32鏁版嵁锛屽垯鍙兘寮曞叆绮惧害涓嶅尮閰嶉棶棰橈紝闇€瑕佷慨姝olden鏁版嵁鐨勭敓鎴愶紝灏嗛噺鍖栧弬鏁扮殑23bits灏炬暟浣嶇殑浣?3bits鏁版嵁浣嶆竻闆跺啀鍙備笌閲忓寲璁＄畻銆?
- 婧愮煩闃礜Z鏍煎紡鍦板潃瑕佹眰6瀵归綈锛岀洰鐨勭煩闃礜Z鏍煎紡鍦板潃闇€瑕佹弧瓒?2瀛楄妭瀵归綈銆?
- 褰撴惉鍑虹殑mSize銆乶Size鎴栬€卬dNum涓殑浠绘剰涓€涓€间负0鏃讹紝璇ユ寚浠や笉浼氳鎵ц銆俷dNum=0鏃朵細鎶arning銆?
- 閲忓寲鍜孯eLU鍙傛暟涓嶈兘涓篒NF/NAN鍜岄潪瑙勬牸鍖栨暟銆?
- 鐩爣鏁版嵁涓嶈兘鏈夐噸鍙犮€傚鏋滃鐩殑鍦板潃鏈夐噸鍙犲啓鍏ワ紝纭欢涓嶄細鎶ュ憡浠讳綍璀﹀憡鍜岄敊璇紝涔熶笉淇濊瘉閲嶅彔鏁版嵁鐨勫啓鍏ラ『搴忋€?
- unitFlag鐗规€у紑鍚渶瑕侀厤鍚圡mad鍚屾椂寮€鍚€?

<cann-filter npu-type = "A3,910b">

- 閽堝濡備笅浜у搧鍨嬪彿锛岀壒娈婂€?杈圭晫鍊肩害鏉熻鏄庡涓嬶細

    <cann-filter npu-type = "A3">

    Atlas A3 璁粌绯诲垪浜у搧/Atlas A3 鎺ㄧ悊绯诲垪浜у搧锛?

    </cann-filter>
    
    <cann-filter npu-type = "910b">

    Atlas A2 璁粌绯诲垪浜у搧/Atlas A2 鎺ㄧ悊绯诲垪浜у搧锛?
    
    </cann-filter>

    瀵逛簬娴偣绫诲瀷INF/NAN杈撳叆杈撳嚭锛屽彲浠ラ€氳繃CTRL瀵勫瓨鍣紙鎺у埗瀵勫瓨鍣級鐨凜TRL\[48\]姣旂壒浣嶈繘琛岃缃紝鎺у埗娴偣鏁伴噺鍖栨惉鍑烘椂鐨勯ケ鍜屾ā寮忥細

    - 闈為ケ鍜屾ā寮忥細CTRL\[48\]璁剧疆鎴?'b1锛孖NF/NAN淇濇寔鍘熻緭鍑恒€?
    - 楗卞拰妯″紡锛欳TRL\[48\]璁剧疆鎴?'b0锛孖NF杈撳嚭浼氳楗卞拰涓郝盡AX锛孨aN杈撳嚭浼氳楗卞拰涓?銆?

        ```cpp
        // 璁剧疆CTRL[48]涓?锛屽紑鍚诞鐐规暟楗卞拰妯″紡
        AscendC::AscendCUtils::SetOverflow(0);
        ```

    瀵逛簬鏁存暟绫诲瀷鍙湁楗卞拰妯″紡銆?

</cann-filter>

## 璋冪敤绀轰緥<a id="zh-cn_topic_0000002511188540_section088124295117"></a>

<cann-filter npu-type = "950,A3,910b">

Fixpipe瀹屾暣鏍蜂緥璇峰弬鑰僛fixpipe\_l0c2l1鏍蜂緥](https://gitcode.com/cann/asc-devkit/tree/master/examples/01_simd_cpp_api/03_basic_api/01_matrix_compute/fixpipe_l0c2l1)锛?

- 绀轰緥锛氶€氳矾L0C Buffer->L1 Buffer銆傝緭鍏鐭╅樀鍜孊鐭╅樀鐨勬暟鎹被鍨嬩负half锛岃緭鍑篊鐭╅樀涓篿nt8_t锛屼笉寮€鍚疦Z2ND鐨勬牸寮忚浆鎹紝寮€鍚痵calar閲忓寲銆?

    ```cpp
        AscendC::LocalTensor<outputType> c1Local(AscendC::TPosition::C1, c1Addr, cSizeAlignL1);
    
    #if defined(__NPU_ARCH__) && __NPU_ARCH__ == 2201
        uint16_t c0Size = 32;
        AscendC::FixpipeParamsV220 fixpipeParams;
        // NZ2NZ鍦烘櫙锛孎ixpipeParamsV220鐨刣stStride鍗曚綅涓篸atablock锛?2瀛楄妭锛?
        fixpipeParams.dstStride = baseM * c0Size * sizeof(outputType) / AscendC::ONE_BLK_SIZE;
        fixpipeParams.mSize = baseM;
        fixpipeParams.srcStride = CeilAlign(baseM, CUBE_BLOCK);
        fixpipeParams.nSize = baseN;
        // scalar quant
        fixpipeParams.quantPre = QuantMode_t::QF322B8_PRE;
        uint64_t deqScalar = static_cast<uint64_t>(*reinterpret_cast<int32_t *>(&quantScalar));
        constexpr bool sign = (AscendC::IsSameType<outputType, int8_t>::value) ? true : false;
        deqScalar = (deqScalar & ~(static_cast<uint64_t>(1) << 46)) | (static_cast<uint64_t>(sign) << 46);
        fixpipeParams.deqScalar = deqScalar;
        AscendC::Fixpipe<outputType, l0cType, AscendC::CFG_NZ>(c1Local, c, fixpipeParams);
    
    #elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
        uint16_t c0Size = 32;
        AscendC::FixpipeParamsArch3510<AscendC::CO2Layout::NZ> fixpipeParams;
        // NZ2NZ鍦烘櫙锛孎ixpipeParamsArch3510鐨刣stStride鍗曚綅涓哄厓绱犱釜鏁?
        fixpipeParams.dstStride = baseM * c0Size;
        fixpipeParams.mSize = baseM;
        fixpipeParams.srcStride = CeilAlign(baseM, CUBE_BLOCK);
        fixpipeParams.nSize = baseN;
        // scalar quant
        fixpipeParams.quantPre = QuantMode_t::QF322B8_PRE;
        uint64_t deqScalar = static_cast<uint64_t>(*reinterpret_cast<int32_t *>(&quantScalar));
        constexpr bool sign = (AscendC::IsSameType<outputType, int8_t>::value) ? true : false;
        deqScalar = (deqScalar & ~(static_cast<uint64_t>(1) << 46)) | (static_cast<uint64_t>(sign) << 46);
        fixpipeParams.deqScalar = deqScalar;
        AscendC::Fixpipe<outputType, l0cType, AscendC::CFG_NZ>(c1Local, c, fixpipeParams);
    #endif
    ```

</cann-filter>
