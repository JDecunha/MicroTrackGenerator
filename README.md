# MicroTrackGenerator
An application to calculate the trajectories and energy deposition events of tracks of ionizing radiation for uses in computational microdosimetry.
## Installation and Requirements
The required software and libraries to compile MicroTrackGenerator include:
1. Geant4. A version which includes Geant4DNA_Option2 is required, 10.7 or greater is recommended.
2. CERN ROOT 6.24 or greater. Must be built from source rather than installed from a binary.
3. CMake 3.3 or greater.
4. Any required dependancies of Geant4 and CERN ROOT.


MicroTrackGenerator is built by:
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
**Command Line**

**-out:** (mandatory)

Specify the output folder and name for the track information file. In addition to the name given the random seed and which thread the track was generated on will be appended to the filename.

**-mac:** (mandatory)

Specify the path and name of the macro file relative to the location of your run script.

**-seed:** (mandatory)

Specify the random seed for the simulation. The singular random seed is used to uniquely seed each thread of the simulation for a multithreaded simulation.

**-mt:**

Specify the number of threads you would like the simulation to run on. If your Geant4 installation has been built with G4Multithreaded enabled the minimum number of threads selectable are two. Available options are either an integer number or "NMAX" which will determine the maximal number of threads available on the system.

**Macro File**

**geometry/voxelSideLength [value] [unit]** (mandatory)

Specify the side length of the cubic voxel within which the generated tracks are constrained. A value and length unit are required.

**/primary/type [name]** (mandatory)

Specify the name of the primary particle you would like to generate. Must be a name of a particle available in the G4ParticleTable.

**/primary/energy [value] [unit]** (mandatory)

Specify the initial energy of the primary particle you create. Requires a value and associated energy unit.



## Description

MicroTrackGenerator has been developed to be as lightweight and simple as possible. Wherever possible end-user control has been removed to prevent the user from running the software in an inappropriate way. The purpose of the software is to generate a track library for a given source of ionizing radiation at a series of  energies. Individual components of the software are described below.

### Geometry

### Physics

### Random Seeding

### Output File Structure

## Software License

[To be decided.]
