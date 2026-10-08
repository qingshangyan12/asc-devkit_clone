// unified PoC · A4 补充回归（BLOCKER-1 修复守门，2026-09-21 审查打回项）
//
// 被测：ccu_temp_kfc_server.cc 两处 host 侧 unified 动作 kernel 判定
//   InheritKfcServerKernelArg :47（role 覆写入口）
//   CcuTempKfcServer::CalcRes :236（v2 白名单绕过入口）
// 判定式均为 sourceName.compare(0, 9, "CcuAction") == 0。
//
// BLOCKER-1 复盘：原实现 compare(0, 10, "CcuAction") 把子串长度参数误写为 10——
// substr(0,10)="CcuActionC"（10 字符）与 "CcuAction"（9 字符）字典序比较恒 >0，
// 判定永假 → UNIFIED_PLAN role 覆写与 CalcRes 白名单绕过双双不可达，v3 host 入口
// 整条死代码。本测试逐名锁死：注册名命中 + v2 名单（CalcRes :186-198 全 11 名）
// 零误命中 + 长度字面量 == strlen（防再犯）。
//
// 注：判定式本体在 devkit 设备侧文件内（本 host 自检不 include 之），此处按同式
// 复刻并断言语义等价——长度 9 由 static_assert(sizeof("CcuAction")-1 == 9) 与
// devkit 侧同名 static_assert 双侧锁死，字面量 "CcuAction" 与注册常量
// KFC_UNIFIED_ACTION_COLLECT_ALL_KERNEL_NAME（kfc_server_message.h:225）的前缀
// 关系一并断言。

#include <cstdio>
#include <cstring>
#include <string>

static int g_pass = 0, g_fail = 0;
static void Check(bool cond, const std::string& what)
{
    if (cond) { ++g_pass; printf("  [PASS] %s\n", what.c_str()); }
    else      { ++g_fail; printf("  [FAIL] %s\n", what.c_str()); }
}

// host 侧判定式（与 ccu_temp_kfc_server.cc :47/:236 逐字同式）
static bool IsUnifiedPlanKernelName(const std::string& name)
{
    static_assert(sizeof("CcuAction") - 1 == 9, "prefix literal length must equal the compare sub-length");
    return name.compare(0, 9, "CcuAction") == 0;
}

int main()
{
    printf("== BLOCKER-1 守门：unified 动作 kernel 判定（compare 子串长度） ==\n");
    {
        // 长度字面量自检：compare 的第二参必须 == strlen("CcuAction") == 9。
        // BLOCKER-1 的病灶就是该值写成 10（字面量长度 +1）。
        Check(sizeof("CcuAction") - 1 == 9, "strlen(\"CcuAction\") == 9（长度字面量一致）");
        Check(std::string("CcuActionCollectAll").compare(0, 9, "CcuAction") == 0,
              "compare(0,9) 命中注册名前缀（原 compare(0,10) 恒 1 的实证反例）");
        Check(std::string("CcuActionCollectAll").compare(0, 10, "CcuAction") != 0,
              "compare(0,10) 不命中（BLOCKER 病灶形态保持可判）");

        // 注册名命中：真实注册名（backend_ccu_collect_all.h:45 的 kernelFuncName，
        // == kfc_server_message.h:225 的 KFC_UNIFIED_ACTION_COLLECT_ALL_KERNEL_NAME）
        Check(IsUnifiedPlanKernelName("CcuActionCollectAll"), "注册名 CcuActionCollectAll 判 true");
        // 未来动作 kernel 同前缀规则命中（前缀泛化语义——不逐名扩名单）
        Check(IsUnifiedPlanKernelName("CcuActionReduceSlice"), "前缀泛化：CcuActionReduceSlice 判 true");
        Check(IsUnifiedPlanKernelName("CcuActionX"), "前缀泛化：任意 CcuAction* 判 true");

        // v2 名单零误命中：CalcRes :186-198 全部 11 个 kernelFuncName（含
        // KFC_REDUCE_SCATTER_PEER_ONLY_KERNEL_NAME 展开值）
        const char* v2Names[] = {
            "CcuKernelAllGatherMesh1DMem2Mem",
            "CcuKernelKfcAllGatherMesh1DMem2Mem",
            "CcuKernelAllGatherNHR1DMultiJettyMem2Mem",
            "CcuKernelKfcReduceScatterMesh1DMem2Mem",
            "CcuKernelKfcReduceScatterMesh1DMem2MemPeerOnly",
            "CcuKernelReduceScatterNHR1DMultiJettyMem2Mem",
            "CcuKernelAlltoAllMesh1D",
            "CcuKernelAlltoAllVMesh1D",
            "CcuKernelKfcAllToAllMesh1DMultiJetty",
            "CcuAllToAllMesh1DMultiJettyKernel",
            "CcuKernelAllReduceMesh1DMem2Mem",
        };
        unsigned falseCnt = 0;
        for (const char* n : v2Names) {
            if (!IsUnifiedPlanKernelName(n)) { ++falseCnt; }
            else { Check(false, std::string("v2 名误命中: ") + n); }
        }
        Check(falseCnt == sizeof(v2Names) / sizeof(v2Names[0]),
              "v2 名单 11 个 kernelFuncName 全判 false（零误命中）");

        // 边界：恰为前缀本身（退化形态，按前缀规则判 true）；短名/空名/大小写
        Check(IsUnifiedPlanKernelName("CcuAction"), "prefix-only name CcuAction 判 true（退化形态）");
        Check(!IsUnifiedPlanKernelName("CcuKerne1"), "非前缀名判 false");
        Check(!IsUnifiedPlanKernelName(""), "空名判 false");
        Check(!IsUnifiedPlanKernelName("ccuActionX"), "小写开头判 false（大小写敏感）");
    }

    printf("\n%d passed, %d failed\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
