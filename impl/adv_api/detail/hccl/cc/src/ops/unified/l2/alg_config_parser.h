/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * CANN Open Software License Agreement Version 2.0.
 *
 * 重构 PoC · algConfig 解析器（方案 §6.8 语法与五条解析规则）
 *
 * 替代 devkit 现状 GetForcedAlgName（hccl_alloc_ctx_res.cc:660）：
 *   现状：algConfig 整串 == 某个 REGISTER_EXEC_V2 注册名，命中则强制走该算法；
 *         含 '=' 视为 legacy 直接放弃 → 回落 selector。96 个名字 ↔ 96 个字符串。
 *   重构：algConfig 是结构化语法 <recipe>[stage列表, 其余参数]，
 *         解析为 AlgConfig 值对象；未写的维度为 AUTO，非法组合 Validate 快速失败。
 *
 * 语法：
 *   <recipe>[stage列表, 其余参数]
 *   stage列表 := 项 (',' 项)*         有序 intra→inter→level2
 *   项        := pattern ('.' 限定词)*  点号链，每类限定词最多一个
 *   限定词    := 路径变体 hd | 段形态 two_shot|one_shot（互斥）
 *                | 切分 chunk（与段形态正交，可叠加）
 *                | 通道 multi-channel[:N]|single-channel
 *   其余参数  := transport裸值 | 装饰器 order
 */
#ifndef REFACTOR_POC_ALG_CONFIG_PARSER_H
#define REFACTOR_POC_ALG_CONFIG_PARSER_H

#include "action_spec.h"
#include <string>
#include <vector>

namespace mc2_ops_hccl::refactor {

enum class Recipe { SOLE, PARALLEL, SEQUENCE, SPLIT, PIPELINE };

struct StageConfig {
    CommPattern pattern{CommPattern::AUTO};
    SegShape shape{SegShape::NONE};          // 段形态：one_shot/two_shot
    bool chunk{false};                       // 段形态：chunk
    bool hd{false};                          // 路径变体：ring.hd
    u32 channelsPerRank{0};                  // 通道：0=AUTO，1=single-channel，N=multi-channel:N
    bool channelsExplicit{false};
};

struct AlgConfig {
    Recipe recipe{Recipe::SOLE};
    std::vector<StageConfig> stages;         // 项数 = 参与域数（无 domain 键）
    TransportPolicy transport{TransportPolicy::AUTO};
    bool order{false};                       // 装饰器（缺省关）
    bool valid{true};
    std::string err;
};

class AlgConfigParser {
public:
    static AlgConfig Parse(const std::string& s)
    {
        AlgConfig cfg;
        if (s.empty()) return cfg;                          // 空 = sole + 全 AUTO

        // 退役名检测：旧枚举名一律报错并提示新名（方案 §6.8）
        if (s.rfind("Ins", 0) == 0 || s.rfind("Ccu", 0) == 0 || s.rfind("Aicpu", 0) == 0) {
            return Fail(cfg, "retired algName '" + s + "', use <recipe>[stage,...] syntax");
        }

        const size_t lb = s.find('[');
        const std::string recipeStr = (lb == std::string::npos) ? s : s.substr(0, lb);
        if (!ParseRecipe(recipeStr, cfg.recipe)) return Fail(cfg, "unknown recipe '" + recipeStr + "'");
        if (lb == std::string::npos) return cfg;             // 只锁配方，其余 AUTO

        if (s.back() != ']') return Fail(cfg, "missing ']'");
        const std::string body = s.substr(lb + 1, s.size() - lb - 2);

        // 逗号切分：stage 项在前（连续的 pattern 项），其后为无序参数
        bool stageZone = true;
        for (const auto& tokRaw : Split(body, ',')) {
            const std::string tok = Trim(tokRaw);
            if (tok.empty()) continue;
            const std::string head = tok.substr(0, tok.find('.'));
            if (stageZone && IsPattern(head)) {
                StageConfig st;
                if (!ParseStage(tok, st, cfg.err)) return Fail(cfg, cfg.err);
                cfg.stages.push_back(st);
            } else {
                stageZone = false;                          // 进入无序参数区
                if (!ParseTail(tok, cfg)) return Fail(cfg, "unknown param '" + tok + "'");
            }
        }
        return cfg;
    }

private:
    static AlgConfig& Fail(AlgConfig& c, const std::string& e)
    {
        c.valid = false;
        c.err = e;
        return c;
    }

    static bool ParseRecipe(const std::string& s, Recipe& r)
    {
        if (s == "sole") { r = Recipe::SOLE; return true; }
        if (s == "parallel") { r = Recipe::PARALLEL; return true; }
        if (s == "sequence") { r = Recipe::SEQUENCE; return true; }
        if (s == "split") { r = Recipe::SPLIT; return true; }
        if (s == "pipeline") { r = Recipe::PIPELINE; return true; }
        return false;
    }

    static bool IsPattern(const std::string& s) { return s == "mesh" || s == "nhr" || s == "ring"; }

    // 点号链解析：pattern.限定词.限定词…，每类最多一个（同类重复报错）
    static bool ParseStage(const std::string& tok, StageConfig& st, std::string& err)
    {
        const auto parts = Split(tok, '.');
        if (parts.empty()) { err = "empty stage"; return false; }
        if (parts[0] == "mesh") st.pattern = CommPattern::MESH;
        else if (parts[0] == "nhr") st.pattern = CommPattern::NHR;
        else if (parts[0] == "ring") st.pattern = CommPattern::RING;
        else { err = "unknown pattern '" + parts[0] + "'"; return false; }

        bool hasPath = false, hasShape = false, hasChunk = false, hasChan = false;
        for (size_t i = 1; i < parts.size(); ++i) {
            const std::string q = parts[i];
            if (q == "hd") {                                        // 路径变体
                if (hasPath) { err = "duplicate path qualifier"; return false; }
                if (st.pattern != CommPattern::RING) { err = ".hd requires ring pattern"; return false; }
                st.hd = true; hasPath = true;
            } else if (q == "chunk") {                              // 切分（与段形态正交）
                if (hasChunk) { err = "duplicate chunk qualifier"; return false; }
                st.chunk = true; hasChunk = true;
            } else if (q == "two_shot" || q == "one_shot") {         // 段形态（互斥）
                if (hasShape) { err = "duplicate segment-shape qualifier"; return false; }
                st.shape = (q == "two_shot") ? SegShape::TWO_SHOT : SegShape::ONE_SHOT;
                hasShape = true;
            } else if (q == "single-channel" || q.rfind("multi-channel", 0) == 0) {  // 通道
                if (hasChan) { err = "duplicate channel qualifier"; return false; }
                st.channelsExplicit = true;
                if (q == "single-channel") {
                    st.channelsPerRank = 1;
                } else {
                    const size_t c = q.find(':');
                    st.channelsPerRank = (c == std::string::npos) ? 0   // 不带 N = AUTO 选档
                                                                  : static_cast<u32>(std::stoul(q.substr(c + 1)));
                }
                hasChan = true;
            } else {
                err = "unknown qualifier '" + q + "'";
                return false;
            }
        }
        return true;
    }

    static bool ParseTail(const std::string& tok, AlgConfig& cfg)
    {
        if (tok == "order") { cfg.order = true; return true; }
        if (tok == "mem2mem") { cfg.transport = TransportPolicy::MEM2MEM; return true; }
        if (tok == "ccubuf") { cfg.transport = TransportPolicy::VIA_CCUBUF; return true; }
        if (tok == "symmem") { cfg.transport = TransportPolicy::SYMMEM; return true; }
        return false;
    }

    static std::vector<std::string> Split(const std::string& s, char sep)
    {
        std::vector<std::string> out;
        size_t p = 0;
        while (true) {
            const size_t q = s.find(sep, p);
            out.push_back(s.substr(p, q == std::string::npos ? std::string::npos : q - p));
            if (q == std::string::npos) break;
            p = q + 1;
        }
        return out;
    }
    static std::string Trim(const std::string& s)
    {
        size_t b = s.find_first_not_of(" \t");
        size_t e = s.find_last_not_of(" \t");
        return (b == std::string::npos) ? "" : s.substr(b, e - b + 1);
    }
};

} // namespace mc2_ops_hccl::refactor

#endif
