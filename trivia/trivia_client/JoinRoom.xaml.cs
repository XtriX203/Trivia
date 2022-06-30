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
    JOIN ROOM
    This class responsible on the JoinRoom window
        * write a text in TextBoxes (welcome)
        * refresh the room list (using thread)
        * add the user to the selected room

    Methods:
        * public JoinRoom(String username, Socket sc)                                       - C'tor
        * private void _bw_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)    
        * private void _bw_ProgressChanged(object sender, ProgressChangedEventArgs e)       - Get room list msg, and check write a new room list
        * private void _bw_DoWork(object sender, DoWorkEventArgs e)                         - Get room list every 3 seconds
        * private void ButtonBack_Click(object sender, RoutedEventArgs e)                   - When the user clicks on the back button, it will back to LoggenInUser window
        * private void rooms_MouseDoubleClick(object sender, MouseButtonEventArgs e)        - When the user clicks on one of the rooms, it will add him to this room
     */
    public partial class JoinRoom : Window
    {
        private String _username;
        private Socket _sc;
        private Rooms _roomList;
        private BackgroundWorker _bw = new BackgroundWorker(); // thread

        /* 
        C'tor
        init a new JoinRoom object
            input:      username, sc (socket)
            output:     new JoinRoom object
         */
        public JoinRoom(String username, Socket sc)
        {
            this._username = username;
            this._sc = sc;

            InitializeComponent();
            welcome.Text = "Hello " + this._username; // welcome msg

            _bw.WorkerSupportsCancellation = true;
            _bw.WorkerReportsProgress = true;
            _bw.DoWork += _bw_DoWork;
            _bw.ProgressChanged += _bw_ProgressChanged;
            _bw.RunWorkerCompleted += _bw_RunWorkerCompleted;
            _bw.RunWorkerAsync();
        }

        private void _bw_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e) { _bw.CancelAsync(); }

        /*
        _bw_ProgressChanged
        get room list msg, and check write a new room list
         */
        private void _bw_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            this.rooms.Items.Clear();
            this._roomList = JsonConvert.DeserializeObject<Rooms>(Helper.getResponse("", ((int)CODES.GETROOM_CODE).ToString(), this._sc)); // get rooms
            if (this._roomList.rooms != null) foreach (Room room in this._roomList.rooms) this.rooms.Items.Add(room.name);
        }

        /*
        _bw_DoWork
        get room list every 3 seconds
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
        BUTTOM BACK CLICK
        When the user clicks on the back button, it will back to LoggenInUser window
        */
        private void ButtonBack_Click(object sender, RoutedEventArgs e)
        {
            LoggedInUser w = new LoggedInUser(this._username, this._sc);
            _bw.CancelAsync();
            w.Show();
            this.Close();
        }

        /*
        ROOMS MOUSE DOUBLE CLICK
        When the user clicks on one of the rooms, it will add him to this room
         */
        private void rooms_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            if (rooms.SelectedItem != null) 
            {
                foreach (Room room in this._roomList.rooms) // find room id
                {
                    if (rooms.SelectedItem.ToString() == room.name)
                    {
                        if (Helper.checkResponse("{" + $"\"id\":{room.id}" + "}", ((int)CODES.JOINROOM_CODE).ToString(), this._sc)) // try to add the user to this room
                        {
                            CurrentRoom w = new CurrentRoom(this._username, "", room.name, this._sc, room.id);
                            w.Show();
                            _bw.CancelAsync();
                            this.Close();
                        }
                        else MessageBox.Show("Couldn't add you to this room :(");
                    }
                }
            }
        }
    }
}
