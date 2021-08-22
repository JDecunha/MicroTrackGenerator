#!/usr/bin/env python2
# -*- coding: utf-8 -*-

from src import configure_single, configure_series, utils

"""
Created on Fri Aug 13 13:41:33 2021

@author: joseph
"""
def print_intro_message():
    
    print "-------------------------------------"
    print "MicroTrackGenerator_runTools v 0.9. \n(c) Joseph DeCunha 2021"
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

if __name__ == "__main__":
    
    print_intro_message()
    generate_base_directories()
    
    build_type = determine_build_type()
    
    if build_type == "single":
        configure_single.determine_single_properties()
        
    if build_type == "series":
        configure_series.determine_series_properties()
