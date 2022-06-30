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
using System.Net.Sockets;
using System.IO;
using Newtonsoft.Json;
using System.Windows.Threading;

namespace trivia_client
{


    /// <summary>
    /// Interaction logic for StartGame.xaml
    /// </summary>
    public partial class StartGame : Window
    {
        private const int NUMBER_OF_BUTTONS = 4;
        private const int MIN_INDEX = 1;
        private int _questionCount;
        private int _questionsPassed;
        private int _answerTimeout;
        private string _username;
        private Socket _sc;
        private GetQuestion _question;
        private List<string> _answers = new List<string>();
        private Dictionary<int, int> _Indexes = new Dictionary<int, int>();//num of answer, id of answer 
        private DispatcherTimer t = new DispatcherTimer();
        private int timeOut;
        private int time = 0;

        /// <summary>
        /// constructor to game page
        /// </summary>
        public StartGame(int questionCount, int answerTimeout, string username, Socket sc)
        {
            this._questionCount = questionCount;
            this._answerTimeout = answerTimeout;
            this.timeOut = answerTimeout;
            this.time = answerTimeout;
            this._username = username;
            this._sc = sc;

            InitializeComponent();
            getNextAnswer();//call to get the first answer
        }

        private void getNextAnswer()
        {
            //if its not last question
            if (this._questionCount >=this._questionsPassed)
            {
                //get new question and write all the answers in the buttons randomly
                this._Indexes.Clear();
                this._question = JsonConvert.DeserializeObject<GetQuestion>(Helper.getResponse("", ((int)CODES.GETQUESTION_CODE).ToString(), this._sc));
                question.Text = this._question.question;
                this.Answer1.Content = this.getAnswer();
                this.Answer2.Content = this.getAnswer();
                this.Answer3.Content = this.getAnswer();
                this.Answer4.Content = this.getAnswer();
                this._questionsPassed++;
            }
        }

        //function that return random possible answer
        private string getAnswer()
        {
            Random rnd = new Random();
            int num = rnd.Next(0, this._question.answers.Count());
            string ans = this._question.answers.ElementAt(num).answer;//get the answer in the index num
            this._question.answers.RemoveAt(num);//remove the answer
            this._Indexes.Add(this._Indexes.Count()+1, num);//add the parameters to the dict
            return ans;
        }

        private void Answer1_Click(object sender, RoutedEventArgs e)
        {

            int value = 0;
            this._Indexes.TryGetValue(1, out value);
            string response = Helper.getResponse("{\"answerId\":" + value+"}",((int)CODES.SUBMITANSWER_CODE).ToString(),this._sc);
            printAns(value);
            getNextAnswer();
        }

        private void Answer2_Click(object sender, RoutedEventArgs e)
        {
            int value = 0;
            this._Indexes.TryGetValue(2, out value);
            string response = Helper.getResponse("{\"answerId\":" + value + "}", ((int)CODES.SUBMITANSWER_CODE).ToString(), this._sc);
            printAns(value);
            getNextAnswer();
        }

        private void Answer3_Click(object sender, RoutedEventArgs e)
        {
            int value = 0;
            this._Indexes.TryGetValue(3, out value);
            string response = Helper.getResponse("{\"answerId\":" + value + "}", ((int)CODES.SUBMITANSWER_CODE).ToString(), this._sc);
            printAns(value);
            getNextAnswer();
        }

        private void Answer4_Click(object sender, RoutedEventArgs e)
        {
            int value = 0;
            this._Indexes.TryGetValue(4, out value);
            string response = Helper.getResponse("{\"answerId\":" + value + "}", ((int)CODES.SUBMITANSWER_CODE).ToString(), this._sc);
            printAns(value);
            getNextAnswer();
        }
        private void printAns(int value)
        {
            if (value == 0)
            {
                flag.Text = "True";
                flag.Foreground = Brushes.Green;
            }
            else
            {
                flag.Text = "false";
                flag.Foreground = Brushes.Red;
            }
        }
    }
}
