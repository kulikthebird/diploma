CC=clang++
CPP_FILES_COMBINER := $(wildcard Modules/Combiner/Src/*.cpp)
CPP_FILES_GRAPHICS := $(wildcard Modules/Graphics/Src/*.cpp)
CPP_FILES_MODEL := $(wildcard Modules/Model/Src/*.cpp)
CPP_FILES_COLLISIONS := $(wildcard Modules/Collisions/Src/*.cpp)

OBJ_FILES_COMBINER := $(addprefix Bin/,$(notdir $(CPP_FILES_COMBINER:.cpp=.o)))
OBJ_FILES_GRAPHICS := $(addprefix Bin/,$(notdir $(CPP_FILES_GRAPHICS:.cpp=.o)))
OBJ_FILES_MODEL := $(addprefix Bin/,$(notdir $(CPP_FILES_MODEL:.cpp=.o)))
OBJ_FILES_COLLISIONS := $(addprefix Bin/,$(notdir $(CPP_FILES_COLLISIONS:.cpp=.o)))

CC_FLAGS := -IModules/Combiner/Inc -IModules/Graphics/Inc -IModules/Model/Inc -IModules/Collisions/Inc -Wall --std=c++11 -O3


compile: main

main: $(OBJ_FILES_GRAPHICS) $(OBJ_FILES_COMBINER) $(OBJ_FILES_MODEL) $(OBJ_FILES_COLLISIONS)
	$(CC) $(CC_FLAGS) Modules/Main.cpp $^ -o Bin/solver -lSDL2 -lGL -lGLEW -lGLU

combiner: $(OBJ_FILES_COMBINER)
	echo Combiner finished compilation!

graphics: $(OBJ_FILES_GRAPHICS)
	echo Graphics finished compilation!

model: $(OBJ_FILES_MODEL)
	echo Model finnished compilation!


Bin/%.o: Modules/Combiner/Src/%.cpp
	$(CC) $(CC_FLAGS) -c -o $@ $<

Bin/%.o: Modules/Graphics/Src/%.cpp
	$(CC) $(CC_FLAGS) -c -o $@ $<

Bin/%.o: Modules/Model/Src/%.cpp
	$(CC) $(CC_FLAGS) -c -o $@ $<

Bin/%.o: Modules/Collisions/Src/%.cpp
	$(CC) $(CC_FLAGS) -c -o $@ $<

documentation:
	evince Docs/Diploma/inzynier.pdf

documentation_compile:
	cd Docs/Diploma/ ; pdflatex inzynier.tex ; bibtex inzynier.tex ;pdflatex inzynier.tex

presentation_compile:
	cd Docs/Presentation/ ; pdflatex presentation_pl.tex ; pdflatex presentation_pl.tex

presentation:
	evince Docs/Presentation/presentation_pl.pdf

clean:
	find  ./Bin -name *.o -delete
	find . -regex '.*\.\(aux\|bbl\|log\|blg\|toc\|out\|snm\|nav\)' -delete
