#!/bin/bash
# sudo apt-get install cimg-dev
# sudo apt-get install libxerces-c-dev
# sudo apt-get install xsdcxx
cd module
xsdcxx cxx-tree --root-element-all --generate-doxygen ../data/config/configurationCPS.xsd 
cd ../build
cmake ..
make
./CPS ../data/config/buddha.xml