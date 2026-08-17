# Resource Lookup Output Isolation Design

## Problem

`ResourceRegistry::Lookup` currently returns a pointer to `ResourceEntry::data`. A lookup also creates a new materialization directory and writes that directory into the same `ResourceData` object. Repeating a lookup for one resource ID therefore returns the same pointer and changes the `resourceDir` observed through every previously returned pointer.

Each successful lookup must instead populate a caller-owned `ResourceData`. Results from separate calls must remain independent, including their materialization directories.

## Interface

Change the lookup entry point from:

```cpp
ResourceStatus Lookup(const char* resourceId, const ResourceData*& resource);
```

to:

```cpp
ResourceStatus Lookup(const char* resourceId, ResourceData& resource);
```

The caller owns the output object and controls its lifetime. Passing the same output object to another lookup intentionally replaces that object's contents. Passing different output objects keeps previous results unchanged.

## Data Flow

`ResourceEntry::data` remains the registered resource template. It contains the manifest JSON and source file path, while its `resourceDir` remains empty.

For each lookup:

1. Reset the caller's output to an empty `ResourceData`.
2. Validate the resource ID and find the registered entry.
3. Copy the entry template into a temporary `ResourceData`.
4. Materialize the entry's files into a newly generated directory and store that path in the temporary output.
5. Move the temporary output into the caller's object only after materialization succeeds.

`Materialize` will receive the output object separately from the immutable resource entry. It will no longer modify `ResourceEntry::data`.

The existing per-entry materialization mutex remains in place. It protects repeated and concurrent materialization for the same resource without expanding the scope of this fix.

## Failure Behavior

Invalid identifiers, missing resources, and materialization failures return their existing `ResourceStatus` values. The caller's output remains an empty `ResourceData`, mirroring the old interface's null output on failure.

If materialization creates a partial directory before failing, the existing cleanup path removes that directory. No partial result is assigned to the caller.

## Tests

Update the focused registry tests to use caller-owned output objects and verify:

- invalid, missing, and failed lookups leave the output empty;
- two successful lookups for the same resource populate two distinct `ResourceData` objects;
- the two lookups use different materialization directories;
- the second lookup does not change the first result's `resourceDir`;
- files in both materialization directories remain readable;
- end-to-end loading and lookup still return the manifest and source path.

