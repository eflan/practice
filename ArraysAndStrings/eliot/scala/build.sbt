// factor out common settings into a sequence
lazy val commonSettings = Seq(
  organization := "eflan",
  name := "ArraysAndStrings",
  version := "0.1",
  // set the Scala version used for the project
  scalaVersion := "2.12.4"
)

libraryDependencies ++= Seq(
  "org.specs2" %% "specs2-core" % "4.0.0" % "test"
)

scalacOptions in Test ++= Seq("-Yrangepos")

lazy val root = (project in file("."))
  .settings(
    commonSettings
  )
