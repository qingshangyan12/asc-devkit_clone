# cann-review reinspection test fixture

> [!IMPORTANT]
> This file is an isolated test fixture for validating `cann-review` reinspection.
> It is not product documentation and must not be merged.

The initial review intentionally leaves the following values unresolved. A follow-up
commit can update them to verify that reinspection checks the original comments
instead of starting a new full review.

## Test cases

- `REINSPECTION-TEST-001` retry limit: `0`
- `REINSPECTION-TEST-002` owner: `TODO`
- `REINSPECTION-TEST-003` cleanup after test: `disabled`

Expected follow-up values:

- retry limit: a positive integer
- owner: `cann-review`
- cleanup after test: `enabled`
