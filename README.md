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







==19703==ERROR: AddressSanitizer: BUS on unknown address 0x00010bb15d8d (pc 0x00010bb15d8d bp 0x7ffee40f07d0 sp 0x7ffee40ef920 T0)
AddressSanitizer:DEADLYSIGNAL
AddressSanitizer: nested bug in the same thread, aborting.

## get the ip addres of the connected client