ifeq ($(SHELL),/bin/sh)
LINUX=1
PATH:=$(dir $(shell pwd))/bin:$(PATH)
endif
ifeq ($(SHELL),sh.exe)
WINDOWS=1
SHELL=zsh
RM=del
endif

include book.mak

DOCS := $(wildcard $(ISBN)ch[0-9][0-9]$(DRAFT).odt $(ISBN)fm$(DRAFT).odt)

ifdef LINUX
all : html production programs

programs : code
	$(MAKE) recurse-programs

DIRS = code html production
code html clean production recurse-programs:
	@for dir in $(DIRS); do $(MAKE) -C $$dir "DOCS=$(DOCS)" $@; done

wc:
	odfwc $(DOCS)

endif

ifdef WINDOWS
all code programs:
	cd code; $(MAKE) $@

endif

.PHONY: all code html clean programs wc production
