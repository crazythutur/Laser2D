#!/usr/bin/python
import subprocess
import os
import psutil
import time
import sys
from numpy import *
import re
import time
import copy
from collections import defaultdict
import findLamb


lvl1=4998
lvl2=5002

cavity_1=0.6
cavity_2=0.5


cores = 12  


J=202.3
Beta=0.1
emitters=5

centre = 0.75
centre1 = centre
centre2 = centre
FWHM_1 = 0.5
FWHM_2 = 1.0


x=array(range(emitters))/(emitters-1.0)

parameters  = defaultdict(list)
parameters["cavity"].append(cavity_1)
parameters["cavity"].append(cavity_2)
parameters["emitters"].append(emitters)
parameters["modes"].append(2)
parameters["lasing_lvl"].append(lvl1)   
parameters["lasing_lvl"].append(lvl2)
parameters["beta"].append(Beta)
parameters["pump"].append(J)
parameters["time"].append(5000)
parameters["energy_levels"].append(10000)

seed_list={"cavity"}

#for emit in range(emitters):
#    parameters["cavity_coupling"].append(1.0/(FWHM_1*sqrt(2.0*pi))*exp(-1.0/2.0*((x[emit]-centre1)/FWHM_1)**2.0))
#    parameters["cavity_coupling"].append(1.0/(FWHM_2*sqrt(2.0*pi))*exp(-1.0/2.0*((x[emit]-centre2)/FWHM_2)**2.0))

cavity_coupling_1 = 1.0/(FWHM_1*sqrt(2.0*pi))*exp(-1.0/2.0*((x-centre1)/FWHM_1)**2.0)
cavity_coupling_2 = 1.0/(FWHM_2*sqrt(2.0*pi))*exp(-1.0/2.0*((x-centre2)/FWHM_2)**2.0)

#normalisation
if(FWHM_1 > FWHM_2):
	max_g2 = max(cavity_coupling_2)	
	cavity_coupling_2 = cavity_coupling_2 / max_g2
	cavity_coupling_1 = cavity_coupling_1 * sum(cavity_coupling_2) / sum(cavity_coupling_1)
else:
	max_g1 = max(cavity_coupling_1)	
	cavity_coupling_1 = cavity_coupling_1 / max_g1
	cavity_coupling_2 = cavity_coupling_2 * sum(cavity_coupling_1) / sum(cavity_coupling_2)


#cavity coupling attribution

for emit in range(emitters):
    parameters["cavity_coupling"].append(cavity_coupling_1[emit])
    parameters["cavity_coupling"].append(cavity_coupling_2[emit])


name = str(1)
findLamb.find_lamb(parameters, seed_list, name, cores)
