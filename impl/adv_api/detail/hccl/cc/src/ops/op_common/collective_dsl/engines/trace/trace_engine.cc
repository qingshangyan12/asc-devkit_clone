// Copyright (c) 2026 Huawei Technologies Co., Ltd.
// Licensed under the CANN Open Software License Agreement Version 2.0.
#include "trace_engine.h"
#include <istream>
#include <ostream>
#include <sstream>
namespace mc2_ops_hccl {
namespace dsl {
namespace {
const char* Name(Primitive p)
{
    return p == Primitive::PLACE_INPUT ? "place_input" :
                                         (p == Primitive::MESH_ALL_GATHER ? "mesh_all_gather" : "nhr_all_gather");
}
template <class T>
void List(const std::vector<T>& v, std::ostream& o)
{
    for (size_t i = 0; i < v.size(); ++i) {
        if (i)
            o << ',';
        o << v[i];
    }
}
} // namespace
void WriteTrace(const Plan& plan, std::ostream& out)
{
    for (const Stage& s : plan.stages) {
        out << "rank=" << plan.rank << " stage=" << s.id << " q=" << s.queue << " primitive=" << Name(s.primitive)
            << " bytes=" << s.bytes.begin << ':' << s.bytes.end << " ranks=";
        List(s.ranks, out);
        out << " depends=";
        List(s.dependsOn, out);
        out << '\n';
    }
}
bool CompareTrace(std::istream& expected, std::istream& actual, std::string& difference)
{
    std::ostringstream a, b;
    a << expected.rdbuf();
    b << actual.rdbuf();
    if (a.str() == b.str()) {
        difference.clear();
        return true;
    }
    difference = "trace differs";
    return false;
}
} // namespace dsl
} // namespace mc2_ops_hccl
