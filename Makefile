#
# modules/dtUtils/Makefile
#
 
# Build the iRODS norStore module
#
 
ifndef buildDir
buildDir = $(CURDIR)/../..
endif

# Set this flag if you intend to build the msiSha1Data microservice
# WARNING: because of the way that iRODS works if you enable this flag
# the first time you build it MUST always be set. The problem is the
# iRODS server libraries have a reference to the module and if you 
# run make with it on and then off and run a make clean you may 
# have an error complaining it cannot find the sha1Data file.
# DTSSL = 1


include $(buildDir)/config/config.mk
include $(buildDir)/config/platform.mk
include $(buildDir)/config/directories.mk
include $(buildDir)/config/common.mk


#
# Directories
#
MSObjDir =	$(modulesDir)/dtUtils/microservices/obj
MSSrcDir =	$(modulesDir)/dtUtils/microservices/src
MSIncDir =	$(modulesDir)/dtUtils/microservices/include

#
# Source files
#
sha1Dir= $(MSSrcDir)/sha1
emailDir= $(MSSrcDir)/email

#
# Object files
SHA1_OBJS= $(MSObjDir)/sha1Data.o
EMAIL_OBJS= $(MSObjDir)/semailMS.o

OBJECTS =	$(EMAIL_OBJS)
ifdef DTSSL
OBJECTS +=	$(SHA1_OBJS)
endif

INCLUDE_FLAGS =	-I$(MSIncDir)

#
# Compile and link flags
#
INCLUDES +=	$(INCLUDE_FLAGS) $(LIB_INCLUDES) $(SVR_INCLUDES)
CFLAGS_OPTIONS := -DRODS_SERVER $(CFLAGS) $(MY_CFLAG)
CFLAGS =	$(CFLAGS_OPTIONS) $(INCLUDES) $(MODULE_CFLAGS)

ifdef DTSSL
LIBS =	-lcrypto
endif

.PHONY: all rules microservices server client clean
.PHONY: server_ldflags client_ldflags server_cflags client_cflags
.PHONY: print_cflags

# Build everything
all:	microservices
	@true

# List module's objects for inclusion in the clients
client_ldflags:
	@true

# List module's includes for inclusion in the clients
client_cflags:
	@true

# List module's objects for inclusion in the server
server_ldflags:
	@echo $(OBJECTS) $(LIBS)

# List module's includes for inclusion in the server
server_cflags:
	@echo $(INCLUDE_FLAGS)

# Build microservices
microservices:	print_cflags $(OBJECTS)
	@true

# Build client additions
client:
	@true

# Build server additions
server:
	@true

# Build rules
rules:
	@true

# Clean
clean:
	@echo "Clean dtUtils module..."
	@rm -f $(OBJECTS)





# Show compile flags
print_cflags:
	@echo "Compile flags:"
	@echo "    $(CFLAGS_OPTIONS)"





#
# Compilation targets
#
ifdef DTSSL
$(SHA1_OBJS): $(MSObjDir)/%.o: $(sha1Dir)/%.c $(DEPEND)
	@echo "Compile dtUtils module `basename $@`..."
	@$(CC) -c $(CFLAGS) -o $@ $<
$(EMAIL_OBJS): $(MSObjDir)/%.o: $(emailDir)/%.c $(DEPEND)
	@echo "Compile dtUtils module `basename $@`..."
	@$(CC) -c $(CFLAGS) -o $@ $<
endif

ifndef DTSSL
$(EMAIL_OBJS): $(MSObjDir)/%.o: $(emailDir)/%.c $(DEPEND)
	@echo "Compile dtUtils module `basename $@`..."
	@$(CC) -c $(CFLAGS) -o $@ $<
endif
