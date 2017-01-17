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

GASKELLEXES=gaskell2plt testgaskell gaskelltriax spud2stl
SPUD2EXES=spud2oogl spud2plate spud2raysh spud2stl spud2topo spud2wrl spud2xplate
EXES=$(GASKELLEXES) $(SPUD2EXES)

all: $(EXES) phobos_quad64q.plt

exes: $(EXES)

$(GASKELLEXES): gaskellutil.o

spud2%: spud2%.c spudshap.c spudplates.c spudview.c spudmisc.c spudface.c spudprint.c
	$(LINK.c) $^ $(CPPFLAGS) $(LDLIBS) -o $@

phobos_quad64q.plt: gaskell2plt
	wget -O - -nv $(ICQURL) | ./gaskell2plt > $@

clean:
	$(RM) $(EXES) gaskellutil.o phobos_quad64q.plt
