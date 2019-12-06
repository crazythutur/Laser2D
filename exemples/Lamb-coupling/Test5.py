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


lvl1=398    # niveau laser 1
lvl2=402    # niveau kaser 2

nbniveaux=800   # nb total de niveau par bande

cavity_1=0.01   # alpha pour chaque cavite
cavity_2=0.01


cores =12  # nb de vcoeurs de calcul


J=5     # pompage
Beta=0.1    # comme son nom l'indique
_emitters=5 # nb de lasers individuels

u=1.0/(nbniveaux*_emitters) # equivalent a 1 transfert d electron / unite de temps

k=0 #couplage electrique

time=5000 # temps total de la simulation

nbmodes=2 # nombre de modes

x=array(range(_emitters))/(_emitters-1.0)

parameters  = defaultdict(list)
parameters["cavity"].append(cavity_1)
parameters["cavity"].append(cavity_2)
parameters["emitters"].append(_emitters)
parameters["modes"].append(nbmodes)
parameters["lasing_lvl"].append(lvl1)
parameters["lasing_lvl"].append(lvl2)
parameters["beta"].append(Beta)
parameters["pump"].append(J)
parameters["time"].append(time)
parameters["energy_levels"].append(nbniveaux)
parameters["electrical_coupling"].append(u*k)


seed_list={"cavity","emitters"}



name = str(_emitters)
findLamb.find_lamb(parameters, seed_list, name, cores)
