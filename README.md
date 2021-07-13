# MicroTrackGenerator (v0.9)
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

All tracks originate from the center of the side of a cubic voxel whose size is specified by voxelSideLength. When any particle in the track reaches the edge of the voxel it is terminated. The initial momentum of all primary particles is fixed to be directed into the voxel. The rationale behind this is to prevent needless track information from being stored. For example, when performing voxel specific microdosimetry (i.e. determing microdosimetric parameters in each voxel of a patient) and a given particle is determined to be in the energy spectra for that voxel, only the portion of that particle's track which lies within that voxel will contribute to the microdosimetric spectra for that voxel. This being the case, if I am interested in the microdosimetric spectra for say a monoenergetic 100 MeV proton beam, I only need to save the portion of that 100 MeV track which lies within the 3x3x3 mm voxel and not the entire track. This saves storage space compared to recording the entire track length.

### Physics

The physics model Geant4DNA_Option2 is used for this application. Users are directed to the Geant4DNA collaboration's website (http://geant4-dna.in2p3.fr/styled-3/styled-8/index.html) and publications for data on the physics model. If you attempt to simulate a primary particle which is not modelled by the Geant4DNA model or does not fall within the specified energy range (i.e. electrons of 1 MeV or greater, protons of 100 MeV or greater) then the software will not export and track information.

### Random Seeding

### Output File Structure

## Determining Microdosimetric Quantities from Track Library

## Software License

[To be decided.]
