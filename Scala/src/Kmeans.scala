package kmeanspp

import language.postfixOps

object Main extends App {

  def usage {
    println("""Usage:
              |    kmeanspp generate_dataset <dataset_size>
              |    kmeanspp clusterize <dataset> <number_of_clusters>
              |    kmeanspp export""" stripMargin)
  }

  usage
}
