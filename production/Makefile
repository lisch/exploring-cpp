ifeq ($(DOCS),)
include book.mak
DOCS := $(patsubst ../%,%,$(wildcard ../$(ISBN)ch[0-9][0-9]$(DRAFT).odt))
endif

WORDDOCS = $(subst .odt,.doc,$(DOCS))
BIN = ../../bin

%.odt : ../%.odt
	cp $< $@
	$(BIN)/apressify -vh $@

%.doc : %.odt
	$(BIN)/odf2doc $<

all : production

production : $(DOCS) $(WORDDOCS)

clean :
	$(RM) *.odt *.doc *.odt.bak

code html programs recurse-programs :

.PHONY: all production code html programs recurse-programs
