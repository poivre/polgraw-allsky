CC = gcc 
CFLAGS  = -I/opt/fftw/3.3.4/gnu/include -Wall -O3 -DPREFIX="./candidates" -DTIMERS=1 \
	-DHAVE_SINCOS -march=native -mtune=native -ffast-math -funroll-loops -funsafe-loop-optimizations
LDFLAGS =
LIBS = -lm -lgsl -lgslcblas -L/opt/fftw/3.3.4/gnu/lib -lfftw3 -lc -lrt

gwsearch-cpu: main.o jobcore.o timer.o settings.o auxi.o init.o 
	$(CC) -gnatv -o $@ $^ $(LIBS)

doc:
	doxygen ../doc/Doxyfile

clean:
	rm -f *.o

uninstall: clean
	rm -f gwsearch-cpu
	rm -fr ../doc/html
