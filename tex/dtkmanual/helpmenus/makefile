
TOP_DIR=../../..

all: helpmenus codecps

DTKBINS= dtkbpdens dtkdemat1 dtkfindcp dtkmomd dtkpoint dtkqdmol \
			dtkcube dtkline dtkplane 

DTKSCRIPTS= dtkeps2pdf dtkpov2png

DTKCODES= $(TOP_DIR)/src/common/custfld-wfnclass.cpp

helpmenus: 
	@for i in $(DTKBINS); do $$i -h | sed 's/\[1m//p' |  sed 's/\[m//p' |\
	  awk '{if (NR>23) print}' > ../hm$${i}.tex; done
	@for i in $(DTKSCRIPTS); do $$i -h | sed 's/\[1m//g' |  sed 's/\[m//g'\
	  | sed 's/\[0m//g' | sed 's/\[0;4m//g' | sed 's/\[4m//g' 	> ../hm$${i}.tex; done

codecps: $(DTKCODES)
	@for i in $(DTKCODES); do cp $$i ..; done


