########################################################################
### To use NAIF SPICE, assuming SPICE C toolkit is in .../cspice:
###
###   ln -s .../cspice ./cspice
###   mv SpiceUsr.h SpiceUsr.h.notUsed
###
### and uncomment the following three lines:
###
###CSPICETOP=cspice
###CPPFLAGS=-I$(CSPICETOP)/include
###LDLIBS=gaskellutil.c $(CSPICETOP)/lib/cspice.a -lm
########################################################################

ICQURL=http://sbn.psi.edu/pds/asteroid/VO1_SA_VISA_VISB_5_PHOBOSSHAPE_V1_0/data/phobos_quad64q.tab
LDLIBS=-lm

EXES=test2gaskell testgaskell gaskelltriax

all: $(EXES) phobos_quad64q.plt

$(EXES): gaskellutil.o

phobos_quad64q.plt: test2gaskell
	wget -O - -nv $(ICQURL) | ./test2gaskell > $@

clean:
	$(RM) $(EXES) gaskellutil.o phobos_quad64q.plt
