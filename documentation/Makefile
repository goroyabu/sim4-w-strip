#///@author Goro Yabu
#///@date 2019/07/03
#///@brief Makefile to compile single file apps.

SOURCE = ../source/*

#--------------------------------------------------
#--------------------------------------------------

OUTDIR=html
INDEX=$(OUTDIR)/index.html

#--------------------------------------------------
.PHONY : help
help : ## Print this message.
	@echo "Usage: make [options] [target] ..."
	@echo "Targets:"
	@grep -Eh '^[a-zA-Z_-]+ : .*?## .*$$' $(MAKEFILE_LIST) | sort | awk -F'[:##]' '{printf "  \033[0m%-25s\033[0m %s\n", $$1, $$4}'

all : $(INDEX) ## Generate the documentations.

$(INDEX) : $(SOURCE)
	doxygen

.PHONY : clean
clean : ## Delete the generated files.
	rm -rf *~ $(OUTDIR)

.PHONY : reset
reset : ## Delete the generated files.
	rm -rf *~ $(OUTDIR)

.PHONY : show-files
show-files : ## Print all of the sources and the app.
	@echo "SOURCEDIR:"
	@$(foreach temp, $(shell find $(SOURCE) -type d), echo "  "$(temp);)
