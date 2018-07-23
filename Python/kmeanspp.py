"""
Usage:
    kmeanspp.py generate_dataset <dataset_size>
    kmeanspp.py clusterize <dataset> <number_of_clusters>
    kmeanspp.py export
"""

import sys
import csv
import math
import random
from copy import deepcopy
from docopt import docopt

class Point:
    def __init__(self, x, y):
        self.x = x
        self.y = y
        self.cluster = None

    distanceTo = lambda self, p : math.sqrt(math.pow(self.x - p.x, 2) + math.pow(self.y - p.y, 2))

class Cluster:
    def __init__(self, center):
        self.center = center
        self.points = []
        
class Dataset:
    def __init__(self, points):
        self.points = points
        self.clusters = []

    def initializeCenters(self, n):
        init_centers = [random.choice(self.points)]
        current_center = init_centers[0]
        
        for i in range(1, n):
            candidate_points = self.points[:]
            for p in init_centers:
                for c in candidate_points:
                    if p.x == c.x and p.y == c.y:
                        candidate_points.remove(c)
           
            weight_dividor = sum(math.pow(p.distanceTo(current_center), 2) for p in candidate_points)
            max_weight = 0
            best_candidate = None
            for p in candidate_points:
                weight = math.pow(p.distanceTo(current_center), 2) / weight_dividor
                if weight > max_weight:
                    max_weight = weight
                    best_candidate = p
            
            current_center = best_candidate
            init_centers.append(current_center)
        
        for c in init_centers:
            self.clusters.append(Cluster(c))

        for p in self.points:
            self.clusters[0].points.append(p)
            p.cluster = self.clusters[0]

    def updateClusterMembers(self):
        for c in self.clusters:
            for p in self.points:
                if p.distanceTo(c.center) < p.distanceTo(p.cluster.center):
                    p.cluster.points.remove(p)
                    c.points.append(p)
                    p.cluster = c
            c.center = barycenterFromList(c.points)

def datasetFromCSV(filename):
    points =[]
    with open(filename, 'r') as f:
        reader = csv.reader(f, delimiter=',')
        for e in reader:
            if not any(p.x == int(e[0]) and p.y == int(e[1]) for p in points):
                points.append(Point(int(e[0]), int(e[1])))

    return Dataset(points)

barycenterFromList = lambda points : Point(sum(p.x for p in points) / len(points), sum(p.y for p in points) / len(points)) 

if __name__ == '__main__':
    args = docopt(__doc__)

    if args["generate_dataset"]:
        print("[ ] Generating " + args["<dataset_size>"] + " 2D points into dataset.csv...")
        with(open("dataset.csv", 'w')) as out:
            w = csv.writer(out)
            for i in range(int(args["<dataset_size>"])):
                w.writerow([random.randint(0, 1000), random.randint(0, 1000)])

        print("[+] Done.")

    elif args["clusterize"]:
        print("[ ] Loading dataset...")
        dataset = datasetFromCSV(args["<dataset>"])
        print("[ ] Choosing initial cluster centers...")
        dataset.initializeCenters(int(args["<number_of_clusters>"]))
        
        centers = list(c.center for c in dataset.clusters)
        previous_centers = [Point(-1, -1)]
        i = 0
        
        while(any(c.x != p.x or c.y != p.y for (c, p) in zip(centers, previous_centers))):
            print("[ ] Updating clusters (iteration " + str(i) + ")...")
            previous_centers = deepcopy(centers)
            dataset.updateClusterMembers()
            centers = list(c.center for c in dataset.clusters)
            i = i+1
   
        print("[ ] Clusters found, writing to out.csv...")
        with(open("out.csv", 'w', newline='')) as out:
            w = csv.writer(out)
            for i in range(len(dataset.clusters)):
                for p in dataset.clusters[i].points:
                    w.writerow([p.x, p.y, str(i)])

        print("[+] Done.")

    elif args["export"]:
        print("[ ] Exporting points from out.csv to out.svg...")
        svg_str = "<svg xmlns='http://www.w3.org/2000/svg' xmlns:xlink='http://www.w3.org/1999/xlink' width='1000' height='1000'>\n"
        svg_colors = ["black", "blue", "red", "green", "yellow", "gray", "gold", "tan", "olivedrab", "aqua"]

        with open("out.csv", 'r') as f:
            reader = csv.reader(f, delimiter=',')
            for e in reader:
                svg_str = svg_str + "   <circle cx='" + e[0] + "' cy='" + e[1] + "' r='5' fill='" + svg_colors[int(e[2]) % len(svg_colors)] + "' />\n"

        svg_str = svg_str + "</svg>\n"
        with open("out.svg", 'w') as out:
            out.write(svg_str)

        print("[+] Done.")
