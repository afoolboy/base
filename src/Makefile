
GCC=g++
RM=rm -rf
TARGET=libbase.a

OUTPUT_OBJS_DIR=../objs
OUTPUT_INCLUDE_DIR=../include
OUTPUT_LIB_DIR=../release

OBJS=$(OUTPUT_OBJS_DIR)/fl_log.o 

INCLUDE_DIR := -I.
#INCLUDE_DIR += xxx


ifeq ( $(DEBUG), 1 )
	CFLAGS :=-g -O0
	# -DFL_LOG_TRACE
else
	CFLAGS :=-O2
	# -DFL_LOG_INFO
endif
CFLAGS+=$(INCLUDE_DIR) -static



LIB_DIR := -L.
#LIB_DIR += -Lxxx

LDFLAGS=


all: $(TARGET)

$(TARGET): $(OBJS)
	ar rcs $(OUTPUT_OBJS_DIR)/$@ $^

$(OUTPUT_OBJS_DIR)/%.o: %.cpp
	g++ $(CFLAGS) -c $< -o $@ 

$(OUTPUT_OBJS_DIR)/%.o: %.c
	gcc  $(CFLAGS) -c $< -o $@ 

$(OUTPUT_OBJS_DIR)/%.d: %.cpp
	@set -e;rm -rf $@;\
	g++ -MM $(CFLAGS) $< > $@.$$$$.tmp; \
	sed 's,\($*\)\.o[ :],$(OUTPUT_OBJS_DIR)/\1.o $@ :.g' < $@.$$$$.tmp > $@; 
	#rm -rf $@.$$$$.tmp

$(OUTPUT_OBJS_DIR)/%.d: %.c
	@set -e;rm -rf $@;\
	gcc -MM $(CFLAGS) $< > $@.$$$$.tmp; \
	sed 's,\($*\)\.o[ :]*,$(OUTPUT_OBJS_DIR)/\1.o $@ : ,g' < $@.$$$$.tmp > $@; 
	#rm -rf $@.$$$$.tmp



install:
	$(RM) ../include/*
	$(RM) ../lib/*
	cp ./*.h ../include/
	cp $(OUTPUT_OBJS_DIR)/$(TARGET) ../lib/

.PHONY:clean install 
	
clean:
	$(RM) $(OUTPUT_OBJS_DIR)/* 




include $(patsubst %.cpp, $(OUTPUT_OBJS_DIR)/%.d, $(wildcard *.cpp))
include $(patsubst %.c, $(OUTPUT_OBJS_DIR)/%.d, $(wildcard *.c))

# -include $(OBJS:.o=.d)
