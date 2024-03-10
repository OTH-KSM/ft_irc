# ft_irc

## IRC MESSAGE STRUCTURE 

Client Message : Command + ASCII space + Parameters . . . 

Server reply   : 


## COMMANDS TO CREATE A NEW CLIENT

# CONNECT TO A server

NICK + (user's nickname) 
- Errors
    1. ERR_NONICNAMEGIVEN
    2. ERR_NICKNAMEINUSE
USER + (user's username)
- Errors
    1. 

# MESSAGING BETWEEN USERS

PRIVMSG + (dst's nickname) + (message)

# JOINING TALKING LEAVING CHANNELS

JOIN + ((#/&)channel's name)

PRIVMSG + ((#/&)channel's name) + (message)

PART + ((#/&)channel's name) + (leaving message)


### THINGS TO BE FIXED :

- when prefixes : when the last params starts with : prefix and the newline get contained in the string of the last param

- all passwords get accepted when using limechat