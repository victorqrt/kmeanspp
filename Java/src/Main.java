public class Main {

    static void usage() {
        System.out.println(
            "Usage:\n"
          + "   kmeanspp generate_dataset <dataset_size>\n"
          + "   kmeanspp clusterize <dataset> <number_of_clusters>\n"
          + "   kmeanspp export"
        );
    }

    public static void main(String[] args) {

        Kmeanspp k;
        try {
            switch(args[0]) {
                case "generate_dataset":
                    k = new Kmeanspp(Integer.parseInt(args[1]));
                    k.run();
                    break;
                case "clusterize":
                    k = new Kmeanspp(args[1], Integer.parseInt(args[2]));
                    k.run();
                    break;
                case "export":
                    k = new Kmeanspp();
                    k.run();
                    break;
                default:
                    usage();
            }
        }

        catch(Exception e) {
            usage();
        }
    }
}
