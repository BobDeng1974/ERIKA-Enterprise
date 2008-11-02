# This is the makefile-included file that contains the specifications
# of all the binary images that have to be created for a given
# distribution

# here is a template for all the entries of this file

# DISTRIBUTION_NAME = LIBCONFi ...
# DISTRIBUTION_NAME_varname = value
# LIBCONFi = __EEOPT_OPTj__ ...


# -------------------------------------------------------------------

# this variable is used in the "help" rule
ALL_DISTRIBUTIONS += MPC5
ALL_DISTRIBUTIONS += NIOS2_ALL NIOS2_FROMBINDIST NIOS2_EXAMPLES NIOS2_TESTCASE
ALL_DISTRIBUTIONS += E7T_ALL E7T_TESTCASE
ALL_DISTRIBUTIONS += PIC30_ALL PIC30_TESTCASE
ALL_DISTRIBUTIONS += AVR5_ALL AVR5_TESTCASE
ALL_DISTRIBUTIONS += COBRA_ICD30_ALL
ALL_DISTRIBUTIONS += EEFLEX_ALL EECOMMERCIAL_ALL EEBASIC_FP EE
ALL_DISTRIBUTIONS += EEAVR5_ALL


# -------------------------------------------------------------------

# This is a source distribution that includes all the sources needed
# to compile a generic Nios2 application

#NIOS2_ALL = NIOS2_ALL_FP NIOS2_ALL_OO
#NIOS2_ALL = NIOS2_ALL_OO NIOS2_ALL_EXAMP NIOS2_ALL_FRSH

NIOS2_ALL = NIOS2_ALL_FP NIOS2_ALL_OO NIOS2_ALL_EXAMP

NIOS2_ALL_FP = __NIOS2__ __FP__ __ALLOW_NESTED_IRQ__ __MONO__ __RN__ __MSRP__ __ALARMS__ __SEM__
NIOS2_ALL_OO = __NIOS2__ __OO_BCC1__ __ALLOW_NESTED_IRQ__ __MONO__ __RN__ __MSRP__
NIOS2_ALL_EXAMP = __NIOS2_EXAMPLES__
NIOS2_ALL_FRSH = __NIOS2__ __FRSH__ __ALLOW_NESTED_IRQ__ __MONO__ __RN__ __MSRP__

# -------------------------------------------------------------------

# This is used when compiling source distributions for Nios II

NIOS2_TESTCASE = NIOS2_TESTCASE_FP NIOS2_TESTCASE_OO

NIOS2_TESTCASE_FP = $(NIOS2_ALL_FP) __ASSERT__
NIOS2_TESTCASE_OO = $(NIOS2_ALL_OO) __ASSERT__

# -------------------------------------------------------------------

# this is called hardcoded by the binary distribution scripts when
# generating a binary distribution for NIOS2. This is needed because
# the scripts for NIOS2 must stay into the nios2 directory, so the
# binary distribution generator first generates a source distribution
# that then is used to compile the binary distribution

NIOS2_FROMBINDIST = NIOS2_FROMBINDIST_THEDIST

NIOS2_FROMBINDIST_THEDIST = __NIOS2__ __OO_BCC1__ __ALLOW_NESTED_IRQ__ __MONO__ __RN__ __MSRP__ __ASSERT__

# -------------------------------------------------------------------

# This source distribution only includes the examples taken from the
# tutorial. Note that the post_install scripts are used to generate
# the correct directory layout!!!

NIOS2_EXAMPLES = NIOS2_EXAMPLES_ALL

NIOS2_EXAMPLES_ALL = __NIOS2_EXAMPLES__

# -------------------------------------------------------------------

MPC5 = MPC5_SOURCE

MPC5_SOURCE = __FP__ __MPC566EVB__ __MPC5XX__

# -------------------------------------------------------------------

E7T_ALL = E7T_ALL_OO

E7T_ALL_OO = __OO_BCC1__ __FP__ __EVALUATOR7T__ __SAMSUNG_KS32C50100__ __ARM7GNU__

# -------------------------------------------------------------------

E7T_TESTCASE = E7T_TESTCASE_OO

E7T_TESTCASE_OO = __OO_BCC1__ __ASSERT__ __EVALUATOR7T__ __SAMSUNG_KS32C50100__ __ARM7GNU__

# -------------------------------------------------------------------

# same as E7T_ALL!!!
E7T_FROMBINDIST = E7T_ALL_OO

# -------------------------------------------------------------------

PIC30_EE = PIC30_EE_OO

PIC30_EE_OO = __PIC30__ __OO_BCC1__ __FP__

# -------------------------------------------------------------------

PIC30_TESTCASE = PIC30_TESTCASE_OO

PIC30_TESTCASE_OO = __PIC30__ __OO_BCC1__ __ASSERT__

# -------------------------------------------------------------------

# same as PIC30_ALL!!!
PIC30_FROMBINDIST = PIC30_ALL_OO

# -------------------------------------------------------------------


# -------------------------------------------------------------------

AVR5_EE = AVR5_EE_OO

AVR5_EE_OO = __AVR5__ __OO_BCC1__ __FP__

# -------------------------------------------------------------------

AVR5_TESTCASE = AVR5_TESTCASE_OO

AVR5_TESTCASE_OO = __AVR5__ __OO_BCC1__ __ASSERT__

# -------------------------------------------------------------------

# same as AVR5_ALL!!!
AVR5_FROMBINDIST = AVR5_ALL_OO

# -------------------------------------------------------------------




# Cobra Automotive Spa
# source code distribution for the STA2051 Vespucci

COBRA_ICD30_ALL = COBRA_ICD30_ALL_OO

COBRA_ICD30_ALL_OO = __ARM7GNU__ __ST_STA2051__ __COBRA_ICD30__ __OO_BCC1__ __FP__


# -------------------------------------------------------------------

# EE Basic, for the distribution on the web site or the commercial distribution

EEBASIC = EEBASIC_FP

EEBASIC_FP = __AVR5__ __PIC30__ __FP__ __EDF__ __AVR5_EXAMPLES__ __PIC30_EXAMPLES__ __ALARMS__ __SEM__

# -------------------------------------------------------------------

# EE, for the commercial distribution

EE = EE_ALL EE_OPENZB

EE_ALL = __PIC30__ __FP__ __EDF__ __PIC30_EXAMPLES__ __OO_BCC1__  __FRSH__ __SEM_FRSH__ __ALARMS__ __SEM__

EE_OPENZB = __LIB_IEEE802154__

#EE_ALL = __AVR5__ __PIC30__ __FP__ __EDF__ __AVR5_EXAMPLES__ __PIC30_EXAMPLES__ __OO_BCC1__  __ALARMS__ __SEM__


# -------------------------------------------------------------------

# EE FLEX, for the Flex CDROM

EEFLEX = EEFLEX_ALL

EEFLEX_ALL = __PIC30__ __FP__ __OO_BCC1__ __EDF__ __PIC30_EXAMPLES__ __ALARMS__ __SEM__


# EE AVR5, by now, for downloading
EEAVR5 = EEAVR5_ALL

EEAVR5_ALL = __AVR5__ __FP__ __EDF__ __AVR5_EXAMPLES__ __ALARMS__ __SEM__

# -------------------------------------------------------------------

# EE for PIC30, AVR5 and ARM7

EE3ARCH = EE3ARCH_FP

EE3ARCH_FP = __AVR5__ __PIC30__ __ARM7GNU__ __SAMSUNG_KS32C50100__ __EVALUATOR7T__ __FP__ __EDF__ __OO_BCC1__ __ALARMS__ __SEM__ __AVR5_EXAMPLES__ __PIC30_EXAMPLES__ __LIB_IEEE802154__
