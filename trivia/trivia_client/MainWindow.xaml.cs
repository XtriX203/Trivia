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
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Net;
using System.Net.Sockets;
using Newtonsoft.Json;

namespace trivia_client
{
    /*
    MAIN WINDOW
    This class responsible on the MainWindow window
        * login user

    Methods:
        * public MainWindow (Socket sc)                                     - C'tor
        * public MainWindow()                                               - C'tor
        * private void ButtonSignin_Click(object sender, RoutedEventArgs e) - This method tries to sign in the clien
        * private void ButtonQuit_Click(object sender, RoutedEventArgs e)   - This method closes this window
        * private void ButtonSignup_Click(object sender, RoutedEventArgs e) - This method opens the sign up window
     */
    public partial class MainWindow : Window
    {
        private Socket _sc;

        /* 
        C'tor
        init a new MainWindow object
            input:      sc (socket)
            output:     new MainWindow object
         */
        public MainWindow (Socket sc)
        {
            this._sc = sc;
            InitializeComponent();
        }

        /* 
        C'tor
        init a new MainWindow object
            input:      none
            output:     new MainWindow object
         */
        public MainWindow()
        {
            byte[] bytes = new byte[1024];

            try
            {
                IPAddress[] ipAddresses = Dns.GetHostAddresses("127.0.0.1");
                IPAddress ipAddress = ipAddresses[0];
                if (ipAddress.IsIPv6LinkLocal) ipAddress = ipAddresses[1]; // check if the IP address is in right format
                IPEndPoint remoteEP = new IPEndPoint(ipAddress, 8876);

                // Create a TCP/IP  socket.    
                this._sc = new Socket(ipAddress.AddressFamily, SocketType.Stream, ProtocolType.Tcp);

                // Connect the socket to the remote endpoint. Catch any errors.    
                try { this._sc.Connect(remoteEP); }
                catch (ArgumentNullException ane)
                {
                    // error
                    this._sc.Close();
                    this.Close();
                }
                catch (SocketException se)
                {
                    // error
                    this._sc.Close();
                    this.Close();
                }
                catch (Exception e)
                {
                    // error
                    this._sc.Close();
                    this.Close();
                }

            }
            catch (Exception e)
            {
                MessageBox.Show("error");
                this._sc.Close();
                this.Close();
            }
            InitializeComponent();
        }

        /*
        BUTTON SIGNIN CLICK
        This method tries to sign in the clien
        */
        private void ButtonSignin_Click(object sender, RoutedEventArgs e)
        {
            if (this.username.Text == "" || this.password.Text == "") MessageBox.Show("Please finish typing all the arguments."); // check if the user writes all the required data
            else
            {
                if (Helper.checkResponse("{\"username\":\"" + this.username.Text + "\",\"password\":\"" + this.password.Text + "\"}" , ((int)CODES.LOGIN_CODE).ToString(), this._sc))
                {
                    LoggedInUser w = new LoggedInUser(this.username.Text, this._sc);
                    w.Show();
                    this.Close();
                }
                else MessageBox.Show("Incorrect information. Please try again :)");
            }
        }

        /*
        BUTTON SIGNIN CLICK
        This method closes this window
        */
        private void ButtonQuit_Click(object sender, RoutedEventArgs e)
        {
            this._sc.Close();
            this.Close();
        }

        /*
        BUTTON SIGNIN CLICK
        This method opens the sign up window
        */
        private void ButtonSignup_Click(object sender, RoutedEventArgs e)
        {
            signup w = new signup(this._sc);
            this.Close();
            w.Show();
        }
    }
}
