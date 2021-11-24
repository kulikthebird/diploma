CPX_PATH  = /opt/ibm/ILOG/CPLEX_Studio128
SYSTEM = x86-64_linux
CPX_INCDIR  = $(CPX_PATH)/cpoptimizer/include
CPX_INCCON  = $(CPX_PATH)/concert/include
CPX_FLAGS = -DIL_STD -DILOUSEMT -D_REENTRANT -DILM_REENTRANT
CC=clang++ -Wno-switch -Wno-deprecated-register -Werror=return-type --std=c++11 -O2
CCFLAGS := -I./Inc -I./Test -I$(CPX_INCDIR) -I$(CPX_INCCON) $(CPX_FLAGS)
CPX_LDFLAGS = -L$(CPX_PATH)/cpoptimizer/lib/$(SYSTEM)/static_pic -lcp -L$(CPX_PATH)/cplex/lib/$(SYSTEM)/static_pic -lcplex -L$(CPX_PATH)/concert/lib/$(SYSTEM)/static_pic -lconcert  -lpthread -lm -ldl 


compile: Bin/RegexConstraint.o
	$(CC) $(CCFLAGS) Test/TestFile.cpp $^ -o Bin/solve1 $(CPX_LDFLAGS)

Bin/%.o: Src/%.cpp
	-mkdir Bin
	$(CC) $(CCFLAGS) -c -o $@ $<

test:
	Bin/./solve1

buildDocs:
	cd Docs/ ; pdflatex masters ; bibtex masters ; pdflatex -halt-on-error masters ; pdflatex -halt-on-error masters

buildPresentation:
	cd Docs/ ; pdflatex presentation ; pdflatex -halt-on-error presentation

presentation:
	evince Docs/presentation.pdf

documentation:
	evince Docs/masters.pdf

clean:
	find . -regex '.*\.\(aux\|bbl\|log\|blg\|toc\|out\|snm\|nav\|o\)' -delete
	-rm -r ./Bin
