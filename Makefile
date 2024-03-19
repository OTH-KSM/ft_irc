CXX = c++ -fsanitize=address
CPPFLAGS = -Wall -Wextra -Werror -std=c++11
SRC = main.cpp server.cpp client.cpp channel.cpp utils.cpp
OBJ = $(SRC:.cpp=.o)
RM = rm -rf
NAME = ircserv

all : $(NAME)
	
$(NAME) : $(OBJ)
	$(CXX) $(CPPFLAGS) $(OBJ) -o $(NAME)

clean : 
	$(RM) $(OBJ)

fclean : clean	
	$(RM) $(NAME)

re : fclean all
