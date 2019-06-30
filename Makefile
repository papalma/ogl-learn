EX_DIRS= glad ex*

all: $(EX_DIRS)

.PHONY: force
$(EX_DIRS): force
	$(MAKE) -C $@ $(MAKECMDGOALS)

clean: all
