#!/usr/bin/Rscript
rm(list=ls())



args = commandArgs(trailingOnly=TRUE)
library(ggplot2)
#library(scales) # Accéder aux fonctions de transformations 
#library(gtable)
#library(grid)
#library(extrafont)# pour les font si bug executer dans R sous sudo extrafont::font_import()

readkey <- function()
{
    cat ("Press [enter] to continue")   
	invisible(scan("stdin", character(), nlines = 1, quiet = TRUE))
}
toread=F
if(length(args)>0)
{
dir_1=as.character(args[1])
}else{
dir_1="data5000"
}
if(length(args)<3){
toread=T
}else{
minx=as.double(args[2])
maxx=as.double(args[3])
}

varc="c1"

base_size=40
#X11(display = "", width=13, height=8)
theme_set(theme_classic(base_size=base_size))
#theme_update(axis.title.y = element_text(angle=0))

source("laser_tools.R")
T0 =T1<-Sys.time() 

poly_order=3
span=0.1


files_1=list.files(dir_1,full.name=T)
lr_1= length(list.files(dir_1))

print("data 1")


print("para reading")
para=sapply(files_1,getPara)

print("get m")
gm1=sapply(files_1,getm,length=4)
gm2=sapply(files_1,getm,mode=2,length=4)


colm1 = "#981414"
colm2= "#47c2e7"
sizeboule=3
sizefit=1


print("read occupancies")
#occ=read.csv("occupancies.csv")


raw_data=data.frame(Beta=para["beta",], c1=para["cavity_1",], c2=para["cavity_2",],m1= gm1["<m>",], var_m1=gm1["var(m)",],fano1=gm1["fano",],shutdown1=gm1["shutdown",]/para["time",], m2= gm2["<m>",], var_m2=gm2["var(m)",],fano2=gm2["fano",],shutdown2=gm2["shutdown",]/para["time",])#


#rm(size_histos,files,Histos,para,m,n)
#gc()
print("aggregate")

data_1=aggregate(list(m1=raw_data$m1,var_m1=raw_data$var_m1,fano1=raw_data$fano1,shutdown1=raw_data$shutdown1,
m2=raw_data$m2,fano2=raw_data$fano2,shutdown2=raw_data$shutdown2), by=list(Beta=raw_data$Beta,c1=raw_data$c1,c2=raw_data$c2),FUN=mean)

if(max(data_1$c1) > min(data_1$c1))
{
varc = "c1"
}else{
varc="c2"
}

if(toread){
data_1[,varc]
minx=min(data_1[,varc])
maxx=max(data_1[,varc])
}
#model_m1_1 <- lm(data_1$m1 ~ poly(data_1$c1,poly_order))
#predicted_m1_1 <- predict(model_m1_1,data.frame(x=data_1$c1),interval='confidence', level=0.99)
#dm1_1	<- diff(predicted_m1_1)/diff(data_1$c1)

#model_m2_1 <- lm(data_1$m2 ~ poly(data_1$c1,poly_order))
#predicted_m2_1 <- predict(model_m2_1,data.frame(x=data_1$c1),interval='confidence', level=0.99)
#dm2_1	<- diff(predicted_m2_1)/diff(data_1$c1)

masque=(data_1[,varc]>=minx & data_1[,varc]<=maxx)
data_masqued=data_1[masque,c("m1","m2")]
print("plot")
if(varc=="c1"){
plotm	<-	ggplot(data_1,aes(x=c1,y=m1))
model1 = lm(formula = m1 ~ c1, data = data_1)
model2 = lm(formula = m2 ~ c1, data = data_1)
x_values = data_1$c1
}else{
plotm	<-	ggplot(data_1,aes(x=c2,y=m1))
model1 = lm(formula = m1 ~ c2, data = data_1)
model2 = lm(formula = m2 ~ c2, data = data_1)
x_values = data_1$c2
}

data_fit = data.frame(x=x_values, m1 = model1$coefficients[2]*x_values + model1$coefficients[1], m2 = model2$coefficients[2]*x_values + model2$coefficients[1])

plotm	<-	plotm +
	geom_point(size=sizeboule,color=colm1)+
	geom_line(data=data_fit, aes(x=x,y=m1),color=colm1,size=sizefit)+
	geom_point(aes(y=m2), color=colm2,size=sizeboule)+
	geom_line(data=data_fit, aes(x=x,y=m2),color=colm2,size=sizefit)+

	
	xlab("α1")	+
	ylab("<m>")	


plotm = plotm + theme(panel.grid.major = element_line(colour = "grey80",size=0.5))
plotm = plotm + theme(panel.grid.minor = element_line(colour = "grey80",size=0.1))

plotm = plotm + theme(axis.text.x = element_text(colour="black"))
plotm = plotm + ggtitle(dir_1)
	
plotm = plotm + theme(axis.text.y = element_text(colour="black"))

plotm = plotm + coord_cartesian(
	xlim=c(minx,maxx),ylim=c(min(data_masqued),max(data_masqued))
)

#plotm = plotm + theme(text = element_text( family = "cmr10"))
	
ff=data_1$m1-data_1$m2

mid = which(abs(ff) == min(abs(ff)))
data_1[which(abs(ff) == min(abs(ff))),varc]

masque2= (mid-2):(mid+2)
data_masqued2=data_1[masque2,c("m1","m2","c1","c2")]
diff = data_masqued2$m2 - data_masqued2$m1

if(varc=="c1"){
model_eg = lm(formula = diff~c1, data = data_masqued2)$coefficients
}else{
model_eg = lm(formula = (diff)~c2, data = data_masqued2)$coefficients
}

center = (model1$coefficients[1] - model2$coefficients[1]) /
 (model2$coefficients[2] - model1$coefficients[2]) 
#center=-model_eg[1]/model_eg[2]

test = data.frame(x=data_fit$x, y= model_eg[2] * data_fit$x + model_eg[1])

plotm = plotm + geom_line(data=test, aes(x=x,y=y),color=1,size=sizefit)


#print(plotm)
name = paste(dir_1,"/../switch.pdf",sep='' )
#ggsave(name)

print(paste("switch mode :", varc ,"=", center))

#difftimff=data_1$m1-data_1$m2ff=data_1$m1-data_1$m2e(Sys.time() , T0)
#readkey()

