CXX = c++ -fsanitize=address
CPPFLAGS = -Wall -Wextra -Werror -std=c++98
SRC = srcs/main.cpp srcs/Server.cpp srcs/Client.cpp srcs/Channel.cpp srcs/File.cpp\
		cmds/JOIN.cpp cmds/INVITE.cpp cmds/MODE.cpp cmds/PRIVMSG.cpp cmds/QUIT.cpp 	\
		cmds/NICK.cpp cmds/USER.cpp cmds/PASS.cpp cmds/TOPIC.cpp \
		cmds/KICK.cpp srcs/Botox.cpp srcs/FileHandling.cpp
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
