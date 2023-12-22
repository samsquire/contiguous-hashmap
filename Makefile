CFILES=$(wildcard *.c)
PROGRAMS=$(subst .c,,$(CFILES))

default: $(PROGRAMS)
$(PROGRAMS): $(CFILES)
	gcc -o $(basename $@) $(basename $@).c -g -O3

debug:
	echo $(CFILES) ;
	echo $(PROGRAMS)

clean:
	rm $(PROGRAMS)

