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

namespace C06
{
    /// <summary>
    /// Interakční logika pro CustomerForm.xaml
    /// </summary>
    public partial class CustomerForm : Window
    {
        public Customer Customer { get; set; }
        public CustomerForm(Customer c = null)
        {
            Customer = c ?? new Customer();
            InitializeComponent();

            this.DataContext = Customer;
        }

        private void Save(object sender, RoutedEventArgs e)
        {
            this.Close();
        }
    }
}
