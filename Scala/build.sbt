scalaVersion := "2.12.7"

scalaSource in Compile := baseDirectory.value / "src/"

assemblyJarName in assembly := "kmeanspp.jar"
mainClass in assembly := Some("kmeanspp.Main")
