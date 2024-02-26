CXX = c++
CPPFLAGS = -Wall -Wextra -Werror
SRC = main.cpp client.cpp
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
