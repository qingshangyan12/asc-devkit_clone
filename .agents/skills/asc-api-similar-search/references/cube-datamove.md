# Cube Data Movement Lookup

For cube data movement APIs, read `usage_dependencies` before interpreting the
signature. Return each `conditional_before.when` value verbatim alongside its
`all_of` or `any_of` path list.

Format-conversion and quantization setup only apply under their recorded conditions.
Never turn an Nz2ND, Nz2DN, scalar-quantization, or tensor-quantization condition
into an unconditional preparation step.
