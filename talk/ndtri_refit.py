#!/usr/bin/python3
# coding=utf-8
"""Isolate fp32 evaluation error properly, then test an fp32-conditioned refit of the central branch.

Clean isolation: BOTH f32 and f64 evaluations start from the SAME fp32-quantized p,
so input quantization cancels and we see only the arithmetic's conditioning.
"""
import numpy as np
from scipy.special import ndtri as scipy_ndtri

A = [-3.969683028665376e+01, 2.209460984245205e+02, -2.759285104469687e+02,
     1.383577518672690e+02, -3.066479806614716e+01, 2.506628277459239e+00]
B = [-5.447609879822406e+01, 1.615858368580409e+02, -1.556989798598866e+02,
     6.680131188771972e+01, -1.328068155288572e+01]
C = [-7.784894002430293e-03, -3.223964580411365e-01, -2.400758277161838e+00,
     -2.549732539343734e+00, 4.374664141464968e+00, 2.938163982698783e+00]
D = [7.784695709041462e-03, 3.224671290700398e-01, 2.445134137142996e+00,
     3.754408661907416e+00]


def horner(co, x, dt):
    acc = dt(co[0])
    for c in co[1:]:
        acc = dt(acc * x + dt(c))
    return acc


def acklam_central(p, dt):
    p = p.astype(dt)
    q = dt(p - dt(0.5))
    r = dt(q * q)
    return dt(dt(horner(A, r, dt) * q) / dt(horner(B, r, dt) * r + dt(1.0)))


def acklam_tail(t, dt):
    t = t.astype(dt)
    q = dt(np.sqrt(dt(dt(-2.0) * np.log(t))))
    return dt(horner(C, q, dt) / dt(horner(D, q, dt) * q + dt(1.0)))


def report(name, p32, f32, f64, gold):
    nz = gold != 0
    e_eval = np.abs(f32[nz] - f64[nz]) / np.abs(f64[nz])
    e_tot = np.abs(f32[nz] - gold[nz]) / np.abs(gold[nz])
    print(f"  {name:<26} eval_err(f32 vs f64, same input) max = {e_eval.max():.3e}"
          f"   total vs scipy = {e_tot.max():.3e}")
    return e_tot.max()


print("=" * 78)
print("STOCK ACKLAM, fp32 evaluation error isolated (same fp32 input for both precisions)")
print("=" * 78)
p32 = np.linspace(0.02425, 1 - 0.02425, 20001).astype(np.float32)
gold = scipy_ndtri(p32.astype(np.float64))
report("central", p32,
       acklam_central(p32, np.float32).astype(np.float64),
       acklam_central(p32.astype(np.float64), np.float64), gold)

t32 = np.logspace(-37, np.log10(0.02425), 20001).astype(np.float32)
goldt = scipy_ndtri(t32.astype(np.float64))
report("tail", t32,
       acklam_tail(t32, np.float32).astype(np.float64),
       acklam_tail(t32.astype(np.float64), np.float64), goldt)

# ---------------------------------------------------------------------------
# REFIT: central branch as z = q * R(r), R(r) = ndtri(p)/(p-0.5), r = q^2.
# R is smooth, positive, R(0)=sqrt(2*pi)=2.5066, R(rmax)~4.14 -> NO cancellation.
# ---------------------------------------------------------------------------
print()
print("=" * 78)
print("REFIT central: z = q * R(r),  R(r) = ndtri(p)/q  -- smooth & positive, well-conditioned")
print("=" * 78)
pf = np.linspace(0.02425, 1 - 0.02425, 200001)
qf = pf - 0.5
rf = qf * qf
mask = np.abs(qf) > 1e-12
Rf = np.empty_like(pf)
Rf[mask] = scipy_ndtri(pf[mask]) / qf[mask]
Rf[~mask] = np.sqrt(2 * np.pi)
print(f"  R(r) range: [{Rf.min():.4f}, {Rf.max():.4f}]  (vs Acklam A(r) peak intermediate ~276)")

for deg in (6, 8, 10, 12):
    # weighted least squares approximating minimax on RELATIVE error of R
    cf = np.polynomial.polynomial.polyfit(rf, Rf, deg, w=1.0 / Rf)
    co = list(cf[::-1])  # -> Horner order, highest first
    R32 = horner(co, rf.astype(np.float32), np.float32)
    z32 = (R32 * qf.astype(np.float32)).astype(np.float32)
    g = scipy_ndtri(pf.astype(np.float32).astype(np.float64))
    nz = g != 0
    e = np.abs(z32.astype(np.float64)[nz] - g[nz]) / np.abs(g[nz])
    nfail = int((e > 1e-4).sum())
    peak = max(abs(c) for c in co)
    print(f"  deg={deg:<3} max|coef|={peak:8.3f}  fp32 max rel err = {e.max():.3e}"
          f"  #>1e-4 = {nfail:<6} frac = {nfail/e.size:.2e}  "
          f"{'PASS' if nfail/e.size <= 1e-4 else 'FAIL'}")
