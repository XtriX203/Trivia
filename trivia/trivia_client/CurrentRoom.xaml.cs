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
using System.Threading;
using System.ComponentModel;

namespace trivia_client
{
    /*
    CURRENT ROOM
    This class responsible on the CurrentRoom window
        * write a text in TextBoxes (usernameMsg, roomName and adminMsg)
        * write a text in the back button (admin - close, member - leave)
        * refresh the player list (using thread)

    Methods:
        * public CurrentRoom(String username, String admin, String name, Socket sc, int id = -1)                    - C'tor
        * private void _bw_DoWork(object sender, DoWorkEventArgs e)                                                 - Get room state every 3 seconds
        * private void _bw_ProgressChanged(object sender, ProgressChangedEventArgs e)                               - Get room state msg, and check if (the game has started, the room has closed or the players list has chaged)
        * private void _bw_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e) { _bw.CancelAsync(); } 
        * private void ButtonBack_Click(object sender, RoutedEventArgs e)                                           - When the user clicks on the back button, it will close the room or leave it (depending on if you are an admin or a member).
        * private bool isAdmin()                                                                                    - This method checks if this user is the admin or not
     */

    public partial class CurrentRoom : Window
    {
        private String _username;                               
        private RoomState _room;                                
        private String _admin;                                  
        private String _name;                                   
        private Socket _sc;                                     
        private int _id;    // by default is -1 
        private BackgroundWorker _bw = new BackgroundWorker();  // this thread responsible on getting data about the current room (status, hasGameBegun, questionCount, answerTimeout and players)

        /* 
        C'tor
        init a new CurrentRoom object
            input:      username, admin, name (room name), sc (socket), id (by default is -1)
            output:     new CurrentRoom object
         */
        public CurrentRoom(String username, String admin, String name, Socket sc, int id = -1)
        {
            this._username = username;
            this._admin = admin;
            this._name = name;
            this._sc = sc;

            if (id == -1)
            {
                Rooms roomList = JsonConvert.DeserializeObject<Rooms>(Helper.getResponse("", ((int)CODES.GETROOM_CODE).ToString(), this._sc)); // get rooms
                if (roomList.rooms != null) foreach (Room room in roomList.rooms) if (room.name == this._name) // find ID
                        {
                            id = room.id;
                            break;
                        }
            }
            this._id = id;

            InitializeComponent();

            //initialling thereads for auto refresh
            _bw.WorkerSupportsCancellation = true;
            _bw.WorkerReportsProgress = true;
            _bw.DoWork += _bw_DoWork;
            _bw.ProgressChanged += _bw_ProgressChanged;
            _bw.RunWorkerCompleted += _bw_RunWorkerCompleted;
            _bw.RunWorkerAsync();

            // write a text in TextBoxes (usernameMsg and roomName)
            this.usernameMsg.Text = this._username;
            this.roomName.Text = "Room name: " + this._name;

            // write a text in the back button (admin - close, member - leave)
            if (this.isAdmin())
            {
                this.back.Content = "Close";
                this.StartGame.Content = "Start game";
            }
            else
            {
                this.back.Content = "Leave";
                this.StartGame.Content = "אני שונא אנשים";
            }
        }

        /*
        _bw_DoWork
        get room state every 3 seconds
         */
        private void _bw_DoWork(object sender, DoWorkEventArgs e)
        {
            while (!_bw.CancellationPending)
            {
                _bw.ReportProgress(0); 
                Thread.Sleep(3000); 
            }
            e.Cancel = true;
        }

        /*
        _bw_ProgressChanged
        get room state msg, and check if
            * the game has started
            * the room has closed
            * the players list has chaged
         */
        private void _bw_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            string temp = Helper.getResponse("", ((int)CODES.GETROOMSTATE_CODE).ToString(), this._sc); // send a get room state packet
            if (!this.isAdmin() && temp == "{\"message\":\"Error\"}") // if this is a member and he gets an error packet, it means that the room was closed by the admin
            { 
                LoggedInUser w = new LoggedInUser(this._username, this._sc);
                _bw.CancelAsync();
                w.Show();
                this.Close();
                MessageBox.Show("Room closed");
            }
            else // If the room is not closed
            {
                this._room = JsonConvert.DeserializeObject<RoomState>(temp);
                this._admin = this._room.players[0];
                this.adminMsg.Text = "Admin: " + this._admin; // admin msg
                // new players list
                players.Items.Clear();
                foreach (String player in this._room.players) players.Items.Add(player);
                if (this._room.hasGameBegun) // check if the game has started
                {
                    StartGame w = new StartGame(this._room.questionCount, this._room.answerTimeout, this._username, this._sc);
                    _bw.CancelAsync();
                    w.Show();
                    this.Close();
                    MessageBox.Show("Game started");
                }
            }
        }

        private void _bw_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e) { _bw.CancelAsync(); }

        /*
        BUTTOM BACK CLICK
        When the user clicks on the back button, it will close the room or leave it (depending on if you are an admin or a member).
        */
        private void ButtonBack_Click(object sender, RoutedEventArgs e)
        {
            if (this.isAdmin()) 
            {
                MessageBoxResult result = MessageBox.Show("Are you sure that you want to close this room?", "", MessageBoxButton.YesNo); // check if the admin want to close this room
                switch (result)
                {
                    case MessageBoxResult.Yes:
                        if (Helper.checkResponse("", ((int)CODES.CLOSEROOM_CODE).ToString(), this._sc)) // close room
                        {
                            LoggedInUser w = new LoggedInUser(this._username, this._sc);
                            _bw.CancelAsync();
                            w.Show();                           
                            this.Close();
                        }
                        else MessageBox.Show("Couldn't close this room");
                        break;
                    case MessageBoxResult.No:
                        MessageBox.Show("ok :)");
                        break;
                }
            }
            else
            {
                if (Helper.checkResponse("", ((int)CODES.LEAVEROOM_CODE).ToString(), this._sc)) // leave room
                {
                    LoggedInUser w = new LoggedInUser(this._username, this._sc);
                    _bw.CancelAsync();
                    w.Show();
                    this.Close();
                }
            }
        }

        /* 
        IS ADMIN
        This method checks if this user is the admin or not
        input:      none
        output:     true (the user is the admin) of false
        */
        private bool isAdmin() { return this._username == this._admin; }

        private void StartGame_Click(object sender, RoutedEventArgs e)
        {
            if (this.isAdmin())
            {
                if (Helper.checkResponse("", ((int)CODES.STARTGAME_CODE).ToString(), this._sc))
                {
                    StartGame w = new StartGame(this._room.questionCount, this._room.answerTimeout, this._username, this._sc);
                    _bw.CancelAsync();
                    w.Show();
                    this.Close();
                }
            }
            else
            {
                bobspongeVideo w = new bobspongeVideo(this._username, this._admin, this._name, this._sc, this._id);
                w.Show();
            }
        }
    }
}
