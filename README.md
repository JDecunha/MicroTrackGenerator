# MicroTrackGenerator (v0.9.1)
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
Each of the above input parameters in the run script are mandatory and the software will throw an exception if any of them are not specified. A full listing of the input parameters is given below.
### Input Parameters
**Command Line**

**-out:** (mandatory)

Specify the output folder and name for the track information file. In addition to the name given the random seed and which thread the track was generated on will be appended to the filename. If the output filepath names a folder, the user must generate that folder, this application will not create an output folder for you.

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

Specify the name of the primary particle you would like to generate. The value given must be a name of a particle listed in the G4ParticleTable.

**/primary/energy [value] [unit]** (mandatory)

Specify the initial energy of the primary particle you create. Requires a value and associated energy unit.

### Example macro file

A macro file for MicroTrackGenerator to record 1 MeV proton tracks in a 3 mm cubic side length voxel is provided:
```
#Set verbosity
	/tracking/verbose 0
	/process/em/verbose 0
	/run/verbose 1

#geometry information
	/geometry/voxelSideLength 3 mm

#Initialize the run manager
	/run/initialize

#source information
	/primary/type proton
	/primary/energy 1 MeV

/run/beamOn 2
```

## Using the runTools

Using the methods described in the previous section, a user can run the application by making their own macro file and invoking the application from the command line or a script. We highly recommend however, that users planning to run MicroTrackGenerator use the bundled runTools to automatically create their own macro files and run scripts. The runTools are a Python2 based command line interface application. A Python2 installation with numpy is all that should be required to make use of the runTools. The runTools are designed to rapidly generate 1.) macro files and 2.) run files for computing cluster schedulers. The runTools are invoked on the command line from the runTools folder by: 
```
python2 MicroTrackGenerator_runTools.py
```
### Configuring the runTools

### Building with the runTools

## Description

MicroTrackGenerator has been developed to be as lightweight and simple as possible. Wherever possible, end-user control has been removed to prevent the user from running the software in an inappropriate way. The purpose of the software is to generate a track library for a given source of ionizing radiation at a series of  energies. Individual components of the software are described below.

### Geometry

All tracks originate from the center of the side of a cubic voxel whose size is specified by voxelSideLength, as shown in Figure 1. The initial momentum of all primary particles is directed into the voxel. When any particle in the track reaches the edge of the voxel it is terminated. 

<p align="center">
<img src="/docs/Track_Voxel_Box.jpg" width="500">
	
<em><b>Figure 1:</b> Diagram of simulation geometry. Primary track of interest, whose momentum is directed into the cubic voxel is depicted in red.</em>
</p>


The rationale behind the use of a voxel geometry is to prevent needless track information from being stored. When a particle exists in the energy spectrum of a voxel, only the portion of that particle's track which lies within that voxel will contribute to the microdosimetric spectra for that voxel. This being the case, if I am interested in the microdosimetric spectra of, for example, a monoenergetic 100 MeV proton beam, I only need to save the portion of that 100 MeV track which lies within the voxel and not the entire track. This saves storage space compared to recording the entire track length.

### Physics

The physics model Geant4DNA_Option2 is used for this application. Users are directed to the Geant4DNA collaboration's website (http://geant4-dna.in2p3.fr/styled-3/styled-8/index.html) and publications for further information on the physics model. This physics model was chosen in order to determine the most accurate information for nanometer scale energy depositions from particles of ionizing radiation, while also spanning an energy range appropriate for medical applications. If you attempt to simulate a primary particle which is not modelled by the Geant4DNA model or does not fall within the specified energy range (i.e. electrons of 1 MeV or greater, protons of 100 MeV or greater) then the software will not export any track information.

### Random Seeding

The seeding strategy is informed by a particular conversation on the Geant4 forums (https://geant4-forum.web.cern.ch/t/different-random-seeds-but-same-results/324/6). Sergio Losilla indicates that for a multithreaded application, a single seed can be used to uniquely seed the random number generators for each thread. I have performed some rudimentary verifications that this is the case and have observed that from a single seed on each thread unique tracks are generated.

Assuming an appropriately recent version of the Geant4 toolkit is compiled against, the MIMXMAX random number generator will be used by default. The MIXMAX generator is favored because a series of sequential random seeds can still yield uncorrelated random number generators.

### Output File Structure

The output file struture is depicted in Figure 2. Each thread the software is run on generates a unique .root file. The file contains a series of headers describing properties of the primary particle and the simulation geometry along with TTrees encoding track information.

<p align="center">
<img src="/docs/TrackFileFig.jpg" width="550">
	
<em><b>Figure 2:</b> Structure of a single .root output file generated by each thread.</em>
</p>
	
The "Tracks" TTree encondes x, y, z position in nanometers as well as energy deposited at each location in electron-Volts. The "Track index" TTree encodes a series of numbers which correspond with the entry number in the "Tracks" tree at which the last step of a given track occurs. The "Track index" TTree exists to speed up analysis times of the tracks as they won't have to be scanned through to determine where each track ends.

## Determining Microdosimetric Quantities from Track Library

At this time I am developing software to take the tracks generated from MicroTrackGenerator and superimpose them in volumes in order to efficiently calculate microdosimetric quantities. This software will be shared in its own respository once I believe it is at a sufficient stage of development. This readme will be updated to direct you to that software once it has been released. 

## Software License

Those wishing to use, modify, or reproduce this software must contact Joseph DeCunha at jdecunha@mdanderson.org to discuss an appropriate collaboration agreement. Copyright is claimed by Joseph M. DeCunha, 2021. All rights not expressly granted under this license are reserved.

This software is provided by the copyright holder "As is" and any express or implied warranties, including, but not limited to, implied warranties of merchantability, of satisfactory quality, and fitness for a particular purpose or use are disclaimed. The copyright holder makes no representation that the software and modifications thereof, will not infringe any patent, copyright, trade secret or other proprietary right.

The copyright holder shall have no liability for direct, indirect, special, incidental, consequential, exemplary, or punitive damages of any character including, without limitation, procurement of substitute goods or services, loss of use, data or profits, or business interruption, however caused and on any theory of contract, warranty, tort (including negligence), product liability or otherwise, arising in any way out of the use of this software, even if advised of the possibility of such damages.
