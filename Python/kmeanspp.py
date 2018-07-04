import sys
import csv
import math
import random
from copy import deepcopy

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

    def initializeCenters(self):
        init_centers = [random.choice(self.points)]
        current_center = init_centers[0]
        
        for i in range(1, int(sys.argv[1])):
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
    if len(sys.argv) != 3:
        print("Usage:\n    kmeanspp.py number_of_clusters dataset_file")
    else:
        print("[ ] Loading dataset...")
        dataset = datasetFromCSV(sys.argv[2])
        print("[ ] Choosing initial cluster centers...")
        dataset.initializeCenters()
        
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
        with(open('out.csv', 'w', newline='')) as out:
            w = csv.writer(out)
            for i in range(len(dataset.clusters)):
                for p in dataset.clusters[i].points:
                    w.writerow([p.x, p.y, i, ''])

        print("[+] Done.")
