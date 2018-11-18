ccflags-y += ${PROJECTINCLUDE}

main_SOURCES = $(notdir $(wildcard $(PROJECT_ROOT)/application/src/*.c))
main_OBJS = $(patsubst %.c, %.o, $(main_SOURCES))

obj-y = main.o
