TARGETS = cm1_l2
PAPILIB_ALAMEDA=/run/current-system/sw/lib/libpapi.so
PAPILIB_TAGUS=/usr/local/lib/libpapi.so

all: $(TARGETS)

cm1_l2: cm1_l2.c
ifeq ($(shell test -e $(PAPILIB_ALAMEDA) && echo -n yes),yes)
	$(CC) $(CFLAGS) cm1_l2.c $(PAPILIB_ALAMEDA) -o cm1_l2
else
	$(CC) $(CFLAGS) cm1_l2.c $(PAPILIB_TAGUS) -o cm1_l2
endif

clean:
	rm -f $(TARGETS) *.o *.stderr *.stdout core *~
	rm *.out *.dat *.png

run:
	./cm1_l2 > ./cm1_l2.out
	./cm1_l2_proc.sh

fmt: cm1_l2.c
	clang-format -i $^
