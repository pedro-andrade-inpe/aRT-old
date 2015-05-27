#  aRT : API R - TerraLib                                                
#  Copyright (C) 2003-2008  LEG                                          
#                                                                        
#  This program is free software; you can redistribute it and/or modify  
#  it under the terms of the GNU General Public License as published by  
#  the Free Software Foundation; either version 2 of the License, or     
#  (at your option) any later version.                                   
#                                                                        
#  This program is distributed in the hope that it will be useful,       
#  but WITHOUT ANY WARRANTY; without even the implied warranty of        
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         
#  GNU General Public License for more details.                          
#                                                                        
#  You should have received a copy of the GNU Lesser General Public      
#  License along with this library.                                      

# needs 'apt-get install dvipng'

OBJS=conn sp intro table gpm fillcell

intro:    aRTintro.pdf
conn:     aRTconn.pdf
sp:       aRTsp.pdf
table:    aRTtable.pdf
fillcell: aRTfillcell.pdf
gpm:      aRTgpm.pdf
rnews:    wrapper.pdf

DVIOBJS=$(patsubst %,aRT%.dvi,$(OBJS))
TEXOBJS=$(patsubst %,aRT%.tex,$(OBJS))
PDFOBJS=$(patsubst %.dvi,%.pdf,$(DVIOBJS))
HTMOBJS=$(patsubst %,aRT%.html,$(OBJS))

all: $(DVIOBJS)
pdf: $(PDFOBJS)
dvi: $(DVIOBJS)
html: $(HTMOBJS)
	replace aesltt aett -- *.html
	replace width= widthx= height= heightx= -- *.html

# $@ is the name of the file to be made.
# $? is the names of the changed dependents.
# $< the name of the related file that caused the action.
# $* the prefix shared by target and dependent files. 

%.html: %.pdf
	htlatex $*.tex

#%.pdf: %.dvi
#	dvipdf $<

%.pdf: %.Rnw
	echo "Sweave(\"$?\")" | R --no-save --no-restore
	pdflatex $*.tex
	pdflatex $*.tex
			
wrapper.dvi: aRT.Rnw
	echo "Sweave(\"aRT.Rnw\")" | R --no-save --no-restore
	pdflatex wrapper.tex
	pdflatex wrapper.tex

clean:
	rm *-*.pdf *.{tex,eps,html,idv,lg,logx,xref,png,log,dvi,4ct,4tc,aux,css,out,R,tmp,toc,gpm,GAL,GWT}
