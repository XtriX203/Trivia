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
    STATISTICS
    This class responsible on the Statistics window
        * displays a small statistics menu (player stats, stats table)

    Methods:
        * public Statistics(String username, Socket sc)                                 - C'tor
        * private void ButtonBack_Click(object sender, RoutedEventArgs e)               - When the user clicks on the back button, it will back to Statistics window
        * private void ButtonYourStatistics_Click(object sender, RoutedEventArgs e)     - When the user clicks on your statistics button, it will switch to PlayerStatistics window
        * private void ButtonStatisticsTable_Click(object sender, RoutedEventArgs e)    - When the user clicks on the statisctics table button, it will switch to StatisticsTable window
     */
    public partial class Statistics : Window
    {
        private String _username;
        private Socket _sc;

        /* 
        C'tor
        init a new Statistics object
            input:      username, sc (socket)
            output:     new Statistics object
         */
        public Statistics(String username, Socket sc)
        {
            this._username = username;
            this._sc = sc;
            InitializeComponent();
            welcome.Text = "Hello " + username;
        }

        /*
         BUTTOM BACK CLICK
         When the user clicks on the back button, it will back to Statistics window
         */
        private void ButtonBack_Click(object sender, RoutedEventArgs e)
        {
            LoggedInUser w = new LoggedInUser(this._username, this._sc);
            w.Show();
            this.Close();
        }

        /*
        BUTTON JOIN ROOM CLICK
        When the user clicks on your statistics button, it will switch to PlayerStatistics window
         */
        private void ButtonYourStatistics_Click(object sender, RoutedEventArgs e)
        {
            PlayerStatistics w = new PlayerStatistics(this._username, this._sc);
            w.Show();
            this.Close();
        }

        /*
        BUTTON STATISTICS TABLE CLICK
        When the user clicks on the statisctics table button, it will switch to StatisticsTable window
         */
        private void ButtonStatisticsTable_Click(object sender, RoutedEventArgs e)
        {
            StatisticsTable w = new StatisticsTable(this._username, this._sc);
            w.Show();
            this.Close();
        }
    }
}
