#!/usr/bin/Rscript
rm(list=ls())
library(ggplot2)
library(scales) # Accéder aux fonctions de transformations 
library(extrafont)# pour les font si bug executer dans R sous sudo extrafont::font_import()

library(reshape2)
base_size=40
X11(display = "", width=13, height=5)
theme_set(theme_minimal(base_size=base_size))
source("/home/thutur/Rwork/script/laser_tools.R")

colC=1
colK12=2
colK21=3

sizeboule=1*base_size/10
sizeline=0.5*base_size/10
sizetext=3*base_size/10
para = "emitters"

textr2 = function(model, name){

return( paste(name, "=",  signif(digits=3,model$coefficients[2]), "x ",para ," +", signif(digits=3,model$coefficients[1]) ,"(R² =", signif(digits=3,summary(model)$r.square), ")" ))
}

data=c("data1","data2","data3","data4","data5","data6")
df=1:6
K12=vector(mode='numeric')
K21=vector(mode='numeric')
uK12=vector(mode='numeric')
uK21=vector(mode='numeric')
C=vector(mode='numeric')
uC=vector(mode='numeric')


for (f in data){
	file =paste(f,"resultat.txt",sep="/")
	K12 = append(K12, as.numeric(strsplit(as.character(read.table(file,skip=0,nrow=1)[[2]])," ")[[1]][3]))
print(K12)
	uK12 = c(uK12, as.numeric(strsplit(as.character(read.table(file,skip=0,nrow=1)[[2]])," ")[[1]][5]))
	K21 = c(K21, as.numeric(strsplit(as.character(read.table(file,skip=1,nrow=1)[[2]])," ")[[1]][3]))
	uK21 = c(uK21, as.numeric(strsplit(as.character(read.table(file,skip=1,nrow=1)[[2]])," ")[[1]][5]))
	C = c(C, as.numeric(strsplit(as.character(read.table(file,skip=2,nrow=1)[[2]])," ")[[1]][3]))
	uC = c(uC, as.numeric(strsplit(as.character(read.table(file,skip=2,nrow=1)[[2]])," ")[[1]][5]))
}




C2=K12*K21


data=data.frame(df,K12,uK12,K21,uK21,C,uC,C2)
dataconf=data.frame(
	K12_min=data$K12-data$uK12,K12_max=data$K12+data$uK12,
	K21_min=data$K21-data$uK21,K21_max=data$K21+data$uK21,
	C_min=data$C-data$uC,C_max=data$C+data$uC,
	df=data$df
)

C_model = lm(formula = C ~ df, data = data)
K12_model = lm(formula = K12 ~ df, data = data)
K21_model = lm(formula = K21 ~ df, data = data)
C_label = paste("C =",  signif(digits=3,C_model$coefficients[2]), "x df +", signif(digits=3,C_model$coefficients[1]) ,"(R² =", signif(digits=3,summary(C_model)$r.square), ")" )


print_int = function(param, plot, color){
	
	plot = plot + geom_point(data=data, mapping=aes(x=df, y= data[,param],color=param), 				size=sizeboule)
	plot = plot + geom_line(data=data, mapping=aes(x=df, y= data[,param],color=param),size=sizeline)
	plot = plot + geom_line(data=dataconf, mapping=aes(x=df, y=dataconf[,paste(param,"min",sep='_')],color=param) ,size=sizeline/10.0)
	plot = plot + geom_line(data=dataconf, mapping=aes(x=df, y=dataconf[,paste(param,"max",sep='_')],color=param) ,size=sizeline/10.0)
	
}


plot	<- ggplot()
plot = print_int("C", plot, colC)

	
	plot = plot + xlab(para)	+
	ylab("C")	
	#plot =  plot + scale_x_log10(minor_breaks=log10_minor_break())+
  	#theme(panel.grid.major.x = element_line(size=1),
	#panel.grid.minor.x = element_line(size=0.5))

plot = plot + theme(panel.grid.major = element_line(colour = "grey80",size=0.5))
plot = plot + theme(panel.grid.minor = element_line(colour = "grey80",size=0.1))

plot = plot + theme(axis.text.x = element_text(colour="black"))
	
plot = plot + theme(axis.text.y = element_text(colour="black"))

plot = plot + theme(text = element_text( family = "cmr10"))

print(plot)

#plot = plot + geom_text(aes(label =textr2(C_model, "C"), y = 0.959, x= 0.75),size=sizetext,family = "cmr10")
 


ggsave("C.pdf")


plot2 <-	ggplot(data)
plot2 = plot2 + scale_color_manual(values=c("#e66101","#5e3c99"))

	plot2 = print_int("K12", plot2, "K12")
	plot2 = print_int("K21", plot2, "K21")

	plot2 = plot2 + xlab(para)	+
	ylab("")	
	#plot2 =  plot2 + scale_x_log10(minor_breaks=log10_minor_break())+
  	#theme(panel.grid.major.x = element_line(size=1),
	#panel.grid.minor.x = element_line(size=0.5))
#plot2 = plot2 + scale_color_manual(values=c("#e66101","#5e3c99"))
plot2 = plot2 + theme(panel.grid.major = element_line(colour = "grey80",size=0.5))
plot2 = plot2 + theme(panel.grid.minor = element_line(colour = "grey80",size=0.1))

plot2 = plot2 + theme(axis.text.x = element_text(colour="black"))
	
plot2 = plot2 + theme(axis.text.y = element_text(colour="black"))

plot2 = plot2 + theme(text = element_text( family = "cmr10"))



#plot2 = plot2 + geom_text(aes(label =textr2(K12_model, "K12"), y = 0.9, x=0.75,color="K12"),size=sizetext,family = "cmr10")

#lot2 = plot2 + geom_text(aes(label =textr2(K21_model, "K21"), y = 1.05, x= 0.75,color="K21"),size=sizetext,family = "cmr10")
	
 
print(plot2)

ggsave("K.pdf")


plot3<-	ggplot(data)
plot3 = plot3 + scale_color_manual(values=c("#5e3c99","#fdb863", "#e66101"))

	plot3 = print_int("C", plot3, "C")
	plot3 = print_int("K12", plot3, "K12")
	plot3 = print_int("K21", plot3, "K21")

	plot3 = plot3 + xlab(para)	+
	ylab("")

#plot2 = plot2 + scale_color_manual(values=c("#e66101","#5e3c99"))
plot3 = plot3 + theme(panel.grid.major = element_line(colour = "grey80",size=0.5))
plot3 = plot3 + theme(panel.grid.minor = element_line(colour = "grey80",size=0.1))

plot3 = plot3 + theme(axis.text.x = element_text(colour="black"))
	
plot3 = plot3 + theme(axis.text.y = element_text(colour="black"))

plot3 = plot3 + coord_cartesian(ylim=c(1.4,10.5))

plot3 = plot3 + scale_x_continuous(breaks=df)
#plot3 = plot3 + theme(legend.position = "none") 

#plot3 = plot3 + geom_text(aes(label =textr2(K12_model, "K12"), y = 0.9, x=0.75,color="K12"),size=sizetext,family = "cmr10")

#plot3 = plot3 + geom_text(aes(label =textr2(K21_model, "K21"), y = 1.05, x= 0.75,color="K21"),size=sizetext,family = "cmr10")
	
#plot3 = plot3 + geom_text(aes(label =textr2(C_model, "C"), y = 0.959, x= 0.75,color="C"),size=sizetext,family = "cmr10")
 
print(plot3)



ggsave("KC.pdf")



plot3 = plot3 + coord_cartesian(ylim=c(0,0.8))
#plot3 = plot3 + scale_y_continuous(breaks=seq(0,3,by=0))
#plot3 = plot3 + theme(legend.position = "none") 

#plot3 = plot3 + geom_text(aes(label =textr2(K12_model, "K12"), y = 0.9, x=0.75,color="K12"),size=sizetext,family = "cmr10")

#plot3 = plot3 + geom_text(aes(label =textr2(K21_model, "K21"), y = 1.05, x= 0.75,color="K21"),size=sizetext,family = "cmr10")
	
#plot3 = plot3 + geom_text(aes(label =textr2(C_model, "C"), y = 0.959, x= 0.75,color="C"),size=sizetext,family = "cmr10")
 
print(plot3)
ggsave("KC2.pdf")


plot3 = plot3 + coord_cartesian(ylim=c(0.85,1.05))
plot3 = plot3 + scale_y_continuous(breaks=seq(0,3,by=0.1))
#plot3 = plot3 + theme(legend.position = "none") 

print(plot3)

ggsave("KC3.pdf")

