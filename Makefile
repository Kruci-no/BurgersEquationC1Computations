# a list of all the programs in your package
PROGS = Burgers/CAProof
OTHERS = DissipativePDE/Algebra/algebra DissipativePDE/Set/set DissipativePDE/VectorField/vectorField DissipativePDE/SolverPDE/solverPDE DissipativePDE/VectorFieldMaker/vectorFieldMaker Burgers/GallerkinProjections/gallerkinProjections Burgers/InOut/inOut Utils/SampleDyn/sampleDyn Utils/FinderAttractingFixedPoint/finderAttractingFixedPoint  Burgers/BurgersVecField/burgersVecField Utils/Debug/debugException#Burgers/BurgersVecFieldC1/burgersVecFieldC1#ChafeeInfante/ChafeeInfanteVecFieldC1/chafeeInfanteVecFieldC1
#
# directory where capd scripts are (e.g. capd-config)
CAPDBINDIR = CAPD_lib/build/bin/

# setting compiler and linker flags
CAPDFLAGS = `${CAPDBINDIR}capd-config --cflags`
CAPDLIBS = `${CAPDBINDIR}capd-config --libs`
CXXFLAGS += ${CAPDFLAGS} -O2 -std=c++20 

# directory where object and dependancy files will be created
OBJDIR = .obj/

#============ the following should not be changed =========

OTHERS_STRIPPED = $(notdir ${OTHERS}) 
OTHERS_OBJ = ${OTHERS:%=${OBJDIR}%.o}
OBJ_FILES = ${OTHERS_OBJ} ${PROGS:%=${OBJDIR}%.o}
$(info $$var is [${OBJ_FILES}])


.PHONY: all
all: ${PROGS}

# rule to link executables
${PROGS}: % : ${OBJDIR}%.o ${OTHERS_OBJ}
	${CXX} -o $@ $< ${OTHERS_OBJ} ${CAPDLIBS}

# include files with dependencies
-include ${OBJ_FILES:%=%.d}

#rule to compile .cpp files and generate corresponding files with dependencies
${OBJ_FILES}: ${OBJDIR}%.o : %.cpp
	@mkdir -p ${OBJDIR}
	@mkdir -p ${OBJDIR}$(dir $<)
	$(CXX) ${CXXFLAGS} -MT $@ -MD -MP -MF ${@:%=%.d} -c -o $@ $<

# rule to clean all object files, dependencies and executables
.PHONY: clean
clean:
	rm -f ${OBJDIR}*.o ${OBJDIR}*.o.d ${PROGS}cd