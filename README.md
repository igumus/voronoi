# Voronoi 

## Overview

Experimentation, and practice with Voronoi spatial partitioning and related algorithms.

Voronoi diagrams divide a plane into regions based on the distance to a given set of points (sites). Each region contains all points closer to one site than to any other.

![thumbnail](./thumbnails/thumbnail.png)

## How It Works

1. The user provides a set of 2D points (sites).
2. The application computes distances between points.
3. The plane is partitioned into Voronoi regions.
4. Each region is assigned to the nearest site.
5. Results are written to an output file (in PPM format).


## Quick Start

```console
$ cc -o nob nob.c
$ ./nob
$ ./build/voronoi
```

## References

- [Voronoi Diagram](https://en.wikipedia.org/wiki/Voronoi_diagram)
- [Euclidean Distance](https://en.wikipedia.org/wiki/Euclidean_distance)
- [Taxicab or Manhattan Distance](https://en.wikipedia.org/wiki/Taxicab_geometry) 
- [Chebyshev](https://en.wikipedia.org/wiki/Chebyshev_distance)
- [PPM File Format](https://en.wikipedia.org/wiki/Netpbm)
- [Tsoding - nob.h](https://github.com/tsoding/nob.h)
- [Tsoding - Procedural Graphics in C (Voronoi Ep.01)](https://www.youtube.com/watch?v=kT-Mz87-HcQ)

