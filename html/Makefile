HTML := $(subst .odt,.html,$(DOCS))

%.html: ../%.odt
	odf2html -f apress.xml $<

all : html

html : index.html $(HTML) ch13.txt

index.html : index.html.in $(HTML)
	maketoc index.html.in index.html $(HTML)

ch14.txt : $(wildcard ????ch14??.html)
	html2text -nobs -ascii -style pretty $< | perl -ne '/^\-+$$/ || print' > $@

clean :
	$(RM) $(HTML)

code programs production recurse-programs :

.PHONY : all html clean code programs recurse-programs production
