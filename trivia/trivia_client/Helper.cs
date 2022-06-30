using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net;
using System.Net.Sockets;
using Newtonsoft.Json;
using System.Windows;

namespace trivia_client
{
    enum CODES
    {
        GETUSERSTATS_CODE = 10,
        GETHIGHSCORE_CODE = 11,
        CREATEROOM_CODE = 20,
        CLOSEROOM_CODE = 21,
        STARTGAME_CODE = 22,
        GETROOMSTATE_CODE = 23,
        LEAVEROOM_CODE = 24,
        LEAVEGAME_CODE = 25,
        GETQUESTION_CODE = 26,
        SUBMITANSWER_CODE = 27,
        GETGAMERESULTS_CODE = 28,
        JOINROOM_CODE = 30,
        GETPLAYERINROOM_CODE = 40,
        GETROOM_CODE = 50,
        LOGOUT_CODE = 60,
        ERROR_CODE = 70,
        LOGIN_CODE = 80,
        SIGNUP_CODE = 90
    };

    /*
    CREATE ROOM
    This class has some methods that help all the other classes needs

    Methods:
        * public static String checkLength(String data)                         - This method returns the length of the data, in a four-character string
        * public static String getResponse(String data, String code, Socket sc) - This method, sends a request and returns the response
        * public static bool checkResponse(String data, String code, Socket sc) - This method check if the response is not an error v
     */
    class Helper
    {
        /*
        CHECK LENGTH
        This method returns the length of the data, in a four-character string
        input:      data
        output:     length of the data, in a four-character string
         */
        public static String checkLength(String data)
        {
            String len = data.Length.ToString();
            if (data.Length == 0) return "0000";
            else if (len.Length == 1) return "000" + len;
            else if (len.Length == 2) return "00" + len;
            else if (len.Length == 3) return "0" + len;
            else if (len.Length == 4) return len;
            return "error";
        }

        /*
        GET RESPONSE
        This method, sends a request and returns the response
        input:      data, request code, sc
        output:     response
         */
        public static String getResponse(String data, String code, Socket sc) 
        {
            byte[] msg = Encoding.ASCII.GetBytes(code + checkLength(data) + data); // create msg
            int bytesSent = sc.Send(msg); // send msg
            int bytesRec = sc.Receive(msg, 2, SocketFlags.None); // get code
            bytesRec = sc.Receive(msg, 4, SocketFlags.None); // get the data length
            int size = int.Parse(Encoding.ASCII.GetString(msg, 0, 4));
            byte[] buffer = new byte[size];
            bytesRec = sc.Receive(buffer, size, SocketFlags.None); // read data
            return Encoding.ASCII.GetString(buffer, 0, bytesRec);
        }

        /*
        CHECK RESPONSE
        This method check if the response is not an error
        input:      data, code, sc
         */
        public static bool checkResponse(String data, String code, Socket sc) { return getResponse(data, code, sc) == "{\"status\":1}"; }
    }

    /* 
    ROOM STATE CLASS
    This class contains all the data that the server has sent about the room state
    */
    public class RoomState
    {
        public int status { get; set; } // status 1 means that there is no error
        public bool hasGameBegun { get; set; } // if the game has begun this field will be true, otherwise false
        public int questionCount { get; set; }  
        public int answerTimeout { get; set; }  
        public String[] players { get; set; } // array with all of the usernames
    }

    /*
    ROOM
    This class contains all the data that the sever has sent about a single room
     */
    public class Room
    {
        public int id { get; set; }
        public bool isActive { get; set; }
        public int maxPlayers { get; set; }
        public String name { get; set; }
        public int numOfQuestionsInGame { get; set; }
        public double timePerQuestion { get; set; }
    }

    /*
    ROOMS
    This class contains all the data that the sever has sent about the room list
     */
    public class Rooms
    {
        public List<Room> rooms { get; set; } // list of rooms
        public int status { get; set; } // status 1 means that there is no error
    }

    /*
    PLAYER STATS
    This class contains all the data that the sever has sent about a single player stats
     */
    public class PlayerStats
    {
        public double averageAnswerTime { get; set; }
        public int correctAnswers { get; set; }
        public int totalAnswers { get; set; }
        public int gamesCount { get; set; }
    }

    /*
    STATS
    This class contains all the data that the sever has sent about the stats response
     */
    public class Stats
    {
        public List<PlayerStats> statistics { get; set; }
        public int status { get; set; } // status 1 means that there is no error
    }

    /*
    TOP FIVE
    This class contains all the data that the sever has sent about the top 5 stats 
     */
    public class TopFive
    {
        public String[] names { get; set; } // usernames array
        public int status { get; set; } // status 1 means that there is no error
    }

    public class Answers
    {
        public string answer { get; set; }
        public int id { get; set; }
    }
    public class GetQuestion
    {
        public List<Answers> answers { get; set; }
        public string question { get; set; }
        public int status { get; set; }
    }
}
