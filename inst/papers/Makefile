TARGET=wrapper

all: $(TARGET).dvi

aRT.tex: aRT.Rnw
	echo "library(tools); Sweave(\"aRT.Rnw\")" | R --no-save --no-restore

$(TARGET).dvi: aRT.tex
	latex $(TARGET).tex
	bibtex wrapper
	latex $(TARGET).tex
	latex $(TARGET).tex

clean:
	rm aRT.tex *.{log,blg,dvi,pdf,ps,out,aux,eps} *.bb* 

   
