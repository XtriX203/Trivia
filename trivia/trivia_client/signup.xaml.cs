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
    SIGNUP
    This class responsible on the signup window
        * signup

    Methods:
        * public signup(Socket sc)                                                  - C'tor
        * private void ButtonBack_Click(object sender, RoutedEventArgs e)           - When the user clicks on the back button, it will back to MainWindow window
        * private void ButtonSubmitSignup_Click(object sender, RoutedEventArgs e)   - When the user clicks on the back button, it will send a signup request
     */
    public partial class signup : Window
    {
        private Socket _sc;

        /* 
        C'tor
        init a new signup object
            input:      sc (socket)
            output:     new signup object
         */
        public signup(Socket sc)
        {
            this._sc = sc;
            InitializeComponent();
        }

        /*
        BUTTON BACK CLICK
        When the user clicks on the back button, it will back to MainWindow window
        */
        private void ButtonBack_Click(object sender, RoutedEventArgs e)
        {
            MainWindow w = new MainWindow();
            w.Show();
            this.Close();
        }

        /*
        BUTTON SUBMIT SIGNUP CLICK
        When the user clicks on the back button, it will send a signup request
        */
        private void ButtonSubmitSignup_Click(object sender, RoutedEventArgs e)
        {
            if (this.username.Text == "" || this.password.Text == "" || this.email.Text == "") MessageBox.Show("Please finish typing all the arguments."); // check if the user writes all the required data
            else if (Helper.checkResponse("{" + $"\"username\":\"{this.username.Text}\",\"password\":\"{this.password.Text}\",\"email\":\"{this.email.Text}\"" + "}", ((int)CODES.SIGNUP_CODE).ToString(), this._sc)) // successfully sign up
            {
                MainWindow w = new MainWindow(this._sc);
                w.Show();
                this.Close();
            }
            else MessageBox.Show("unsuccessfully sign up, try another username");
        }
    }
}
