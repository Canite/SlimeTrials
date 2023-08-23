#
# A Makefile that compiles all .c and .s files in "src" and "res" 
# subdirectories and places the output in a "obj" subdirectory
#

# If you move this project you can change the directory 
# to match your GBDK root directory (ex: GBDK_HOME = "C:/GBDK/"
ifdef OS
	GBDK_HOME = "F:/gbdk/"
	RGBDS_HOME = "F:/GBDev/rgbds-0.6.1-win64/"
	EMULATOR = "F:/bgb/bgb.exe"
else
	GBDK_HOME = "/home/canight/gbdk/"
	RGBDS_HOME = ""
	EMULATOR = "java"
	EMULATORFLAGS += -jar /home/canight/Downloads/emulicious/Emulicious.jar
endif

LCC = $(GBDK_HOME)bin/lcc
PNGCONVERTER = $(GBDK_HOME)bin/png2asset
PNGCONVERTERFLAGS = -tiles_only -spr8x8# -keep_palette_order

# You can set flags for LCC here
# For example, you can uncomment the line below to turn on debug output
LCCFLAGS += -debug # Uncomment to enable debug output
# LCCFLAGS += -v     # Uncomment for lcc verbose output


# You can set the name of the .gb ROM file here
PROJECTNAME    = Unknown

SRCDIR      = src
OBJDIR      = obj
RESDIR      = res
UTILDIR     = utils
BINS	    = $(OBJDIR)/$(PROJECTNAME).gb
CSOURCES    = $(foreach dir,$(SRCDIR),$(notdir $(wildcard $(dir)/*.c))) $(foreach dir,$(RESDIR),$(notdir $(wildcard $(dir)/*.c)))
ASMSOURCES  = $(foreach dir,$(SRCDIR),$(notdir $(wildcard $(dir)/*.s)))
IMAGEFILES  = $(foreach dir,$(RESDIR),$(notdir $(wildcard $(dir)/*.png)))
TILEDFILES  = $(foreach dir,$(RESDIR),$(notdir $(wildcard $(dir)/*.tmj)))
OBJS        = $(CSOURCES:%.c=$(OBJDIR)/%.o) $(ASMSOURCES:%.s=$(OBJDIR)/%.o)
RESOBJS		= $(IMAGEFILES:%.png=$(RESDIR)/%.c $(TILEDFILES:%.tmj=$(RESDIR)/%.c))

all:	prepare $(BINS)

compile.bat: Makefile
	@echo "REM Automatically generated from Makefile" > compile.bat
	@make -sn | sed y/\\//\\\\/ | sed s/mkdir\ -p\/mkdir\/ | grep -v make >> compile.bat

# Compile .png files in "res/" to bin files
$(RESDIR)/%.c:  $(RESDIR)/%.png
	$(PNGCONVERTER) $< $(PNGCONVERTERFLAGS) -o $@
	$(LCC) $(LCCFLAGS) -c -o $(OBJDIR)/$(*F).o $@

$(RESDIR)/%.c: 	$(RESDIR)/%.tmj
	$(UTILDIR)/convert_tile_json.py -i $< -o $@
	$(LCC) $(LCCFLAGS) -c -o $(OBJDIR)/$(*F).o $@

# Compile .c files in "src/" to .o object files
$(OBJDIR)/%.o:	$(SRCDIR)/%.c
	$(LCC) $(LCCFLAGS) -c -o $@ $<

# Compile .c files in "res/" to .o object files
$(OBJDIR)/%.o:	$(RESDIR)/%.c
	$(LCC) $(LCCFLAGS) -c -o $@ $<

# Compile .s assembly files in "src/" to .o object files
$(OBJDIR)/%.o:	$(SRCDIR)/%.s
	$(LCC) $(LCCFLAGS) -c -o $@ $<

# If needed, compile .c files in "src/" to .s assembly files
# (not required if .c is compiled directly to .o)
$(OBJDIR)/%.s:	$(SRCDIR)/%.c
	$(LCC) $(LCCFLAGS) -S -o $@ $<

# Link the compiled object files into a .gb ROM file
$(BINS):	$(RESOBJS) $(OBJS)
	$(LCC) $(LCCFLAGS) -o $(BINS) $(OBJS)

prepare:
	mkdir -p $(OBJDIR)

clean:
#	rm -f  *.gb *.ihx *.cdb *.adb *.noi *.map
	rm -f  $(OBJDIR)/*.*
	rm -f  $(RESDIR)/*.c
	rm -f  $(RESDIR)/*.h
	rm -f  $(RESDIR)/*.2bpp

run:
	$(EMULATOR) $(EMULATORFLAGS) $(OBJDIR)/$(PROJECTNAME).gb
