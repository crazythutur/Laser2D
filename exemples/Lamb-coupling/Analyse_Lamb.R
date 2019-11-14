#!/usr/bin/Rscript
rm(list=ls())
library(ggplot2)

source("../laser_tools.R")
T0 =T1<-Sys.time() 

incertitude <- function(model, x, y){

x=model$model[,2]
y=model$model[,1]
n= length(x)
if (n!=length(y) || n==0)
	{
		print("error incertitude")
		return(0)
	}

incertitude = 2/sqrt(n)*sqrt(
	(1/(n-2)*sum((y - model$coefficients[2]*x - model$coefficients[1])^2) ) /
	(sum( (x-ave(x) ) ^2))
)

#incertitude = 2* sqrt( 1/((n-1)*n) * sum((y - model$coefficients[2]*x - model$coefficients[1])^2))
return (incertitude)
}


signif.ceiling <- function(x, n){
  pow <- floor( log10( abs(x) ) ) + 1 - n
  y <- ceiling(x / 10 ^ pow) * 10^pow
  # handle the x = 0 case
  y[x==0] <- 0
  y
}

print_incertitude <- function(x,ux, name)
{

ux_rond= signif.ceiling(2*ux,2)
x_rond=round(x=x,digits=abs(floor(log10(ux_rond)) + 1)+2)
print(paste(name, "=", x_rond, "+/-" , ux_rond, "avec un niveau de confiance à 95%"))

}


dir_1="data1"
files_1=list.files(dir_1,full.name=T)
lr_1= length(list.files(dir_1))

dir_2="data2"
files_2=list.files(dir_2,full.name=T)
lr_2= length(list.files(dir_2))

print("reading data 1")

para_1=sapply(files_1,getPara)

gm1_1=sapply(files_1,getm, length= 4)
gm2_1=sapply(files_1,getm,mode=2, length= 4)

print("reading data 2")

para_2=sapply(files_2,getPara)

gm1_2=sapply(files_2,getm, length= 4)
gm2_2=sapply(files_2,getm,mode=2, length= 4)


print("processing")
raw_data_a1=data.frame(Beta=para_1["beta",], c1=para_1["cavity_1",], c2=para_1["cavity_2",],m1= gm1_1["<m>",], var_m1=gm1_1["var(m)",],fano1=gm1_1["fano",],shutdown1=gm1_1["shutdown",]/para_1["time",], m2= gm2_1["<m>",], var_m2=gm2_1["var(m)",],fano2=gm2_1["fano",],shutdown2=gm2_1["shutdown",]/para_1["time",])#



raw_data_a2=data.frame(Beta=para_2["beta",], c1=para_2["cavity_1",], c2=para_2["cavity_2",],m1= gm1_2["<m>",], var_m1=gm1_2["var(m)",],fano1=gm1_2["fano",],shutdown1=gm1_2["shutdown",]/para_2["time",], m2= gm2_2["<m>",], var_m2=gm2_2["var(m)",],fano2=gm2_2["fano",],shutdown2=gm2_2["shutdown",]/para_2["time",])#



data_truc_a1 = raw_data_a1#[(raw_data_a1$c1>0.615 &  raw_data_a1$c1<0.625),]
data_truc_a2 = raw_data_a2#[(raw_data_a2$c2>0.565 &  raw_data_a2$c2<0.575),]

dS1da1_model = lm(formula = m1 ~ c1, data = data_truc_a1)
dS1da2_model = lm(formula = m1 ~ c2, data = data_truc_a2)
dS2da1_model = lm(formula = m2 ~ c1, data = data_truc_a1)
dS2da2_model = lm(formula = m2 ~ c2, data = data_truc_a2)

#calcul des position d'égalité des modes

a1=as.numeric(-(dS2da1_model$coefficients[1]-dS1da1_model$coefficients[1])/(dS2da1_model$coefficients[2]-dS1da1_model$coefficients[2]))
a2=as.numeric(-(dS2da2_model$coefficients[1]-dS1da2_model$coefficients[1])/(dS2da2_model$coefficients[2]-dS1da2_model$coefficients[2]))

a1 = 0.6194 # valeur trouvé par l'ancien programme
a2 = 0.570653

dS1da1 = dS1da1_model$coefficients[2]#*a1 + dS1da1_model$coefficients[1]
dS1da2 = dS1da2_model$coefficients[2]#*a2 + dS1da2_model$coefficients[1]
dS2da1 = dS2da1_model$coefficients[2]#*a1 + dS2da1_model$coefficients[1]
dS2da2 = dS2da2_model$coefficients[2]#*a2 + dS2da2_model$coefficients[1]

print(paste("R²(dS1/da1) = ", summary(dS1da1_model)$r.square))
print(paste("R²(dS1/da2) = ", summary(dS1da2_model)$r.square))
print(paste("R²(dS2/da1) = ", summary(dS2da1_model)$r.square))
print(paste("R²(dS2/da2) = ", summary(dS2da2_model)$r.square))





K12 = - dS1da2/dS2da2
K21 = - dS2da1/dS1da1
C=K12*K21

u_12=sqrt( (incertitude(dS1da2_model)/dS1da2)^2 + (incertitude(dS2da2_model)/dS2da2)^2)
u_21=sqrt( (incertitude(dS2da1_model)/dS2da1)^2 + (incertitude(dS1da1_model)/dS1da1)^2)

u_C=sqrt((u_12)^2 + (u_21)^2 )*C

#rm(size_histos,files,Histos,para,m,n)
#gc()

print_incertitude(K12,u_12, "K12")
print_incertitude(K21,u_21, "K21")
print_incertitude(C,u_C, "C")

difftime(Sys.time() , T0)


