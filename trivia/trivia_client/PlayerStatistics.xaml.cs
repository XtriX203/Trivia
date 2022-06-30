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
    PLAYER STATISTICS
    This class responsible on the PlayerStatistics window
        * print the user stats (avg answer time, num of currect answers, total answers and num of games)
        * welcome TextBlock

    Methods:
        * public PlayerStatistics(String username, Socket sc)               - C'tor
        * private void ButtonBack_Click(object sender, RoutedEventArgs e)   - When the user clicks on the back button, it will back to Statistics window
     */

    public partial class PlayerStatistics : Window
    {
        private String _username;
        private Socket _sc;
        private Stats _statsList;

        /* 
        C'tor
        init a new PlayerStatistics object
            input:      username, sc (socket)
            output:     new PlayerStatistics object
         */
        public PlayerStatistics(String username, Socket sc)
        {
            this._username = username;
            this._sc = sc;
            InitializeComponent();

            welcome.Text = "Hello " + username;

            this._statsList = JsonConvert.DeserializeObject<Stats>(Helper.getResponse("", ((int)CODES.GETUSERSTATS_CODE).ToString(), this._sc)); // get the user stats
            if (this._statsList.statistics != null) // write the result in the TextBlocks
            {
                avgAnsTime.Text = this._statsList.statistics[0].averageAnswerTime.ToString();
                numOfCorrectAns.Text = this._statsList.statistics[0].correctAnswers.ToString();
                totalAns.Text = this._statsList.statistics[0].totalAnswers.ToString();
                numOfGames.Text = this._statsList.statistics[0].gamesCount.ToString();
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
