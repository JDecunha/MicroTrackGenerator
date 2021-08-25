#!/usr/bin/env python2
# -*- coding: utf-8 -*-
#internal
import utils
import templates
import command
import commandcondition
#external
import random
import sys


"""
Created on Fri Aug 13 13:41:33 2021

@author: joseph
"""

def determine_single_properties(templateString):
      
   print "\n ** MACRO PROPERTIES: ** \n"
    
   sidelengthCommand = command.Command("Input voxel side length in mm: ")
   sidelengthCommand.AddCondition(commandcondition.FloatCondition())
   sidelength = float(sidelengthCommand.GetInput())
   
   particleCommand = command.Command("Input primary particle name: ")
   particleCommand.AddCondition(commandcondition.StringCondition())
   particleCommand.AddCondition(commandcondition.ConfirmCondition())
   particle = str(particleCommand.GetInput())
      
   energyCommand = command.Command("Input primary particle energy in MeV: ")
   energyCommand.AddCondition(commandcondition.FloatCondition())
   energy = float(energyCommand.GetInput())
        
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
           
   macroNameAndPath = generate_macrofile(sidelength,particle,energy,nbeamon)
   generate_runfile(particle,energy,macroNameAndPath,walltime,jobname,jobfiledir,templateString)
   
   print "build complete."
   
   return 1

def generate_macrofile(sidelength,particle,energy,nparticles):
    
    name = "%s_%sMeV" % (particle,energy)
    macro_filename = ""

    utils.make_directory("../output/")
    utils.make_directory("../output/%s" % particle)
    utils.make_directory("../output/%s/%sMeV" % (particle,energy))

    # Render the template
    macro_template_filled = templates.macro_template.format(sidelength=sidelength,particle=particle,energy=energy,nbeamon=nparticles)

    with file("../macros/%s.mac" % name, "w") as f:
        f.write(macro_template_filled)
        macro_filename = "../macros/%s.mac" % name

    return macro_filename

def generate_runfile(particle,energy,macro,walltime,jobname,jobdir,templateString):
        
    utils.make_directory(jobdir)

    # Render the template
    runfile_template_filled = templates.run_command_template.format(output_location = ("../output/" + particle +"_" + str(energy) + "MeV/"),macro=macro,seed=str(random.randint(1,sys.maxint)))
    seadragon_template_filled = templateString[0].format(walltime_request=walltime,job_name=jobname,jobdir=jobdir,run_command = runfile_template_filled)

    with file("%s/%s%s" % (jobdir,jobname,templateString[1]) , "w") as f:
        f.write(seadragon_template_filled)

    return 1
