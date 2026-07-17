#!/usr/bin/python3
# coding=utf-8
"""Diagnose WHERE the fp32 Acklam error comes from: the formula, or its evaluation?

Splits total error into:
  formula error     = Acklam(float64) vs scipy   -> intrinsic to the rational approximation
  evaluation error  = Acklam(float32) vs Acklam(float64) -> fp32 rounding/conditioning
"""
import numpy as np
from scipy.special import ndtri as scipy_ndtri

A = [-3.969683028665376e+01, 2.209460984245205e+02, -2.759285104469687e+02,
     1.383577518672690e+02, -3.066479806614716e+01, 2.506628277459239e+00]
B = [-5.447609879822406e+01, 1.615858368580409e+02, -1.556989798598866e+02,
     6.680131188771972e+01, -1.328068155288572e+01]


def horner(coeffs, x, dt):
    acc = dt(coeffs[0])
    for c in coeffs[1:]:
        acc = dt(acc * x + dt(c))
    return acc


def central(p, dt):
    """Acklam central branch in dtype dt."""
    p = p.astype(dt)
    q = dt(p - dt(0.5))
    r = dt(q * q)
    num = dt(horner(A, r, dt) * q)
    den = dt(horner(B, r, dt) * r + dt(1.0))
    return dt(num / den)


# central branch only
p = np.linspace(0.02425, 1 - 0.02425, 20001)
gold = scipy_ndtri(p)
f64 = central(p, np.float64)
f32 = central(p.astype(np.float32), np.float32).astype(np.float64)

nz = gold != 0
e_formula = np.abs(f64[nz] - gold[nz]) / np.abs(gold[nz])
e_total = np.abs(f32[nz] - gold[nz]) / np.abs(gold[nz])
e_eval = np.abs(f32[nz] - f64[nz]) / np.abs(f64[nz])

print("CENTRAL branch, p in [0.02425, 0.97575]")
print(f"  formula error   (f64 vs scipy) : max {e_formula.max():.3e}   <- Acklam's intrinsic ~1.15e-9")
print(f"  evaluation error(f32 vs f64)   : max {e_eval.max():.3e}   <- fp32 rounding/conditioning")
print(f"  TOTAL           (f32 vs scipy) : max {e_total.max():.3e}   <- what we must keep <=1e-4")
print(f"  fp32 epsilon                   :     {np.finfo(np.float32).eps:.3e}")
print(f"  => evaluation error is {e_eval.max()/np.finfo(np.float32).eps:.0f}x fp32 eps"
      f"  -> conditioning, not rounding\n")

# Show the cancellation directly: Horner intermediates for A(r)
print("Why: Horner intermediates for A(r) are huge, the result is tiny -> catastrophic cancellation")
for rv in (0.0, 0.05, 0.15, 0.2264):
    acc = A[0]
    inter = [acc]
    for c in A[1:]:
        acc = acc * rv + c
        inter.append(acc)
    peak = max(abs(x) for x in inter)
    print(f"  r={rv:<7} |intermediate| peak = {peak:8.2f}   final A(r) = {acc:9.4f}"
          f"   amplification = {peak/abs(acc):8.1f}x")

print("\n  fp32 eps * amplification ~= achievable relative error floor:")
for rv in (0.15, 0.2264):
    acc = A[0]
    inter = [acc]
    for c in A[1:]:
        acc = acc * rv + c
        inter.append(acc)
    peak = max(abs(x) for x in inter)
    amp = peak / abs(acc)
    print(f"    r={rv}: 6e-8 * {amp:.0f} = {6e-8*amp:.2e}")
