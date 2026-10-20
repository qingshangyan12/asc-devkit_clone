#ifndef VECTOR_POINTER_PRECISION_METRICS_H
#define VECTOR_POINTER_PRECISION_METRICS_H
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdint>
#include <type_traits>

// Host-only validation. Fixed absolute caps are the conservative alternative
// to the standard's ULP option. Original stronger checks remain in each test.
struct VectorPrecisionMetrics {
    uint64_t elements = 0, matched = 0;
    double maxAbs = 0, atol = 0, rtol = 0, cap = 0;
    template<class T> void Observe(T actual, T golden) {
        if constexpr (std::is_integral<T>::value) { atol = rtol = cap = 0; }
        else if constexpr (sizeof(T) == 2) { atol = rtol = std::ldexp(1.0, -9); cap = 0.1; }
        else { atol = std::ldexp(1.0, -16); rtol = std::ldexp(1.0, -10); cap = 0.01; }
        const double a = double(actual), g = double(golden);
        ++elements;
        if (!std::isfinite(a) || !std::isfinite(g)) {
            if ((std::isnan(a) && std::isnan(g)) || a == g) { ++matched; }
            else { maxAbs = INFINITY; }
            return;
        }
        const double error = std::abs(a - g);
        maxAbs = std::max(maxAbs, error);
        matched += error <= atol + rtol * std::abs(g);
    }
    bool Report(const char* name, uint32_t count, int form) const {
        const double ratio = elements ? double(matched)/elements : 0;
        const bool pass = elements && ratio >= 0.99 && maxAbs <= cap;
        std::printf("PRECISION api=%s count=%u form=%d elements=%llu matched_ratio=%.17g max_abs_error=%.17g atol=%.17g rtol=%.17g max_abs_error_limit=%.17g status=%s\n",
            name,count,form,(unsigned long long)elements,ratio,maxAbs,atol,rtol,cap,pass?"PASS":"FAIL");
        return pass;
    }
};
#endif
