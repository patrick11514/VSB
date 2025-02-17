
MAIN_NAME=$(notdir $(shell pwd) )

CPPFLAGS+=-g
LDLIBS=-lcudart -lcuda

# detect opencv lib
OPENCVLIB=$(shell pkgconf --list-package-names | grep opencv )

ifeq ($(OPENCVLIB),)

all:
	@echo OpenCV lib not found!
	@exit 

else

CPPFLAGS+=$(shell pkgconf --cflags $(OPENCVLIB))
LDFLAGS+=$(shell pkgconf --libs-only-L $(OPENCVLIB))
LDLIBS+=$(shell pkgconf --libs-only-l $(OPENCVLIB))

all: $(MAIN_NAME)

endif

CUDA_OBJS=$(addsuffix .o, $(basename $(wildcard *.cu) ) )

%.o: %.cu $(wildcard *.h)
	nvcc -c $(CPPFLAGS) $<

$(MAIN_NAME): $(wildcard *.cpp) $(CUDA_OBJS) $(wildcard *.h)
	g++ $(CPPFLAGS) $(LDFLAGS) $(filter %.cpp %.o, $^) $(LDLIBS) -o $@

clean:
	rm -f *.o $(MAIN_NAME)


