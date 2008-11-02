# ###*B*###
# Copyright (C) Roberto Bucher (SUPSI- Lugano)
#               Simone Mannori (Scilab / INRIA / DIGITEO)
#
# Copyright (C) METALAU Project (INRIA)
# ###*E*###
 
 
## Author: 2007 Mauro Marinoni
## CVS: $Id: cfg.mk,v 1.5 2008/07/18 11:24:24 pj Exp $

ifeq ($(findstring __LIB_SCICOS__,$(LIB_OPT)),__LIB_SCICOS__)

##
## Application files
##

ifeq ($(findstring __NIOS2__,$(EEOPT)) , __NIOS2__)

EE_SRCS +=      contrib/scicos/src/nios2/led.c \
		contrib/scicos/src/nios2/button.c \
		contrib/scicos/src/nios2/sevenseg.c 

endif

#ifeq ($(findstring __PIC30__,$(EEOPT)) , __PIC30__)
ifeq ($(findstring __EE_FLEX__,$(EEOPT)) , __EE_FLEX__)

EE_SRCS_SCICOS_DSPIC := $(addprefix contrib/scicos/src/pic30/, $(notdir $(shell ls -1 $(EEBASE)/contrib/scicos/src/pic30/*.c)))
EE_SRCS += $(EE_SRCS_SCICOS_DSPIC)

ifeq ($(findstring __LIB_IEEE802154__,$(LIB_OPT)),__LIB_IEEE802154__)
EE_SRCS_SCICOS_DSPIC_OPENZB := $(addprefix contrib/scicos/src/pic30/openzb/, $(notdir $(shell ls -1 $(EEBASE)/contrib/scicos/src/pic30/openzb/*.c)))
EE_SRCS += $(EE_SRCS_SCICOS_DSPIC_OPENZB)
endif


endif

##
## Add Scicos to the linker list of libs
##
OPT_LIBS += -lscicos

endif
