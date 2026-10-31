# Background and Core Concepts

<!-- md-trans-meta sourceCommit=5df1d1c781b51fcb7826db7f2afab5f8bc453e93 translatedAt=2026-10-21T03:01:10.612Z pushedAt=2026-10-28T12:21:43.280Z nl=2 -->

The Cube unit adopts a tiled computation logic. For example, it can process a 16×16 cube tile in parallel. In a traditional linear storage layout (such as row major or column major), data is stored contiguously by row or by column. When a 16×16 rectangular data block needs to be read, fetching it directly from memory requires accessing multiple non-contiguous memory addresses to "gather" the data row by row. Such non-contiguous access significantly degrades memory efficiency.

To address this problem, we can introduce the cube fractal storage format. This format rearranges data at the software layer so that each 16×16 computation block is stored contiguously in physical memory. The hardware only needs to issue a single contiguous read to load the entire data block into on-chip registers, avoiding the overhead of multi-address access and thereby greatly improving the data throughput of the Cube unit.

To achieve extremely high data throughput and compute density, the Cube unit of Ascend AI Processors adopts a proprietary hardware architecture design. Unlike the contiguous linear storage commonly used by general-purpose CPUs, this unit requires input and output data to conform to a specific fractal storage format. When you develop operators using the asc_mmad class of APIs in Ascend C, a thorough understanding of these data formats is essential for correct data movement, tiling, and performance optimization.

For ease of understanding and memorization, this document adopts the intuitive "big Y, small x" naming convention:

- Big Y (Z/N): arrangement order between fractal matrices (Z is row major, N is column major).
- Small x (z/n): arrangement order of elements within a fractal cube (z is row major, n is column major).
