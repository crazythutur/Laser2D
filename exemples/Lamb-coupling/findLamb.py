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
import fnmatch
from collections import defaultdict









#--------------------------------------------------






def run(directory, num, parameters, seed_list, bg):
	if(bg):
		last_chr=' &'
	else:
		last_chr=''
	command = './laser2D.exe --seed=' + str(num) + " --seed=" + str(random.randint(1, 35164564))+  ' -o ' + directory + 'out_' +str(num) +'_c1_' +str(parameters["cavity"][0])+'_c2_' +str(parameters["cavity"][1])+'.csv' 
	
	for param in parameters.items():
		for value in param[1]:
			command = command + " --"  +param[0] + "=" + str(value)
	seedi=1
	for seed in seed_list :
		seedi=1
		for param in parameters[seed] : 
			seedi = (seedi * hash(param))%2147483647
		command = command + " --seed=" + str(seedi)
	
	command = command   + last_chr
	while psutil.virtual_memory().percent>80:
		time.sleep(1)
	#print(command)
	os.system(command)
	
	
	
	

def run_multiple(directory, parameters, seed_list, cores):	
	for k in  range(1,cores):
		run(directory, k,  parameters, seed_list, True)
	
	run(directory, cores, parameters, seed_list, False)
	

#--------------------------------------------------

def find_lamb(parameters, seed_list, name, cores) :

	start = time.time()
	borne =0.001
	
	random.seed()
	
	directory="data" + name
	dir_Jg= directory + "/dataJ1/"
	dir_J= directory + "/dataJ2/"
	dir_1= directory + "/data1/"
	dir_2= directory + "/data2/"
	
	if not os.path.exists(directory):
		os.makedirs(directory)
		os.makedirs(dir_Jg)
		os.makedirs(dir_J)
		os.makedirs(dir_1)
		os.makedirs(dir_2)

        #sys.stdout = open(directory + "/logs.txt", 'w')
        #sys.err = open(directory + "/err.txt", 'w')
	pattern = re.compile(r"<m>=([\d\.]+)")

	cavity_2_mark=1
	parameters["cavity"][1] = 0.5
	test_eq_point=True
	compteur = 1

	#*************************************************
	#* RECHERCHE DE L EGALITE DES MODES GROSSIERE ****
	#*************************************************

	print("recherche de l egalite des modes par dichotomie, time = " + str( time.time() - start) )

	interval = 0.2

	centre = 0.3501
        sys.stdout.flush()
	parameters_tmp_1 =  copy.deepcopy(parameters)
	parameters_tmp_2 =  copy.deepcopy(parameters)
	half_cores = cores /2
	
        
	while(test_eq_point):
		#Lance les runs
		compteur = compteur +1
		
		parameters_tmp_1["cavity"][1]= centre - interval/2.0
		parameters_tmp_2["cavity"][1]= centre + interval/2.0

		
		run_multiple(dir_Jg, parameters_tmp_1, seed_list, cores)
		run_multiple(dir_Jg, parameters_tmp_2, seed_list, cores)

		m1_N1=list()
    	        m1_N2=list()
		m2_N1=list()
    	        m2_N2=list()
		
		patter_N1 = "out_*_c1_0.6_c2_" + str(parameters_tmp_1["cavity"][1]) + ".csv"
		patter_N2 = "out_*_c1_0.6_c2_" + str(parameters_tmp_2["cavity"][1]) + ".csv"
		
		x_1 =  parameters_tmp_1["cavity"][1]
		x_2 =  parameters_tmp_2["cavity"][1]
		while subprocess.Popen("find " + dir_Jg +" -empty", shell=True, stdout=subprocess.PIPE).stdout.read().decode()[:-1] !='' :
			time.sleep(1)

		for f in os.listdir(dir_Jg):
			
			if fnmatch.fnmatch(f, patter_N1):
				fo=open(dir_Jg + f,"r")
				fo.readline()
				m1_N1.append(float(pattern.findall(fo.readline())[0]))
				m2_N1.append(float(pattern.findall(fo.readline())[0]))
				fo.close()
			elif fnmatch.fnmatch(f, patter_N2):
				fo=open(dir_Jg + f,"r")
				fo.readline()
				m1_N2.append(float(pattern.findall(fo.readline())[0]))
				m2_N2.append(float(pattern.findall(fo.readline())[0]))
				fo.close()
		print(patter_N1)

                N1_arr = (array(m1_N1) - array(m2_N1))/(array(m1_N1) + array(m2_N1))  
		N1=mean(N1_arr)  
		N2_arr=(array(m1_N2) - array(m2_N2))/(array(m1_N2) + array(m2_N2))  
		N2=mean(N2_arr)  
                sd_N12 = std(N1_arr) + std(N2_arr)
		a = (N2-N1) / (x_2 - x_1)
		b = (N2 - a * x_2 + N1 - a * x_1 ) / 2.0
		x_0 = -b / a 

                if 	x_0 < x_1 :#decale a gauche
                            centre = centre - interval*(1.0-2.0**(-x_1/interval)) 

                elif	x_0 > x_2 :#decale a droite
                        centre = centre + interval

                else:
                        centre = x_0
                        interval = interval / (2.0+ 0.001/sd_N12)

			#f = open("data5005/dataJ/out_1_c1_0.6425_c2_0.57.csv", "r")
			
		print('test : '+ str(compteur-1) + ", X0 = " + str(x_0) + " x1 " + str(x_1) + ", x_2 = "  + str(x_2)   +  ", N2 = " + str(N2) +  ", N1 = " + str(N1)+ " a = " + str(a) + ", b = " + str(b) + "sum sd = " + str(std(N1_arr) + std(N2_arr) ))
	        sys.stdout.flush()

		if(interval<borne):
			test_eq_point=False
		
	#********************************************
	#* RECHERCHE DE L EGALITE DES MODES FINE ****
	#********************************************	
	print("recherche de l egalite des modes fine, time = " + str( time.time() - start) + "cavity_2 grossier = " + str(centre))
        sys.stdout.flush()
	borne = 0.0025
	nmb_i=8
	compteur=0
	#parameters_tmp =  copy.deepcopy(parameters)

	min_i=centre-borne
	max_i=centre+borne


	parameters_tmp =  copy.deepcopy(parameters)
        parameters_tmp["time"][0]=parameters["time"][0] 

	for i in linspace(min_i,max_i,nmb_i):
		compteur= compteur+1
		
		print("run ", compteur , " / ", nmb_i)
                sys.stdout.flush()
		parameters_tmp["cavity"][1] = i
		run_multiple(dir_J, parameters_tmp, seed_list, cores)
		
	while subprocess.Popen("find " + dir_J +" -empty", shell=True, stdout=subprocess.PIPE).stdout.read().decode()[:-1] !='' :
		time.sleep(1)


	os.system("./Analyse.R " + dir_J + " 2> /dev/null  | grep -P -o 'switch mode : c2 = \K(\d+\.\d+)' > "+ directory + "/eq_point.txt")
	#os.system("echo 0.5 >> " + directory + "/eq_point.txt")
        sys.stdout.flush()
	
        while not(os.path.exists(directory + "/eq_point.txt")) or (subprocess.Popen("find " + directory + "/eq_point.txt" +" -empty", shell=True, stdout=subprocess.PIPE).stdout.read().decode()[:-1] !='') :
		time.sleep(1)
	
	fo=open(directory + "/eq_point.txt","r")
	eq_point=float(fo.readline())
	fo.close()
	#os.remove(directory + "/eq_point.txt")

	parameters["cavity"][1] = eq_point

	#********************************************
	#* VARIATION DES FUITES DE LA CAVITE ********
	#********************************************	
	print("Variation des fuites de la cavite du mode 1, time = " + str( time.time() - start) ) 
        sys.stdout.flush()
	min_i=parameters["cavity"][0]-borne
	max_i=parameters["cavity"][0]+borne

	compteur=0
	nmb_i=32
	parameters_tmp =  copy.deepcopy(parameters)

	for i in linspace(min_i,max_i,nmb_i):
		compteur= compteur+1
		
		print("run ", compteur , " / ", nmb_i*2)
	        sys.stdout.flush()	
		parameters_tmp["cavity"][0] = i
		run_multiple(dir_1, parameters_tmp, seed_list, cores)
		

	print("Variation des fuites de la cavite du mode 2, time = " + str( time.time() - start) ) 
	min_i=parameters["cavity"][1]-borne
	max_i=parameters["cavity"][1]+borne

	parameters_tmp =  copy.deepcopy(parameters)

	for i in linspace(min_i,max_i,nmb_i):
		compteur= compteur+1
		
		print("run ", compteur , " / ", nmb_i*2)
                sys.stdout.flush()
		parameters_tmp["cavity"][1] = i
		
		run_multiple(dir_2, parameters_tmp, seed_list, cores)

	while subprocess.Popen("find " + directory +"/data*/ -empty", shell=True, stdout=subprocess.PIPE).stdout.read().decode()[:-1] !='' :
		time.sleep(1)


	os.system("( cd " + directory +  " ; ../Analyse_Lamb.R )  | grep 'niveau' >  " + directory +  "/resultat.txt")

	print("end of finding the lamb factor, results are located in " + directory +  "/resultat.txt" + ", time = " + str( time.time() - start) )


