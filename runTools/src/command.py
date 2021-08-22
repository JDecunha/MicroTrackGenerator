#!/usr/bin/env python2
# -*- coding: utf-8 -*-
"""
Created on Sat Aug 14 13:09:28 2021

@author: joseph
"""

class Command(object):
    
    def __init__(self,message):
        
        self._message = message
        self._rawInput = ""
        self._input = ""
        self._conditions = list()
        
    def _PromptUserInput(self):
        self._rawInput = raw_input(self._message)
        
    def _CheckConditions(self):
        
        returnval = 1

        for condition in self._conditions:

            #if even 1 condition returns 0, returnval will stay 0 
            returnval = min(returnval,condition.call(self._rawInput))

        return returnval
    
    def AddCondition(self,condition):
                
        self._conditions.append(condition)
    
    def GetInput(self):
        
        while self._input == "":
            
            self._PromptUserInput()
            conditionflag = Command._CheckConditions(self)
            
            #if conditions not met, then while loop continues
            #and the user will be prompted to input again
            if conditionflag == 1: 
               self._input = self._rawInput
               
        return self._input