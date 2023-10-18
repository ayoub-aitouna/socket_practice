OBJ_DIR			:= .compiled

io_lib_name	= io/io.a

io_srcs		= $(wildcard io/**/*.c)

tcp_client_name = tcp_client
server = server

server_srcs	= $(wildcard server_core/*.c)
tcp_srcs	= $(wildcard tcp/*.c)

io_obj		= $(patsubst %.c, $(OBJ_DIR)/%.o, $(io_srcs))
server_obj	= $(patsubst %.c, $(OBJ_DIR)/%.o, $(server_srcs))
tcp_obj	= $(patsubst %.c, $(OBJ_DIR)/%.o, $(tcp_srcs))

$(OBJ_DIR)/%.o : %.c
	@mkdir -p $(dir $@)
	@echo "=> $@  ${GREEN}Created.${RESET} "
	@gcc -c $< -o $@

all: $(tcp_client_name) $(server)

$(io_lib_name) : $(io_obj)
	ar crs $(io_lib_name) $(io_obj)

$(tcp_client_name) : $(io_lib_name) $(tcp_obj)
	gcc $(tcp_obj) $(io_lib_name) -o $(tcp_client_name)

$(server) : $(io_lib_name) $(server_obj)
	gcc $(server_obj) $(io_lib_name) -pthread -o $(server)

clean:
	rm -rf $(OBJ_DIR) $(tcp_client_name) $(server) $(io_lib_name)
