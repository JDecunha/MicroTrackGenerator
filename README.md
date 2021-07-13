# MicroTrackGenerator
An application to calculate the trajectories and energy deposition events of tracks of ionizing radiation for uses in computational microdosimetry.
## Installation and Requirements
MicroTrackGenerator requires the end user have a version of Geant4 which includes Geant4DNA_Option2 and CERN ROOT installed. Geant 10.7 or greater as well as ROOT 6.24 or greater are recommended. In order to link ROOT libraries with this software ROOT must have been built from source on the user's machine rather than installed from a binary otherwise this software will not compile. Additionally all of the libraries required for Geant4 and ROOT are required for this software as well. MicroTrackGenerator is built by:
```
#from MicroTrackGenerator directory, in BASH, on Linux
mkdir build
cd build
cmake ..
make
```
## Running the Software
The software is run from the project's main directory. This can be accomplished with the packaged run.sh script. Any script which takes the following form will be appropriate for running the software:
```
./build/MicroTrackGenerator -out [path here] -mac [macro file path here] -seed [random seed here]
```
Each of the above input parameters in the run script are mandatory and the software will through an exception if any of them are not specified. A full listing of the input parameters is given below.
### Input Parameters
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; **Command Line**

**-out:** (mandatory)

Specify the output folder and name for the track information file. In addition to the name given the random seed and which thread the track was generated on will be appended to the filename.

**-mac:** (mandatory)

Specify the path and name of the macro file relative to the location of your run script.

**-seed:** (mandatory)

Specify the random seed for the simulation. The singular random seed is used to uniquely seed each thread of the simulation for a multithreaded simulation.

**-mt:**

Specify the number of threads you would like the simulation to run on. If your Geant4 installation has been built with G4Multithreaded enabled the minimum number of threads selectable are two. Available options are either an integer number or "NMAX" which will determine the maximal number of threads available on the system.

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; **Macro File**

**geometry/voxelSideLength [value] [unit]**

**/primary/type [name]**

**/primary/energy [value] [unit]**


## Description

### Geometry

### Physics

### Random Seeding

### Output File Structure

## Software License

[To be decided.]
