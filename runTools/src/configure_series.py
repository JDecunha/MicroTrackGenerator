#!/usr/bin/env python2
# -*- coding: utf-8 -*-
#internal
import templates
import utils
import command
import commandcondition
#external
import os
import numpy as np
import random
import sys

"""
Created on Fri Aug 13 13:41:33 2021

@author: joseph
"""

def determine_series_properties(templateString):
    
   print "\n ** MACRO PROPERTIES: ** \n"
    
   sidelengthCommand = command.Command("Input voxel side length in mm: ")
   sidelengthCommand.AddCondition(commandcondition.FloatCondition())
   sidelength = float(sidelengthCommand.GetInput())
   
   particleCommand = command.Command("Input primary particle name: ")
   particleCommand.AddCondition(commandcondition.StringCondition())
   particleCommand.AddCondition(commandcondition.ConfirmCondition())
   particle = str(particleCommand.GetInput())
      
   energylowCommand = command.Command("Input minimum primary particle energy for series in MeV: ")
   energylowCommand.AddCondition(commandcondition.FloatCondition())
   energy_lowlim = float(energylowCommand.GetInput())
   
   energyhighCommand = command.Command("Input maximum primary particle energy for series in MeV: ")
   energyhighCommand.AddCondition(commandcondition.FloatCondition())
   energyhighCommand.AddCondition(commandcondition.GreaterThanCondition(energy_lowlim))
   energy_highlim = float(energyhighCommand.GetInput())
   
   spacingCommand = command.Command("Input spacing type between energies (lin or log): ")
   spacingCommand.AddCondition(commandcondition.StringCondition())
   spacingType = str(spacingCommand.GetInput())
   
   energyString = ""
   if spacingType == "lin":
       energyString = "Input spacing between particle energy of series in MeV: "
   elif spacingType == "log":
       energyString = "Input number of discrete energies to simulate: "
   else:
       raise Exception("You made a typo. Insert 25 cents to continue.")
   
   energyspacingCommand = command.Command(energyString)
   energy_spacing = 0.
   
   if spacingType == "lin":
       energyspacingCommand.AddCondition(commandcondition.FloatCondition())
       energy_spacing = float(energyspacingCommand.GetInput())
   elif spacingType == "log":
       energyspacingCommand.AddCondition(commandcondition.IntCondition())
       energy_spacing = int(energyspacingCommand.GetInput())
   
   nbeamonCommand = command.Command("Input number of primary particles to generate: ")
   nbeamonCommand.AddCondition(commandcondition.IntCondition())
   nbeamon = int(nbeamonCommand.GetInput())
   
   print "\n ** RUNFILE PROPERTIES: ** \n"
                      
   walltimeCommand = command.Command("Input walltime request: ")
   walltimeCommand.AddCondition(commandcondition.StringCondition())
   walltime = str(walltimeCommand.GetInput())
           
   jobNameCommand = command.Command("Input jobname for scheduler: ")
   jobNameCommand.AddCondition(commandcondition.StringCondition())
   jobname = str(jobNameCommand.GetInput())

   fileDirCommand = command.Command("Input directory for jobfile relative to the MicroTrackGenerator main directory: ")
   fileDirCommand.AddCondition(commandcondition.StringCondition())
   jobfiledir = "../" + str(fileDirCommand.GetInput())
           
   macro_filenames = generate_macrofile_series(sidelength,particle,energy_lowlim,energy_highlim,energy_spacing,nbeamon,spacingType)
   generate_runfile_series(particle,energy_lowlim,energy_highlim,energy_spacing,macro_filenames,walltime,jobname,jobfiledir,templateString,spacingType)
   
   print "build complete."        
   
   return  1

def generate_macrofile_series(sidelength,particle,energy_lowlim,energy_highlim,energy_spacing,nparticles,spacingType):
    
    
    energy_linspace = []
    
    if spacingType == "lin":
        energy_linspace = np.linspace(energy_lowlim,energy_highlim,float(energy_highlim-energy_lowlim)/float(energy_spacing)+1)
    elif spacingType == "log":
        energy_linspace = np.geomspace(energy_lowlim,energy_highlim,energy_spacing)
        
    macro_filenames = []
    
    for i in energy_linspace:
        energy = str(float(i))
        name = "%s_%sMeV" % (particle,energy)
        
        utils.make_directory("../output/")
        utils.make_directory("../output/%s" % particle)
        utils.make_directory("../output/%s/%sMeV" % (particle,energy))
        
        # Render the template
        macro_template_filled = templates.macro_template.format(sidelength=sidelength,particle=particle,energy=energy,energybinwidth=energy_spacing,nbeamon=nparticles)
        
        if spacingType == "log":
            macro_template_filled = templates.macro_template.format(sidelength=sidelength,particle=particle,energy=energy,energybinwidth=0.,nbeamon=nparticles)
            
    
        with file("../macros/%s.mac" % name, "w") as f:
            f.write(macro_template_filled)
            macro_filenames.append("../macros/%s.mac" % name)

    return macro_filenames

def generate_runfile_series(particle,energy_lowlim,energy_highlim,energy_spacing,macronames,walltime,jobname,jobdir,templateString,spacingType):
        
    try:
        os.mkdir(jobdir)
    except OSError as err: 
        if err[0] == 17: #error no. 17 is file exists
            pass
        else: #any other type of error then raise it
            raise
            
    energy_linspace = []
    
    if spacingType == "lin":
        energy_linspace = np.linspace(energy_lowlim,energy_highlim,float(energy_highlim-energy_lowlim)/float(energy_spacing)+1)
    elif spacingType == "log":
        energy_linspace = np.geomspace(energy_lowlim,energy_highlim,energy_spacing)
    
    with file("%s/%s%s" % (jobdir,jobname,templateString[1]) , "w") as f:
        seadragon_template_filled = templateString[0].format(walltime_request=walltime,job_name=jobname,jobdir=jobdir,run_command = "")
        f.write(seadragon_template_filled)
    
        for i in range(0,len(energy_linspace)):
            energy = str(float(energy_linspace[i]))
    
            # Render the template
            runfile_template_filled = templates.run_command_template.format(output_location = ("../output/" + particle + "/" + str(energy) + "MeV/"),macro=macronames[i],seed=str(random.randint(1,sys.maxint)))
            f.write(runfile_template_filled)
        
            
            

    return 1

