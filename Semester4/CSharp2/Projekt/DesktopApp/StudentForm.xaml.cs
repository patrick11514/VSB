using DataLayer.Structures;
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

namespace DesktopApp
{
    /// <summary>
    /// Interakční logika pro StudentForm.xaml
    /// </summary>
    public partial class StudentForm : Window
    {
        public Student student { get; set; }
        public StudentForm(Student? student = null)
        {
            this.student = student ?? new Student();
            InitializeComponent();
            this.DataContext = this.student;
        }

        private void Save(object sender, RoutedEventArgs e)
        {
            this.Close();
        }
    }
}
