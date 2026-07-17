#!/usr/bin/python3
# coding=utf-8
"""Fix the central branch for fp32 by factoring out the leading term (Cephes structure).

Acklam central:  z = A(r)*q / B(r)          -> A(r) Horner peaks at ~276 and collapses to ~0.01
                                               => amplification up to 2e4 => fp32 err ~1.5e-4  FAIL
Cephes  central:  z = s2pi * q * (1 + r*P0(r)/Q0(r))
                                            -> the rational is a *correction* on a dominant term,
                                               every contribution positive-ish => no cancellation.
scipy.special.ndtri IS Cephes, so the formula error against scipy is ~0 by construction.
"""
import numpy as np
from scipy.special import ndtri as scipy_ndtri

F32 = np.float32
S2PI = 2.50662827463100050242

# --- Cephes ndtri central-branch coefficients (P0 deg4, Q0 monic deg8)
P0 = [-5.99633501014107895267E1, 9.80010754185999661536E1, -5.66762857469070293439E1,
      1.39312609387279679503E1, -1.23916583867381258016E0]
Q0 = [1.95448858338141759834E0, 4.67627912898881538453E0, 8.63602421390890590575E1,
      -2.25462687854119370527E2, 2.00260212380060660359E2, -8.20372256168333339912E1,
      1.59056225126211695515E1, -1.18331621121330003142E0]

# --- Acklam central, for comparison
A = [-3.969683028665376e+01, 2.209460984245205e+02, -2.759285104469687e+02,
     1.383577518672690e+02, -3.066479806614716e+01, 2.506628277459239e+00]
B = [-5.447609879822406e+01, 1.615858368580409e+02, -1.556989798598866e+02,
     6.680131188771972e+01, -1.328068155288572e+01]


def horner(co, x, dt):
    acc = dt(co[0])
    for c in co[1:]:
        acc = dt(acc * x + dt(c))
    return acc


def p1evl(co, x, dt):
    """monic: leading coefficient 1 implied"""
    acc = dt(x + dt(co[0]))
    for c in co[1:]:
        acc = dt(acc * x + dt(c))
    return acc


def cephes_central(p, dt):
    p = p.astype(dt)
    q = dt(p - dt(0.5))
    r = dt(q * q)
    num = horner(P0, r, dt)
    den = p1evl(Q0, r, dt)
    corr = dt(dt(r * num) / den)
    return dt(dt(q + dt(q * corr)) * dt(S2PI))


def acklam_central(p, dt):
    p = p.astype(dt)
    q = dt(p - dt(0.5))
    r = dt(q * q)
    return dt(dt(horner(A, r, dt) * q) / dt(horner(B, r, dt) * r + dt(1.0)))


p32 = np.linspace(0.02425, 1 - 0.02425, 200001).astype(F32)
gold = scipy_ndtri(p32.astype(np.float64))
nz = gold != 0

print(f"{'central variant':<20} {'fp32 max rel err':>17} {'#>1e-4':>8} {'frac':>10}  verdict")
print("-" * 66)
for name, fn in (("Acklam A(r)q/B(r)", acklam_central), ("Cephes s2pi*q*(1+..)", cephes_central)):
    z = fn(p32, F32).astype(np.float64)
    e = np.abs(z[nz] - gold[nz]) / np.abs(gold[nz])
    nf = int((e > 1e-4).sum())
    print(f"{name:<20} {e.max():>17.3e} {nf:>8} {nf/e.size:>10.2e}  "
          f"{'PASS' if nf/e.size <= 1e-4 else 'FAIL'}")

# show conditioning: correction term magnitude stays bounded & positive-ish
print("\nWhy Cephes is safe -- the rational is a bounded CORRECTION, not the whole answer:")
for rv in (0.0, 0.05, 0.15, 0.2264):
    r = np.float64(rv)
    corr = r * np.polyval(P0, r) / (np.polyval([1.0] + Q0, r))
    print(f"  r={rv:<7} correction r*P0/Q0 = {corr:+.5f}   -> factor (1+corr) = {1+corr:.5f}")

print("\nAcklam A(r) Horner: peak |intermediate| vs final (the cancellation):")
for rv in (0.0, 0.15, 0.2264):
    acc = A[0]; inter = [acc]
    for c in A[1:]:
        acc = acc * rv + c; inter.append(acc)
    peak = max(abs(x) for x in inter)
    print(f"  r={rv:<7} peak={peak:7.2f}  final={acc:9.4f}  amplification={peak/abs(acc):9.1f}x")
