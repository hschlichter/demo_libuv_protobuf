CC = clang++
CFLAGS = -std=c++14 -O0 -g -Wall -MMD -pthread
LDFLAGS = -luv -lprotobuf
INCLUDE = -I. -I./external/libuv/include -I./external/protobuf-3.6.1/include
LDINCLUDE = -L./external/libuv -L./external/protobuf-3.6.1/lib

OUT = out
GENERATED = generated

PROTO_FILES = rpc.proto\
			  hello.proto

SERVER_SRCS = server.cpp
CLIENT_SRCS = client.cpp
SHARED_SRCS = common.cpp

SERVER_OBJS = $(foreach obj, $(SERVER_SRCS:.cpp=.o), $(OUT)/$(obj))
CLIENT_OBJS = $(foreach obj, $(CLIENT_SRCS:.cpp=.o), $(OUT)/$(obj))
SHARED_OBJS = $(foreach obj, $(SHARED_SRCS:.cpp=.o), $(OUT)/$(obj))

PROTO_OBJS = $(foreach obj, $(PROTO_SRCS:.cc=.o), $(OUT)/$(obj))
PROTO_SRCS = $(foreach obj, $(PROTO_FILES:.proto=.pb.cc), $(GENERATED)/$(obj))\
 
# Main build rules.
.PHONY: all
all: server client

# Protobuffer build rules
$(PROTO_SRCS): $(GENERATED)/%.pb.cc: %.proto
	@ echo [PROTOC] $<
	@./external/protobuf-3.6.1/bin/protoc -I. --cpp_out=$(GENERATED) $<

$(PROTO_OBJS): $(OUT)/%.o: %.cc
	@echo [CC] $<
	@$(CC) -c $(CFLAGS) -o $@ $< $(INCLUDE)

-include $(PROTO_OBJS:%.o=%.d)

client: $(PROTO_OBJS) $(CLIENT_OBJS) $(SHARED_OBJS) 
	@echo [LINK] $@
	@$(CC) $^ -o $(OUT)/$@ -Wl,-rpath,. $(CFLAGS) $(INCLUDE) $(LDINCLUDE) $(LDFLAGS)

server: $(PROTO_OBJS) $(SERVER_OBJS) $(SHARED_OBJS) 
	@echo [LINK] $@
	@$(CC) $^ -o $(OUT)/$@ -Wl,-rpath,. $(CFLAGS) $(INCLUDE) $(LDINCLUDE) $(LDFLAGS)

$(SERVER_OBJS): $(OUT)/%.o: %.cpp
	@echo [CC] $<
	@$(CC) -c $(CFLAGS) -o $@ $< $(INCLUDE)

-include $(SERVER_OBJS:%.o=%.d)

$(CLIENT_OBJS): $(OUT)/%.o: %.cpp
	@echo [CC] $<
	@$(CC) -c $(CFLAGS) -o $@ $< $(INCLUDE)

-include $(CLIENT_OBJS:%.o=%.d)

$(SHARED_OBJS): $(OUT)/%.o: %.cpp
	@echo [CC] $<
	@$(CC) -c $(CFLAGS) -o $@ $< $(INCLUDE)

-include $(SHARED_OBJS:%.o=%.d)

# Clean rules
.PHONY: clean
clean:
	@rm -rf $(OUT)
	@mkdir -p $(OUT)/$(GENERATED)
	@rm -rf $(GENERATED)
	@mkdir -p $(GENERATED)

# Debugging
print-%  : ; @echo $* = $($*)

