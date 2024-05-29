NAME = webserver
#webserver is better!!

SRCS = main.cpp \
		src/ConfigParser.cpp \
		src/ConfigBlock.cpp \
		src/Socket.cpp \
		src/Server.cpp \
		src/Cgi.cpp \
		src/Client.cpp \
		src/Location.cpp \
		src/Request.cpp \
		src/Response.cpp \
		src/Webserver.cpp \
		src/utility.cpp \


OBJS = $(SRCS:.cpp=.o)
OBJS_DIR = ./objs/


INCLUDE_DIR = include/
CC = c++ -I$(INCLUDE_DIR)
FLAG = -Wall -Wextra -Werror -g -std=c++98


%.o: %.cpp
	@$(CC) $(FLAG) -c $< -o $@ -g


$(NAME): $(OBJS)
	@$(CC) $(FLAG) $(OBJS) -o $(NAME) -g
	@mkdir -p $(OBJS_DIR)
	@mv $(OBJS) $(OBJS_DIR).
	@echo "./$(NAME) has been created"


all: $(NAME)


clean:
	@rm -rf $(OBJS_DIR)
	@echo "objs/ deleted"


fclean: clean
	@rm -rf $(NAME)
	@echo "./$(NAME) deleted"


git:
	@git add .
	@git commit -m "$$MSG"
	@git push


re: fclean all clean


.PHONY: all clean fclean re
