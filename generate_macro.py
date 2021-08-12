import os
import numpy as np

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

niagara_macro_template = """#!/bin/bash
#SBATCH --nodes=1
#SBATCH --cpus-per-task=40
#SBATCH --time={time}
#SBATCH --job-name {slurmname}
#SBATCH -o ./slurm_output/%x_%j.out

cd /scratch/s/senger/jdecunha/eventdose

source /home/s/senger/jdecunha/configure.sh

for i in {{1..40}};
do ./build/sim {macroname} $( shuf -i 1-2000000000 -n 1 ) $( shuf -i 1-2000000000 -n 1 ) & sleep 1.1
done
wait"""

def generate_macrofile(sidelength,particle,energy,nparticles):
    
    name = "%s_%sMeV" % (particle,energy)
    
    try:
        os.mkdir("output/")
    except OSError as err: 
        if err[0] == 17: #error no. 17 is file exists
            pass
        else: #any other type of error then raise it
            raise
    
    try:
        os.mkdir("output/%s" % particle)
    except OSError as err: 
        if err[0] == 17: #error no. 17 is file exists
            pass
        else: #any other type of error then raise it
            raise

    try:
        os.mkdir("output/%s/%sMeV" % (particle,energy))
    except OSError as err: 
        if err[0] == 17: #error no. 17 is file exists
            pass
        else: #any other type of error then raise it
            raise          
            

    # Render the template
    macro_template_filled = macro_template.format(sidelength=sidelength,particle=particle,energy=energy,nbeamon=nparticles)

    with file("macros/%s.mac" % name, "w") as f:
        f.write(macro_template_filled)

    return 1 

def generate_macrofile_series(sidelength,particle,energy_lowlim,energy_highlim,energy_spacing,nparticles):
    
    energy_linspace = np.linspace(energy_lowlim,energy_highlim,float(energy_highlim-energy_lowlim)/float(energy_spacing)+1)
    
    for i in energy_linspace:
        energy = str(i)
        name = "%s_%sMeV" % (particle,energy)
        
        
        try:
            os.mkdir("output/")
        except OSError as err: 
            if err[0] == 17: #error no. 17 is file exists
                pass
            else: #any other type of error then raise it
                raise
        
        
        try:
            os.mkdir("output/%s" % particle)
        except OSError as err: 
            if err[0] == 17: #error no. 17 is file exists
                pass
            else: #any other type of error then raise it
                raise
    
        try:
            os.mkdir("output/%s/%sMeV" % (particle,energy))
        except OSError as err: 
            if err[0] == 17: #error no. 17 is file exists
                pass
            else: #any other type of error then raise it
                raise          
                
        # Render the template
        macro_template_filled = macro_template.format(sidelength=sidelength,particle=particle,energy=energy,nbeamon=nparticles)
    
        with file("macros/%s.mac" % name, "w") as f:
            f.write(macro_template_filled)

    return 1 

def determine_build_type():
    
   build_type = ""
   while build_type == "": #keep asking for input until a valid one is received
       
       cmd_input = raw_input("Do you want to build a single macro file, or a series?: ")
        
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
           
           
   generate_macrofile(sidelength,particle,energy,nbeamon)
   
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
           
           
   generate_macrofile_series(sidelength,particle,energy_lowlim,energy_highlim,energy_spacing,nbeamon)
   
   return 1

if __name__ == "__main__":

    build_type = determine_build_type()
    
    if build_type == "single":
        determine_single_properties()
        
    if build_type == "series":
        determine_series_properties()