#!/usr/bin/Rscript
rm(list=ls())
library(ggplot2)
library(scales) # Accéder aux fonctions de transformations 
library(extrafont)# pour les font si bug executer dans R sous sudo extrafont::font_import()

library(reshape2)
base_size=40
X11(display = "", width=13, height=8)
theme_set(theme_minimal(base_size=base_size))
source("/home/thutur/Rwork/script/laser_tools.R")

colC=1
colK12=2
colK21=3

sizeboule=1*base_size/10
sizeline=0.5*base_size/10
sizetext=3*base_size/10
para = "Beta"

textr2 = function(model, name){

return( paste(name, "=",  signif(digits=3,model$coefficients[2]), "x ",para ," +", signif(digits=3,model$coefficients[1]) ,"(R² =", signif(digits=3,summary(model)$r.square), ")" ))
}

K12=c(0.8971,0.8973,0.9,0.9014,0.906,0.909,0.908,0.908,0.906)
uK12=c(0.0074,0.0072,0.0072,0.008,0.011,0.012,0.02,0.037,0.08)
K21=c(1.0744,1.0759,1.078,1.0801,1.082,1.084,1.084,1.085,1.085)
uK21=c(0.0065,0.0064,0.0069,0.0076,0.011,0.014,0.016,0.036,0.053)
C=c(0.9638,0.9653,0.9702,0.974,0.98,0.985,0.984,0.986,0.983)
uC=c(0.0095,0.0093,0.0097,0.011,0.015,0.018,0.025,0.051,0.094)
df=c(1,0.9, 0.7, 0.5, 0.25, 0.1, 0.05, 0.01, 0.001)

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
	plot =  plot + scale_x_log10(minor_breaks=log10_minor_break())+
  	theme(panel.grid.major.x = element_line(size=1),
	panel.grid.minor.x = element_line(size=0.5))

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
	plot2 =  plot2 + scale_x_log10(minor_breaks=log10_minor_break())+
  	theme(panel.grid.major.x = element_line(size=1),
	panel.grid.minor.x = element_line(size=0.5))
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

plot3 = plot3 + theme(text = element_text( family = "cmr10"))
	plot3 =  plot3 + scale_x_log10(minor_breaks=log10_minor_break())+
  	theme(panel.grid.major.x = element_line(size=1),
	panel.grid.minor.x = element_line(size=0.5))
#plot3 = plot3 + theme(legend.position = "none") 

#plot3 = plot3 + geom_text(aes(label =textr2(K12_model, "K12"), y = 0.9, x=0.75,color="K12"),size=sizetext,family = "cmr10")

#plot3 = plot3 + geom_text(aes(label =textr2(K21_model, "K21"), y = 1.05, x= 0.75,color="K21"),size=sizetext,family = "cmr10")
	
#plot3 = plot3 + geom_text(aes(label =textr2(C_model, "C"), y = 0.959, x= 0.75,color="C"),size=sizetext,family = "cmr10")
 
print(plot3)

ggsave("KC.pdf")

