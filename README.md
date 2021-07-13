# MicroTrackGenerator
An application to calculate the trajectories and energy deposition events of tracks of ionizing radiation for uses in computational microdosimetry.
## Installation and Requirements
MicroTrackGenerator requires the end user have a version of Geant4 which includes Geant4DNA_Option2 and CERN ROOT installed. In order to link the ROOT libraries with this software ROOT must have been built from source on the user's machine otherwise this software will not compile. Additionally all of the required libraries for Geant4 and ROOT are required for this software as well. MicroTrackGenerator is built by:
```
#from MicroTrackGenerator directory, in BASH, on Linux
mkdir build
cd build
cmake ..
make
```
## Running the Software
The software is run from the project's main directory. This can be accomplished with the packaged run.sh. Any script which takes the following form will be appropriate for running the software:
```
./build/MicroTrackGenerator -out [path here] -mac [macro file path here] -seed [random seed here]
```


## Description

### Summary

### Geometry

### Physics

### Input Parameters

### Random Seeding

### Output File Structure

## Software License

[To be decided.]
