# Matmul鍜孡eakyRelu铻嶅悎璁＄畻鏍蜂緥

## 绠楀瓙姒傝堪

鏈牱渚嬪熀浜庨潤鎬乀ensor缂栫▼妯″紡锛堢紪璇戞湡纭畾寮犻噺褰㈢姸鍜屽唴瀛樺竷灞€鐨勭紪绋嬫柟寮忥級瀹炵幇Matmul鍜孡eakyRelu铻嶅悎璁＄畻锛屽睍绀篊ube鍗曞厓锛堢煩闃佃绠楀崟鍏冿紝鎵цMatmul绛夌煩闃佃繍绠楋級鍜孷ector鍗曞厓锛堝悜閲忚绠楀崟鍏冿紝鎵ц閫愬厓绱犺繍绠楋級鍗忓悓璁＄畻鐨勭紪绋嬫ā寮忋€侰ube鏍稿畬鎴愮煩闃典箻璁＄畻锛孷ector鏍稿畬鎴怢eakyRelu婵€娲昏绠椼€?

## 鏈牱渚嬫敮鎸佺殑浜у搧鍙奀ANN杞欢鐗堟湰

| 浜у搧 | CANN杞欢鐗堟湰 |
|------|-------------|
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |
| Atlas A3 璁粌绯诲垪浜у搧/Atlas A3 鎺ㄧ悊绯诲垪浜у搧 | >= CANN 9.0.0 |
| Atlas A2 璁粌绯诲垪浜у搧/Atlas A2 鎺ㄧ悊绯诲垪浜у搧 | >= CANN 9.0.0 |

## 鐩綍缁撴瀯浠嬬粛

```
鈹溾攢鈹€ matmul_leakyrelu_basic_api
鈹?  鈹溾攢鈹€ scripts
鈹?  鈹?  鈹溾攢鈹€ gen_data.py                 // 杈撳叆鏁版嵁鍜岀湡鍊兼暟鎹敓鎴愯剼鏈枃浠?
鈹?  鈹?  鈹斺攢鈹€ verify_result.py            // 鐪熷€煎姣旀枃浠?
鈹?  鈹溾攢鈹€ CMakeLists.txt                  // 缂栬瘧宸ョ▼鏂囦欢
鈹?  鈹溾攢鈹€ data_utils.h                    // 鏁版嵁璇诲叆鍐欏嚭鍑芥暟
鈹?  鈹溾攢鈹€ matmul_leakyrelu_basic_api.asc  // Ascend C鏍蜂緥瀹炵幇 & 璋冪敤鏍蜂緥
鈹?  鈹斺攢鈹€ README.md                       // 鏍蜂緥璇存槑鏂囨。
```

## 绠楀瓙鎻忚堪

- 鏍蜂緥鍔熻兘锛?
  瀹炵幇Matmul鍜孡eakyRelu铻嶅悎璁＄畻锛岃绠楀叕寮忓涓嬶細

  Matmul璁＄畻锛?
  $$
  C_{ij} = \sum_{k} A_{ik} \times B_{kj}
  $$

  LeakyRelu璁＄畻锛?
  $$
  C_{ij} = \begin{cases}
  C_{ij} & \text{if } C_{ij} \geq 0 \\
  C_{ij} \times 0.001 & \text{if } C_{ij} < 0
  \end{cases}
  $$

  鍏朵腑锛孉涓哄乏鐭╅樀锛屽舰鐘朵负[M, K]锛汢涓哄彸鐭╅樀锛屽舰鐘朵负[K, N]锛汣涓鸿緭鍑虹煩闃碉紝褰㈢姸涓篬M, N]銆?

- 鏍蜂緥瑙勬牸锛?
  鏈牱渚嬪弬鏁癕 = 512, K = 128, N = 128锛岃皟鐢?涓狢ube鏍稿拰4涓猇ector鏍稿畬鎴愯绠楋紝杈撳叆瑙勬牸濡備笅琛ㄦ墍绀猴細

  <table>
  <tr><td rowspan="1" align="center">鏍蜂緥绫诲瀷(OpType)</td><td colspan="4" align="center">Matmul+LeakyRelu铻嶅悎</td></tr>
  <tr><td rowspan="3" align="center">鏍蜂緥杈撳叆</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">A锛堝乏鐭╅樀锛?/td><td align="center">[512, 128]</td><td align="center">half</td><td align="center">ND</td></tr>
  <tr><td align="center">B锛堝彸鐭╅樀锛?/td><td align="center">[128, 128]</td><td align="center">half</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">鏍蜂緥杈撳嚭</td><td align="center">C</td><td align="center">[512, 128]</td><td align="center">half</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">鏍稿嚱鏁板悕</td><td colspan="4" align="center">mmad_vec_custom</td></tr>
  </table>

  **鍒嗘牳閫昏緫**锛?

  杈撳嚭鐭╅樀C鎸塎鏂瑰悜锛坰hape绗竴缁达紝琛屾柟鍚戯級鍒嗕负2涓垎鍧楋紝N鏂瑰悜锛坰hape绗簩缁达紝鍒楁柟鍚戯級涓嶅垎鍧楋紝浣跨敤2涓狢ube鏍稿畬鎴怣atmul璁＄畻锛涙瘡涓狢ube鏍稿搴?涓猇ector鏍革紝鍥犳浣跨敤4涓猇ector鏍稿畬鎴怢eakyRelu璁＄畻銆傛瘡涓狢ube鏍镐骇鐢熶竴涓猙aseM脳baseN缁撴灉鍧楋紝瀵瑰簲2涓猇ector鏍稿悇澶勭悊baseM/2脳baseN鐨勬暟鎹€?

  **M/N/K鍙傛暟鍥捐В**锛?

  鍦ㄧ煩闃典箻 C = A 脳 B 涓紝M/N/K鍒嗗埆瀵瑰簲鐭╅樀鐨勪笉鍚岀淮搴︼紝鍒嗘牳鍋忕Щ涔熷洿缁曡繖浜涚淮搴﹀睍寮€锛?

  ```
           K                N
       鈹屸攢鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹?     鈹屸攢鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹?
       鈹?        鈹?     鈹?         鈹?
     M 鈹?   A    鈹? 脳 K 鈹?   B     鈹? =  C (M 脳 N)
       鈹?        鈹?     鈹?         鈹?
       鈹斺攢鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹?     鈹斺攢鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹?
  ```

  - M鏂瑰悜鍒嗗潡鏁帮細M / singleCoreM = 512 / 256 = 2
  - N鏂瑰悜鍒嗗潡鏁帮細N / singleCoreN = 128 / 128 = 1
  - 鎬籆ube鏍告暟锛? 脳 1 = 2
  - 鎬籚ector鏍告暟锛? 脳 2 = 4

- 绠楀瓙瀹炵幇锛?
  - **鏁翠綋璁＄畻娴佺▼**锛?

    鏍蜂緥鏁翠綋娴佺▼濡備笅锛堝睍绀篊ube鏍稿拰Vector鏍稿崗鍚岃绠楋級锛?

      > **鍓嶇疆璇存槑锛欰scend AI Core鍐呭瓨灞傜骇涓庢暟鎹€氳矾**
      >
      > Ascend AI Core鍐呴儴鏈夊绾х墖涓婄紦瀛橈紝鏁版嵁浠庡閮―DR锛圙lobal Memory, GM锛夐€愮骇鎼繍鑷宠绠楀崟鍏冿細
      > - **GM锛圙lobal Memory锛?*锛氳姱鐗囧閮―DR锛屽瓨鏀捐緭鍏?杈撳嚭鏁版嵁锛屽閲忔渶澶т絾璁块棶寤惰繜鏈€楂?
      > - **L1**锛氱墖涓婄紦瀛橈紙Cube渚т富瑕佸瓨鏀続/B鐭╅樀鏁版嵁锛夛紝鐢盡TE2寮曟搸浠嶨M鎼叆
      > - **L0A/L0B**锛欳ube鐭╅樀璁＄畻鍗曞厓鐨勮緭鍏ョ紦瀛橈紝鐢盡TE1寮曟搸浠嶭1鎼叆
      > - **L0C**锛欳ube鐭╅樀璁＄畻鍗曞厓鐨勮緭鍑虹紦瀛橈紝瀛樻斁Mmad璁＄畻缁撴灉
      > - **UB锛圲nified Buffer锛孉scendC涓搴擳Position::VECCALC锛?*锛歏ector渚х墖涓婄紦瀛橈紝瀛樻斁Vector璁＄畻鎵€闇€鐨勮緭鍏?杈撳嚭鏁版嵁
      >
      > 鏁版嵁闇€鍦ㄦ惉杩愯繃绋嬩腑杩涜鏍煎紡杞崲锛圢D鈫扤z/Zn锛夛紝鍥犱负Cube璁＄畻鍗曞厓瑕佹眰鐗瑰畾鐨勫垎褰紙Fractal锛夋帓甯冩牸寮忥紝鑰岄潪ND鎺掑竷銆?
      >
      > 鍚屾鏈哄埗璇存槑锛?
      > - **鏍稿唴鍚屾**锛歔SetFlag](https://gitcode.com/cann/asc-devkit/blob/master/docs/api/SIMD-API/鍩虹API/鍚屾鎺у埗/鏍稿唴鍚屾/SetFlag-WaitFlag(ISASI).md)/[WaitFlag](https://gitcode.com/cann/asc-devkit/blob/master/docs/api/SIMD-API/鍩虹API/鍚屾鎺у埗/鏍稿唴鍚屾/SetFlag-WaitFlag(ISASI).md)鐢ㄤ簬鍚屼竴鏍稿唴涓嶅悓娴佹按绾垮紩鎿庝箣闂寸殑渚濊禆鍚屾锛屽MTE2鎼繍瀹屾垚鍚庨€氱煡MTE1鍙互寮€濮嬫惉杩?
      > - **鏍搁棿鍚屾**锛歔CrossCoreSetFlag](https://gitcode.com/cann/asc-devkit/blob/master/docs/api/SIMD-API/鍩虹API/鍚屾鎺у埗/鏍搁棿鍚屾/CrossCoreSetFlag(ISASI).md)/[CrossCoreWaitFlag](https://gitcode.com/cann/asc-devkit/blob/master/docs/api/SIMD-API/鍩虹API/鍚屾鎺у埗/鏍搁棿鍚屾/CrossCoreSetFlag(ISASI).md)鐢ㄤ簬涓嶅悓鏍镐箣闂寸殑渚濊禆鍚屾锛屽Cube鏍稿畬鎴愯绠楀悗閫氱煡Vector鏍稿彲浠ュ紑濮嬭鍙栨暟鎹?

      **1锛夋暟鎹祦璺緞**

      Cube渚ф暟鎹祦锛?
      ```
      GM(A:ND,half) -> L1(A:Nz,half) -> L0A(A:Nz,half) -
                  鈹?                 鈹?                鈹?
                DataCopy            LoadData            鈹?
                ND->Nz              Nz->Zz/Nz           鈹?
                                                        鈹?-->L0C(Nz,float) -> GM(C:ND,half)
                                                        鈹? 鈹?                 鈹?
                                                        鈹?Mmad             Fixpipe
                                                        鈹?C=A脳B       Nz->ND, float32->half
                                                        鈹?
      GM(B:ND,half) -> L1(B:Nz,half) -> L0B(B:Zn,half) -
                  鈹?                 鈹?
                DataCopy            LoadData
                ND->Nz              Nz->Zn(杞疆)
      ```

      > **娉ㄦ剰**锛歀0A鍒嗗瀷鍦ㄤ笉鍚屼骇鍝佷笂鏈夊樊寮傦細
      > - Ascend 950PR/Ascend 950DT浜у搧锛歀0A鐨勫垎鍨嬩负Nz
      > - Atlas A2/A3绯诲垪浜у搧锛歀0A鐨勫垎鍨嬩负Zz

      鏍搁棿鍚屾涓嶸ector渚ф暟鎹祦锛?涓狢ube鏍哥殑缁撴灉鐢?涓猇ector鏍稿鐞嗭級锛?
      ```
      Cube鏍?  Fixpipe -> GM(C:ND,half)
                          鈹?
                          鈹?CrossCoreSetFlag / CrossCoreWaitFlag
                          鈻?
      Vector鏍? GM(C:ND,half) -> UB(VECCALC,half) -> UB(VECCALC,half) -> GM(C:ND,half)
                            鈹?                  鈹?                  鈹?
                        DataCopyPad          LeakyRelu          DataCopyPad
                  MTE2鎼繍锛坆aseM/2脳baseN锛?   VEC璁＄畻            MTE3鍐欏嚭
      ```

      **2锛夋祦绋嬭瑙?*锛?

      1. **Cube鏍歌绠楅樁娈?*锛?

          **GlobalTensor瀹氫箟涓庡垎鏍稿亸绉?*锛氫娇鐢╗GlobalTensor](https://gitcode.com/cann/asc-devkit/blob/master/docs/api/SIMD-API/鍩虹鏁版嵁缁撴瀯/GlobalTensor/GlobalTensor绠€浠?md)锛堝叏灞€鍐呭瓨寮犻噺锛夎闂姱鐗囧閮―DR涓殑杈撳叆/杈撳嚭鏁版嵁锛屽苟閫氳繃[GetBlockIdx](https://gitcode.com/cann/asc-devkit/blob/master/docs/api/SIMD-API/鍩虹API/宸ュ叿鎺ュ彛/绯荤粺璧勬簮涓庡彉閲?GetBlockIdx.md)锛堣幏鍙栧綋鍓嶆牳鐨勯€昏緫缂栧彿锛夎绠楀垎鏍稿亸绉伙細

          ```cpp
          class KernelMatmul {
          public:
              __aicore__ inline void Init(__gm__ uint8_t* xMatrix, __gm__ uint8_t* yMatrix, __gm__ uint8_t* zMatrix)
              {
                  // 璁剧疆GlobalTensor鐨勮捣濮嬪湴鍧€锛屾寚鍚慓M涓搴旂殑杈撳叆/杈撳嚭鐭╅樀
                  aGM.SetGlobalBuffer((__gm__ half*)xMatrix);
                  bGM.SetGlobalBuffer((__gm__ half*)yMatrix);
                  cGM.SetGlobalBuffer((__gm__ half*)zMatrix);

                  // Cube鏍告寜M鏂瑰悜鍒嗘牳锛屾瘡涓狢ube鏍歌礋璐ingleCoreM * N澶у皬杈撳嚭鍧?
                  if ASCEND_IS_AIC {
                      aGM = aGM[AscendC::GetBlockIdx() * singleCoreM * K];  // 鍋忕Щ鍒板綋鍓嶆牳璐熻矗鐨凙鐭╅樀琛?
                      cGM = cGM[AscendC::GetBlockIdx() * singleCoreM * N];  // 鍋忕Щ鍒板綋鍓嶆牳璐熻矗鐨凜鐭╅樀琛?
                  }
                  // Vector鏍告暟鏄疌ube鏍告暟鐨?鍊嶏紝1涓狢ube鏍哥殑缁撴灉鐢?涓猇ector鏍稿鐞?
                  // GetBlockIdx() / 2 灏哣ector鏍哥紪鍙锋槧灏勫洖瀵瑰簲鐨凜ube鏍歌緭鍑哄潡璧峰鍦板潃
                  if ASCEND_IS_AIV {
                      cGM = cGM[AscendC::GetBlockIdx() / 2 * singleCoreM * N];
                  }
              }
          // ...
          private:
              AscendC::GlobalTensor<half> aGM;  // 宸︾煩闃礎锛孾M, K]
              AscendC::GlobalTensor<half> bGM;  // 鍙崇煩闃礏锛孾K, N]
              AscendC::GlobalTensor<half> cGM;  // 杈撳嚭鐭╅樀C锛孾M, N]
          };
          ```

          **鍐呭瓨鎼繍涓庤绠楁祦绋?*锛?

          - **LocalTensor鍒涘缓**锛氫娇鐢╗LocalMemAllocator](https://gitcode.com/cann/asc-devkit/blob/master/docs/api/SIMD-API/鍩虹API/璧勬簮绠＄悊/鍐呭瓨绠＄悊/LocalMemAllocator/LocalMemAllocator绠€浠?md)锛堢墖涓婂唴瀛樺垎閰嶅櫒锛屾寜鐢宠椤哄簭鑷姩鍒嗛厤锛岄伩鍏嶆墜鍔ㄧ淮鎶ゅ湴鍧€鍋忕Щ锛変负鍚勭墖涓婄紦瀛樺垱寤篬LocalTensor](https://gitcode.com/cann/asc-devkit/blob/master/docs/api/SIMD-API/鍩虹鏁版嵁缁撴瀯/LocalTensor/LocalTensor绠€浠?md)锛堢墖涓婂唴瀛樺紶閲忥級銆傚叾涓瑼銆丅鐭╅樀鍦↙1涓殑涓存椂绌洪棿鐢卞悓涓€涓狶1 allocator鎸夌敵璇烽『搴忓垎閰嶏紝閬垮厤鎵嬪姩缁存姢L1鍦板潃鍋忕Щ
          - **GM 鈫?L1**锛氫娇鐢╗DataCopy](https://gitcode.com/cann/asc-devkit/blob/master/docs/api/SIMD-API/鍩虹API/鐭╅樀璁＄畻锛圛SASI锛?鐭╅樀璁＄畻鐨勬惉鍏?鐭╅樀鏁版嵁鎼叆鑷矻1-Buffer/GMToL1闅忚矾杞崲-ND2NZ鎼繍锛圖ataCopy锛?md)灏咥銆丅鐭╅樀浠嶨M鎼繍鍒癓1锛屽畬鎴怤D鍒癗z鏍煎紡杞崲锛圕ube璁＄畻鍗曞厓瑕佹眰Nz鍒嗗舰鎺掑竷锛屽洜姝ら渶鍦ㄦ惉杩愭椂灏哊D鏍煎紡杞负Nz鏍煎紡锛?
          - **L1 鈫?L0A/L0B**锛氫娇鐢╗LoadData](https://gitcode.com/cann/asc-devkit/blob/master/docs/api/SIMD-API/鍩虹API/鐭╅樀璁＄畻锛圛SASI锛?鏁版嵁鎼繍/LoadData/LoadData.md)灏嗘暟鎹惉杩愬埌L0A鍜孡0B锛孊鐭╅樀闇€瑕佽浆缃紙Nz鈫抁n锛?
          - **L0A/L0B 鈫?L0C**锛氫娇鐢╗Mmad](https://gitcode.com/cann/asc-devkit/blob/master/docs/api/SIMD-API/鍩虹API/鐭╅樀璁＄畻锛圛SASI锛?鐭╅樀璁＄畻/Mmad.md)锛堢煩闃典箻绱姞鎸囦护锛夋墽琛岀煩闃典箻鍔狅紝绱姞K杞存柟鍚戠殑鎵€鏈夋暟鎹潡
          - **L0C 鈫?GM**锛氫娇鐢╗Fixpipe](https://gitcode.com/cann/asc-devkit/blob/master/docs/api/SIMD-API/鍩虹API/鐭╅樀璁＄畻锛圛SASI锛?鏁版嵁鎼繍/Fixpipe.md)灏嗙粨鏋滄惉鍑哄埌GM锛屽畬鎴怤z鍒癗D鏍煎紡杞崲鍜宖loat32鍒癶alf绫诲瀷杞崲

          浠ｇ爜瀹炰緥濡備笅锛?

          ```cpp
          __aicore__ inline void Process()
          {
              if ASCEND_IS_AIC {
                  // ==================== Local Tensor 鍒嗛厤 ====================
                  // 浣跨敤LocalMemAllocator鎸夌敵璇烽『搴忚嚜鍔ㄥ垎閰嶇墖涓婂唴瀛橈紝閬垮厤鎵嬪姩缁存姢鍦板潃鍋忕Щ
                  AscendC::LocalMemAllocator<AscendC::Hardware::L1> l1Allocator;
                  AscendC::LocalMemAllocator<AscendC::Hardware::L0A> l0aAllocator;
                  AscendC::LocalMemAllocator<AscendC::Hardware::L0B> l0bAllocator;
                  AscendC::LocalMemAllocator<AscendC::Hardware::L0C> l0cAllocator;
                  // L1: A/B鐭╅樀鐨勪复鏃跺瓨鍌? L0A/L0B: Cube璁＄畻鍗曞厓杈撳叆; L0C: Cube璁＄畻鍗曞厓杈撳嚭
                  // Alloc鐨凾Position妯℃澘鍙傛暟鍙己鐪侊紝Alloc鍑芥暟浼氭牴鎹產llocator瀵瑰簲鐨勭墿鐞嗕綅缃粰鍑洪粯璁ゅ€?
                  AscendC::LocalTensor<half> a1Local = l1Allocator.Alloc<half>(baseM * baseK);
                  AscendC::LocalTensor<half> b1Local = l1Allocator.Alloc<half>(baseK * baseN);
                  AscendC::LocalTensor<half> a2Local = l0aAllocator.Alloc<half>(baseM * baseK);
                  AscendC::LocalTensor<half> b2Local = l0bAllocator.Alloc<half>(baseK * baseN);
                  AscendC::LocalTensor<float> cLocal = l0cAllocator.Alloc<float>(baseM * baseN);

                  // ==================== GM 鈫?L1: DataCopy + ND鈫扤z鏍煎紡杞崲 ====================
                  // DataCopy鎼繍鏃跺悓鏃跺畬鎴怤D鍒癗z鐨勬牸寮忚浆鎹紙Cube璁＄畻鍗曞厓瑕佹眰Nz鍒嗗舰鎺掑竷锛?
                  AscendC::DataCopy(a1Local, aGM, AscendC::Nd2NzParams{1, baseM, baseK, 0, K, baseM, 1, 0});
                  AscendC::DataCopy(b1Local, bGM, AscendC::Nd2NzParams{1, baseK, baseN, 0, N, baseK, 1, 0});
                  // 鏍稿唴鍚屾锛歁TE2鎼繍瀹屾垚鍚庨€氱煡MTE1鍙互寮€濮嬫惉杩?
                  AscendC::SetFlag<AscendC::HardEvent::MTE2_MTE1>(EVENT_ID0);
                  AscendC::WaitFlag<AscendC::HardEvent::MTE2_MTE1>(EVENT_ID0);

                  // ==================== L1 鈫?L0A/L0B: LoadData ====================
                  // 鈿狅笍 鏋舵瀯绾︽潫锛氫笉鍚屼骇鍝佺殑L0A鎺掑竷鏍煎紡涓嶅悓锛孡oadData鍙傛暟闇€瀵瑰簲璋冩暣
                  // A2/A3(2201)涓篫z鎺掑竷闇€閫愬潡鎼繍锛?50PR(3510)涓篘z鎺掑竷鍙竴娆″畬鎴?
                  // A2/A3鏋舵瀯 (dav-2201): L0A鎺掑竷涓篫z鏍煎紡锛岄渶閫愬潡鎼繍
          #if defined(__NPU_ARCH__) && __NPU_ARCH__ == 2201
                  for (int i = 0; i < baseM / CUBE_BLOCK; ++i) {
                      AscendC::LoadData(a2Local[i * baseK * CUBE_BLOCK], a1Local[i * 512 / sizeof(half)],
                          AscendC::LoadData2DParams{0, baseK / CUBE_BLOCK, baseM / CUBE_BLOCK, 0, 0, false, 0});
                  }
                  // B鐭╅樀Nz鈫抁n杞疆鎼繍
                  for (int i = 0; i < baseK / CUBE_BLOCK; ++i) {
                      AscendC::LoadData(b2Local[i * baseN * CUBE_BLOCK], b1Local[i * 512 / sizeof(half)],
                          AscendC::LoadData2DParams{0, baseN / CUBE_BLOCK, baseK / CUBE_BLOCK, 0, 0, true, 0});
                  }
                  // Ascend 950PR鏋舵瀯 (dav-3510): L0A鎺掑竷涓篘z鏍煎紡锛屼竴娆℃惉杩愬畬鎴?
          #elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
                  AscendC::LoadData(a2Local, a1Local,
                      AscendC::LoadData2DParamsV2{0, 0, baseM / CUBE_BLOCK, baseK / CUBE_BLOCK,
                          baseM / CUBE_BLOCK, baseM / CUBE_BLOCK, false, 0});
                  AscendC::LoadData(b2Local, b1Local,
                      AscendC::LoadData2DParamsV2{0, 0, baseK / CUBE_BLOCK, baseN / CUBE_BLOCK,
                          baseK / CUBE_BLOCK, baseN / CUBE_BLOCK, true, 0});
          #endif
                  // 鏍稿唴鍚屾锛歁TE1鎼繍瀹屾垚鍚庨€氱煡M娴佹按鍙互寮€濮婱mad璁＄畻
                  AscendC::SetFlag<AscendC::HardEvent::MTE1_M>(EVENT_ID0);
                  AscendC::WaitFlag<AscendC::HardEvent::MTE1_M>(EVENT_ID0);

                  // ==================== L0A/L0B 鈫?L0C: Mmad鐭╅樀涔樼疮鍔?====================
                  // cmatrixInitVal=true: 棣栨璁＄畻鏃跺垵濮嬪寲L0C绱姞鍣ㄤ负0
                  AscendC::Mmad(cLocal, a2Local, b2Local,
                      AscendC::MmadParams{baseM, baseN, baseK, 0, false, true});

                  // ==================== L0C 鈫?GM: Fixpipe鎼嚭 + Nz鈫扤D + fp32鈫抐p16 ====================
                  // 鏍稿唴鍚屾锛歁娴佹按瀹屾垚鍚庨€氱煡FIX娴佹按鍙互寮€濮婩ixpipe
                  AscendC::SetFlag<AscendC::HardEvent::M_FIX>(EVENT_ID0);
                  AscendC::WaitFlag<AscendC::HardEvent::M_FIX>(EVENT_ID0);
                  // Fixpipe灏哃0C缁撴灉鎼嚭鍒癎M锛屽悓鏃跺畬鎴怤Z鈫扤D鏍煎紡杞崲鍜宖loat32鈫抙alf绮惧害杞崲
                  AscendC::Fixpipe(cGM, cLocal,
                      AscendC::FixpipeParamsV220{baseN, baseM, baseM, N, false, QuantMode_t::F322F16, 0, 1, 0, 0, 0});

                  // ==================== 鏍搁棿鍚屾锛氶€氱煡Vector鏍告暟鎹氨缁?====================
                  AscendC::CrossCoreSetFlag<0x2, PIPE_FIX>(0);
              }
              // ... Vector鏍搁儴鍒嗚涓嬫柟
          }
          ```

      2. **Vector鏍歌绠楅樁娈?*锛?
          - **鏍搁棿鍚屾**锛歏ector鏍搁€氳繃[CrossCoreWaitFlag](https://gitcode.com/cann/asc-devkit/blob/master/docs/api/SIMD-API/鍩虹API/鍚屾鎺у埗/鏍搁棿鍚屾/CrossCoreSetFlag(ISASI).md)锛堟牳闂村悓姝ユ爣蹇楃瓑寰咃紝闃诲鐩村埌鏍囧織琚缃級绛夊緟Cube鏍稿畬鎴怓ixpipe鍐欏洖锛岀‘淇滿atmul璁＄畻瀹屾垚鍚庢墠寮€濮婰eakyRelu
          - **LocalTensor鍒涘缓**锛氫娇鐢║B allocator鍒涘缓`VECCALC`浣嶇疆鐨刐LocalTensor](https://gitcode.com/cann/asc-devkit/blob/master/docs/api/SIMD-API/鍩虹鏁版嵁缁撴瀯/LocalTensor/LocalTensor绠€浠?md)锛岀敤浜庢壙杞藉綋鍓峍ector鏍稿鐞嗙殑鍗婂潡缁撴灉
          - **GM 鈫?UB**锛氫娇鐢╗DataCopyPad](https://gitcode.com/cann/asc-devkit/blob/master/docs/api/SIMD-API/鍩虹API/Memory鐭㈤噺璁＄畻/鏁版嵁鎼繍/GM涓嶶B鏁版嵁鎼繍/GMToUB闈炲榻愭暟鎹惉杩?DataCopyPad).md)锛堝甫Padding鐨凣M涓嶶B涔嬮棿鏁版嵁鎼繍锛夊皢Matmul缁撴灉鎼繍鍒癠B锛屾瘡涓猇ector鏍稿鐞哹aseM/2脳baseN鐨勬暟鎹?
          - **UB璁＄畻**锛氫娇鐢╗LeakyRelu](https://gitcode.com/cann/asc-devkit/blob/master/docs/api/SIMD-API/鍩虹API/Memory鐭㈤噺璁＄畻/鍩虹绠楁湳/LeakyRelu.md)锛圠eakyReLU婵€娲诲嚱鏁帮紝璐熷€奸儴鍒嗕箻浠egativeSlope锛夋墽琛屾縺娲昏绠楋紝璐熷€奸儴鍒嗕箻浠?.001
          - **UB 鈫?GM**锛氫娇鐢╗DataCopyPad](https://gitcode.com/cann/asc-devkit/blob/master/docs/api/SIMD-API/鍩虹API/Memory鐭㈤噺璁＄畻/鏁版嵁鎼繍/GM涓嶶B鏁版嵁鎼繍/UBToGM闈炲榻愭暟鎹惉杩?DataCopyPad).md)灏嗙粨鏋滃啓鍥濭M锛屽畬鎴愯瀺鍚堣绠?

          浠ｇ爜瀹炰緥濡備笅锛?

          ```cpp
              if ASCEND_IS_AIV {
                  // ==================== UB Local Tensor 鍒嗛厤 ====================
                  AscendC::LocalMemAllocator<AscendC::Hardware::UB> ubAllocator;
                  // 姣忎釜Vector鏍稿鐞哹aseM/2 脳 baseN涓厓绱狅紙1涓狢ube鏍哥殑缁撴灉鐢?涓猇ector鏍稿钩鍒嗭級
                  AscendC::LocalTensor<half> vecLocal = ubAllocator.Alloc<half>(baseM / 2 * baseN);

                  // ==================== 鏍搁棿鍚屾锛氱瓑寰匔ube鏍窮ixpipe瀹屾垚 ====================
                  AscendC::CrossCoreWaitFlag(0);

                  // ==================== GM 鈫?UB: DataCopyPad璇诲彇Matmul缁撴灉 ====================
                  // 璁＄畻褰撳墠Vector鏍稿湪GM涓殑璧峰鍋忕Щ
                  // GetBlockIdx() % 2 鍖哄垎鍚屼竴閫昏緫鏍稿唴鐨?涓狝IV锛屽鐞嗕笉鍚屽崐鍧楁暟鎹?
                  uint32_t gmOffset = AscendC::GetBlockIdx() % 2 * (baseM / 2 * N);
                  uint32_t blockLen = baseN * sizeof(half);
                  uint32_t srcStride = (N - baseN) * sizeof(half);  // GM涓浉閭昏闂寸殑璺宠浆姝ラ暱
                  AscendC::DataCopyPad<half>(
                      vecLocal, cGM[gmOffset],
                      AscendC::DataCopyExtParams{static_cast<uint16_t>(baseM / 2), blockLen, srcStride, 0, 0},
                      AscendC::DataCopyPadExtParams<half>{true, 0, 0, 0});
                  // 鏍稿唴鍚屾锛歁TE2鎼繍瀹屾垚鍚庨€氱煡V娴佹按鍙互寮€濮嬭绠?
                  AscendC::SetFlag<AscendC::HardEvent::MTE2_V>(EVENT_ID0);
                  AscendC::WaitFlag<AscendC::HardEvent::MTE2_V>(EVENT_ID0);

                  // ==================== UB璁＄畻: LeakyRelu婵€娲?====================
                  // LeakyRelu(x) = x >= 0 ? x : x * 0.001
                  AscendC::LeakyRelu(vecLocal, vecLocal, (half)0.001, baseM / 2 * baseN);
                  // 鏍稿唴鍚屾锛歏娴佹按瀹屾垚鍚庨€氱煡MTE3鍙互寮€濮嬪啓鍥?
                  AscendC::SetFlag<AscendC::HardEvent::V_MTE3>(EVENT_ID0);
                  AscendC::WaitFlag<AscendC::HardEvent::V_MTE3>(EVENT_ID0);

                  // ==================== UB 鈫?GM: DataCopyPad鍐欏洖缁撴灉 ====================
                  AscendC::DataCopyPad<half>(cGM[gmOffset], vecLocal,
                      AscendC::DataCopyExtParams{static_cast<uint16_t>(baseM / 2), blockLen, 0, srcStride, 0});
              }
          }
          ```

  - **绾︽潫鏉′欢**锛?
    1. baseM/baseK/baseN婊¤冻16瀵归綈
    2. baseM/baseK/baseN鑳借singleCoreM/singleCoreK/singleCoreN鏁撮櫎
    3. singleCoreM/singleCoreK/singleCoreN鑳借M/K/N鏁撮櫎锛屼笉鏀寔闈炴暣鍒囧満鏅?
    4. Vector鏍告暟鏄疌ube鏍告暟鐨?鍊?

  - **璋冪敤瀹炵幇**锛?
    鏍歌皟鐢ㄧ`__mix__(1, 2)`瀹炵幇Cube鍜孷ector鏍哥殑鍗忓悓璋冪敤锛屽叾涓弬鏁癭(1, 2)`琛ㄧず姣忎釜閫昏緫鏍哥敱1涓狝IC锛圕ube鏍革級鍜?涓狝IV锛圴ector鏍革級缁勬垚锛孋ube:Vector鏍告暟姣斾緥涓?:2銆?

    ```cpp
    // __global__: 鏍稿嚱鏁板０鏄庝慨楗扮
    // __mix__(1,2): 娣峰悎鏍稿嚱鏁帮紝1涓狝IC(Cube) + 2涓狝IV(Vector)缁勬垚涓€涓€昏緫鏍?
    __global__ __mix__(1, 2) void mmad_vec_custom(__gm__ uint8_t* xMatrix, __gm__ uint8_t* yMatrix, __gm__ uint8_t* zMatrix)
    {
        AscendC::InitSocState();  // 鍒濆鍖栫‖浠剁姸鎬?
        KernelMatmul<M, K, N, singleCoreM, singleCoreK, singleCoreN,
                     baseM, baseK, baseN> op;
        op.Init(xMatrix, yMatrix, zMatrix);   // GlobalTensor鍒濆鍖栦笌鍒嗘牳鍋忕Щ
        op.Process();        // AIC渚ф墽琛孧atmul锛孉IV渚ф墽琛孡eakyRelu
        AscendC::PipeBarrier<PIPE_ALL>();  // 绛夊緟鎵€鏈夋祦姘撮樁娈靛畬鎴?
    }
    ```

- **鎺ュ彛鍙傛暟璇存槑**

  浠ヤ笅缁撴瀯浣撳潎浠ヨ姳鎷彿`{}`鏂瑰紡浼犲弬锛屽悇瀛楁鍚箟濡備笅锛堝瓧娈甸『搴忎笌API鏂囨。淇濇寔涓€鑷达紝瀹為檯struct澹版槑涓儴鍒嗗瓧娈甸『搴忓彲鑳戒笉鍚岋級锛?

  **[AscendC::Nd2NzParams](https://gitcode.com/cann/asc-devkit/blob/master/docs/api/SIMD-API/鍩虹API/鐭╅樀璁＄畻锛圛SASI锛?鐭╅樀璁＄畻鐨勬惉鍏?鐭╅樀鏁版嵁鎼叆鑷矻1-Buffer/GMToL1闅忚矾杞崲-ND2NZ鎼繍锛圖ataCopy锛?md)** 鈥?`DataCopy`鎺ュ彛浣跨敤锛屾弿杩癗D鈫扤z鏍煎紡杞崲鍙傛暟锛?
  ```cpp
  struct Nd2NzParams {
      int32_t  ndNum;              // 浼犺緭ND鐭╅樀鐨勬暟鐩紝[0, 4095]
      uint16_t nValue;             // ND鐭╅樀鐨勮鏁帮紝[0, 16384]
      int32_t  dValue;             // ND鐭╅樀鐨勫垪鏁帮紝[0, 65535]
      int32_t  srcNdMatrixStride;  // 鐩搁偦ND鐭╅樀璧峰鍦板潃鍋忕Щ锛屽崟浣嶏細鍏冪礌锛孾0, 65535]
      int32_t  srcDValue;          // 鍚屼竴ND鐭╅樀鐩搁偦琛屽亸绉伙紝鍗曚綅锛氬厓绱狅紝[1, 65535]
      uint16_t dstNzC0Stride;      // 鐩殑Nz涓悓婧愯杞崲鍚庡琛岀浉閭诲亸绉伙紝鍗曚綅锛欳0_SIZE(32B)锛孾1, 16384]
      uint16_t dstNzNStride;       // 鐩殑Nz涓璟鍨嬬煩闃电浉閭昏鍋忕Щ锛屽崟浣嶏細C0_SIZE(32B)锛孾1, 16384]
      int32_t  dstNzMatrixStride;  // 鐩殑Nz涓浉閭籒z鐭╅樀璧峰鍦板潃鍋忕Щ锛屽崟浣嶏細鍏冪礌锛孾1, 65535]
  };
  ```
  渚嬪鎼繍A鐭╅樀鏃禶{1, baseM, baseK, 0, K, baseM, 1, 0}`锛屽皢baseM脳baseK鐨凬D鏁版嵁杞负Nz鏍煎紡銆?

  **[AscendC::LoadData2DParams](https://gitcode.com/cann/asc-devkit/blob/master/docs/api/SIMD-API/鍩虹API/鐭╅樀璁＄畻锛圛SASI锛?鏁版嵁鎼繍/LoadData/LoadData.md)** 鈥?`LoadData`鎺ュ彛浣跨敤锛屾弿杩癆tlas A2 璁粌绯诲垪浜у搧/Atlas A2 鎺ㄧ悊绯诲垪浜у搧銆丄tlas A3 璁粌绯诲垪浜у搧/Atlas A3 鎺ㄧ悊绯诲垪浜у搧涓瑼鐭╅樀L1鍒癓0A鍜孊鐭╅樀L1鍒癓0B鐨勬暟鎹惉杩愬弬鏁帮細
  ```cpp
  struct LoadData2DParams {
      int32_t startIndex;   // 鍒嗗舰鐭╅樀ID锛?涓虹1涓級锛屽崟浣嶏細512B锛孾0, 65535]
      int32_t repeatTimes;  // 杩唬娆℃暟锛屾瘡涓凯浠ｅ鐞?12B锛孾1, 255]
      int32_t srcStride;    // 鐩搁偦杩唬婧愬垎褰㈣捣濮嬪湴鍧€闂撮殧锛屽崟浣嶏細512B锛孾0, 65535]
      int32_t sid;          // 棰勭暀锛岄厤缃负0
      int32_t dstGap;       // 鐩殑绔浉閭昏凯浠ｅ垎褰㈤棿闅旓紝鍗曚綅锛?12B锛孾0, 65535]
      bool    ifTranspose;  // 鏄惁杞疆姣忎釜鍒嗗舰锛岄粯璁alse
      bool    addrMode;     // 鍦板潃鏇存柊鏂瑰紡锛宖alse=閫掑锛宼rue=閫掑噺锛岄粯璁alse
  };
  ```
  渚嬪锛欰tlas A2 璁粌绯诲垪浜у搧/Atlas A2 鎺ㄧ悊绯诲垪浜у搧銆丄tlas A3 璁粌绯诲垪浜у搧/Atlas A3 鎺ㄧ悊绯诲垪浜у搧涓紝L0A涓婄殑鎺掑竷鏍煎紡涓篫z锛屾惉杩怉鐭╅樀鏃禶{0, baseK / CUBE_BLOCK, baseM / CUBE_BLOCK, 0, 0, false, 0}`锛?br>
  鎼繍B鐭╅樀鏃禶ifTranspose=true`锛屽畬鎴怤z鍒癦n鐨勮浆缃惉杩愩€?

  **[AscendC::LoadData2DParamsV2](https://gitcode.com/cann/asc-devkit/blob/master/docs/api/SIMD-API/鍩虹API/鐭╅樀璁＄畻锛圛SASI锛?鏁版嵁鎼繍/LoadData/LoadData.md)** 鈥?`LoadData`鎺ュ彛浣跨敤锛屾弿杩癆scend 950PR/Ascend 950DT浜у搧涓瑼鐭╅樀L1鍒癓0A鍜孊鐭╅樀L1鍒癓0B鐨勬暟鎹惉杩愬弬鏁帮細
  ```cpp
  struct LoadData2DParamsV2 {
      uint32_t mStartPosition;  // M鏂瑰悜璧峰浣嶇疆锛屽崟浣嶏細512B
      uint32_t kStartPosition;  // K鏂瑰悜璧峰浣嶇疆锛屽崟浣嶏細512B
      uint16_t mStep;           // M鏂瑰悜鎼繍鍒嗗舰鏁?
      uint16_t kStep;           // K鏂瑰悜鎼繍鍒嗗舰鏁?
      int32_t  srcStride;       // 婧愮鐩搁偦K鏂瑰悜鍒嗗舰闂撮殧锛屽崟浣嶏細512B
      uint16_t dstStride;       // 鐩殑绔浉閭籏鏂瑰悜鍒嗗舰闂撮殧锛屽崟浣嶏細512B
      bool     ifTranspose;     // 鏄惁杞疆姣忎釜鍒嗗舰锛岄粯璁alse
      uint8_t  sid;             // 棰勭暀锛岄厤缃负0
  };
  ```
  Ascend 950PR/Ascend 950DT浜у搧涓紝L0A涓婄殑鎺掑竷鏍煎紡涓篘z锛屾惉杩怉鐭╅樀鏃朵娇鐢╜{0, 0, baseM / CUBE_BLOCK, baseK / CUBE_BLOCK, baseM / CUBE_BLOCK, baseM / CUBE_BLOCK, false, 0}`锛屼竴娆″畬鎴怉鐭╅樀Nz鍒癗z鎼繍锛涙惉杩怋鐭╅樀鏃朵娇鐢╜{0, 0, baseK / CUBE_BLOCK, baseN / CUBE_BLOCK, baseK / CUBE_BLOCK, baseN / CUBE_BLOCK, true, 0}`锛屼竴娆″畬鎴怋鐭╅樀Nz鍒癦n鎼繍銆?

  **[AscendC::MmadParams](https://gitcode.com/cann/asc-devkit/blob/master/docs/api/SIMD-API/鍩虹API/鐭╅樀璁＄畻锛圛SASI锛?鐭╅樀璁＄畻/Mmad.md)** 鈥?`Mmad`鎺ュ彛浣跨敤锛屾弿杩扮煩闃典箻鍙傛暟锛?
  ```cpp
  struct MmadParams {
      uint16_t m;               // 宸︾煩闃礖eight锛圡缁达級锛孾0, 4095]
      uint16_t n;               // 鍙崇煩闃礧idth锛圢缁达級锛孾0, 4095]
      uint16_t k;               // 宸︾煩闃礧idth/鍙崇煩闃礖eight锛圞缁达級锛孾0, 4095]
      uint16_t unitFlag;        // Mmad涓嶧ixpipe缁嗙矑搴﹀苟琛屾帶鍒讹紝榛樿0
      bool     cmatrixSource;   // C鐭╅樀鍒濆鍊兼潵婧愶紝false=CO1锛宼rue=C2锛岄粯璁alse
      bool     cmatrixInitVal;  // C鐭╅樀鍒濆鍊兼槸鍚︿负0锛岄粯璁rue
  };
  ```
  渚嬪`{baseM, baseN, baseK, 0, false, true}`锛岃绠梑aseM脳baseN杈撳嚭鍧楀苟鍦↘鏂瑰悜绱姞baseK闀垮害銆?

  **[AscendC::FixpipeParamsV220](https://gitcode.com/cann/asc-devkit/blob/master/docs/api/SIMD-API/鍩虹API/鐭╅樀璁＄畻锛圛SASI锛?鏁版嵁鎼繍/Fixpipe.md)** 鈥?`Fixpipe`鎺ュ彛浣跨敤锛屾弿杩癓0C鍒癎M鐨勬暟鎹惉杩愬拰绮惧害杞崲鍙傛暟锛?
  ```cpp
  struct FixpipeParamsV220 {
      int32_t     nSize;        // 婧怤z鐭╅樀N鏂瑰悜澶у皬锛孾1, 4095]
      uint16_t    mSize;        // 婧怤z鐭╅樀M鏂瑰悜澶у皬锛圢z2ND鏃禰1, 8192]锛?
      uint16_t    srcStride;    // 婧怤z鐩搁偦Z鎺掑竷璧峰鍋忕Щ锛屽崟浣嶏細C0_SIZE锛孾0, 65535]
      int32_t     dstStride;    // Nz2ND鏃剁洰鐨凬D鐭╅樀姣忚鍏冪礌鏁帮紝鍗曚綅锛歟lement
      bool        reluEn;       // 鏄惁浣胯兘ReLU
      QuantMode_t quantPre;     // 閲忓寲妯″紡锛孎322F16琛ㄧずfloat鈫抙alf
      uint64_t    deqScalar;    // scalar閲忓寲鍙傛暟锛屽崟涓猻cale鍊?
      int32_t     ndNum;        // 婧怤z鐭╅樀鏁扮洰锛孾1, 65535]
      int32_t     srcNdStride;  // 涓嶅悓Nz鐭╅樀璧峰鍦板潃闂撮殧锛屽崟浣嶏細16脳C0_SIZE锛孾1, 512]
      int32_t     dstNdStride;  // 鐩殑鐩搁偦ND鐭╅樀鍋忕Щ锛屽崟浣嶏細element锛孾1, 65535]
      int32_t     unitFlag;     // Mmad涓嶧ixpipe骞惰鎺у埗
  };
  ```
  渚嬪`{baseN, baseM, baseM, N, false, F322F16, 0, 1, 0, 0, 0}`锛屽皢L0C涓殑baseM脳baseN float32缁撴灉杞负half骞跺啓鍥濭M銆?

  **[AscendC::DataCopyExtParams](https://gitcode.com/cann/asc-devkit/blob/master/docs/api/SIMD-API/鍩虹API/Memory鐭㈤噺璁＄畻/鏁版嵁鎼繍/GM涓嶶B鏁版嵁鎼繍/GMToUB闈炲榻愭暟鎹惉杩?DataCopyPad).md)** 鈥?`DataCopyPad`鎺ュ彛浣跨敤锛屾弿杩癎M涓嶶B涔嬮棿鎸夊潡鎼繍鐨勫弬鏁帮細
  ```cpp
  struct DataCopyExtParams {
      uint16_t blockCount;  // 杩炵画浼犺緭鏁版嵁鍧椾釜鏁?
      uint32_t blockLen;    // 姣忎釜鏁版嵁鍧楅暱搴︼紝鍗曚綅锛欱yte
      uint32_t srcStride;   // 婧愮鐩搁偦鏁版嵁鍧楅棿闅旓紝鍗曚綅锛欱yte
      uint32_t dstStride;   // 鐩殑绔浉閭绘暟鎹潡闂撮殧锛屽崟浣嶏細Byte
      uint32_t rsv;         // 棰勭暀瀛楁锛岄厤缃负0
  };
  ```
  渚嬪GM鎼繍鍒癠B鏃禶{static_cast<uint16_t>(baseM / 2), blockLen, srcStride, 0, 0}`锛屾瘡涓猇ector鏍歌鍙朾aseM/2琛岀粨鏋溿€?

  **[AscendC::DataCopyPadExtParams\<half\>](https://gitcode.com/cann/asc-devkit/blob/master/docs/api/SIMD-API/鍩虹API/Memory鐭㈤噺璁＄畻/鏁版嵁鎼繍/GM涓嶶B鏁版嵁鎼繍/GMToUB闈炲榻愭暟鎹惉杩?DataCopyPad).md)** 鈥?`DataCopyPad`鎺ュ彛浣跨敤锛屾弿杩板熬鍧楄ˉ榻愬弬鏁帮細
  ```cpp
  template <typename T>
  struct DataCopyPadExtParams {
      bool isPad;       // 鏄惁寮€鍚ˉ榻?
      uint8_t leftPad;  // 宸︿晶琛ラ綈闀垮害
      uint8_t rightPad; // 鍙充晶琛ラ綈闀垮害
      T paddingValue;   // 琛ラ綈鍊?
  };
  ```
  渚嬪`{true, 0, 0, 0}`琛ㄧず寮€鍚ˉ榻愯兘鍔涳紝浣嗗乏鍙充袱渚у潎涓嶉澶栬ˉ榻愩€?

## 缂栬瘧杩愯

鍦ㄦ湰鏍蜂緥鏍圭洰褰曚笅鎵ц濡備笅姝ラ锛岀紪璇戝苟鎵ц鏍蜂緥銆?

- 閰嶇疆鐜鍙橀噺

  璇锋牴鎹綋鍓嶇幆澧冧笂CANN寮€鍙戝浠跺寘鐨刐瀹夎鏂瑰紡](https://gitcode.com/cann/asc-devkit/blob/master/docs/quick_start.md#prepare&install)锛岄厤缃幆澧冨彉閲忋€?
  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **璇存槑锛?* `${install_path}` 涓篊ANN鍖呭畨瑁呯洰褰曪紝鏈寚瀹氬畨瑁呯洰褰曟椂榛樿瀹夎鑷?`/usr/local/Ascend` 涓嬨€?

- 鏍蜂緥鎵ц

  鍦ㄦ湰鏍蜂緥鐩綍涓嬫墽琛屽涓嬪懡浠ゃ€?
  ```bash
  mkdir -p build && cd build;                                               # 鍒涘缓骞惰繘鍏uild鐩綍
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j;                      # 缂栬瘧宸ョ▼锛堥粯璁pu妯″紡锛?
  python3 ../scripts/gen_data.py                                            # 鐢熸垚娴嬭瘯杈撳叆鏁版嵁
  ./demo                                                                    # 鎵ц缂栬瘧鐢熸垚鐨勫彲鎵ц绋嬪簭锛屾墽琛屾牱渚?
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin   # 楠岃瘉杈撳嚭缁撴灉鏄惁姝ｇ‘锛岀‘璁ょ畻娉曢€昏緫姝ｇ‘
  ```

  浣跨敤 CPU璋冭瘯 鎴?NPU浠跨湡 妯″紡鏃讹紝娣诲姞 `-DCMAKE_ASC_RUN_MODE=cpu` 鎴?`-DCMAKE_ASC_RUN_MODE=sim` 鍙傛暟鍗冲彲銆?

  绀轰緥濡備笅锛?
  ```bash
  cmake -DCMAKE_ASC_RUN_MODE=cpu -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j; # cpu璋冭瘯妯″紡
  cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j; # NPU浠跨湡妯″紡
  ```

  > **娉ㄦ剰锛?* 鍒囨崲缂栬瘧妯″紡鍓嶉渶娓呯悊 cmake 缂撳瓨锛屽彲鍦?build 鐩綍涓嬫墽琛?`rm CMakeCache.txt` 鍚庨噸鏂?cmake銆?

- 缂栬瘧閫夐」璇存槑

  | 閫夐」銆€銆€銆€銆€銆€ | 鍙€夊€笺€€銆€銆€銆€銆€銆€銆€銆€銆€銆€銆€| 璇存槑銆€銆€銆€銆€銆€銆€銆€銆€銆€銆€銆€銆€銆€銆€銆€銆€銆€銆€銆€銆€銆€銆€銆€銆€銆€銆€銆€銆€銆€銆€銆€銆€銆€銆€銆€銆€銆€銆€銆€銆€ |
  | ----------------| -----------------------------| --------------------------------------------------------------------------------------|
  | `CMAKE_ASC_RUN_MODE` | `npu`锛堥粯璁わ級銆乣cpu`銆乣sim` | 杩愯妯″紡锛歂PU 杩愯銆丆PU璋冭瘯銆丯PU浠跨湡銆€銆€銆€銆€銆€銆€銆€銆€銆€銆€銆€銆€銆€銆€銆€銆€銆€銆€銆€銆€銆€銆€銆€銆€ |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-2201`锛堥粯璁わ級銆乣dav-3510` | NPU 鏋舵瀯锛歞av-2201 瀵瑰簲 Atlas A2 璁粌绯诲垪浜у搧/Atlas A2 鎺ㄧ悊绯诲垪浜у搧鍜?Atlas A3 璁粌绯诲垪浜у搧/Atlas A3 鎺ㄧ悊绯诲垪浜у搧锛宒av-3510 瀵瑰簲 Ascend 950PR/Ascend 950DT |

- 鎵ц缁撴灉

  鎵ц缁撴灉濡備笅锛岃鏄庣簿搴﹀姣旀垚鍔熴€?
  ```bash
  test pass!
  ```

## 瀹炵幇娴佺▼瑙ｆ瀽

涓嬭〃鎸夋椂闂撮『搴忚缁嗘媶瑙ube鏍稿拰Vector鏍哥殑姣忎竴姝ユ搷浣滐細

### Cube鏍告祦绋?

| 闃舵 | 鏁版嵁娴佸姩/琛屼负 | 瀹炵幇鐩殑/鍘熷洜 |
|:---|:---|:---|
| 鍒濆鍖?| 浣跨敤[LocalMemAllocator](https://gitcode.com/cann/asc-devkit/blob/master/docs/api/SIMD-API/鍩虹API/璧勬簮绠＄悊/鍐呭瓨绠＄悊/LocalMemAllocator/LocalMemAllocator绠€浠?md)鍒嗛厤L1銆丩0A/L0B銆丩0C绛夌墖涓婄紦瀛樼殑[LocalTensor](https://gitcode.com/cann/asc-devkit/blob/master/docs/api/SIMD-API/鍩虹鏁版嵁缁撴瀯/LocalTensor/LocalTensor绠€浠?md) | 鎸夌敵璇烽『搴忚嚜鍔ㄥ垎閰嶇墖涓婂唴瀛橈紝閬垮厤鎵嬪姩缁存姢鍦板潃鍋忕Щ |
| GM 鈫?L1 | [DataCopy](https://gitcode.com/cann/asc-devkit/blob/master/docs/api/SIMD-API/鍩虹API/鐭╅樀璁＄畻锛圛SASI锛?鐭╅樀璁＄畻鐨勬惉鍏?鐭╅樀鏁版嵁鎼叆鑷矻1-Buffer/GMToL1闅忚矾杞崲-ND2NZ鎼繍锛圖ataCopy锛?md)灏咥/B鐭╅樀浠嶨M鎼叆L1锛屽悓鏃跺畬鎴怤D鈫扤z鏍煎紡杞崲 | Cube璁＄畻鍗曞厓瑕佹眰Nz鍒嗗舰鎺掑竷鏍煎紡锛屽洜姝ゅ湪鎼繍鏃跺繀椤诲皢ND鏍煎紡杞负Nz鏍煎紡锛岄伩鍏嶉澶栬浆鎹㈠紑閿€ |
| L1 鈫?L0A/L0B | [LoadData](https://gitcode.com/cann/asc-devkit/blob/master/docs/api/SIMD-API/鍩虹API/鐭╅樀璁＄畻锛圛SASI锛?鏁版嵁鎼繍/LoadData/LoadData.md)灏咥鐭╅樀浠嶭1鎼叆L0A锛圢z鈫抁z/Nz锛夛紝B鐭╅樀浠嶭1鎼叆L0B锛圢z鈫抁n杞疆锛?| B鐭╅樀闇€瑕佽浆缃槸鍥犱负Mmad鎸囦护瑕佹眰B鐭╅樀浠n锛堣浆缃甆z锛夋牸寮忚緭鍏?|
| L0A/L0B 鈫?L0C | [Mmad](https://gitcode.com/cann/asc-devkit/blob/master/docs/api/SIMD-API/鍩虹API/鐭╅樀璁＄畻锛圛SASI锛?鐭╅樀璁＄畻/Mmad.md)鎵ц鐭╅樀涔樺姞锛屽湪K鏂瑰悜绱姞鎵€鏈夋暟鎹潡 | 瀹屾垚A脳B鐨勭煩闃典箻璁＄畻锛孠鏂瑰悜鍒嗗潡绱姞纭繚姝ｇ‘鎬?|
| 鏍稿唴鍚屾 | [SetFlag](https://gitcode.com/cann/asc-devkit/blob/master/docs/api/SIMD-API/鍩虹API/鍚屾鎺у埗/鏍稿唴鍚屾/SetFlag-WaitFlag(ISASI).md)/[WaitFlag](https://gitcode.com/cann/asc-devkit/blob/master/docs/api/SIMD-API/鍩虹API/鍚屾鎺у埗/鏍稿唴鍚屾/SetFlag-WaitFlag(ISASI).md)纭繚鏁版嵁鎼繍瀹屾垚鍚庡啀寮€濮嬩笅涓€姝ユ搷浣?| 閬垮厤LoadData璇诲彇灏氭湭鎼繍瀹屾垚鐨勬暟鎹紝閬垮厤Mmad璇诲彇灏氭湭鍔犺浇瀹屾垚鐨勬暟鎹?|
| L0C 鈫?GM | [Fixpipe](https://gitcode.com/cann/asc-devkit/blob/master/docs/api/SIMD-API/鍩虹API/鐭╅樀璁＄畻锛圛SASI锛?鏁版嵁鎼繍/Fixpipe.md)灏哃0C缁撴灉鎼嚭鍒癎M锛屽悓鏃跺畬鎴怤z鈫扤D鏍煎紡杞崲鍜宖p32鈫抐p16绮惧害杞崲 | 杈撳嚭缁撴灉闇€鍥炲埌GM渚沄ector鏍歌鍙栵紝鏍煎紡闇€杞负ND鎺掑竷锛岀簿搴﹂渶浠巉p32闄嶄负fp16浠ュ尮閰嶈緭鍑鸿姹?|
| 鏍搁棿鍚屾 | [CrossCoreSetFlag](https://gitcode.com/cann/asc-devkit/blob/master/docs/api/SIMD-API/鍩虹API/鍚屾鎺у埗/鏍搁棿鍚屾/CrossCoreSetFlag(ISASI).md)閫氱煡Vector鏍告暟鎹氨缁?| 纭繚Vector鏍镐笉浼氬湪Fixpipe瀹屾垚鍓嶅紑濮嬭鍙朑M鏁版嵁 |

### Vector鏍告祦绋?

| 闃舵 | 鏁版嵁娴佸姩/琛屼负 | 瀹炵幇鐩殑/鍘熷洜 |
|:---|:---|:---|
| 鏍搁棿鍚屾 | [CrossCoreWaitFlag](https://gitcode.com/cann/asc-devkit/blob/master/docs/api/SIMD-API/鍩虹API/鍚屾鎺у埗/鏍搁棿鍚屾/CrossCoreSetFlag(ISASI).md)绛夊緟Cube鏍窮ixpipe瀹屾垚 | 闃诲鐩村埌Cube鏍搁€氱煡鏁版嵁灏辩华锛岀‘淇濊鍙栧埌瀹屾暣鐨凪atmul缁撴灉 |
| 鍒濆鍖?| 浣跨敤UB allocator鍒嗛厤VECCALC浣嶇疆鐨刐LocalTensor](https://gitcode.com/cann/asc-devkit/blob/master/docs/api/SIMD-API/鍩虹鏁版嵁缁撴瀯/LocalTensor/LocalTensor绠€浠?md) | 涓篏M鈫扷B鎼繍鍜孷ector璁＄畻鍒嗛厤UB缂撳啿鍖?|
| GM 鈫?UB | [DataCopyPad](https://gitcode.com/cann/asc-devkit/blob/master/docs/api/SIMD-API/鍩虹API/Memory鐭㈤噺璁＄畻/鏁版嵁鎼繍/GM涓嶶B鏁版嵁鎼繍/GMToUB闈炲榻愭暟鎹惉杩?DataCopyPad).md)灏哅atmul缁撴灉浠嶨M鎼叆UB锛屾瘡涓猇ector鏍歌鍙朾aseM/2脳baseN琛?| Vector鏍镐粠GM璇诲彇Cube鏍稿啓鍥炵殑Matmul缁撴灉锛屾瘡涓猇ector鏍稿鐞嗗崐鍧楁暟鎹?|
| UB璁＄畻 | [LeakyRelu](https://gitcode.com/cann/asc-devkit/blob/master/docs/api/SIMD-API/鍩虹API/Memory鐭㈤噺璁＄畻/鍩虹绠楁湳/LeakyRelu.md)鎵ц婵€娲昏绠楋紝璐熷€奸儴鍒嗕箻浠?.001 | 瀵筂atmul缁撴灉鏂藉姞LeakyRelu婵€娲诲嚱鏁帮紝瀹屾垚铻嶅悎璁＄畻 |
| UB 鈫?GM | [DataCopyPad](https://gitcode.com/cann/asc-devkit/blob/master/docs/api/SIMD-API/鍩虹API/Memory鐭㈤噺璁＄畻/鏁版嵁鎼繍/GM涓嶶B鏁版嵁鎼繍/UBToGM闈炲榻愭暟鎹惉杩?DataCopyPad).md)灏哃eakyRelu缁撴灉鍐欏洖GM | 灏嗘渶缁堣绠楃粨鏋滆緭鍑哄埌GM渚涘悗缁娇鐢?|

## 鍙紭鍖栨柟鍚戝垎鏋?

| 鍙紭鍖栨柟鍚?| 褰撳墠瀹炵幇鐨勯棶棰?| 棰勬湡浼樺寲鏀剁泭 |
|:---|:---|:---|
| L1/L0鍙岀紦鍐叉祦姘寸嚎 | Cube鏍稿唴GM鈫扡1鈫扡0A/L0B鈫扡0C鈫扜M鍚勯樁娈典覆琛屾墽琛岋紝鎼繍鍜岃绠楁棤娉曢噸鍙?| Ping-Pong鍙岀紦鍐蹭娇MTE2鎼繍鍜孧TE1/Mmad璁＄畻骞惰锛屽ぇ骞呮彁鍗囧悶鍚愰噺 |
| Fixpipe涓嶮mad缁嗙矑搴﹀苟琛?| 褰撳墠Mmad鍜孎ixpipe鏁村潡涓茶锛孧mad璁＄畻鏃禙ixpipe绌洪棽锛孎ixpipe鎼嚭鏃禡mad绌洪棽 | 鎷嗗垎涓烘洿灏忕矑搴︾殑鍧楋紝Mmad鍜孎ixpipe浜ゆ浛鎵ц锛屽噺灏戞祦姘寸嚎姘旀场 |
| UB鍙岀紦鍐?| Vector鏍稿唴GM鈫扷B鈫掕绠椻啋GM涓茶鎵ц | UB Ping-Pong鍙岀紦鍐蹭娇MTE2鎼繍鍜孷EC璁＄畻骞惰 |
| Vector鏍竀F铻嶅悎 | 褰撳墠浣跨敤MemBase API锛屼腑闂寸粨鏋滈渶鍐欏洖UB | 浣跨敤RegBase + [asc_vf_call](https://gitcode.com/cann/asc-devkit/blob/master/docs/api/SIMD-API/鍩虹API/Reg鐭㈤噺璁＄畻/asc_vf_call.md)杩涜VF铻嶅悎锛屼腑闂磋绠楀湪瀵勫瓨鍣ㄥ畬鎴愶紝鍑忓皯UB璇诲啓娆℃暟 |
| 澶у寘鎼繍 | 褰撳墠浠呮惉杩愬崟鍧梑aseM脳baseN鏁版嵁 | 澧炲ぇsingleCoreM/singleCoreN锛屽噺灏戞惉杩愭鏁帮紝鎻愰珮甯﹀鍒╃敤鐜?|
| GM涓浆浼樺寲 | Cube鏍哥粨鏋滈渶缁廏M涓浆缁橵ector鏍革紙L0C鈫扜M鈫扷B锛夛紝涓ゆ鎼繍寮€閿€澶?| Ascend 950PR鏀寔Fixpipe鐩存帴鍐欏叆UB锛圠0C鈫扷B锛夛紝鐪佸幓GM涓浆寮€閿€ |

## 鍔熻兘璋冭瘯

### printf

璇ユ帴鍙ｆ彁渚汣PU鍩?NPU鍩熻皟璇曞満鏅笅鐨勬牸寮忓寲杈撳嚭鍔熻兘銆?

鍦ㄧ畻瀛恔ernel渚у疄鐜颁唬鐮佷腑闇€瑕佽緭鍑烘棩蹇椾俊鎭殑鍦版柟璋冪敤printf鎺ュ彛鎵撳嵃鐩稿叧鍐呭銆?

绀轰緥濡備笅锛?

```cpp
AscendC::printf("matmul blockIdx=%d\n", AscendC::GetBlockIdx());
```

> **娉ㄦ剰锛?* printf锛圥RINTF锛夋帴鍙ｆ墦鍗板姛鑳戒細瀵圭畻瀛愬疄闄呰繍琛岀殑鎬ц兘甯︽潵涓€瀹氬奖鍝嶏紝閫氬父鍦ㄨ皟娴嬮樁娈典娇鐢ㄣ€傚紑鍙戣€呭彲浠ユ寜闇€閫氳繃璁剧疆ASCENDC_DUMP=0鐨勬柟寮忓叧闂墦鍗板姛鑳姐€?

### DumpTensor

鍩轰簬绠楀瓙宸ョ▼寮€鍙戠殑绠楀瓙锛屽彲浠ヤ娇鐢ㄨ鎺ュ彛Dump鎸囧畾[LocalTensor](https://gitcode.com/cann/asc-devkit/blob/master/docs/api/SIMD-API/鍩虹鏁版嵁缁撴瀯/LocalTensor/LocalTensor绠€浠?md)鐨勫唴瀹广€傚悓鏃舵敮鎸佹墦鍗拌嚜瀹氫箟鐨勯檮鍔犱俊鎭紙浠呮敮鎸乽int32\_t鏁版嵁绫诲瀷鐨勪俊鎭級锛屾瘮濡傛墦鍗板綋鍓嶈鍙风瓑銆?

鍦ㄧ畻瀛恔ernel渚у疄鐜颁唬鐮佷腑闇€瑕佹墦鍗癟ensor鏁版嵁鐨勫湴鏂硅皟鐢―umpTensor鎺ュ彛鎵撳嵃鐩稿叧鍐呭銆傛牱渚嬪涓嬶細

```cpp
// Vector鏍窵eakyRelu缁撴灉
AscendC::LeakyRelu(zLocal, xLocal, ..., len);
AscendC::DumpTensor(zLocal, 1, 16);
```

> **娉ㄦ剰锛?* [DumpTensor](https://gitcode.com/cann/asc-devkit/blob/master/docs/api/SIMD-API/鍩虹API/璋冭瘯鎺ュ彛/涓婃澘鎵撳嵃/DumpTensor.md)鎺ュ彛鎵撳嵃鍔熻兘浼氬绠楀瓙瀹為檯杩愯鐨勬€ц兘甯︽潵涓€瀹氬奖鍝嶏紝閫氬父鍦ㄨ皟娴嬮樁娈典娇鐢ㄣ€傚紑鍙戣€呭彲浠ユ寜闇€閫氳繃璁剧疆ASCENDC_DUMP=0鏉ュ叧闂墦鍗板姛鑳姐€?

## 鎬ц兘璋冭瘯

浣跨敤 `msprof` 宸ュ叿鑾峰彇璇︾粏鎬ц兘鏁版嵁锛?

```bash
msprof ./demo   # 鍒嗘瀽鎬ц兘
```

褰撳墠鐩綍涓嬩細鐢熸垚PROF_鍓嶇紑鐨勬枃浠跺す锛宍mindstudio_profiler_output`鐩綍淇濆瓨Host鍜屽悇涓狣evice鐨勬€ц兘鏁版嵁姹囨€伙紝鎬ц兘鏁版嵁鍒嗘瀽鎺ㄨ崘鏌ョ湅璇ョ洰褰曚笅鏂囦欢

```bash
PROF_xxxx_XXXXXX
鈹溾攢鈹€ device_{id}
鈹斺攢鈹€ host
鈹斺攢鈹€ mindstudio_profiler_log
鈹斺攢鈹€ mindstudio_profiler_output    # 淇濆瓨Host鍜屽悇涓狣evice鐨勬€ц兘鏁版嵁姹囨€?
    鈹溾攢鈹€ msprof_*.json
    鈹溾攢鈹€ xx_*.csv
    鈹斺攢鈹€ README.txt
```

鏌ョ湅鍏蜂綋鐨勬€ц兘鍒嗘瀽缁撴灉锛?
```
# 鏌ョ湅Task Duration 浠ュ強鍚勯」鏁版嵁
cat ./PROF_*/mindstudio_profiler_output/op_summary_*.csv
```
