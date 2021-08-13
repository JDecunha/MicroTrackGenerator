#!/usr/bin/env python2
# -*- coding: utf-8 -*-
import os
import numpy as np
import random
import sys

"""
Created on Fri Aug 13 13:41:33 2021

@author: joseph
"""

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

lsf_template = """#!/bin/bash
#BSUB -W {walltime_request}
#BSUB -o {path_to_run_logfiles}
#BSUB -cwd {path_to_application_directory}
#BSUB -q {queue}
#BSUB -n {cores_per_node}
#BSUB -u jdecunha@mdanderson.org
#BSUB -J {job_name}

source /rsrch3/home/imag_phy/jdecunha/configure.sh

{run_command}
"""

seadragon_lsf_template = """#!/bin/bash
#BSUB -W {walltime_request}
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

def generate_macrofile(sidelength,particle,energy,nparticles):
    
    name = "%s_%sMeV" % (particle,energy)
    macro_filename = ""
    
    try:
        os.mkdir("../output/")
    except OSError as err: 
        if err[0] == 17: #error no. 17 is file exists
            pass
        else: #any other type of error then raise it
            raise
    
    try:
        os.mkdir("../output/%s" % particle)
    except OSError as err: 
        if err[0] == 17: #error no. 17 is file exists
            pass
        else: #any other type of error then raise it
            raise

    try:
        os.mkdir("../output/%s/%sMeV" % (particle,energy))
    except OSError as err: 
        if err[0] == 17: #error no. 17 is file exists
            pass
        else: #any other type of error then raise it
            raise          
            

    # Render the template
    macro_template_filled = macro_template.format(sidelength=sidelength,particle=particle,energy=energy,nbeamon=nparticles)

    with file("../macros/%s.mac" % name, "w") as f:
        f.write(macro_template_filled)
        macro_filename = "../macros/%s.mac" % name

    return macro_filename

def generate_macrofile_series(sidelength,particle,energy_lowlim,energy_highlim,energy_spacing,nparticles):
    
    energy_linspace = np.linspace(energy_lowlim,energy_highlim,float(energy_highlim-energy_lowlim)/float(energy_spacing)+1)
    macro_filenames = []
    
    for i in energy_linspace:
        energy = str(i)
        name = "%s_%sMeV" % (particle,energy)
        
        
        try:
            os.mkdir("../output/")
        except OSError as err: 
            if err[0] == 17: #error no. 17 is file exists
                pass
            else: #any other type of error then raise it
                raise
        
        
        try:
            os.mkdir("../output/%s" % particle)
        except OSError as err: 
            if err[0] == 17: #error no. 17 is file exists
                pass
            else: #any other type of error then raise it
                raise
    
        try:
            os.mkdir("../output/%s/%sMeV" % (particle,energy))
        except OSError as err: 
            if err[0] == 17: #error no. 17 is file exists
                pass
            else: #any other type of error then raise it
                raise          
                
        # Render the template
        macro_template_filled = macro_template.format(sidelength=sidelength,particle=particle,energy=energy,nbeamon=nparticles)
    
        with file("../macros/%s.mac" % name, "w") as f:
            f.write(macro_template_filled)
            macro_filenames.append(f)

    return macro_filenames

def determine_build_type():
    
   build_type = ""
   while build_type == "": #keep asking for input until a valid one is received
       
       cmd_input = raw_input("Do you want to configure a single run, or a series?: ")
        
       if cmd_input == "single":
           build_type = cmd_input
       elif cmd_input == "series":
           build_type = cmd_input
       else:
           print "Valid options are single or series."
   
   return build_type

def determine_single_properties():
    
   sidelength = ""

   while sidelength == "": #keep asking for input until a valid one is received
       
       cmd_input = raw_input("Input voxel side length in mm: ")
       
       try:
           cmd_input = float(cmd_input)
           sidelength = cmd_input
       except:
           print "Must input a valid number. No units."
           
   particle = ""
   while particle == "": #keep asking for input until a valid one is received
       
       cmd_input = raw_input("Input primary particle name: ")
       
       particle_name_string = "Primary particle name %s has been chosen. Type y to proceed: " % cmd_input
       cmd_input_2 = raw_input(particle_name_string)
       if cmd_input_2 == "y":
           particle = cmd_input
   
   energy = ""
   
   while energy == "": #keep asking for input until a valid one is received
       
       cmd_input = raw_input("Input primary particle energy in MeV: ")
       
       try:
           cmd_input = float(cmd_input)
           energy = cmd_input
       except:
           print "Must input a valid number. No units."
           
   nbeamon = ""
   
   while nbeamon == "": #keep asking for input until a valid one is received
       
       cmd_input = raw_input("Input number of primary particles to generate: ")
       
       try:
           cmd_input = int(cmd_input)
           nbeamon = cmd_input
       except:
           print "Must input a valid integer."
           
           
   walltime = ""

   while walltime == "": #keep asking for input until a valid one is received
       
       cmd_input = raw_input("Input walltime request: ")
       
       try:
           cmd_input = str(cmd_input)
           walltime = cmd_input
       except:
           print "Could not parse walltime request as a string. Try again."
           
   jobname = ""
   while jobname == "": #keep asking for input until a valid one is received
       
       cmd_input = raw_input("Input jobname for scheduler: ")
       
       try:
           cmd_input = str(cmd_input)
           jobname = cmd_input
       except:
           print "Could not parse jobname as a string. Try again."
           
   jobfiledir = "../"
   
   while jobfiledir == "../": #keep asking for input until a valid one is received
       
       cmd_input = raw_input("Input directory for jobfile relative to the MicroTrackGenerator main directory: ")
       
       try:
           cmd_input = str(cmd_input)
           jobfiledir += cmd_input
       except:
           print "Could not parse directory as a string. Try again."  
           
   macroNameAndPath = generate_macrofile(sidelength,particle,energy,nbeamon)
   generate_runfile(particle,energy,macroNameAndPath,walltime,jobname,jobfiledir)
   
   return 1


def determine_series_properties():
    
   sidelength = ""

   while sidelength == "": #keep asking for input until a valid one is received
       
       cmd_input = raw_input("Input voxel side length in mm: ")
       
       try:
           cmd_input = float(cmd_input)
           sidelength = cmd_input
       except:
           print "Must input a valid number. No units."
           
   particle = ""
   while particle == "": #keep asking for input until a valid one is received
       
       cmd_input = raw_input("Input primary particle name: ")
       
       particle_name_string = "Primary particle name %s has been chosen. Type y to proceed: " % cmd_input
       cmd_input_2 = raw_input(particle_name_string)
       if cmd_input_2 == "y":
           particle = cmd_input
   
   energy_lowlim = ""
   
   while energy_lowlim == "": #keep asking for input until a valid one is received
       
       cmd_input = raw_input("Input minimum primary particle energy for series in MeV: ")
       
       try:
           cmd_input = float(cmd_input)
           energy_lowlim = cmd_input
       except:
           print "Must input a valid number. No units."
           
   energy_highlim = ""
   
   while energy_highlim == "": #keep asking for input until a valid one is received
       
       cmd_input = raw_input("Input maximum primary particle energy for series in MeV: ")
       
       try:
           cmd_input = float(cmd_input)
       except:
           print "Must input a valid number. No units."
           
       if cmd_input <= energy_lowlim:
           print "Upper energy limit must be greater than lower energy limit."
       else:
           energy_highlim = cmd_input
           
   energy_spacing = ""
   
   while energy_spacing == "": #keep asking for input until a valid one is received
       
       cmd_input = raw_input("Input spacing between particle energy of series in MeV: ")
       
       try:
           cmd_input = float(cmd_input)
           energy_spacing = cmd_input
       except:
           print "Must input a valid number. No units."           
           
   nbeamon = ""
   
   while nbeamon == "": #keep asking for input until a valid one is received
       
       cmd_input = raw_input("Input number of primary particles to generate: ")
       
       try:
           cmd_input = int(cmd_input)
           nbeamon = cmd_input
       except:
           print "Must input a valid integer."
           
           
   return generate_macrofile_series(sidelength,particle,energy_lowlim,energy_highlim,energy_spacing,nbeamon)

def generate_runfile(particle,energy,macro,walltime,jobname,jobdir):
        
    try:
        os.mkdir(jobdir)
    except OSError as err: 
        if err[0] == 17: #error no. 17 is file exists
            pass
        else: #any other type of error then raise it
            raise

    # Render the template
    runfile_template_filled = run_command_template.format(output_location = ("../output/" + particle +"_" + str(energy) + "MeV/"),macro=macro,seed=str(random.randint(1,sys.maxint)))
    seadragon_template_filled = seadragon_lsf_template.format(walltime_request=walltime,job_name=jobname,jobdir=jobdir,run_command = runfile_template_filled)

    with file("%s/%s.lsf" % (jobdir,jobname) , "w") as f:
        f.write(seadragon_template_filled)

    return 1

if __name__ == "__main__":
    
    build_type = determine_build_type()
    
    if build_type == "single":
        macroName = determine_single_properties()
        
    if build_type == "series":
        determine_series_properties()
