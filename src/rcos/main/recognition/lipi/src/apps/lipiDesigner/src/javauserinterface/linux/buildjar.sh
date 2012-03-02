#!/bin/bash
if [ "$LIPI_ROOT" = "" ]
then
echo "Environment variable LIPI_ROOT not set. Exiting..."
exit
fi
JAR_FILE_NAME=lipiDesigner.jar
CURRENT_WORKING_DIR=$PWD
cd $LIPI_ROOT/src/apps/lipiDesigner/src/javauserinterface/
javac *.java
jar cfm $JAR_FILE_NAME Manifest.txt *.class images
mv $JAR_FILE_NAME $LIPI_ROOT/src/apps/lipiDesigner/bin
rm *.class
cd $CURRENT_WORKING_DIR
echo "The output Jar is available under $LIPI_ROOT/src/apps/lipiDesigner/bin"
