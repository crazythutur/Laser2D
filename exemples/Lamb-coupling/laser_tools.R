#!/usr/bin/Rscript	      

#rm(list=ls())
#dir="~/archives/Stage_IES/data/test6-heat/data-1/fabrice_Old_35"
#dir="~/archives/Stage_IES/data/test6-heat/data-1/fabrice_heat_200"
#dir="~/archives/Stage_IES/data/test6-heat/data-1/"
library(ggplot2)
log10_minor_break = function (...){
  function(x) {
    minx         = floor(min(log10(x), na.rm=T))-1;
    maxx         = ceiling(max(log10(x), na.rm=T))+1;
    n_major      = maxx-minx+1;
    major_breaks = seq(minx, maxx, by=1)
    minor_breaks = 
      rep(log10(seq(1, 9, by=1)), times = n_major)+
      rep(major_breaks, each = 9)
    return(10^(minor_breaks))
  }
}


fit <- function(x,y,col=1,span=0.7){
	z=predict(loess(y~x,span=span),x)
	#lines(z~x,lwd=1,col=col)
	return(z)
}
rowVars <- function(x) {
  rowSums((x - rowMeans(x))^2)/(dim(x)[2] - 1)
}
barreErreurs <-function(x,y,dx=0,dy=0,col=1) {
	for (i in 1:length(x)){
		segments(x0=x[i]-dx[i],y0=y[i] ,x1= x [i]+dx[i],y1=y[i], col=col)
		segments(x0=x[i],y0=y[i] -dy[i], x1=x [i],y1=y[i] + dy[i] , col=col)
		#rect(x[i]-dx[i],y[i] -dy[i], x [i]+dx[i],y[i] + dy[i] , border=col)
	}
}

cutString<- function(chaine,start=0,end=0) {
	substr(chaine,start=start+1,stop=nchar(as.character(chaine), type = "chars")-end)
}
':=' <- function(lhs, rhs) {
  frame <- parent.frame()
  lhs <- as.list(substitute(lhs))
  if (length(lhs) > 1)
    lhs <- lhs[-1]
  if (length(lhs) == 1) {
    do.call(`=`, list(lhs[[1]], rhs), envir=frame)
    return(invisible(NULL)) 
  }
  if (is.function(rhs) || is(rhs, 'formula'))
    rhs <- list(rhs)
  if (length(lhs) > length(rhs))
    rhs <- c(rhs, rep(list(NULL), length(lhs) - length(rhs)))
  for (i in 1:length(lhs))
    do.call(`=`, list(lhs[[i]], rhs[[i]]), envir=frame)
  return(invisible(NULL)) 
}

Extract	<-function(dir,pattern=""){
	list_files <- list.files(dir,pattern=pattern)
	nmb_files = length(list_files)
	if(nmb_files==0){
		print(paste("error No files in ", dir, " with pattern ", pattern))
		return
	}

	fileTest<-paste(dir, list_files[[1]], sep='/')
	outTest	<- read.csv(file<-fileTest,header=TRUE,comment.char="%")
	EventTest	<-outTest["Event"]
	EventName	<-EventTest[EventTest!=""]
	EventSize	<-length(EventName)
	para		<-vector()
	
	event	<-matrix(ncol=nmb_files,nrow=EventSize)
	m	<-vector(mode="numeric")

	rownames(event)	<-outTest[1:11,"Event"]
	colnames(event)	<-as.character(list_files)

	paraRead<- read.table(fileTest,skip=0,nrows=1)
	for (i in 1:length(paraRead)){
		slp <- unlist(strsplit(as.character(paraRead[[i]]),split='='))
		if(length(slp)>1){
			parai <-as.numeric(slp[2])
			names(parai)<-gsub("-", "",slp[1])
			para <- c(para,parai)
		}

	}
	tmps=as.numeric(paraRead[length(paraRead)-1])
	names(tmps)="time"
	para <- c(para,tmps)

	for (i in 1:nmb_files) {
	
		fichier<-paste(dir, list_files[[i]], sep='/')
		linem<- read.table(fichier,skip=1,nrows=1)
		out	<- read.csv(file<-fichier,header=TRUE,comment.char="%")
		event[,i]	<-out[1:EventSize,"Count"]
		m[i]	<-	as.numeric(substr(linem[[2]],5,stop=nchar(as.character(linem[[2]]), type = "chars")-1))
	

	}

	names(m)<-list_files
	returnValue	<-list(m=m,Event=event,para=para)
	
	return(returnValue)
}
getPara <-function(file=""){
	
	input<- read.table(file,skip=0,nrows=1)
	para		<-vector()
	for (i in 1:length(input))
	{
		slp <- unlist(strsplit(as.character(input[[i]]),split='='))
		value=slp[2]
		name=gsub("-", "",slp[1])
		if(length(slp)>1)
		{			
			if(length(grep("[{}]",value)))
			{
				value=substr(value,start=2,stop=nchar(value)-1)
				multivalue=unlist(strsplit(as.character(value),split=','))
				for(j in 1:length(multivalue)){
					parai=as.numeric(multivalue[j] )
					names(parai)=paste(name,j,sep='_')
para <- c(para,parai)
				}

			}else
			{
					parai <-suppressWarnings(as.numeric(value))
					names(parai)<-name
					para <- c(para,parai)
			}
			
			
		}

	}
	tmps=as.numeric(input[length(input)])
	names(tmps)="time"
	para <- c(para,tmps)	
return(para)

}



getAllPara	<-function(dir="~/archives/Stage_IES/data/test6-heat/data-1/fabrice_heat_200",pattern=""){
	list_files <- list.files(dir,pattern=pattern)
	nmb_files = length(list_files)
	if(nmb_files==0){
		print(paste("error No files in ", dir, " with pattern ", pattern))
		return
	}

	fileTest<-paste(dir, list_files[[1]], sep='/')
	paratest		<-vector()

	paraRead<- read.table(fileTest,skip=0,nrows=1)
	for (i in 1:length(paraRead)){
		slp <- unlist(strsplit(as.character(paraRead[[i]]),split='='))
		if(length(slp)>1){
			parai <-as.numeric(slp[2])
			names(parai)<-gsub("-", "",slp[1])
			paratest <- c(paratest,parai)
		}

	}
	tmps=as.numeric(paraRead[length(paraRead)-1])
	names(tmps)="time"
	paratest <- c(paratest,tmps)	
	
	ParaName	<-names(paratest)
	ParaSize	<-length(ParaName)
	para	<-matrix(ncol=nmb_files,nrow=ParaSize)
	rownames(para)	<-ParaName
	colnames(para)	<-as.character(list_files)
	for (f in 1:nmb_files) {
		file<-paste(dir, list_files[[f]], sep='/')
		paraRead<- read.table(file,skip=0,nrows=1)
		parai=vector()
		for (i in 1:length(paraRead)){
			slp <- unlist(strsplit(as.character(paraRead[[i]]),split='='))
			if(length(slp)>1){
				parai <- c(parai,as.numeric(slp[2]))
			}
			

		}
		parai=c(parai,as.numeric(paraRead[length(paraRead)-1]))
		para[,f]=parai
	}



	
	return(para)



}
#' give a vector of "<m>","var(m)","<n_l²> and fano for specifiate file
#'
#' @param file A file to extract data
#' @param mode The mode to extract data (default is  1)
#' @param Old specified if it is for old versions of software (default is  F)
#' @param length permit to augment if more variable are to extracted (default is  5)
#' @return a vector of "<m>","var(m)","<n_l²> and fano
#' @examples
#' getm(file)
#' sapply(directory,getm,mode=2)
getm	<-function(file,mode=1,Old=F,length=5){

	value=vector(mode="numeric",length = length)
	name=vector(mode="character",length = length-1)
	linem<- read.table(file,skip=mode,nrows=1)
	zaapTxt=4
	if(Old==T){
		zaapTxt=1

	}
	for(j in 1:length-1){
		chaine =  sub(",", "", linem[[j+zaapTxt]])			
		value[j]=as.numeric(strsplit(chaine,"=")[[1]][2])
		name[j]=strsplit(chaine,"=")[[1]][1]						
	}
	
	names(value)=c(name,"fano")
	value["fano"]=value["var(m)"]/value["<m>"]
	return (value)
}

get_burning	<-function(file=""){
	eps=1.602176565e-22#1meV
	kB=1.38064852e-23


	para=getPara(file)
	event=getEvent(file)
	n=get_n(file)[["<n>"]]
	m=getm(file)[["<m>"]]
	nth=get_nth(m,para[["cavity"]])
	#nth=get_ne(m,event[["emi"]],para[["time"]])
	T0=-eps/(kB*log(para["q"]))
	Te=eps * (para[["lvl"]] + 1 - n)/(kB * log(1/nth-1))
	return(Te-T0)
}
get_numb_lines<-function(file=""){
	outer<- read.csv(file<-file,header=TRUE,comment.char="%")
	return (length(outer[,1]))
}

get_n	<-function(file="",modes=1){
	
	if(modes>1){
		modes=modes+1 #la ligne mode total
	}
	linen<- read.table(file,skip=modes+1,nrows=1)
	
	chaine =  sub(",", "", linen[[5]])			
	value=as.numeric(strsplit(chaine,"=")[[1]][2])
	name=strsplit(chaine,"=")[[1]][1]						
	
	names(value)=name
	return (value)
}
get_nth <- function(m=0, a=1){
	return (-m + sqrt(m*(m+ a + 1)))
}
get_ne<- function(m=0, E=1, t=1){
	return ( sqrt(E/t/(m+1)))
}

getEvent	<-function(file="",Old=F){
	if(file==""){
		print("error No files ")
		return
	}
	out	<- read.csv(file<-file,header=TRUE,comment.char="%")
	if(Old){
		event	<-out[,"Count"]
		names(event)=out[,"Event"]
	}else{
		event	<-out[,"Event_count"]
		names(event)=out[,"Event_name"]
	}
	
	
	
	return(event[!is.na(event)])
}

getHisto	<-function(file="",max=256){
	if(file==""){
		print("error No files ")
		return
	}
	histo=vector(mode="numeric",length=max)
	names(histo)=as.character(0:(max-1))

	out<- read.csv(file<-file,header=TRUE,comment.char="%")
	tmp<-out[,"P.m_0."]
	names(tmp)=out[,"m"]
	#enlève les Na quand la taille est inférieurs aux events
	tmp = tmp[!is.na(tmp)]

	histo[names(tmp)]=tmp

	
	
	return(histo)
}

get_g2	<-function(file=""){
	if(file==""){
		print("error No files ")
		return
	}
	

	out<- read.csv(file<-file,header=TRUE,comment.char="%")
	g2<-out[,"g2.t."]
	

	names(g2)=out[,"t"]

	
	
	return(g2[!is.na(g2)])
}


get_P0	<-function(file=""){
	if(file==""){
		print("error No files ")
		return
	}	
	
	csv=read.csv(file<-file,header=TRUE,comment.char="%")
	if(csv[1,]["m"]>0){
		return(0)
	}
	
	return(csv[1,"P.m_0."])
}


getDS 	<-function(fichier,mode=""){
	name=paste("S_",mode,".Omega.",sep='')
	out	<- read.csv(file<-fichier,header=TRUE,comment.char="%")
	DS=out[,name]
	names(DS)=out[,"Omega"]	
	return(DS)
}
getOldDS<-function(dir,pattern="",mode=0){
	list_files <- list.files(dir,pattern=pattern)
	nmb_files = length(list_files)
	DS=vector(mode='numeric')
	if(!is.element(mode,c(0,1,2,"0","1","2"))){
		mode=3
	}
	Omega="Omega"
	m=19+2*as.numeric(mode)
	if(nmb_files==0){
		print(paste("error No files in ", dir, " with pattern ", pattern))
		return
	}
	
	
	for (i in 1:nmb_files) {
		
		fichier<-paste(dir, list_files[[i]], sep='/')
		r	<-	read.table(fichier,skip=m,nrows=1,sep=',')
		r[[1]]=cutString(r[[1]],start=1)
		l=length(r)
		if(m<25){
			r=r[-l]
			l=l-1
		}
		
		r[[l]]=cutString(r[[l]],end=1)
		DS=cbind(DS,as.numeric(r))

		

	}
	
	colnames(DS)	<-c(list_files)	
	return(DS)
}

getBim	<-function(dir,pattern=""){
	list_files <- list.files(dir,pattern=pattern)
	nmb_files = length(list_files)
	if(nmb_files==0){
		print(paste("error No files in ", dir, " with pattern ", pattern))
		return
	}
	ret	<-matrix(ncol=nmb_files,nrow=6)
	rownames(ret)	<-	c("m1","var1","m2","var2","mt","vart")
	colnames(ret)	<-list_files
	for (i in 1:nmb_files) {
		fichier<-paste(dir, list_files[[i]], sep='/')
		linem1<- read.table(fichier,skip=1,nrows=1)
		linem2<- read.table(fichier,skip=2,nrows=1)
		linemt<- read.table(fichier,skip=3,nrows=1)
		m1	<-	as.numeric(substr(linem1[[5]],5,stop=nchar(as.character(linem1[[5]]), type = "chars")-1))
		m2	<-	as.numeric(substr(linem2[[5]],5,stop=nchar(as.character(linem2[[5]]), type = "chars")-1))
		mt	<-	as.numeric(substr(linemt[[4]],5,stop=nchar(as.character(linemt[[4]]), type = "chars")-1))
		
		var1	<-	as.numeric(substr(linem1[[6]],8,stop=nchar(as.character(linem1[[6]]), type = "chars")) )
		var2	<-	as.numeric(substr(linem2[[6]],8,stop=nchar(as.character(linem2[[6]]), type = "chars")) )
		vart	<-	as.numeric(substr(linemt[[5]],8,stop=nchar(as.character(linemt[[5]]), type = "chars")) )

		ret[,i]	<- c(m1,var1,m2,var2,mt,vart)

		

	}	
	return(ret)
}


getBiOldm<-function(dir,pattern="[1-9]$"){
	list_files <- list.files(dir,pattern=pattern)
	nmb_files = length(list_files)
	if(nmb_files==0){
		print(paste("error No files in ", dir, " with pattern ", pattern))
		return
	}
	ret	<-matrix(ncol=nmb_files,nrow=6)
	rownames(ret)	<-	c("m1","var1","m2","var2","mt","vart")
	colnames(ret)	<-list_files
	for (i in 1:nmb_files) {
		
		fichier<-paste(dir, list_files[[i]], sep='/')
		l	<-	read.table(fichier,skip=8,nrows=1,sep=',')
		
		ret[,i]	<-  as.numeric(c(cutString(l[[2]],start=1),l[3],l[4],l[5],l[6],cutString(l[[7]],end=2)))

		

	}	
	return(ret)
}


