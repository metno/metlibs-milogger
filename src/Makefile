LIBOBJ=logger.o

LIBSRC=$(LIBOBJ:.o=.cc)

VPATH=$(LIBDIR)

.SUFFIXES: .cc .o

.cc.o:
	$(CXX) $(CCFLAGS) -c $< $(INCLUDE)

all: $(LIBDIR)/libmiLogger.a

depends:
	rm -f $(DEPENDSFILE)
	echo '# Automatically-generated dependencies list:' > $(DEPENDSFILE)
	$(CXX) $(CCFLAGS) $(INCLUDE) -M $(LIBSRC) >> $(DEPENDSFILE)

$(LIBDIR)/libmiLogger.a: $(LIBOBJ)
	$(AR) $(ARFLAGS) $@ $(LIBOBJ)


include $(DEPENDSFILE)

