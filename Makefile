#makefile for recomm
#SET UP HELP
# 1.(for compilation)
# 1.1 check and correct these paths in USER SET-UP section
#  BOOSTLD=/opt/boost/stage/lib
#  BOOSTINC=-I/opt/boost
# 1.2 root must be set up beforehand (in path + libraries)
# ex. source thisroot.sh

# 2. for running)
# uses shared libraries , paths may need to be set up
# LD_LIBRARY_PATH=$LD_LIBRARY_PATH:path-to-boost-lib:path-to-root-lib
# DYLD_LIBRARY_PATH=$DYLD_LIBRARY_PATH:path-to-boost-lib:path-to-root-lib
#source LD_LIBRARY_PATH
#source DYLD_LIBRARY_PATH


################################################################################
# USER SET-UP
################################################################################
# check and correct these paths
BOOSTLD=/opt/boost/stage/lib
BOOSTINC=-I/opt/boost


################################################################################
# DO NOT CHANGE ANYTHING BELOW THIS LINE
################################################################################
#http://make.paulandlesley.org/autodep.html
DEPDIR = .deps
df = $(DEPDIR)/$(*F)
MKDIR_P = mkdir -p

DEPDIR = .deps
df = $(DEPDIR)/$(*F)
MKDIR_P = mkdir -p

SRCGUI = GuiBase.cpp GuiMainFrame.cpp GuiMainFrameOffline.cpp GuiMainFrameOnline.cpp GuiTab.cpp GuiTabChamber.cpp GuiTabDaqMonitor.cpp GuiTabDetector.cpp GuiTabDetectorCrosstalk.cpp GuiTabDetectorEvent1D.cpp GuiTabDetectorRawUdp.cpp GuiTabMainOptions.cpp GuiTabMultiLayer.cpp GuiTabPlane.cpp GuiTabPlaneAll1D.cpp GuiTabReadout.cpp GuiTabReadout1D.cpp GuiTabReadout2D.cpp GuiTabReadoutStats.cpp GuiTabReadoutProps.cpp GuiTabStrip.cpp RootThread.cpp GuiTabDetectorStats.cpp GuiTabChamberEvent1D.cpp GuiTabChamberEvent2D.cpp GuiTabChamberStats.cpp
SRCDEF=Dict.cpp CApvRawTree.cpp Configuration.cpp DaqClientIpc.cpp DetBase.cpp DetChamber.cpp DetDetector.cpp DetPlane.cpp DetMultiLayer.cpp DetReadout.cpp DetStrip.cpp DetStripId.cpp DetectorBuilder.cpp EventBrowser.cpp EventBrowserOffline.cpp EventBrowserOnline.cpp  ParserApvRawTree.cpp ParserMmdaq3BnlMiniRawTree.cpp ParserTEnv.cpp ParserXml.cpp ProgException.cpp PropertyTreeParserChamber.cpp PropertyTreeParser.cpp ReaderBase.cpp RootReader.cpp StripsTree.cpp SrsChannelId.cpp SrsChipId.cpp main.cpp TreeBase.cpp TreeRawBnlMini.cpp TreeRawBnlTrigger.cpp Mmdaq3TreeApvRaw.cpp ParserMmdaq3ApvRawTree.cpp RootTreeVmm1Tac.cpp ShmemReader.cpp TreeRawApvPedestals.cpp GuiTabReadoutPedestals.cpp CSrsPedestalsRoot.cpp GuiTabDetectorPedestal1D.cpp GuiTabXmlCreator.cpp GuiTabFec.cpp ConfigXml.cpp GuiSaveFec.cpp GuiTabDetXml.cpp GuiSaveDet.cpp GuiTabSaveXml.cpp GuiAddApv.cpp ConfigRun.cpp GuiConfig.cpp ConfigDatabase.cpp
SRC = $(SRCDEF) $(SRCGUI)

OBJ=$(SRC:.cpp=.o)
HDR=$(SRC:.cpp=.h)

ROOTDICTHDRS=GuiBase.h GuiMainFrame.h GuiMainFrameOnline.h GuiMainFrameOffline.h GuiTab.h GuiTabDetector.h GuiTabMainOptions.h GuiTabPlane.h GuiTabReadout.h GuiTabDetectorCrosstalk.h ParserApvRawTree.h GuiTabChamber.h GuiTabXmlCreator.h GuiTabFec.h GuiSaveFec.h GuiTabDetXml.h GuiSaveDet.h GuiTabSaveXml.h GuiAddApv.h GuiConfig.h

EXEC=browser

CC=g++
CFLAGSBASE=-c -O3 -g -Wall 
#-Wextra
#-Wmost does not work in SLC 6 ?

BOOSTLIB= -L$(BOOSTLD) -lboost_date_time -lboost_program_options -lboost_filesystem -lboost_system -lboost_thread -lrt 

#for this to work root must be set up already (source thisroot.sh)
ROOTINC=-I`root-config --incdir`
ROOTCFLAGS=`root-config --cflags --auxcflags`
ROOTLIB=`root-config --glibs` -lGeom -lMinuit
ROOTLDFLAGS=`root-config --ldflags` 

CFLAGS=$(CFLAGSBASE) $(BOOSTINC) $(ROOTINC) $(ROOTCFLAGS)
LDFLAGS= $(ROOTLDFLAGS) $(ROOTLIB) $(BOOSTLIB)

# ‘$^’ is a list of all the prerequisites of the rule, including the names of the directories in which they were found
# '$@' is the target. 
# ‘$<’ is just the first prereq. Often the prereqs. include header files, which you do not want to mention in the recipe.
# from 4.5.4 Writing Recipes with Directory Search 
# http://www.gnu.org/software/make/manual/make.html
################################################################################



.PHONY : clean directories

all: directories $(EXEC)

directories: $(DEPDIR)

$(DEPDIR):
	@$(MKDIR_P) $(DEPDIR)

$(EXEC): $(OBJ)
	$(CC) $^ -o $@ $(LDFLAGS)  

%.o: %.cpp    
	$(CC) $(CFLAGS) -MD -o $@ $<
	@cp $*.d $(df).P; \
	sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
	-e '/^$$/ d' -e 's/$$/ :/' < $*.d >> $(df).P; \
	rm -f $*.d

-include $(SRC:%.cpp=$(DEPDIR)/%.P)

Dict.cpp: $(ROOTDICTHDRS) LinkDef.h
	@echo "* Generating root dictionary"
	rootcint -f $@ -c $^ 

clean:
	@echo "* Clean"
	@rm -f $(OBJ) Dict.cpp Dict.h
	@rm -f $(DEPDIR)/*.P

