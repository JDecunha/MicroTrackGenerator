#!/usr/bin/env python2
# -*- coding: utf-8 -*-
"""
Created on Sat Aug 14 12:36:34 2021

@author: joseph
"""

import os

def make_directory(directory):
    
    try:
        os.mkdir(directory)
    except OSError as err: 
        if err[0] == 17: #error no. 17 is file exists
            pass
        else: #any other type of error then raise it
            raise
            
    return 1

def make_directory_and_print_message(directory,message):
    
    try:
        os.mkdir(directory)
        print message
    except OSError as err: 
        if err[0] == 17: #error no. 17 is file exists
            pass
        else: #any other type of error then raise it
            raise
            
    return 0 