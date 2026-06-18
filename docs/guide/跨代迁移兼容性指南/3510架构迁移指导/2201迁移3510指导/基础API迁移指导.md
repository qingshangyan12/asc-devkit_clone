# 鍩虹API杩佺Щ鎸囧<a name="ZH-CN_TOPIC_0000002470508258"></a>

鏈妭閽堝[NPU鏋舵瀯鐗堟湰3510](../../../缂栫▼鎸囧崡/璇█鎵╁睍灞?SIMD-BuiltIn鍏抽敭瀛?md#table65291052154114)鐨勮姱鐗囧彉鏇村鍩虹API鍏煎鎬т骇鐢熺殑褰卞搷杩涜璇存槑锛屽苟鎻愪緵鍩虹API鐨勫吋瀹规€ч€傞厤鏂规銆?

## 鐭㈤噺璁＄畻<a name="section7364115741514"></a>

-   **3510鏋舵瀯榛樿涓嶆敮鎸丼ubnormal鍔熻兘銆?*

    **璇存槑**锛歋ubNormal娴偣鏁版寚鐨勬槸鎸囨暟浣嶅叏涓?銆佸熬鏁颁笉涓?鐨勬诞鐐规暟锛岀敤浜庤〃绀烘瘮鏈€灏忔甯告暟鏇村皬鐨勫€硷紝閬垮厤鈥滀笅婧负0鈥濄€?510鐗堟湰榛樿涓嶆敮鎸丼ubnormal锛孲ubnormal娴偣鏁板湪璁＄畻涓瑙嗕负0銆?

    **鍏煎鏂规**锛氶€氳繃璁剧疆config妯℃澘鍙傛暟鏉ラ厤缃甋ubnormal璁＄畻妯″紡銆傝蒋浠舵ā鎷熷Subnormal鏁版嵁鐨勫鐞嗭紝閫氳繃绮惧害鎵╁睍绛夊鐞嗘柟寮忔潵閬垮厤Subnormal娴偣鏁颁笅婧负0銆?

    **琛?1**  娑夊強Subnormal鐨凙PI鍜宑onfig鍙傛暟璇存槑

    <a name="table288510533427"></a>
    <table><thead align="left"><tr id="row388575318425"><th class="cellrowborder" align="center" valign="top" width="27%" id="mcps1.2.3.1.1"><p id="p12885175314420"><a name="p12885175314420"></a><a name="p12885175314420"></a>Ascend C鍩虹API</p>
    </th>
    <th class="cellrowborder" align="center" valign="top" width="73%" id="mcps1.2.3.1.2"><p id="p1288585311428"><a name="p1288585311428"></a><a name="p1288585311428"></a>鍏煎璇存槑</p>
    </th>
    </tr>
    </thead>
    <tbody><tr id="row188851453184214"><td class="cellrowborder" align="center" valign="top" width="27%" headers="mcps1.2.3.1.1 "><p id="p1188545310427"><a name="p1188545310427"></a><a name="p1188545310427"></a>Exp銆丩n銆丷eciprocal銆丼qrt銆丷sqrt銆丏iv</p>
    </td>
    <td class="cellrowborder" align="left" valign="top" width="73%" headers="mcps1.2.3.1.2 "><p id="p1055013361204"><a name="p1055013361204"></a><a name="p1055013361204"></a>浠n鎺ュ彛涓轰緥鏉ヨ繘琛岃鏄庛€?/p>
    <p id="p151399151208"><a name="p151399151208"></a><a name="p151399151208"></a>閫氳繃LnConfig缁撴瀯浣撶殑鍙傛暟algo鏉ラ厤缃甋ubnormal璁＄畻妯″紡銆俛lgo鍙栧€煎涓嬶細</p>
    <a name="ul118851253124216"></a><a name="ul118851253124216"></a><ul id="ul118851253124216"><li>LnAlgo::INTRINSIC銆丩nAlgo::PRECISION_1ULP_FTZ_TRUE锛屼娇鐢ㄥ崟鎸囦护璁＄畻寰楀嚭缁撴灉锛屾墍鏈塖ubnormal琚繎浼间负0銆?/li><li>LnAlgo::PRECISION_1ULP_FTZ_FALSE锛屾敮鎸丼ubnormal鏁版嵁璁＄畻銆?/li></ul>
    <p id="p9885135319427"><a name="p9885135319427"></a><a name="p9885135319427"></a>璇ュ弬鏁伴粯璁ゅ€糄EFAULT_LN_CONFIG鐨勫彇鍊煎涓嬶細</p>
    <a name="screen18681133513203"></a><a name="screen18681133513203"></a><pre class="screen" codetype="Cpp" id="screen18681133513203">constexpr LnConfig DEFAULT_LN_CONFIG = { LnAlgo::INTRINSIC };</pre>
    </td>
    </tr>
    </tbody>
    </table>

    鍙互鍙傝€冧互涓嬩唬鐮佺墖娈碉細

    ```
    // 瀹氫箟妯℃澘鍙傛暟
    constexpr AscendC::LnConfig CONFIG = {
        AscendC::LnAlgo::PRECISION_1ULP_FTZ_FALSE
    };
    constexpr uint32_t DATA_COUNT = 1024;
    constexpr uint32_t xAddr = 0;
    constexpr uint32_t yAddr = xAddr + DATA_COUNT * sizeof(half);

    // 鏍稿嚱鏁板叆鍙ｏ細浣跨敤__vector__鏄惧紡澹版槑Vector鏍稿嚱鏁帮紝鎼厤闈欐€乀ensor鏂瑰紡缂栫▼
    __vector__ __global__ void ln_custom(__gm__ uint8_t* src, __gm__ uint8_t* dst)
    {
        // 闈欐€乀ensor闇€瑕佹墜鍔ㄨ皟鐢↖nitSocState鍒濆鍖栧叏灞€鐘舵€佸瘎瀛樺櫒
        AscendC::InitSocState();

        AscendC::GlobalTensor<half> srcGlobal;
        AscendC::GlobalTensor<half> dstGlobal;
        srcGlobal.SetGlobalBuffer((__gm__ half*)src, DATA_COUNT);
        dstGlobal.SetGlobalBuffer((__gm__ half*)dst, DATA_COUNT);

        // 鐩存帴鏋勯€犳寚瀹氬湴鍧€鍜屽瓨鍌ㄤ綅缃殑LocalTensor锛堥潤鎬乀ensor鏂瑰紡锛?
        AscendC::LocalTensor<half> srcLocal(AscendC::TPosition::VECCALC, xAddr, DATA_COUNT);
        AscendC::LocalTensor<half> dstLocal(AscendC::TPosition::VECCALC, yAddr, DATA_COUNT);

        AscendC::DataCopy(srcLocal, srcGlobal, DATA_COUNT);
        AscendC::SetFlag<AscendC::HardEvent::MTE2_V>(EVENT_ID0);
        AscendC::WaitFlag<AscendC::HardEvent::MTE2_V>(EVENT_ID0);

        // 璋冪敤Ln鍩虹API锛屼紶鍏ユā鏉垮弬鏁?
        AscendC::Ln<half, CONFIG>(dstLocal, srcLocal, DATA_COUNT);

        AscendC::SetFlag<AscendC::HardEvent::V_MTE3>(EVENT_ID0);
        AscendC::WaitFlag<AscendC::HardEvent::V_MTE3>(EVENT_ID0);
        AscendC::DataCopy(dstGlobal, dstLocal, DATA_COUNT);

        AscendC::PipeBarrier<PIPE_ALL>();
    }
    ```

## 鏁版嵁鎼繍<a name="section7530159122210"></a>

-   **DataCopy鎺ュ彛涓嶆敮鎸丩1 Buffer-\>GM閫氳矾銆?*

    **璇存槑**锛氱‖浠跺垹闄1 Buffer鍒癎M鐨勯€氳矾锛屾棤娉曞皢鏁版嵁浠嶭1 Buffer鐩存帴鎼繍鍒癎M涓€傜幇鏈夋帴鍙ｄ笉鏀寔L1 Buffer鍒癎M鐨勭洿鎺ユ惉杩愩€?

    **鍏煎鏂规**锛氬浜庣函Cube璁＄畻鍦烘櫙锛氬湪GM澶氬垎閰嶄竴涓崟浣嶇煩闃碉紝閫氳繃Mmad鐭╅樀涔樻硶璁＄畻杈撳嚭鍒癓0C Buffer锛屽啀浠嶭0C Buffer閫氳繃Fixpipe鎼繍鍒癎M銆傚浜嶸ector鍜孋ube璁＄畻铻嶅悎鍦烘櫙锛屽彲浠ラ€氳繃L1 Buffer鎼繍鍒癠B锛屽啀鎼繍鍒癎M銆備互涓嬩互绾疌ube璁＄畻鍦烘櫙涓轰緥杩涜璇存槑锛屼粙缁嶇畻瀛愭牳蹇冩祦绋嬶紝鍏蜂綋鍙弬鑰僛L1鍒癎M鎼繍鍏煎鎬ф牱渚媇(https://gitcode.com/cann/asc-devkit/tree/master/examples/01_simd_cpp_api/06_compatibility_guide/data_copy_l1togm)銆?

    1.  灏嗙煩闃礎浠嶨M鎼繍鍒癓1 Buffer銆?

        ```
        __aicore__ inline void CopyGmToL1A(AscendC::LocalTensor<T> a1Local)
        {
            AscendC::Nd2NzParams intriParams1{1, M, K, 0, K, M, 1, 0};
            AscendC::DataCopy(a1Local, aGlobal, intriParams1);
        }
        ```

    2.  灏嗙煩闃礏锛堢煩闃礏涓哄崟浣嶇煩闃碉級浠嶨M鎼繍鍒癓1 Buffer銆?

        ```
        __aicore__ inline void CopyGmToL1B(AscendC::LocalTensor<U> b1Local)
        {
            AscendC::Nd2NzParams intriParams2{1, K, N, 0, N, K, 1, 0};
            AscendC::DataCopy(b1Local, bGlobal, intriParams2);
        }
        ```

    3.  灏嗙煩闃礎浠嶭1 Buffer鎼繍鍒癓0A Buffer銆?

        ```
        __aicore__ inline void Load2DL1AToL0A(AscendC::LocalTensor<T> a1Local, AscendC::LocalTensor<T> a2Local)
        {
            AscendC::LoadData2DParamsV2 loadDataParams;
            ...
            AscendC::LoadData(a2Local, a1Local, loadDataParams);
        }
        ```

    4.  灏嗙煩闃礏浠嶭1 Buffer鎼繍鍒癓0B Buffer銆?

        ```
        __aicore__ inline void Load2DL1BToL0B(AscendC::LocalTensor<U> b1Local, AscendC::LocalTensor<U> b2Local)
        {
            AscendC::LoadData2DParamsV2 loadDataParams;
            ...
            AscendC::LoadData(b2Local, b1Local, loadDataParams);
        }
        ```

    5.  杩涜Mmad鐭╅樀璁＄畻锛岀粨鏋滆緭鍑哄埌L0C Buffer銆?

        ```
        __aicore__ inline void Compute(
            AscendC::LocalTensor<S> co1Local, AscendC::LocalTensor<T> a2Local, AscendC::LocalTensor<U> b2Local)
        {
            AscendC::MmadParams mmadParams;
            ...
            AscendC::Mmad(co1Local, a2Local, b2Local, mmadParams);
        }
        ```

    6.  閫氳繃FixPipe灏嗙煩闃礐浠嶭0C Buffer鎷疯礉鍒癎M銆?

        ```
        __aicore__ inline void CopyL0CToGm(AscendC::LocalTensor<S> co1Local)
        {
            AscendC::FixpipeParamsV220 fixpipeParams;
            ...
            AscendC::Fixpipe<S, S, AscendC::CFG_ROW_MAJOR>(cGlobal, co1Local, fixpipeParams);
        }
        ```

-   **涓嶆敮鎸丼etLoadDataBoundary鎺ュ彛銆?*

    **璇存槑**锛?510鏋舵瀯纭欢鍒犻櫎浜哃1 Buffer鐨勮竟鐣屽€艰瀹氱浉鍏冲瘎瀛樺櫒锛屼笉鍐嶆敮鎸丼etLoadDataBoundary鎺ュ彛銆傝鎺ュ彛鐢ㄤ簬璁剧疆Load3D鏃禠1 Buffer鐨勮竟鐣屽€笺€傚鏋滄寚浠ゅ湪澶勭悊婧愭搷浣滄暟鏃讹紝婧愭搷浣滄暟鍦↙1 Buffer涓婄殑鍦板潃瓒呭嚭璁剧疆鐨勮竟鐣岋紝鍒欎細浠嶭1 Buffer鐨勮捣濮嬪湴鍧€寮€濮嬭鍙栨暟鎹€傝缃负0琛ㄧず鏃犺竟鐣岋紝鍙互浣跨敤鏁翠釜L1 Buffer銆?

    **鍏煎鏂规**锛?

    -   [NPU鏋舵瀯鐗堟湰2201](../../../缂栫▼鎸囧崡/璇█鎵╁睍灞?SIMD-BuiltIn鍏抽敭瀛?md#table65291052154114)鐨勬帴鍙ｅ弬鏁癰oundaryValue璁剧疆涓?鏃朵笌3510鏋舵瀯鐗堟湰绛変环銆?
    -   濡傛灉闇€瑕佸湪L1 Buffer涓婂惊鐜鍙栨搷浣滄暟锛岄渶瑕佸皢瀵瑰簲鐨凩oad3D鎺ュ彛鎵嬪姩鎷嗗垎鎴愬鏉℃寚浠わ紝鎵嬪姩缁曞洖銆傚叿浣撲唬鐮佸彲鍙傝€僛SetLoadDataBoundary鍏煎鎬ф牱渚媇(https://gitcode.com/cann/asc-devkit/tree/master/examples/01_simd_cpp_api/06_compatibility_guide/set_loaddata_boundary)銆?

    ![](../../../figures/1_zn.png)

    濡備笂鍥炬墍绀猴紝浠1 Buffer鍒癓0A Buffer鐨勬惉杩愪负渚嬨€傜煩闃礎涓篽alf鏁版嵁绫诲瀷锛屽ぇ灏忎负32 \* 32鐨勭煩闃碉紝鍋囪杈圭晫涓?12B锛屽彲浠ラ噸澶嶆惉杩愭暟鎹埌L0A Buffer锛屽湪姣忔鎼繍鏃惰缃洰鐨勬搷浣滄暟鐨勫湴鍧€鍋忕Щ閲忋€?

    1.  灏嗙煩闃礎浠嶨M鎼繍鍒癓1 Buffer銆?

        ```
        __aicore__ inline void CopyGmToA1Nd2Nz(AscendC::LocalTensor<T>& leftMatrix)
        {
            AscendC::Nd2NzParams nd2nzParams;
            ...
            AscendC::DataCopy(leftMatrix, aGlobal, nd2nzParams);
        }
        ```

    2.  灏嗙煩闃礏浠嶨M鎼繍鍒癓1 Buffer銆?

        ```
        __aicore__ inline void CopyGmToB1Nd2Nz(AscendC::LocalTensor<U>& rightMatrix)
        {
            AscendC::Nd2NzParams nd2nzParams;
            ...
            AscendC::DataCopy(rightMatrix, bGlobal, nd2nzParams);
        }
        ```

    3.  灏嗙煩闃礎浠嶭1 Buffer鎼繍鍒癓0A Buffer銆?

        ```
            __aicore__ inline void Load3DA1ToL0A(AscendC::LocalTensor<T>& leftMatrix, AscendC::LocalTensor<T>& a2)
            {
                AscendC::LoadData3DParamsV2Pro loadData3dParamsPro;
                ...
        #if defined(__NPU_ARCH__) && __NPU_ARCH__ == 2201
                ...
                AscendC::SetLoadDataRepeat({0, 1, 0});
                AscendC::SetLoadDataBoundary(1024);
                AscendC::LoadData(a2, leftMatrix, loadData3dParamsPro);
        #elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
                uint16_t dstStride = AscendC::DivCeil(M / 2, 16);
                ...
                AscendC::SetLoadDataRepeatWithStride({0, 1, 0, dstStride});
                // 澶氭璋冪敤LoadData杩涜鎵嬪姩缁曞洖
                AscendC::LoadData(a2, leftMatrix, loadData3dParamsPro);
                AscendC::LocalTensor<T> a3 = a2[256];
                AscendC::LoadData(a3, leftMatrix, loadData3dParamsPro);
                AscendC::LocalTensor<T> a4 = a2[512];
                AscendC::LoadData(a4, leftMatrix, loadData3dParamsPro);
                AscendC::LocalTensor<T> a5 = a2[768];
                AscendC::LoadData(a5, leftMatrix, loadData3dParamsPro);
        #endif
            }
        ```

    4.  灏嗙煩闃礏浠嶭1 Buffer鎼繍鍒癓0B Buffer銆?

        ```
            __aicore__ inline void Load3DB1ToL0B(AscendC::LocalTensor<U>& rightMatrix, AscendC::LocalTensor<U>& b2)
            {
                AscendC::LoadData3DParamsV2<U> loadData3dParams;
                ...
                uint8_t padList[AscendC::PAD_SIZE] = {0, 0, 0, 0};
                static constexpr AscendC::IsResetLoad3dConfig LOAD3D_CONFIG = {false, false};
                AscendC::SetFmatrix(N, 1, padList, AscendC::FmatrixMode::FMATRIX_LEFT);
        #if defined(__NPU_ARCH__) && __NPU_ARCH__ == 2201
                AscendC::SetLoadDataRepeat({0, 1, 0});
                AscendC::SetLoadDataBoundary(0);
                AscendC::SetLoadDataPaddingValue(0);
                AscendC::LoadData<U, LOAD3D_CONFIG>(b2, rightMatrix, loadData3dParams);
        #elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
                uint16_t dstStride = AscendC::DivCeil(N, 16);
                AscendC::SetLoadDataRepeatWithStride({0, 1, 0, dstStride});
                AscendC::SetLoadDataPaddingValue(0);
                AscendC::LoadDataWithStride<U, LOAD3D_CONFIG>(b2, rightMatrix, loadData3dParams);
        #endif
            }
        ```

    5.  鐭╅樀璁＄畻銆?

        ```
        __aicore__ inline void Compute(AscendC::LocalTensor<T>& a2, AscendC::LocalTensor<U>& b2,
                                    AscendC::LocalTensor<V>& co1Local)
        {
            AscendC::MmadParams mmadParams;
            ...
            AscendC::Mmad(co1Local, a2, b2, mmadParams);
        }
        ```

    6.  灏嗙煩闃礐浠嶭0C Buffer鎼繍鍒癎M銆?

        ```
        __aicore__ inline void CopyL0CToGm(AscendC::LocalTensor<V>& co1Local, const AscendC::GlobalTensor<S>& gm)
        {
            AscendC::FixpipeParamsV220 fixpipeParams(N, static_cast<uint16_t>(M),
                                            AscendC::DivCeil(M, AscendC::BLOCK_CUBE) * AscendC::BLOCK_CUBE, static_cast<uint16_t>(N), 0);
            ...
            AscendC::Fixpipe<S, V, AscendC::CFG_ROW_MAJOR>(gm, co1Local, fixpipeParams);
        }
        ```

## 鐭╅樀璁＄畻<a name="section17560113713122"></a>

-   **Cube璁＄畻鍗曞厓鍒犻櫎int4b\_t鏁版嵁绫诲瀷銆?*

    **璇存槑**锛氱浉杈冧簬2201鏋舵瀯鐗堟湰锛?510鏋舵瀯鐗堟湰鐨凜ube璁＄畻鍗曞厓涓嶆敮鎸乮nt4b\_t銆傜浉鍏崇殑鍩虹API鏈塋oadData銆丮mad鍜孡oadDataWithTranspose锛岃繖浜涙帴鍙ｄ笉鍐嶆敮鎸乮nt4b\_t銆?

    **鍏煎鏂规**锛氱畻瀛愪晶閫氳繃缂栧啓CV铻嶅悎绠楀瓙鍦╒ector Core杩涜int4b\_t鍒癷nt8\_t鐨凜ast杞崲锛屽啀閫氳繃UB鎼繍鍒癓1鍚庤繘琛孧mad璁＄畻銆傚叿浣撲唬鐮佸彲鍙傝€僛int4鏁版嵁绫诲瀷涓婱atmul鍏煎鎬ф牱渚媇(https://gitcode.com/cann/asc-devkit/tree/master/examples/01_simd_cpp_api/06_compatibility_guide/matmul_s4)銆傚浘灞傞潰鍙互鍦ㄨ绠楀瓙鍓嶅鍔燙ast鑺傜偣杩涜int4b\_t鍒癷nt8\_t鐨勮浆鎹€?

    1.  鍦╒ector Core杩涜int4b\_t鍒癷nt8\_t鐨凜ast杞崲锛岃浆鎹㈠悗鐨勬暟鎹繚瀛樺埌鏂扮殑GM绌洪棿涓€?

        ```
        __aicore__ inline void Unzip(AscendC::GlobalTensor<int8_t>& dstGlobalTensor,
            AscendC::GlobalTensor<int8_t>& srcGlobalTensor, uint32_t count)
        {
            constexpr uint32_t oneBlockBytes = 32;
            uint32_t srcAddr = 0;
            uint32_t tmpAddr = srcAddr + AscendC::AlignUp(count * sizeof(int8_t), oneBlockBytes);
            uint32_t dstAddr = tmpAddr + AscendC::AlignUp(count * 2 * sizeof(half), oneBlockBytes);

            AscendC::LocalTensor<int8_t> srcLocalTensor(AscendC::TPosition::VECCALC, srcAddr, count);
            AscendC::LocalTensor<half> tmpTensor(AscendC::TPosition::VECCALC, tmpAddr, count * 2);
            AscendC::LocalTensor<int8_t> dstLocalTensor(AscendC::TPosition::VECCALC, dstAddr, count * 2);

            AscendC::DataCopy(srcLocalTensor, srcGlobalTensor, count);
            AscendC::SetFlag<AscendC::HardEvent::MTE2_V>(EVENT_ID0);
            AscendC::WaitFlag<AscendC::HardEvent::MTE2_V>(EVENT_ID0);

            AscendC::LocalTensor<AscendC::int4b_t> int4SrcLocalTensor =
                srcLocalTensor.ReinterpretCast<AscendC::int4b_t>();
            AscendC::Cast<half, AscendC::int4b_t>(tmpTensor, int4SrcLocalTensor, AscendC::RoundMode::CAST_NONE, count * 2);
            AscendC::PipeBarrier<PIPE_V>();
            AscendC::Cast<int8_t, half>(dstLocalTensor, tmpTensor, AscendC::RoundMode::CAST_CEIL, count * 2);

            AscendC::SetFlag<AscendC::HardEvent::V_MTE3>(EVENT_ID0);
            AscendC::WaitFlag<AscendC::HardEvent::V_MTE3>(EVENT_ID0);
            AscendC::DataCopy(dstGlobalTensor, dstLocalTensor, count * 2);
        }
        ```

    2.  杩涜int8\_t鏁版嵁绫诲瀷鐨勭煩闃佃绠椼€?

        ```
        __aicore__ inline void RunMatmul()
        {
            ...
            int offsetA = 0;
            int offsetB = 0;
            int offsetC = 0;
            ...
            auto gmA = aGlobal[offsetA];
            auto gmB = bGlobal[offsetB];
            auto gmC = cGlobal[offsetC];

            AscendC::MatmulImpl<A_TYPE, B_TYPE, C_TYPE, BIAS_TYPE, CFG_MDL> mm;
            mm.SetSubBlockIdx(0);
            mm.Init(&tiling, pipe);
            mm.SetTensorA(gmA, false);
            mm.SetTensorB(gmB, false);
            mm.IterateAll(gmC);
        }
        ```

-   **L0A Buffer鍒嗗舰鏀瑰彉锛屼粠ZZ杞崲涓篫N鏍煎紡銆?*

    **璇存槑**锛氭秹鍙婄殑API鏈塋oadData銆丮mad鍜孡oadDataWithTranspose銆?

    -   2201鏋舵瀯鐗堟湰锛屽弬涓庣煩闃典箻璁＄畻锛圓 \* B = C锛夋椂锛?ABC鐭╅樀鐨勬暟鎹帓甯冩牸寮忓垎鍒负Zz锛孼n锛孨z銆侫銆丅銆丆鐭╅樀鍒嗗埆浣嶄簬L0A Buffer銆丩0B Buffer銆丩0C Buffer銆?

        鐭╅樀A锛氭瘡涓垎褰㈢煩闃靛唴閮ㄦ槸琛屼富搴忥紝鍒嗗舰鐭╅樀涔嬮棿鏄涓诲簭銆傚垎褰hape涓?6 x \(32B/sizeof\(AType\)\)锛屽ぇ灏忎负512Byte銆?

        鐭╅樀B锛氭瘡涓垎褰㈢煩闃靛唴閮ㄦ槸鍒椾富搴忥紝鍒嗗舰鐭╅樀涔嬮棿鏄涓诲簭銆傚垎褰hape涓?\(32B/sizeof\(BType\)\) x 16锛屽ぇ灏忎负512Byte銆?

        鐭╅樀C锛氭瘡涓垎褰㈢煩闃靛唴閮ㄦ槸琛屼富搴忥紝鍒嗗舰鐭╅樀涔嬮棿鏄垪涓诲簭銆傚垎褰hape涓?6 x 16锛屽ぇ灏忎负256涓厓绱犮€?

        ![](../../../figures/2_zn.png)

    -   3510鏋舵瀯鐗堟湰锛屽弬涓庣煩闃典箻璁＄畻锛圓 \* B = C锛夋椂锛?ABC鐭╅樀鐨勬暟鎹帓甯冩牸寮忓垎鍒负Nz锛孼n锛孨z銆?

        鐭╅樀A锛氭瘡涓垎褰㈢煩闃靛唴閮ㄦ槸琛屼富搴忥紝鍒嗗舰鐭╅樀涔嬮棿鏄垪涓诲簭銆傚叾Shape涓?6 x \(32B/sizeof\(AType\)\)锛屽ぇ灏忎负512Byte銆?

        鐭╅樀B锛氭瘡涓垎褰㈢煩闃靛唴閮ㄦ槸鍒椾富搴忥紝鍒嗗舰鐭╅樀涔嬮棿鏄涓诲簭銆傚叾Shape涓?\(32B/sizeof\(BType\)\) x 16锛屽ぇ灏忎负512Byte銆?

        鐭╅樀C锛氭瘡涓垎褰㈢煩闃靛唴閮ㄦ槸琛屼富搴忥紝鍒嗗舰鐭╅樀涔嬮棿鏄垪涓诲簭銆傚叾Shape涓?6 x 16锛屽ぇ灏忎负256涓厓绱犮€?

        ![](../../../figures/3_zn.png)

    **鍏煎鏂规**锛氶潪L0A Buffer鍒囧垎鐨勫満鏅吋瀹?201鐗堟湰锛孡0A Buffer鍒囧垎鐨勫満鏅渶瑕佹牴鎹柊鐨勫垎褰㈤噸鏂伴€傞厤銆傚叿浣撲唬鐮佸彲鍙傝€僛pattern_transformation鍏煎鎬ф牱渚媇(https://gitcode.com/cann/asc-devkit/tree/c6dc78cfbccfb1a8f910ed4b6845bfacde5d2d9f/examples/01_simd_cpp_api/06_compatibility_guide/pattern_transformation)銆?

    鍦?201鏋舵瀯涓紝鐭╅樀璁＄畻瑕佹眰宸︾煩闃典负ZZ鍒嗗舰锛?510涓负NZ锛夛紝鍙崇煩闃典负ZN鍒嗗舰锛岀敱浜嶭1 Buffer鐨勬暟鎹垎褰负NZ锛屾墍浠?201鏋舵瀯涓嬪皢宸︾煩闃典粠L1 Buffer鎼繍鍒癓0A Buffer闇€瑕侀澶栧仛NZ鍒嗗舰鍒癦Z鍒嗗舰鐨勮浆鎹紝3510鏋舵瀯涓嬪垯涓嶇敤杞崲鍒嗗舰銆?

    ![](../../../figures/4_zn.png)

    鍒嗗舰鍙樺寲甯︽潵鐨勫彉鍔ㄤ富瑕佷綋鐜板湪L1 Buffer鍒癓0A Buffer鐨勬惉杩愯繃绋嬶紝浠ヤ笅浠ｇ爜鐗囨杩涜灞曠ず锛?

    ```
        __aicore__ inline void DataLoadA(AscendC::LocalTensor<T> a1, AscendC::LocalTensor<T> a2)
        {
    #if defined(__NPU_ARCH__) && __NPU_ARCH__ == 2201
            constexpr uint32_t mBlocks = M / CUBE_BLOCK;
            constexpr uint32_t kBlocks = K * sizeof(T) / C0_SIZE;
            int srcOffset = 0;
            int dstOffset = 0;
            for (uint32_t i = 0; i < mBlocks; ++i) {
                AscendC::LoadData2DParams loadDataParams;
                loadDataParams.repeatTimes = kBlocks;
                loadDataParams.srcStride = mBlocks;
                loadDataParams.ifTranspose = false;
                AscendC::LoadData(a2[dstOffset], a1[srcOffset], loadDataParams);
                srcOffset += CUBE_BLOCK * CUBE_BLOCK;
                dstOffset += K * CUBE_BLOCK;
            }
    #elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
            AscendC::LoadData2DParamsV2 loadDataParams;
            loadDataParams.mStartPosition = 0;
            loadDataParams.kStartPosition = 0;
            loadDataParams.mStep = AscendC::DivCeil(M, CUBE_BLOCK);
            loadDataParams.kStep = AscendC::DivCeil(K * sizeof(T), C0_SIZE);
            loadDataParams.srcStride = AscendC::DivCeil(M, CUBE_BLOCK);
            loadDataParams.dstStride = AscendC::DivCeil(M, CUBE_BLOCK);
            loadDataParams.sid = 0;
            loadDataParams.ifTranspose = false;
            AscendC::LoadData(a2, a1, loadDataParams);
    #endif
        }
    ```

-   **3510鏋舵瀯鐗堟湰纭欢鏋舵瀯鍒犻櫎4锛?缁撴瀯鍖栫█鐤忓姛鑳姐€?*<a name="li69092585134"></a>

    **璇存槑**锛歀oadDataWithSparse鐢ㄤ簬灏嗗瓨鍌ㄥ湪L1 Buffer涓殑512B绋犲瘑鏉冮噸鐭╅樀鎼繍鍒癓0B buffer锛屽苟鍚屾椂璇诲彇128B鐨勭储寮曠煩闃典互瀹炵幇绋犲瘑鐭╅樀鐨勭█鐤忓寲銆傜敱浜?510鏋舵瀯鐗堟湰涓嶆敮鎸佺粨鏋勫寲绋€鐤忓姛鑳斤紝鍥犳LoadDataWithSparse鍦ㄦ鐗堟湰涓苟涓嶉€傜敤銆傚彟涓€鏂归潰锛孧madWithSparse璐熻矗鎵ц鐭╅樀涔樺姞鎿嶄綔锛屽叾涓彸鐭╅樀B涓虹瀵嗙煩闃碉紝闇€瑕侀€氳繃璋冪敤LoadDataWithSparse杩涜杞藉叆銆傜敱浜?510鏋舵瀯涓嶆敮鎸丩oadDataWithSparse锛屽洜姝madWithSparse涔熸棤娉曞湪3510鏋舵瀯鐗堟湰涓娇鐢ㄣ€?

    **鍏煎鏂规**锛氬湪绠楀瓙渚у彲浠ヤ笉璋冪敤LoadDatawithSparse杩涜鐭╅樀绋犲瘑杞█鐤忔搷浣滐紝鐒跺悗浣跨敤Mmad杩涜姝ｅ父鐨勭瀵嗙煩闃佃绠椼€傜█鐤忕煩闃电浉鍏崇畻娉曞彲鍙傝€僊madWithSparse涓殑浠嬬粛銆?

-   **3510鏋舵瀯鐗堟湰鍒犻櫎GM-\>L0A Buffer/L0B Buffer閫氳矾**

    **璇存槑**锛氱‖浠跺垹闄M-\>L0A Buffer/L0B Buffer閫氳矾锛岃皟鐢↙oadData鏃讹紝涓嶅啀鏀寔杩欎簺閫氳矾銆?

    **鍏煎鏂规**锛氬疄鐜癎M-\>L0A Buffer/L0B Buffer鎼繍闇€鎷嗗垎鎴愪袱姝ヨ繘琛岋紝鍏堜粠GM鎼繍鍒癓1 Buffer锛屽啀浠嶭1 Buffer鎼繍鍒癓0A Buffer銆丩0B Buffer銆?

     浠M-\>L1 Buffer-\>L0A Buffer閫氳矾涓轰緥鍙互鍙傝€冧互涓嬫楠わ細

    1.  灏嗙煩闃礎浠嶨M鎼繍鍒癓1 Buffer銆?

        ```
        __aicore__ inline void CopyGmToA1(AscendC::LocalTensor<T>& leftMatrix)
        {
            AscendC::Nd2NzParams intriParams1{1, 64, 128, 0, 128, 64, 1, 0};
            AscendC::DataCopy(leftMatrix, aGlobal, intriParams1);
        }
        ```

    2.  灏嗙煩闃礎浠嶭1 Buffer鎼繍鍒癓0A Buffer銆?

        ```
        __aicore__ inline void Load2DA1ToL0A(AscendC::LocalTensor<T>& a1, AscendC::LocalTensor<T>& a2)
        {
            AscendC::LoadData2DParamsV2 loadDataParams;
            ...
            AscendC::LoadData(a2, a1, loadDataParams);
        }
        ```

-   **3510鏋舵瀯鐗堟湰鍒犻櫎L0A Buffer/L0B Buffer鍒濆鍖栫殑鐩稿叧纭欢鎸囦护銆?*

    **璇存槑**锛欶ill鎺ュ彛灏嗙壒瀹氬瓨鍌ㄤ綅缃殑LocalTensor鍒濆鍖栦负鏌愪竴鍏蜂綋鏁板€硷紝涓嶆敮鎸佺洿鎺ュ垵濮嬪寲L0A Buffer銆丩0B Buffer銆?

    **鍏煎鏂规**锛氬厛閫氳繃Fill鎺ュ彛鍒濆鍖朙1 Buffer锛屽啀閫氳繃LoadData鎺ュ彛灏哃1 Buffer涓婄殑鏁版嵁鎼繍鍒癓0A Buffer銆丩0B Buffer銆傚叿浣撲唬鐮佸彲鍙傝€僛Fill鍏煎鎬ф牱渚媇(https://gitcode.com/cann/asc-devkit/tree/master/examples/01_simd_cpp_api/06_compatibility_guide/fill)銆?

    浠M-\>L1 Buffer-\>L0A Buffer鐨勬暟鎹€氳矾涓轰緥锛?

    1.  鍒濆鍖朙1 Buffer銆?

        ```
        __aicore__ inline void InitConstA1(AscendC::LocalTensor<T>& a1Local)
        {
            AscendC::Fill(a1Local, {1, static_cast<uint16_t>(M * K * sizeof(T) / 32), 0, 1});
        }
        ```

    2.  璋冪敤LoadData鎺ュ彛灏哃1 Buffer涓婄殑鏁版嵁鎼繍鍒癓0A Buffer銆?

        ```
        __aicore__ inline void Load2DA1ToA2(AscendC::LocalTensor<T>& a1Local, AscendC::LocalTensor<T>& a2Local)
        {
            AscendC::LoadData2DParamsV2 loadDataParams;
            ...
            AscendC::LoadData(a2Local, a1Local, loadDataParams);
        }
        ```

## 绯荤粺鍙橀噺璁块棶<a name="section34615332215"></a>

-   **涓嶆敮鎸丆heckLocalMemoryIA锛?510鏋舵瀯鐗堟湰鐩稿叧瀵勫瓨鍣ㄥ垹闄ゃ€?*

    **璇存槑**锛欳heckLocalMemoryIA鐩戣璁惧畾鑼冨洿鍐呯殑UB璇诲啓琛屼负锛屽鏋滅洃瑙嗗埌鏈夎瀹氳寖鍥寸殑璇诲啓琛屼负鍒欎細鍑虹幇EXCEPTION鎶ラ敊锛屾湭鐩戣鍒拌瀹氳寖鍥寸殑璇诲啓琛屼负鍒欎笉浼氭姤閿欍€?

    **鍏煎鏂规**锛氳鎺ュ彛涓鸿皟娴嬫帴鍙ｏ紝瀵瑰姛鑳芥棤褰卞搷銆?

## 璋冩祴API<a name="section12611957102116"></a>

-   **L1 Buffer涓婁笉鏀寔Tensor淇℃伅鐨勬墦鍗般€?*

    **璇存槑**锛氬洜鑺墖鍒犻櫎L1 Buffer-\>GM閫氳矾锛屼笉鏀寔L1 Buffer-\>GM鐨勫姛鑳姐€?

    **鍏煎鏂规**锛氳鎺ュ彛涓鸿皟娴嬫帴鍙ｏ紝瀵瑰姛鑳芥棤褰卞搷銆?
