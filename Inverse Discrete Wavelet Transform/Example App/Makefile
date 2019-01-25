OS := $(shell uname)
SYS := $(shell gcc -dumpmachine)

CXXFLAGS  = -std=gnu++11 -O2 -g -Wall -Wno-unknown-pragmas
#-std=gnu++14 
CFLAGS  = -g -Wall -Wno-unknown-pragmas


program_NAME := idwtphantom
program_OBJS =  $(UIO_OBJ) $(EASYBMP_OBJ) $(IDWT_ADAPTER_OBJ) $(MAIN_FILE)

SOURCE_DIR = /home/vivado/Petalinux-Projects/phantom_ip_tst/project-spec/meta-user/recipes-apps/idwtphantom/idwtphantom/src

CXXFLAGS += -I$(SOURCE_DIR)/easybmp -I$(SOURCE_DIR)/idwtadapter/drivers -I$(SOURCE_DIR)/idwtadapter
LDFLAGS += -Llib
LDLIBS += -lm


# --- Objects ---

EASYBMP_DIR = $(SOURCE_DIR)/easybmp
EASYBMP_OBJ = $(EASYBMP_DIR)/EasyBMP.o 

UIO_DIR = $(SOURCE_DIR)/idwtadapter/drivers
UIO_OBJ = $(UIO_DIR)/xidwt.o $(UIO_DIR)/xidwt_linux.o

IDWT_ADAPTER_DIR = $(SOURCE_DIR)/idwtadapter
IDWT_ADAPTER_OBJ = $(IDWT_ADAPTER_DIR)/idwt_adapter.o 

MAIN_FILE = idwtphantom.o

# ---

.PHONY: all clean distclean

all: $(program_NAME)

$(program_NAME):	$(program_OBJS)
	$(CXX) -static -static-libgcc -static-libstdc++ -o $(program_NAME) $(program_OBJS) -lpthread

dyn: $(program_OBJS)
	$(CXX) -o $(program_NAME) $(program_OBJS) -lpthread

clean:
	@- $(RM) $(program_NAME)
	@- $(RM) $(program_OBJS)

distclean: clean

