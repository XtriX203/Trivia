import socket
import json
import time
HOST = "127.0.0.1"
PORT = 8876

#MENU CODES
LOGIN = 1
SIGNUP = 2
LOGOUT = 3
GET_ROOMS = 4
GET_PLAYERS_IN_ROOM = 5
GET_STATISTICS = 6
JOIN_ROOM = 7
CREATE_ROOM = 8
EXIT = 0

#CODES
GETUSERSTATS_CODE = 10
GETHIGHSCORE_CODE = 11
CREATEROOM_CODE =  20
JOINROOM_CODE =  30
GETPLAYERINROOM_CODE =  40
GETROOM_CODE =  50
SIGNOUT_CODE =  60
ERROR_CODE =  70
LOGIN_CODE =  80
SIGNUP_CODE =  90


#MINIMALC LENGTHS
MIN_LOGIN_LEN = 29
MIN_SIGNUP_LEN = 40
MIN_LOGOUT_LEN = 5
MIN_JOIN_ROOM_LEN = 12
MIN_GET_PLAYERS_IN_ROOM = 12
MIN_CREATE_ROOM_LEN = 67


#CREATE_ROOM
room_id = 0
room_name = ""
answer_timeout = ""
max_users = ""
question_count = ""


# This function sends a msg from the client to the server, and prints the answer
def send_msg(client, msg):

    client.send(msg.encode())
    data = client.recv(1024)
    print(data.decode())

def wrapLen(msg):
    if(len(str(msg))==1):
        return "000"+str(msg)
    if(len(str(msg))==2):
        return "00"+str(msg)
    if(len(str(msg))==1):
        return "0"+str(msg)
    if(len(str(msg))==4):
        return str(msg)

def main():

    command = 1
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((HOST, PORT))



    print("""
    Menu:
    Login               -  1
    Signup              -  2
    Logout              -  3
    Get rooms           -  4
    Get players in room -  5
    Get statistics      -  6
    Join room           -  7
    Create room         -  8
    EXIT                -  0
    """)
    
    while (command != EXIT):
        command = int(input("Enter the thing you want to do: "))
        data = ""
        if (command == LOGIN):
            print("You chose to login!\n")
            username = input("Enter username: ")
            password = input("Enter password: ")
            msglen = MIN_LOGIN_LEN + len(username) + len(password)
            data = str(LOGIN_CODE) +wrapLen(msglen) + "{"
            data += f'"username":"{username}","password":"{password}"'
            data += "}"
            print(data)

        if (command == SIGNUP):
            print("You chose to signup!")
            username = input("Enter username: ")
            password = input("Enter password: ")
            email = input("Enter email: ")
            msglen = MIN_SIGNUP_LEN + len(username) + len(password) + len(email)
            data = str(SIGNUP_CODE) + wrapLen(msglen) + "{"
            data += f'"username":"{username}","password":"{password}","email":"{email}"'
            data += "}"
            print(data)

        if (command == LOGOUT):
            print("You chose to logout!")
            data = str(SIGNOUT_CODE) + chr(0) +chr(0) +chr(0) +chr(0)
            print(data)

        if (command == GET_ROOMS):
            print("You chose to get the existing rooms!")
            data = str(GETROOM_CODE) + "0000"
            print(data)

        if (command == GET_PLAYERS_IN_ROOM):
            print("You chose to get the players in the room!")
            room_id = input("Enter the room id: ")
            msglen = MIN_GET_PLAYERS_IN_ROOM + len(room_id)
            data = str(GETPLAYERSINROOM_CODE) + wrapLen(msglen)  + "{"
            data += f'"room id":{room_id}' + "}"    
            print(data)                                

        if(command == GET_STATISTICS):
            print("You chose to get statistics!")
            data = str(GETUSERSTATS_CODE) + \
                chr(0) + chr(0) + chr(0) + chr(0)

        if (command == JOIN_ROOM):
            print("You chose to join a room!")
            room_id = input("Enter the id of the room you want to join")
            msglen = MIN_JOIN_ROOM_LEN + len(room_id)
            data = str(JOINROOM_CODE) + wrapLen(msglen)  + "{"
            data += f'"id":{room_id}' 
            data += "}"

        if (command == CREATE_ROOM):
            print("You chose to create a room!")
            room_name = input("Enter the name of the room you want to create: ")
            max_users = input("Enter max num of users in the room: ")
            question_count = input("Enter number of questions: ")
            answer_timeout = input("Enter answer timeout in seconds: ")
            msglen = MIN_CREATE_ROOM_LEN + \
                len(room_name) + len(max_users) + \
                len(question_count) + len(answer_timeout)
            data = str(CREATEROOM_CODE) + wrapLen(msglen)  + "{"
            data += f'"name":"{room_name}","maxPlayers":{max_users},"numOfQuestionInGame":{question_count},"timePerQuestion":{answer_timeout}'
            data += "}"
            print(data)

        if (command == EXIT):
            s.close()
        else:
            send_msg(s,data)



if __name__ == '__main__':
    main()