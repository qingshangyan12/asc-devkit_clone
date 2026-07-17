#!/usr/bin/python3
# coding=utf-8
"""
Phase 1 host golden for prims_ndtri.

Two jobs:
  1. Build a sampling set shaped like the FUNCTION, not like the interval.
     The shipped self_test gen_data.py uses np.random.uniform(0,1) -> 95% of points land in
     Acklam's central branch and the deep tail (p<1e-6) is never sampled. See talk/ndtri.md F9.
  2. Validate the EXACT branchless fp32 algorithm we intend to emit on the NPU (3510 Reg:: ops)
     against scipy.special.ndtri, under the 双万分之一 rule.

fp32 fidelity notes:
  - numpy has no FMA; the NPU Horner uses Reg::FusedMulDstAdd (single rounding per step).
    So numpy fp32 here is a CONSERVATIVE (pessimistic) bound on NPU accuracy.
  - Select is modelled with np.where = lane pick, exactly like Reg::Select (NOT an arithmetic blend).
"""

import numpy as np
from scipy.special import ndtri as scipy_ndtri

F32 = np.float32

# ---------------------------------------------------------------- coefficients
# TAIL: Acklam C/D. Measured fp32 eval error 3.7e-7 -> well-conditioned, keep as-is.
C = [-7.784894002430293e-03, -3.223964580411365e-01, -2.400758277161838e+00,
     -2.549732539343734e+00, 4.374664141464968e+00, 2.938163982698783e+00]
D = [7.784695709041462e-03, 3.224671290700398e-01, 2.445134137142996e+00,
     3.754408661907416e+00]

# CENTRAL: Cephes (scipy IS Cephes). Acklam's A(r)q/B(r) suffers catastrophic cancellation in fp32
# (Horner peaks ~276, collapses to ~0.01 => 2e4x amplification => 1.5e-4 err => FAILS 双万分之一).
# Cephes factors out the leading term: z = s2pi*q*(1 + r*P0(r)/Q0(r)) -> rational is a bounded
# CORRECTION in [0, 0.655], no cancellation. Measured fp32 err 4.7e-5 -> PASS.
S2PI = 2.50662827463100050242
P0 = [-5.99633501014107895267E1, 9.80010754185999661536E1, -5.66762857469070293439E1,
      1.39312609387279679503E1, -1.23916583867381258016E0]
Q0 = [1.95448858338141759834E0, 4.67627912898881538453E0, 8.63602421390890590575E1,
      -2.25462687854119370527E2, 2.00260212380060660359E2, -8.20372256168333339912E1,
      1.59056225126211695515E1, -1.18331621121330003142E0]

P_LOW = F32(0.02425)
P_HIGH = F32(1.0 - 0.02425)


def _horner(coeffs, x):
    """Horner in fp32. Mirrors the Reg::FusedMulDstAdd chain (minus FMA, so slightly worse)."""
    acc = F32(coeffs[0])
    for c in coeffs[1:]:
        acc = (acc * x + F32(c)).astype(F32)
    return acc


def _p1evl(coeffs, x):
    """Monic Horner (leading coefficient 1 implied)."""
    acc = (x + F32(coeffs[0])).astype(F32)
    for c in coeffs[1:]:
        acc = (acc * x + F32(c)).astype(F32)
    return acc


def ndtri_fp32(p):
    """Branchless hybrid (Cephes central + Acklam tail) in fp32 — what we emit on 3510."""
    p = p.astype(F32)
    with np.errstate(divide='ignore', invalid='ignore', over='ignore'):
        # ---- tail form, evaluated on EVERY lane (incl. central/out-of-domain).
        # Safe only because the combine below is a lane pick, never an arithmetic blend.
        # Sterbenz: for p in [0.5,1], (1-p) is EXACT in fp32 -> no cancellation.
        t = np.minimum(p, (F32(1.0) - p).astype(F32)).astype(F32)
        q_t = np.sqrt((F32(-2.0) * np.log(t)).astype(F32)).astype(F32)
        num_c = _horner(C, q_t)
        den_d = (_horner(D, q_t) * q_t + F32(1.0)).astype(F32)
        x_tail = (num_c / den_d).astype(F32)
        x_tail = np.where(p > F32(0.5), -x_tail, x_tail).astype(F32)

        # ---- central form (Cephes factored)
        q_c = (p - F32(0.5)).astype(F32)          # exact for p in [0.25,1] (Sterbenz)
        r = (q_c * q_c).astype(F32)
        corr = ((r * _horner(P0, r)).astype(F32) / _p1evl(Q0, r)).astype(F32)
        x_cent = ((q_c + (q_c * corr).astype(F32)).astype(F32) * F32(S2PI)).astype(F32)

        # ---- combine: lane pick (Reg::Select semantics)
        in_central = (p >= P_LOW) & (p <= P_HIGH)
        x = np.where(in_central, x_cent, x_tail).astype(F32)

        # ---- boundary patch
        x = np.where(p == F32(0.0), F32(-np.inf), x).astype(F32)
        x = np.where(p == F32(1.0), F32(np.inf), x).astype(F32)
        x = np.where((p < F32(0.0)) | (p > F32(1.0)), F32(np.nan), x).astype(F32)
        x = np.where(np.isnan(p), F32(np.nan), x).astype(F32)
    return x


# ---------------------------------------------------------------- sampling
def make_samples():
    """Sample the way the function is shaped. Returns {region_name: p_array_fp32}."""
    s = {}
    # fp32 granularity near 1: ulp(1)=1.19e-7, so the smallest representable (1-p) is 2^-24=5.96e-8.
    # => upper tail saturates at z~5.4 while lower tail reaches z~-14. The domain is ASYMMETRIC in fp32.
    s['deep_lower']  = np.logspace(-37, -6, 4000).astype(F32)
    s['tail_lower']  = np.logspace(-6, np.log10(0.02425), 4000).astype(F32)
    s['central']     = np.linspace(0.02425, 1 - 0.02425, 8000).astype(F32)
    s['tail_upper']  = (F32(1.0) - np.logspace(np.log10(0.02425), -7.2, 4000).astype(F32)).astype(F32)
    # breakpoint seams: where the Select mask flips. Branch-seam bugs live exactly here.
    for bp, nm in ((0.02425, 'seam_low'), (1 - 0.02425, 'seam_high')):
        d = np.concatenate([-np.logspace(-7, -3, 500), [0.0], np.logspace(-7, -3, 500)])
        s[nm] = np.clip(F32(bp) + d.astype(F32), 0, 1).astype(F32)
    s['uniform']     = np.random.default_rng(0).uniform(0, 1, 10000).astype(F32)  # what harness ships
    return s


def rel_err(got, want):
    got = got.astype(np.float64)
    want = want.astype(np.float64)
    with np.errstate(divide='ignore', invalid='ignore'):
        e = np.abs(got - want) / np.abs(want)
    # golden==0 (p==0.5): exact-match -> 0 err, else count as failure
    e = np.where(want == 0.0, np.where(got == 0.0, 0.0, np.inf), e)
    return e


def main():
    print(f"{'region':<12} {'N':>6} {'max rel err':>12} {'#>1e-4':>7} {'frac':>10}  verdict")
    print("-" * 66)
    worst = 0.0
    all_fail = 0
    all_n = 0
    for name, p in make_samples().items():
        golden = scipy_ndtri(p.astype(np.float64)).astype(np.float64)
        got = ndtri_fp32(p)
        finite = np.isfinite(golden) & np.isfinite(got)
        e = rel_err(got[finite], golden[finite])
        n = e.size
        nfail = int((e > 1e-4).sum())
        frac = nfail / n if n else 0.0
        ok = "PASS" if frac <= 1e-4 else "FAIL"
        worst = max(worst, float(np.nanmax(e)) if n else 0.0)
        all_fail += nfail
        all_n += n
        print(f"{name:<12} {n:>6} {np.nanmax(e):>12.3e} {nfail:>7} {frac:>10.2e}  {ok}")

    print("-" * 66)
    tot_frac = all_fail / all_n
    print(f"{'TOTAL':<12} {all_n:>6} {worst:>12.3e} {all_fail:>7} {tot_frac:>10.2e}  "
          f"{'PASS' if tot_frac <= 1e-4 else 'FAIL'}")
    print(f"\n双万分之一 rule: frac of elements with rel err > 1e-4 must be <= 1e-4")

    # ---- boundary / special values
    print("\nboundary behaviour:")
    edge = np.array([0.0, 1.0, 0.5, -0.1, 1.1, np.nan, np.inf, -np.inf], dtype=F32)
    out = ndtri_fp32(edge)
    ref = scipy_ndtri(edge.astype(np.float64))
    for pv, ov, rv in zip(edge, out, ref):
        mark = "ok" if (np.isnan(ov) and np.isnan(rv)) or ov == rv else "DIFF"
        print(f"  p={pv:<8} -> {ov:<10} scipy={rv:<10} {mark}")

    # ---- what the shipped harness actually covers
    u = make_samples()['uniform']
    tail_frac = float(((u < 0.02425) | (u > 1 - 0.02425)).mean())
    print(f"\nshipped uniform sampling: {tail_frac*100:.2f}% of points hit a tail branch; "
          f"deep tail (p<1e-6) count = {int((u < 1e-6).sum())}")


if __name__ == "__main__":
    main()
