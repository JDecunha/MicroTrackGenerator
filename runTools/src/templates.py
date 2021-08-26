#!/usr/bin/env python2
# -*- coding: utf-8 -*-

macro_template = """#Set verbosity
	/tracking/verbose 0
	/process/em/verbose 0
	/run/verbose 1

#geometry information
	/geometry/voxelSideLength {sidelength} mm

#Initialize the run manager
	/run/initialize

#source information
	/primary/type {particle}
	/primary/energy {energy} MeV

/run/beamOn {nbeamon} """

run_command_template = """../build/MicroTrackGenerator -out {output_location} -mac {macro} -seed {seed} -mt NMAX \n"""
