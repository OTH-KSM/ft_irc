CXX = c++ -fsanitize=address
CPPFLAGS = -Wall -Wextra -Werror -std=c++11
SRC = srcs/main.cpp srcs/server.cpp srcs/client.cpp srcs/channel.cpp srcs/utils.cpp	\
		cmds/JOIN.cpp cmds/INVITE.cpp cmds/MODE.cpp cmds/PRIVMSG.cpp cmds/QUIT.cpp 	\
		cmds/WHOIS.cpp cmds/NICK.cpp cmds/USER.cpp cmds/PASS.cpp
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
