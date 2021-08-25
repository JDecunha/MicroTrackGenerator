#!/usr/bin/env python2
# -*- coding: utf-8 -*-

import pickle
from src import configure_single, configure_series, utils, command, commandcondition

"""
Created on Fri Aug 13 13:41:33 2021

@author: joseph
"""
def print_intro_message():
    
    print "-------------------------------------"
    print "MicroTrackGenerator_runTools v 1.0 \n(c) Joseph DeCunha 2021"
    print "-------------------------------------"
    print "NOTICE: MicroTrackGenerator_runTools is dependant on relative paths.\nDo not move the application from its original folder.\n"
    
    return 1

def generate_base_directories():
    """ The user may not have the output or macros folders the first time they run this tool."""
    
    message = "Output folder does not currently exist in application directory. \nGenerating output folder.\n"
    utils.make_directory_and_print_message("../output/",message)
 
    message = "Macros folder does not currently exist in application directory. \nGenerating macros folder.\n"
    utils.make_directory_and_print_message("../macros/",message)

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

def configure_template():
    
    templatePathCommand = command.Command("Provide filename of desired run-file template within templates folder: ")
    templatePathCommand.AddCondition(commandcondition.StringCondition())
    template = str(templatePathCommand.GetInput())
    
    jobfileExtenstionCommand = command.Command("Provide desired file extension for your run-file: ")
    jobfileExtenstionCommand.AddCondition(commandcondition.StringCondition())
    jobfileExtension = str(jobfileExtenstionCommand.GetInput())
    
    templatefile = open("./templates/" + template)
    templatestring = templatefile.read()
    templatefile.close()
    
    persistency = []
    persistency.append(templatestring)
    persistency.append(jobfileExtension)
    
    pickle.dump(persistency, open("./data/current_template.p","wb"))
    print "Configuation succesfully completed."
    
    return 1

def application_update_persistent_elements():
    
    try:
        persistency = pickle.load(open("./data/current_template.p","rb"))
    except:
        print "Current template file not configured. Run configure to set appropriate path."
        return 0
    
    return persistency

if __name__ == "__main__":
    
    print_intro_message()
    generate_base_directories()
    templateString = application_update_persistent_elements()
    
    main_loop = 1
    
    while main_loop == 1:
        
       mainCommand = command.Command(">")
       mainCommand.AddCondition(commandcondition.StringCondition())
       main = str(mainCommand.GetInput())
       
       if main == "build" and templateString != 0:
    
           build_type = determine_build_type()
        
           if build_type == "single":
               configure_single.determine_single_properties(templateString)
            
           if build_type == "series":
               configure_series.determine_series_properties(templateString)
               
       elif main == "build" and templateString == 0:
           print "Run-file template not yet configured. Call configure to set path to your template."
           
       elif main == "configure":
           configure_template()
           templateString = application_update_persistent_elements()
           
       elif main == "quit":
           main_loop = 0
           
       else:
           print "Valid options are: build, configure, help, quit"
           
       print""
