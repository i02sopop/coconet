PROJ_DIR=..
INCLUDE_DIR=${PROJ_DIR}/include
AM_CFLAGS=-I${INCLUDE_DIR} -O3
SRC_DIR=${PROJ_DIR}/src
OBJS_DIR=${PROJ_DIR}/.objs
DEST_DIR=${PROJ_DIR}
CLEANFILES=${OBJS_DIR}/*.o
bin_PROGRAMS=${DEST_DIR}/coconet

__DEST_DIR__coconet_SOURCES = 

__DEST_DIR__coconet_LDADD = ${OBJS_DIR}/crearpoblaciones.o ${OBJS_DIR}/error.o ${OBJS_DIR}/evolucionarpoblaciones.o ${OBJS_DIR}/ficheros.o ${OBJS_DIR}/generarsalida.o ${OBJS_DIR}/imprimir.o ${OBJS_DIR}/liberarmemoria.o ${OBJS_DIR}/main.o ${OBJS_DIR}/math.o ${OBJS_DIR}/mediraptitud.o ${OBJS_DIR}/medircambio.o ${OBJS_DIR}/mutacionestructural.o ${OBJS_DIR}/ordenar.o

${OBJS_DIR}/crearpoblaciones.o:${SRC_DIR}/crearpoblaciones.c ${OBJS_DIR}
	$(COMPILE) -c -o $@ $< 

${OBJS_DIR}/error.o:${SRC_DIR}/error.c ${OBJS_DIR}
	$(COMPILE) -c -o $@ $< 

${OBJS_DIR}/evolucionarpoblaciones.o:${SRC_DIR}/evolucionarpoblaciones.c ${OBJS_DIR}
	$(COMPILE) -c -o $@ $< 

${OBJS_DIR}/ficheros.o:${SRC_DIR}/ficheros.c ${OBJS_DIR}
	$(COMPILE) -c -o $@ $< 

${OBJS_DIR}/generarsalida.o:${SRC_DIR}/generarsalida.c ${OBJS_DIR}
	$(COMPILE) -c -o $@ $< 

${OBJS_DIR}/imprimir.o:${SRC_DIR}/imprimir.c ${OBJS_DIR}
	$(COMPILE) -c -o $@ $< 

${OBJS_DIR}/liberarmemoria.o:${SRC_DIR}/liberarmemoria.c ${OBJS_DIR}
	$(COMPILE) -c -o $@ $< 

${OBJS_DIR}/main.o:${SRC_DIR}/main.c ${OBJS_DIR}
	$(COMPILE) -c -o $@ $< 

${OBJS_DIR}/math.o:${SRC_DIR}/math.c ${OBJS_DIR}
	$(COMPILE) -c -o $@ $< 

${OBJS_DIR}/mediraptitud.o:${SRC_DIR}/mediraptitud.c ${OBJS_DIR}
	$(COMPILE) -c -o $@ $< 

${OBJS_DIR}/medircambio.o:${SRC_DIR}/medircambio.c ${OBJS_DIR}
	$(COMPILE) -c -o $@ $< 

${OBJS_DIR}/mutacionestructural.o:${SRC_DIR}/mutacionestructural.c ${OBJS_DIR}
	$(COMPILE) -c -o $@ $< 

${OBJS_DIR}/ordenar.o:${SRC_DIR}/ordenar.c ${OBJS_DIR}
	$(COMPILE) -c -o $@ $< 

${OBJS_DIR}:
	mkdir ${OBJS_DIR}
