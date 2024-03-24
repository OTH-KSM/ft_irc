CXX = c++ -fsanitize=address
CPPFLAGS = -Wall -Wextra -Werror -std=c++98
SRC = srcs/main.cpp srcs/Server.cpp srcs/Client.cpp srcs/Channel.cpp srcs/File.cpp \
		cmds/JOIN.cpp cmds/INVITE.cpp cmds/MODE.cpp cmds/PRIVMSG.cpp cmds/QUIT.cpp 	\
		cmds/NICK.cpp cmds/USER.cpp cmds/PASS.cpp cmds/TOPIC.cpp cmds/Unknown.cpp \
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


# Use ctrl+D to send the command in several parts: ’com’, then ’man’, then ’d\n’
# Just like in the subject, using nc, try to send partial commands. Check that the server answers correctly. With a partial command sent, ensure that other connections still run fine.

# Your reference client must be able to connect to your server without encountering
#  any error.

# Unexpectedly kill a client. Then check that the server is still operational for the other connections and for any new incoming client.

# the lower case function should be in every command contained the channel as parameter
