#include "int64_div.hpp"
#include <stdexcept>

namespace {

struct u64 { uint32_t lo, hi; };

// --- constructors & queries ---
inline u64 u64_of(uint32_t l, uint32_t h)    { u64 r; r.lo = l; r.hi = h; return r; }
inline bool u64_zero(u64 a)                  { return (a.lo | a.hi) == 0; }
inline bool u64_eq(u64 a, u64 b)             { return a.lo == b.lo && a.hi == b.hi; }
inline bool u64_ge(u64 a, u64 b)             { return (a.hi != b.hi) ? (a.hi > b.hi) : (a.lo >= b.lo); }
inline bool u64_gt(u64 a, u64 b)             { return (a.hi != b.hi) ? (a.hi > b.hi) : (a.lo > b.lo); }

// --- arithmetic ---
inline u64 u64_add_raw(u64 a, u64 b) {
    uint32_t lo = a.lo + b.lo;
    uint32_t hi = a.hi + b.hi + (lo < a.lo ? 1u : 0u);
    return {lo, hi};
}

inline u64 u64_sub(u64 a, u64 b) {
    uint32_t lo = a.lo - b.lo;
    uint32_t borrow = (a.lo < b.lo) ? 1u : 0u;
    uint32_t hi = a.hi - b.hi - borrow;
    return {lo, hi};
}

// 32x32 → 64
inline u64 mul32_64(uint32_t a, uint32_t b) {
    uint64_t p = static_cast<uint64_t>(a) * b;
    return {static_cast<uint32_t>(p), static_cast<uint32_t>(p >> 32)};
}

// 64x64 → 128: lo = bits[0:63], hi = bits[64:127]
inline void mul64_128(u64 a, u64 b, u64* lo, u64* hi) {
    u64 p00 = mul32_64(a.lo, b.lo);
    u64 p01 = mul32_64(a.lo, b.hi);
    u64 p10 = mul32_64(a.hi, b.lo);
    u64 p11 = mul32_64(a.hi, b.hi);

    uint32_t c = 0, t;

    { uint64_t s = static_cast<uint64_t>(p00.hi) + p01.lo;          t = static_cast<uint32_t>(s); c = static_cast<uint32_t>(s >> 32); }
    { uint64_t s = static_cast<uint64_t>(t)       + p10.lo;          t = static_cast<uint32_t>(s); c +=static_cast<uint32_t>(s >> 32); }

    lo->lo = p00.lo;  lo->hi = t;

    { uint64_t s = static_cast<uint64_t>(p01.hi)  + p10.hi + c;     t = static_cast<uint32_t>(s); c = static_cast<uint32_t>(s >> 32); }
    { uint64_t s = static_cast<uint64_t>(t)       + p11.lo;          t = static_cast<uint32_t>(s); c +=static_cast<uint32_t>(s >> 32); }

    hi->lo = t;  hi->hi = p11.hi + c;
}

// --- float helpers ---
constexpr float F32_2POW32 = 4294967296.0f;

inline float u64_to_float(u64 x) {
    return static_cast<float>(x.hi) * F32_2POW32 + static_cast<float>(x.lo);
}

// f must be ≥ 0 and < 2^64; converts f → floor(f) as u64
inline u64 f32_floor_to_u64(float f) {
    float hf = f * (1.0f / F32_2POW32);   // f / 2^32
    uint32_t hi = static_cast<uint32_t>(hf);
    float rem = f - static_cast<float>(hi) * F32_2POW32;
    uint32_t lo = static_cast<uint32_t>(rem);
    return {lo, hi};
}

// --- absolute value ---
inline u64 abs64(int64_t x) {
    uint32_t lo = static_cast<uint32_t>(static_cast<uint64_t>(x));
    uint32_t hi = static_cast<uint32_t>(static_cast<uint64_t>(x) >> 32);
    if ((hi & 0x80000000u) == 0) return {lo, hi};

    uint32_t nl = ~lo, nh = ~hi;
    uint32_t rl = nl + 1u;
    uint32_t carry = (rl < nl) ? 1u : 0u;
    uint32_t rh = nh + carry;
    return {rl, rh};
}

// --- Newton-Raphson reciprocal: compute R = floor(2^64 / D) ---

// initial guess via float32: R0 = floor( (1/D_f32) * 2^64 )
inline u64 reciprocal_init(u64 d) {
    float fd  = u64_to_float(d);
    float fr  = 1.0f / fd;                 // ≈ 1/D
    float fs  = fr * F32_2POW32;           // = fr * 2^32 (hi part of fixed-point)
    uint32_t hi = static_cast<uint32_t>(fs);
    float rem   = fs - static_cast<float>(hi);
    uint32_t lo = static_cast<uint32_t>(rem * F32_2POW32);
    u64 X = {lo, hi};
    return u64_zero(X) ? u64_of(1, 0) : X;
}

// One Newton iteration:
//   P = D * X   (128-bit)
//   if P < 2^64:  X += (X * (2^64 - P_lo)) >> 64
//   if P >= 2^64: X -= (X * P_lo) >> 64    [when P_hi == 1]
// Assumes X is close enough that P_hi ∈ {0, 1}.
inline u64 reciprocal_newton(u64 d, u64 X) {
    u64 Pl, Ph;
    mul64_128(d, X, &Pl, &Ph);

    u64 zero = {0, 0};
    u64 CorrL, CorrH;

    if (u64_eq(Ph, zero)) {
        // P < 2^64: X too small. Err = 2^64 - Pl = (~Pl) + 1
        u64 delta = {~Pl.lo, ~Pl.hi};
        if (delta.lo != 0xFFFFFFFFu) { delta.lo++; } else { delta.lo = 0; delta.hi++; }
        mul64_128(X, delta, &CorrL, &CorrH);
        // correction = CorrH + (CorrL != 0)
        u64 corr = CorrH;
        if (!u64_zero(CorrL)) corr = u64_add_raw(corr, u64_of(1, 0));
        return u64_add_raw(X, corr);
    }

    // Ph == 1: P ∈ [2^64, 2^65-1], X too large (or exact when Pl==0)
    if (u64_zero(Pl)) return X;

    mul64_128(X, Pl, &CorrL, &CorrH);
    u64 corr = CorrH;
    if (!u64_zero(CorrL)) corr = u64_add_raw(corr, u64_of(1, 0));
    return (u64_ge(X, corr)) ? u64_sub(X, corr) : u64_of(0, 0);
}

// --- unsigned division ---
inline u64 udiv64(u64 a, u64 b) {
    if (!u64_ge(a, b)) return u64_of(0, 0);

    u64 X = reciprocal_init(b);
    X = reciprocal_newton(b, X);
    X = reciprocal_newton(b, X);

    // Q = floor(a * X / 2^64) = upper 64 bits of a*X
    u64 Ql, Qh;
    mul64_128(a, X, &Ql, &Qh);
    u64 Q = Qh;

    // remainder correction
    u64 Pl, Ph;
    mul64_128(Q, b, &Pl, &Ph);

    if (!u64_ge(a, Pl)) {
        // Q overshoot, decrement it
        Q = u64_sub(Q, u64_of(1, 0));
        mul64_128(Q, b, &Pl, &Ph);
    }

    u64 R = u64_sub(a, Pl);
    while (u64_ge(R, b)) {
        Q = u64_add_raw(Q, u64_of(1, 0));
        R = u64_sub(R, b);
    }
    return Q;
}

} // anonymous namespace

// === public API ===

int64_t int64_div(int64_t dividend, int64_t divisor) {
    if (dividend == 0 && divisor == 0) {
        throw std::domain_error("int64_div: 0/0 undefined");
    }
    if (divisor == 0) {
        return (dividend > 0) ? INT64_MAX : INT64_MIN;
    }
    if (dividend == 0) return 0;

    if (divisor == 1)  return dividend;
    if (divisor == -1) return (dividend == INT64_MIN) ? INT64_MAX : static_cast<int64_t>(0) - dividend;

    bool neg = (dividend < 0) ^ (divisor < 0);
    u64 a = abs64(dividend);
    u64 b = abs64(divisor);

    u64 two63 = u64_of(0, 0x80000000u);
    u64 q;
    if (u64_eq(b, two63)) {
        q = u64_ge(a, b) ? u64_of(1, 0) : u64_of(0, 0);
    } else {
        q = udiv64(a, b);
    }

    uint64_t qv = (static_cast<uint64_t>(q.hi) << 32) | q.lo;
    if (qv > static_cast<uint64_t>(INT64_MAX))
        return neg ? INT64_MIN : INT64_MAX;

    int64_t r = static_cast<int64_t>(qv);
    return neg ? -r : r;
}
