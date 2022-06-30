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
    LOGGED IN USER
    This class responsible on the LoggedInUser window
        * displays a menu to the user (signout, create room, join room, statistics)

    Methods:
        * public LoggedInUser(String username, Socket sc)                       - C'tor
        * private void ButtonSignout_Click(object sender, RoutedEventArgs e)    - When the user clicks on the sign out button, it will sign out the user and open the MainWindow window
        * private void ButtonCreateRoom_Click(object sender, RoutedEventArgs e) - When the user clicks on the create room button, it will switch to CreateRoom window
        * private void ButtonJoinRoom_Click(object sender, RoutedEventArgs e)   - When the user clicks on the join room button, it will switch to JoinRoom window
        * private void ButtonStatistics_Click(object sender, RoutedEventArgs e) - When the user clicks on the statistics button, it will switch to Statistics window 
     */

    public partial class LoggedInUser : Window
    {
        private String _username;
        private Socket _sc;

        /* 
        C'tor
        init a new LoggedInUser object
            input:      username, sc (socket)
            output:     new LoggedInUser object
         */
        public LoggedInUser(String username, Socket sc)
        {
            this._username = username;
            this._sc = sc;
            InitializeComponent();
            welcome.Text = "Hello " + username; // welcome msg to the client
        }

        /*
        BUTTON SIGNOUT CLICK
        When the user clicks on the back button, it will sign out the user and open the MainWindow window
         */
        private void ButtonSignout_Click(object sender, RoutedEventArgs e)
        {
            if (Helper.checkResponse("", ((int)CODES.LOGOUT_CODE).ToString(), this._sc))
            {
                MainWindow w = new MainWindow(this._sc);
                w.Show();
                this.Close();
            }
            else MessageBox.Show("Unsuccessfully signout.");
        }

        /*
        BUTTON CREATE ROOM CLICK
        When the user clicks on the create room button, it will switch to CreateRoom window
         */
        private void ButtonCreateRoom_Click(object sender, RoutedEventArgs e)
        {
            CreateRoom w = new CreateRoom(this._username, this._sc);
            w.Show();
            this.Close();
        }

        /*
        BUTTON JOIN ROOM CLICK
        When the user clicks on the join room button, it will switch to JoinRoom window
         */
        private void ButtonJoinRoom_Click(object sender, RoutedEventArgs e)
        {
            JoinRoom w = new JoinRoom(this._username, this._sc);
            w.Show();
            this.Close();
        }

        /*
        BUTTON STATISTICS CLICK
        When the user clicks on the statistics button, it will switch to Statistics window
         */
        private void ButtonStatistics_Click(object sender, RoutedEventArgs e)
        {
            Statistics w = new Statistics(this._username, this._sc);
            w.Show();
            this.Close();
        }
    }
}
