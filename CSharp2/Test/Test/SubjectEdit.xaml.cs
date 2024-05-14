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

namespace Test
{
    /// <summary>
    /// Interakční logika pro SubjectEdit.xaml
    /// </summary>
    public partial class SubjectEdit : Window
    {
        public Subjekt subject { get; set; }
        public SubjectEdit(Subjekt subjekt)
        {
            this.subject = subjekt;

            InitializeComponent();

            this.DataContext = subject;
        }

        private void Close(object sender, RoutedEventArgs e)
        {
            this.Close();
        }
    }
}
