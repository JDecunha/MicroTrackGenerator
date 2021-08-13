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

lsf_template = """#BSUB -W {walltime_request}
#BSUB -o {path_to_run_logfiles}
#BSUB -cwd {path_to_application_directory}
#BSUB -q {queue}
#BSUB -n {cores_per_node}
#BSUB -u jdecunha@mdanderson.org
#BSUB -J {job_name}

source /rsrch3/home/imag_phy/jdecunha/configure.sh

{run_command}
"""

seadragon_lsf_template = """#BSUB -W {walltime_request}
#BSUB -o /rsrch3/home/imag_phy/jdecunha/MicroTrackGenerator/run_logfiles
#BSUB -cwd /rsrch3/home/imag_phy/jdecunha/MicroTrackGenerator
#BSUB -q short
#BSUB -n 28
#BSUB -u jdecunha@mdanderson.org
#BSUB -J {job_name}

source /rsrch3/home/imag_phy/jdecunha/configure.sh

{run_command}
"""

run_command_template = """./build/MicroTrackGenerator -out {output_location} -mac {macro} -seed {seed} -mt NMAX \n"""
