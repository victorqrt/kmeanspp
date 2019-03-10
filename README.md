# kmeanspp

This repo contains my implementation of the kmeans clustering algorithm in different languages.
It is a constant work in progress, as part of my learning process of said languages.

- `make` at the root of the repo will build everything that needs to be.
- Each of the binaries can generate a random 2D points dataset, perform a k-means on a dataset to clusterize its points, and export a clusterized dataset to a .svg image.

# Build tool by language

- C: gcc
- Rust: cargo
- Java: javac, jar
- Scala: sbt

# Draw dataset

The program can generate random datasets, but you can generate one by hand with the `draw-dataset` included tool. Requires python3-tk.
