#idIRCcli
Idiot('s) IRC Client.

#DISCLAIMER
Do NOT try this at home. It is not safe and, either running or reading this code, will make you sick. Progress at your own will.

#Instructions                                                                                                                                                                                                   
Just run:

```make main```

It needs at least: a standard cpp compiler with c++14 support (change in makefile)

#Documentation
The client will ask, when started: your nick, node, port, and password so it can access the server.

##Commands 
```-j CHANNEL_NAME``` : To join a channel (can only be connected with one at the same time).

```-r COMMAND``` : To send a raw command to the server.

```-s USER_NAME MESSAGE``` : To send a personal message to a user.

```-q``` : To quit the server.

Default behaviour is to send a message to the channel you're connected.

#Thanks
* [sic](http://tools.suckless.org/sic/)
* [weercd](https://github.com/weechat/weercd)
