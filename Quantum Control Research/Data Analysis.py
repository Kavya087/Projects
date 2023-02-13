#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu Oct  6 10:36:41 2022

@author: kavya
"""

import matplotlib.pyplot as plt
import numpy as np

delta2=[]
real_rho33= []
imag_rho33= []


for line in open('DeltaChange.txt', 'r'): 
    lines = [i for i in line.split(',')]
    delta2.append(float(lines[0]))
    real_rho33.append(float(lines[1]))
    imag_rho33.append(float(lines[2]))
    
plt.plot(delta2, real_rho33, 'indigo')
plt.plot(delta2, real_rho33, 'k.')
plt.xlabel('Delta 2')
plt.ylabel('Rho33 Real')
plt.show()