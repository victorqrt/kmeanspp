import java.io.FileWriter;
import java.io.FileReader;
import java.io.BufferedReader;
import java.util.Random;
import java.util.ArrayList;
import java.lang.Math;

public class Kmeanspp {

    enum Op {
        generate,
        clusterize,
        export
    }

    Op op;
    int dsize, clusters;
    String dfile;

    public Kmeanspp() {
        op = Op.export;
    }

    public Kmeanspp(int dsize) {
        op = Op.generate;
        this.dsize = dsize;
    }

    public Kmeanspp(String dfile, int clusters) {
        op = Op.clusterize;
        this.clusters = clusters;
        this.dfile = dfile;
    }

    public void run() {
        switch(op) {
            case generate:
                generate_dataset();
                break;

            case clusterize:
                clusterize();
                break;

            case export:
                export("out.csv", "out.svg");
                break;
        }
    }

    private void generate_dataset() {
        System.out.println("[ ] Generating " + dsize + " 2D points into dataset.csv...");

        try(FileWriter fw = new FileWriter("dataset.csv")) {
            Random rand = new Random();

            for(int i = 0; i < dsize; i++)
                fw.write(rand.nextInt(1000) + "," + rand.nextInt(1000) + "\n");

            fw.close();
            System.out.println("[+] Done.");
        }

        catch(Exception e) {
            System.out.println("[!] File error");
        }
    }

    private void clusterize() {
        try {
            System.out.println("[ ] Loading dataset...");
            Dataset data = new Dataset(dfile, clusters);

            System.out.println("[ ] Choosing initial cluster centers...");
            data.initializeCenters();

            data.updateClustersLoop();

            System.out.println("[ ] Clusters found, writing to out.csv...");
            FileWriter fw = new FileWriter("out.csv");

            for(Point p : data.points)
                fw.write(p.toString() + "\n");

            fw.close();
            System.out.println("[+] Done.");
        }

        catch(Exception e) {
            e.printStackTrace();
        }
    }

    // Write svg from clusterized csv
    private void export(String ffrom, String fto) {
        System.out.println("[ ] Exporting points from out.csv to out.svg...");

        try(BufferedReader br = new BufferedReader(new FileReader(ffrom))) {

            String svgStr = "<svg xmlns='http://www.w3.org/2000/svg' xmlns:xlink='http://www.w3.org/1999/xlink' width='1000' height='1000'>\n";

            for(String line; (line = br.readLine()) != null;) {
                String[] l = line.trim().split(",");
                svgStr += "    <circle cx='" + l[0] + "' cy='" + l[1] + "' r='5' fill='"
                       + Dataset.Colors[Integer.parseInt(l[2]) % Dataset.Colors.length] + "' />\n";
            }

            br.close();
            svgStr += "</svg>\n";

            FileWriter fw = new FileWriter(fto);
            fw.write(svgStr);

            fw.close();
            System.out.println("[+] Done");
        }

        catch(Exception e) {
            System.out.println("[!] File error");
            e.printStackTrace();
        }
    }
}

class Point {

    Cluster cluster;
    double x, y;

    public Point(double x, double y) {
        this.x = x;
        this.y = y;
    }

    public double squaredDistanceTo(Point p) {
        return Math.pow(x - p.x, 2) + Math.pow(y - p.y, 2);
    }

    public double distanceTo(Point p) {
        return Math.sqrt(squaredDistanceTo(p));
    }

    public boolean equals(Point p) {
        return x == p.x && y == p.y;
    }

    public String toString() {
        return (int) x + "," + (int) y + (cluster == null ? "" : "," + cluster.id);
    }
}

class Cluster {

    ArrayList<Point> points;
    Point center;
    int id;

    public Cluster(Point center, int id) {
        this.center = center;
        this.id = id;
        points = new ArrayList<Point>();
    }

    Point barycenter() {
        double xs = 0;
        double ys = 0;
        int s = points.size();

        for(Point p: points) {
            xs += p.x;
            ys += p.y;
        }

        return new Point(xs / s, ys / s);
    }
}

class Dataset {

    int nclust;
    ArrayList<Point> points;
    ArrayList<Cluster> clusters;

    static String[] Colors = {"black", "blue", "red", "green", "yellow", "gray", "gold", "tan", "olivedrab", "aqua"};

    // Read dataset from csv
    public Dataset(String file, int nclust) throws Exception {

        this.nclust = nclust;
        points = new ArrayList<Point>();
        clusters = new ArrayList<Cluster>(nclust);

        try(BufferedReader br = new BufferedReader(new FileReader(file))) {

            for(String line; (line = br.readLine()) != null;) {
                String[] l = line.trim().split(",");
                points.add(
                    new Point(
                        Integer.parseInt(l[0]),
                        Integer.parseInt(l[1])
                    )
                );
            }

            br.close();

            if(nclust > points.size()) {
                System.out.println("[!] Error: we are looking for more clusters than there are points in the dataset");
                throw new IllegalArgumentException();
            }
        }

        catch(java.io.FileNotFoundException e) {
            System.out.println("[!] File error");
        }
    }

    // Choose the initial cluster centers
    void initializeCenters() {

        Random rand = new Random();
        ArrayList<Point> initCenters = new ArrayList<Point>(nclust);

        initCenters.add(points.get(rand.nextInt(points.size())));
        Point currentCenter = initCenters.get(0);

        for(int i=1; i<nclust; i++) {
            ArrayList<Point> candidatePoints = new ArrayList<Point>(points);
            ArrayList<Point> toRemove = new ArrayList<Point>();

            for(Point p : initCenters)
                for(Point c : candidatePoints)
                    if(c.equals(p))
                        toRemove.add(p);

            candidatePoints.removeAll(toRemove);

            double weightDividor = 0;

            for(Point p : candidatePoints)
                weightDividor += p.squaredDistanceTo(currentCenter);

            double maxWeight = 0;
            Point bestCandidate = null;

            for(Point p : candidatePoints) {
                double weight = p.squaredDistanceTo(currentCenter) / weightDividor;
                if(weight > maxWeight) {
                    maxWeight = weight;
                    bestCandidate = p;
                }
            }

            currentCenter = bestCandidate;
            initCenters.add(currentCenter);
        }

        // Create the clusters
        for(int i=0; i<initCenters.size(); i++)
            clusters.add(new Cluster(initCenters.get(i), i));

        // Affect every point to the first cluster
        for(Point p : points) {
            clusters.get(0).points.add(p);
            p.cluster = clusters.get(0);
        }
    }

    boolean updateClusters() {

        boolean nomodif = true;
        for(Cluster c : clusters) {

            if(c.points.size() > 0)
                c.center = c.barycenter();

            ArrayList<Point> toUpdate = new ArrayList<Point>();

            for(Point p : points) {
                if(p.distanceTo(p.cluster.center) > p.distanceTo(c.center)) {
                    p.cluster = c;

                    if(!toUpdate.contains(p))
                        toUpdate.add(p);

                    nomodif = false;
                }
            }

            for(Point p : toUpdate) {
                c.points.remove(p);
                p.cluster.points.add(p);
            }
        }

        return nomodif;
    }

    // Keep recomputing the clusters until we get no change by doing so
    void updateClustersLoop() {
        int i = 0;

        do {
            System.out.println("[ ] Updating clusters (iteration " + i + ")...");
            i++;
        } while(!updateClusters());
    }
}
