use std::fs::File;
use std::io::prelude::*;

struct Point {
    x: f64,
    y: f64
}

struct Cluster {
    points: Vec<Point>
}

struct Dataset {
    size: usize,
    points: Vec<Point>,
}
