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

namespace trivia_client
{
    /// <summary>
    /// Interaction logic for bobspongeVideo.xaml
    /// </summary>
    public partial class bobspongeVideo : Window
    {
        private String _username;
        private String _admin;
        private String _name;
        private Socket _sc;
        private int _id;         
        public bobspongeVideo(String username, String admin, String name, Socket sc, int id)
        {
            this._username = username;
            this._sc = sc;
            InitializeComponent();
        }

        private void ButtonBack_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
        }
    }
}
