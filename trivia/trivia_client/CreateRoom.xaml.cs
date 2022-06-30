using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using System.Net;
using System.Net.Sockets;
using Newtonsoft.Json;

namespace trivia_client
{
    /*
    CREATE ROOM
    This class responsible on the CreateRoom window
        * write a text in TextBoxes (msg the username)
        * reception the room data that the user wants to create
        * check input
        * create room

    Methods:
        * public CreateRoom(String username, Socket sc)                         - C'tor
        * private void ButtonBack_Click(object sender, RoutedEventArgs e)       - When the user clicks on the back button, it will back to LoggenInUser window
        * private void ButtonContinue_Click(object sender, RoutedEventArgs e)   - When the user clicks on the continue button, this method checks input, and then send a create room packet
        * private static bool isNegative(string msg, int value)                 - This method checks if the value is negative
     */
    public partial class CreateRoom : Window
    {
        private String _username;
        private String _roomName;
        private int _timeForQuestion;
        private int _numOfPlayers;
        private int _numOfQuestions;
        private Socket _sc;

        /* 
        C'tor
        init a new CreateRoom object
            input:      username, sc (socket)
            output:     new CreateRoom object
         */
        public CreateRoom(String username, Socket sc)
        {
            this._username = username;
            this._sc = sc;
            InitializeComponent();
            msg.Text = "Admin: " + username; // write text in this TextBlock
        }

        /*
        BUTTOM BACK CLICK
        When the user clicks on the back button, it will back to LoggenInUser window
        */
        private void ButtonBack_Click(object sender, RoutedEventArgs e)
        {
            LoggedInUser w = new LoggedInUser(this._username, this._sc);
            w.Show();
            this.Close();
        }

        /*
        BUTTON CONTINUE CLICK
        When the user clicks on the continue button, this method checks input, and then send a create room packet
         */
        private void ButtonContinue_Click(object sender, RoutedEventArgs e)
        {
            if (this.name.Text == "" || this.time.Text == "" || this.numOfPlayers.Text == "" || this.numOfQueststions.Text == "") MessageBox.Show("Please finish typing all the arguments."); // check if the user writes all the required data
            else
            {
                try
                {
                    // check input (is invalid chars)
                    this._roomName = this.name.Text;
                    this._timeForQuestion = int.Parse(this.time.Text);
                    this._numOfPlayers = int.Parse(this.numOfPlayers.Text);
                    this._numOfQuestions = int.Parse(this.numOfQueststions.Text);
                    
                    if ((!(isNegative("num of players", this._numOfPlayers) || isNegative("num of questions", this._numOfQuestions) || isNegative("time per question", this._timeForQuestion))) // check if the values valid
                            && Helper.checkResponse("{" + $"\"name\":\"{ this._roomName }\",\"maxPlayers\":{this._numOfPlayers},\"numOfQuestionInGame\":{this._numOfQuestions},\"timePerQuestion\":{this._timeForQuestion}" + "}", ((int)CODES.CREATEROOM_CODE).ToString(), this._sc)) // try to create the room
                    {
                        CurrentRoom w = new CurrentRoom(this._username, this._username, this._roomName, this._sc);
                        w.Show();
                        this.Close();
                    }
                    else MessageBox.Show("Unsuccessfully create room.");
                }
                catch (Exception exc) { MessageBox.Show("Invalid chars"); }
            }
        }

        /*
        IS NEGATIVE
        This method checks if the value is negative
        input:      msg (if the value is negative it will print this msg), value
        output:     
         */
        private static bool isNegative(string msg, int value)
        {
            if (value <= 0)
            {
                MessageBox.Show($"Incorrect {msg}");
                return true;
            }
            return false;
        }
    }
}
