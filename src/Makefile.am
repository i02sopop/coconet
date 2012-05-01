PROJ_DIR=..
INCLUDE_DIR=${PROJ_DIR}/include
AM_CFLAGS=-I${INCLUDE_DIR} -O3
SRC_DIR=${PROJ_DIR}/src
OBJS_DIR=${PROJ_DIR}/.objs
DEST_DIR=${PROJ_DIR}
CLEANFILES=${OBJS_DIR}/*.o
bin_PROGRAMS=${DEST_DIR}/coconet

__DEST_DIR__coconet_SOURCES = 

__DEST_DIR__coconet_LDADD = ${OBJS_DIR}/createPopulations.o ${OBJS_DIR}/error.o ${OBJS_DIR}/evolvePopulations.o ${OBJS_DIR}/files.o ${OBJS_DIR}/generateOutput.o ${OBJS_DIR}/print.o ${OBJS_DIR}/freeMemory.o ${OBJS_DIR}/main.o ${OBJS_DIR}/math.o ${OBJS_DIR}/measureAptitude.o ${OBJS_DIR}/measureChange.o ${OBJS_DIR}/mutation.o ${OBJS_DIR}/sort.o

${OBJS_DIR}/createPopulations.o:${SRC_DIR}/createPopulations.c ${OBJS_DIR}
	$(COMPILE) -c -o $@ $< 

${OBJS_DIR}/error.o:${SRC_DIR}/error.c ${OBJS_DIR}
	$(COMPILE) -c -o $@ $< 

${OBJS_DIR}/evolvePopulations.o:${SRC_DIR}/evolvePopulations.c ${OBJS_DIR}
	$(COMPILE) -c -o $@ $< 

${OBJS_DIR}/files.o:${SRC_DIR}/files.c ${OBJS_DIR}
	$(COMPILE) -c -o $@ $< 

${OBJS_DIR}/generateOutput.o:${SRC_DIR}/generateOutput.c ${OBJS_DIR}
	$(COMPILE) -c -o $@ $< 

${OBJS_DIR}/print.o:${SRC_DIR}/print.c ${OBJS_DIR}
	$(COMPILE) -c -o $@ $< 

${OBJS_DIR}/freeMemory.o:${SRC_DIR}/freeMemory.c ${OBJS_DIR}
	$(COMPILE) -c -o $@ $< 

${OBJS_DIR}/main.o:${SRC_DIR}/main.c ${OBJS_DIR}
	$(COMPILE) -c -o $@ $< 

${OBJS_DIR}/math.o:${SRC_DIR}/math.c ${OBJS_DIR}
	$(COMPILE) -c -o $@ $< 

${OBJS_DIR}/measureAptitude.o:${SRC_DIR}/measureAptitude.c ${OBJS_DIR}
	$(COMPILE) -c -o $@ $< 

${OBJS_DIR}/measureChange.o:${SRC_DIR}/measureChange.c ${OBJS_DIR}
	$(COMPILE) -c -o $@ $< 

${OBJS_DIR}/mutation.o:${SRC_DIR}/mutation.c ${OBJS_DIR}
	$(COMPILE) -c -o $@ $< 

${OBJS_DIR}/sort.o:${SRC_DIR}/sort.c ${OBJS_DIR}
	$(COMPILE) -c -o $@ $< 

${OBJS_DIR}:
	mkdir ${OBJS_DIR}
