NAME = webserver
#webserver is better!!

SRCS = main.cpp \
		src/ConfigParser.cpp \
		src/Socket.cpp \
		src/Server.cpp \
		src/Cgi.cpp \
		src/Location.cpp \
		src/Client.cpp \
		src/Request.cpp \
		src/Response.cpp \
		src/Webserver.cpp \
		src/utility.cpp \
		# src/ConfigBlock.cpp \


OBJS = $(patsubst src/%.cpp,objs/%.o,$(SRCS))

OBJS_DIR = ./objs/

INCLUDE_DIR = include/
CC = c++ -I$(INCLUDE_DIR)
FLAG = -Wall -Wextra -Werror -g #-std=c++98


objs/%.o: src/%.cpp
	@mkdir -p objs
	@$(CC) $(FLAG) -c $< -o $@ -g


all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(FLAG) $(OBJS) -o $(NAME) -g
#	@mkdir -p $(OBJS_DIR)
#	@mv $(OBJS) $(OBJS_DIR).
	@echo "./$(NAME) has been created"




clean:
	@rm -rf $(OBJS_DIR)
	@echo "objs/ deleted"


fclean: clean
	@rm -rf $(NAME)
	@rm -f main.o src/*.o
	@echo "./$(NAME) deleted"


git:
	@git add .
	@git commit -m "$$MSG"
	@git push


re: fclean all clean


.PHONY: all clean fclean re
