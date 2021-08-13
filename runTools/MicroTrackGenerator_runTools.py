#!/usr/bin/env python2
# -*- coding: utf-8 -*-
import os
from src import configure_single, configure_series

"""
Created on Fri Aug 13 13:41:33 2021

@author: joseph
"""

def generate_base_directories():
    """ The user may not have output of macros folders the first time they run this tool."""
    
    try:
        os.mkdir("../output/")
        print "Output folder does not currently exist in application directory. \nGenerating output folder.\n"
    except OSError as err: 
        if err[0] == 17: #error no. 17 is file exists
            pass
        else: #any other type of error then raise it
            raise
 
    try:
        os.mkdir("../macros/")
        print "Macros folder does not currently exist in application directory. \nGenerating macros folder.\n"
    except OSError as err: 
        if err[0] == 17: #error no. 17 is file exists
            pass
        else: #any other type of error then raise it
            raise

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
    
    generate_base_directories()
    build_type = determine_build_type()
    
    if build_type == "single":
        configure_single.determine_single_properties()
        
    if build_type == "series":
        configure_series.determine_series_properties()
