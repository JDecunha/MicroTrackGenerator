#!/usr/bin/env python2
# -*- coding: utf-8 -*-
"""
Created on Mon Aug 16 12:17:15 2021

@author: joseph
"""

#CommandCondition is a class for a function, and a tuple containing arguments.
#CommandCondition provides a call function, which takes the command line input from the user from Command, and
#sends it to the condition function to be called, along with any additional arguments.
class CommandCondition(object):
    
    def __init__(self,func,args=None):
        self.func = func
        self.args = args
        
    def call(self,inputValue,args=None):
        
        if self.args == None:
            return self.func(inputValue)
        else:
            #the * operator, "unwraps" a tuple into individual elements. Basically sending each tuple member to the function call.
            return self.func(*(inputValue,self.args)) 
        
"""
Library of premade commandconditions.
Have your function return 0 for the condition to fail, or 1 to pass.
"""

""" Type Conditions """

class StringCondition(CommandCondition):
     
    def __init__(self):
        CommandCondition.__init__(self,self.condition)
    
    @staticmethod
    def condition(inputValue):

        returnval = 0
        
        try:
            inputValue = str(inputValue)
            returnval = 1
        except:
            print "Failed to parse as string. Try again."
            
        return returnval
    
class FloatCondition(CommandCondition):
     
    def __init__(self):
        CommandCondition.__init__(self,self.condition)
    
    @staticmethod
    def condition(inputValue):

        returnval = 0
        
        try:
            inputValue = float(inputValue)
            returnval = 1
        except:
            print "Failed to parse as float. Try again."
        
        return returnval
    
class IntCondition(CommandCondition):
     
    def __init__(self):
        CommandCondition.__init__(self,self.condition)
    
    @staticmethod
    def condition(inputValue):

        returnval = 0
        try:
            inputValue = int(inputValue)
            returnval = 1
        except:
            print "Failed to parse as int. Try again."

        return returnval

""" Comparison Conditions """

class GreaterThanCondition(CommandCondition):
     
    def __init__(self,args):
        CommandCondition.__init__(self,self.condition,args)
    
    @staticmethod
    def condition(inputValue,compareValue):

        if float(inputValue) < compareValue:
            print"input value is less than required %f" % float(compareValue)
            return 0
        else:
            return 1

class LesserThanCondition(CommandCondition):
     
    def __init__(self,args):
        CommandCondition.__init__(self,self.condition,args)
    
    @staticmethod
    def condition(inputValue,compareValue):

        if float(inputValue) > compareValue:
            print"input value is greater than required %f" % float(compareValue)
            return 0
        else:
            return 1
  
""" Special Conditions """
    
class ConfirmCondition(CommandCondition):
     
    def __init__(self):
        CommandCondition.__init__(self,self.condition)
    
    @staticmethod
    def condition(inputValue):

        returnval = -1
        
        while returnval == -1:
            confirmInput = raw_input("%s chosen (y/n): " % str(inputValue))
            if confirmInput == "y":
                returnval = 1
            elif confirmInput == "n":
                returnval = 0
        
        return returnval