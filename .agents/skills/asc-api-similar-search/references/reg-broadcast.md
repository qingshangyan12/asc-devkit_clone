# Register Broadcast Lookup

For `reg_compute/reg_broadcast` questions, inspect the current API documentation in
this order: broadcast behavior, `mask` semantics, supported `dtype`, then merge-mode
overloads or parameters. This sequence describes one API's local contract.

Do not infer a cross-API call-before relationship from broadcast, mask, dtype, or
merge-mode wording. When the current API has no `usage_dependencies` OKF field, report
that dependency review has not been completed; an `unconfirmed` status means it found
no confirmed cross-API prerequisite.
