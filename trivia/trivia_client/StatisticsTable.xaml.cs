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
    STATISTICS TABLE
    This class responsible on the StatisticsTable window
        * print the top five users
        * welcome TextBlock

    Methods:
        * public StatisticsTable(String username, Socket sc)                - C'tor
        * private void ButtonBack_Click(object sender, RoutedEventArgs e)   - When the user clicks on the back button, it will back to Statistics window
     */

    public partial class StatisticsTable : Window
    {
        private String _username;
        private Socket _sc;
        private TopFive _topFiveList;

        /* 
        C'tor
        init a new PlayerStatistics object
            input:      username, sc (socket)
            output:     new PlayerStatistics object
         */
        public StatisticsTable(String username, Socket sc)
        {
            this._username = username;
            this._sc = sc;

            InitializeComponent();
            welcome.Text = "Hello " + username; // welcome TextBlock

            this._topFiveList = JsonConvert.DeserializeObject<TopFive>(Helper.getResponse("", ((int)CODES.GETHIGHSCORE_CODE).ToString(), this._sc)); // get top 5
            if (this._topFiveList.names != null) // write the result in the TextBlocks
            {
                first.Text = this._topFiveList.names[0];
                second.Text = this._topFiveList.names[1];
                third.Text = this._topFiveList.names[2];
                fourth.Text = this._topFiveList.names[3];
                fifth.Text = this._topFiveList.names[4];
            }
        }

        /*
        BUTTOM BACK CLICK
        When the user clicks on the back button, it will back to Statistics window
        */
        private void ButtonBack_Click(object sender, RoutedEventArgs e)
        {
            Statistics w = new Statistics(this._username, this._sc);
            w.Show();
            this.Close();
        }
    }
}
