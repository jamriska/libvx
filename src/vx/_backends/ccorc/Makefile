ORCC = orcc
#ORCC_FLAGS=--implementation --header

%.c: %.orc $(DEPS)
	$(ORCC) -o $@ $< --implementation

%.h: %.orc $(DEPS)
	$(ORCC) -o $@ $< --header
	
ccorc_interface.c:ccorc_interface.orc

ccorc_interface.h:ccorc_interface.orc


.PHONY: clean
	
all:ccorc_interface.c ccorc_interface.h
	
clean:
	rm -f ccorc_interface.c ccorc_interface.h