# MicroTrackGenerator
A Geant4 based application to generate tracks for applications in microdosimetry.
## Description

## Installation and Requirements
MicroTrackGenerator requires the end user have a version of Geant4 which includes Geant4DNA_Option2 and ROOT installed. In order to link the ROOT libraries with this software ROOT must have been built from source on the user's machine otherwise this software will not compile. Additionally all of the required libraries for Geant4 and ROOT are required for this software as well. MicroTrackGenerator is built by:
```
#from MicroTrackGenerator directory, in BASH, on Linux
mkdir build
cd build
cmake ..
make
```
## Running the Software
