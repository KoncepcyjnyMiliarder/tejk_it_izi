# Backend for online chat

Work is still in progress. Actually, the project is a one big playground. In current state, chat and friend subsystems are fully functional 8-)

## Features
+ Multiple, dynamically created chatrooms
+ Multiple identities per account
+ Friend system, notifies when your friend logs in or logs out of the server, allows you to add and remove users from your friendlist

## Requirements
+ c++11 compiler
+ installed PostgreSQL database
+ installed boost libraries

## How to build?
Create `build` directory, then call `cmake ..` from it (it may take a little while since CMake downloads dependencies like gtest and postgresql connector). Next `make`, wait for the binaries to be built. Head to `server` directory and run `initialize_database.sh` to create database tables to be used by server (you will be propmted for the postgres password). You are ready to go! The resulting executables are `build/server/tejk_it_izi/tejk_it_izi` (the server), `./client/commandline_client` (basic client).

## How to use the commandline client utility?
As you may have noticed, this is not a full blown chat client, but enough to see the backend in action.

Just after starting, you are prompted for server IP (optionally press `l` for localhost). If everything goes OK, you are in the first phase, the `login state`. All you can do right now is enter your username (for now there is no separate loginserver, the possible logins are hardcoded, see below). After succesfully logging in, you are moved into `lobby phase`. Here you can create (key `c`), delete (`d`) characters (up to 5) or select (`s`) to enter the "world". In the world the hotkeys go as follows:
+ `j` join chatroom (expecting chatroom name, you can join multiple rooms in one session!)
+ `l` leave chatroom (expecting chatroom name)
+ `s` send message (expecting chatroom name and message to be sent)
+ `a` add friend - the person you request to be added as your friend has to be online and reply with `a` aswell
+ `r` remove friend
+ `q` quit to lobby (also, quitting from server in lobby state in gentle way)

![Usage](https://raw.githubusercontent.com/KoncepcyjnyMiliarder/tejk_it_izi/master/tejk_it_izi.png)

## Known issues

+ 32 bit client is incompatible with 64 bit server because of message serialization method (did not think ahead, but it's pretty easy to fix)
+ it is possible to login the same account twice, please don't do it :)
+ there is no actual login server (i focused only on the "gameserver"), possible account names are hardcoded as `login == "krzysztof" || login == "mirek" || login == "foo"`