CC = clang++
CFLAGS = -std=c++14 -O0 -g -Wall -MMD -Wl,-rpath,.
LDFLAGS = -luv
INCLUDE = -I. -I./libuv/include
LDINCLUDE = -L./libuv

SRCS = main.cpp

OUTDIR = ./out
OBJS = $(foreach obj, $(SRCS:.cpp=.o), $(OUTDIR)/$(obj))

# Main build rules.
.PHONY: all
all: $(OBJS)
	$(CC) $^ -o $(OUTDIR)/main $(CFLAGS) $(INCLUDE) $(LDINCLUDE) $(LDFLAGS)

$(OBJS): $(OUTDIR)/%.o: %.cpp
	$(CC) -c $(CFLAGS) -o $@ $< $(INCLUDE)

-include $(OBJS:%.o=%.d)

# Clean rules
.PHONY: clean
	clean:
	@rm -rf $(OUTDIR)
	@mkdir $(OUTDIR)
