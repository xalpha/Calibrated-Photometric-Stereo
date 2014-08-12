Calibrated-Photometric-Stereo
=============================

This project is to implement simple calibrated photometric stereo that computes normal vector map given a set of images taken under different light conditions and the light source information. 

Assumed reflectance model;
- current version assumes the Lambertian reflection model
- future version will handle specular reflection models

The code assumes that:
- you have photometric stereo image datasets provided by [1] in PSMDATA_DIR
-- PSMDATA_DIR is set as data/psmImages/
- you have an xml file containing the configuration for each dataset
-- this code stores all configuration files in data/config/ and an xml file X.xml contains the configuration for dataset X

Required libraries:
- CMake 2.8.12.2
- Eigen 3.2.2 for linear equation solver
- Xerces-C 3.1.1.5 and CodeSynthesis XSD 3.3.0 for configuration loader
- CImg 1.5.7 for data visualization
- Boost 1.54.0 for handling filesystem and file name
Note that the project may not work with different versions of the libraries.

Commands to install the required libraries:
- sudo apt-get install cmake
- sudo apt-get install libeigen3-dev
- sudo apt-get install libxerces-c-dev
- sudo apt-get install xsdcxx
- sudo apt-get install cimg-dev
- sudo apt-get install libboost-all-dev

The project contains
- c++ codes, module/main.cpp, module/***.hpp
- cmake related files, CMakeLists.txt and extern/Find***.cmake
- configuration files, data/config/configurationCPS.xsd and data/config/***.xml

To run the code,
- cd module
- xsdcxx cxx-tree --root-element-all --generate-doxygen ../data/config/configurationCPS.xsd 
- cd ../data
- wget http://courses.cs.washington.edu/courses/cse455/04wi/projects/project3/psmImages.zip
- unzip psmImages.zip
- cd ../build
- cmake ..
- make
- ./CPS ../data/config/buddha.xml
- ./CPS ../data/config/cat.xml
- ./CPS ../data/config/chrome.xml
- ./CPS ../data/config/gray.xml
- ./CPS ../data/config/horse.xml
- ./CPS ../data/config/owl.xml
- ./CPS ../data/config/rock.xml

- [1] http://courses.cs.washington.edu/courses/cse455/04wi/projects/project3/psmImages.zip
