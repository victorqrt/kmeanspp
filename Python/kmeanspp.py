import sys
import csv
import math
import random

class Point:
    def __init__(self, x, y):
        self.x = x
        self.y = y
        self.cluster = None
    
    def distanceTo(self, p):
        return math.sqrt(math.pow(self.x - p.x, 2) + math.pow(self.y - p.y, 2))

    def __str__(self):
        return str(self.x) + ' ' + str(self.y)

class Cluster:
    def __init__(self, center, points):
        self.center = center
        self.points = points
        
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
                    if p.x == p.y and c.x == c.y:
                        candidate_points.remove(c)
           
            weight_dividor = sum(math.pow(p.distanceTo(current_center), 2) for p in candidate_points)
            max_weight = 0
            best_candidate = None
            for p in candidate_points:
                weight = math.pow(p.distanceTo(current_center), 2) / weight_dividor
                if weight > max_weight:
                    max_weight = weight
                    best_candidate = p
                    candidate_points.remove(p)

            current_center = best_candidate
            init_centers.append(current_center)
        
        for c in init_centers:
            print(str(c))
         

def datasetFromCSV(filename):
    points =[]
    with open(filename, 'r') as f:
        reader = csv.reader(f, delimiter=',')
        for e in reader:
            if not any(p.x == int(e[0]) and p.y == int(e[1]) for p in points):
                points.append(Point(int(e[0]), int(e[1])))

    return Dataset(points)

if __name__ == '__main__':
    if len(sys.argv) != 3:
        print("Usage:\n    kmeanspp.py number_of_clusters dataset_file")
    else:
        dataset = datasetFromCSV(sys.argv[2])
        dataset.initializeCenters()
