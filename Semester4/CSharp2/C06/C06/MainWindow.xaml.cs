using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace C06
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public ObservableCollection<Customer> customers { get; set; } = new()
        {
            new Customer() {Id = 1, Name = "Pepa", Surname = "Jánský", Age = 23},
            new Customer() {Id = 2, Name = "Karel", Surname = "Vodstrčil", Age = 15},
            new Customer() {Id = 3, Name = "Lojza", Surname = "Malý", Age = 40}
        };

        public string BtnText { get; set; } = "Přidat zákazníka";

        public MainWindow()
        {
            InitializeComponent();

            this.DataContext = this;
        }

        private void AddCustomer(object sender, RoutedEventArgs e)
        {
            //customers.Add(new Customer() { Id = 4, Name = "Tomáš", Surname = "Blbý", Age = 50 });
            CustomerForm window = new();
            window.ShowDialog();

            customers.Add(window.Customer);
        }

        private void DeleteCustomer(object sender, RoutedEventArgs e)
        {
            Button btn = sender as Button;
            Customer c = btn.DataContext as Customer;
            this.customers.Remove(c);
        }

        private void AnonymizeCustomer(object sender, RoutedEventArgs e)
        {
            Button btn = sender as Button;
            Customer c = btn.DataContext as Customer;
            c.Name = "********";
            c.Surname = "********";
        }

        private void EditCustomer(object sender, RoutedEventArgs e)
        {
            Button btn = sender as Button;
            Customer c = btn.DataContext as Customer;

            CustomerForm window = new(c);
            window.ShowDialog();
        }
    }
}