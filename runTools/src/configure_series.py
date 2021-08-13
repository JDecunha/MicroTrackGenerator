#!/usr/bin/env python2
# -*- coding: utf-8 -*-
import os
import numpy as np
import templates
import random
import sys

"""
Created on Fri Aug 13 13:41:33 2021

@author: joseph
"""

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
           
   macro_filenames = generate_macrofile_series(sidelength,particle,energy_lowlim,energy_highlim,energy_spacing,nbeamon)
   generate_runfile_series(particle,energy_lowlim,energy_highlim,energy_spacing,macro_filenames,walltime,jobname,jobfiledir)
           
   return  1

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
        macro_template_filled = templates.macro_template.format(sidelength=sidelength,particle=particle,energy=energy,nbeamon=nparticles)
    
        with file("../macros/%s.mac" % name, "w") as f:
            f.write(macro_template_filled)
            macro_filenames.append("../macros/%s.mac" % name)

    return macro_filenames

def generate_runfile_series(particle,energy_lowlim,energy_highlim,energy_spacing,macronames,walltime,jobname,jobdir):
        
    try:
        os.mkdir(jobdir)
    except OSError as err: 
        if err[0] == 17: #error no. 17 is file exists
            pass
        else: #any other type of error then raise it
            raise
            
    energy_linspace = np.linspace(energy_lowlim,energy_highlim,float(energy_highlim-energy_lowlim)/float(energy_spacing)+1)
    
    with file("%s/%s.lsf" % (jobdir,jobname) , "w") as f:
        seadragon_template_filled = templates.seadragon_lsf_template.format(walltime_request=walltime,job_name=jobname,jobdir=jobdir,run_command = "")
        f.write(seadragon_template_filled)
    
        for i in range(0,len(energy_linspace)):
            energy = str(energy_linspace[i])
    
            # Render the template
            runfile_template_filled = templates.run_command_template.format(output_location = ("../output/" + particle +"_" + energy + "MeV/"),macro=macronames[i],seed=str(random.randint(1,sys.maxint)))
            f.write(runfile_template_filled)
        
            
            

    return 1

