TARGETS = mm2_transposition
PAPILIB_ALAMEDA=/run/current-system/sw/lib/libpapi.so
PAPILIB_TAGUS=/usr/local/lib/libpapi.so
CFLAGS=-O1

all: $(TARGETS)

mm2_transposition: mm2_transposition.c
ifeq ($(shell test -e $(PAPILIB_ALAMEDA) && echo -n yes),yes)
	$(CC) $(CFLAGS) mm2_transposition.c $(PAPILIB_ALAMEDA) -o mm2_transposition
else
	$(CC) $(CFLAGS) mm2_transposition.c $(PAPILIB_TAGUS) -o mm2_transposition
endif

clean:
	rm -f $(TARGETS) *.o *.stderr *.stdout core *~

fmt: mm2_transposition.c
	clang-format -i $^
