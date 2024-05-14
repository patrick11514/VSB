using System.Collections.ObjectModel;
using System.Diagnostics;
using System.IO;
using System.Net.Http;
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
using System.Xml;
using System.Xml.Serialization;

namespace Test
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public List<string> towns { get; set; } = new()
        {
               "Ostrava",
               "Brno",
               "Praha"
        };

        public ObservableCollection<Subjekt> subjects { get; set; } = new();

        public string selectedTown { get; set; }

        public MainWindow()
        {
            InitializeComponent();

            this.DataContext = this;
        }

        private async void SelectTown(object sender, RoutedEventArgs e)
        {
            string URL = "https://dataor.justice.cz/api/file/zp-full-" + this.selectedTown.ToLower() + "-2024.xml";
            using var client = new HttpClient();
            var result = await client.GetAsync(URL);

            if (result.StatusCode != System.Net.HttpStatusCode.OK)
            {
                MessageBox.Show("Nepovedlo se stáhnout data", "Chyba", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            var serializer = new XmlSerializer(typeof(List<Subjekt>), new XmlRootAttribute("xml"));
            var readStream = result.Content.ReadAsStream();

            List<Subjekt> subjects = (List<Subjekt>)serializer.Deserialize(readStream);

            this.subjects.Clear();

            foreach (var subject in subjects)
            {
                this.subjects.Add(subject);
            }
        }

        private void OpenEdit(object sender, RoutedEventArgs e)
        {
            if (sender is not Button button)
            {
                return;
            }

            if (button.DataContext is not Subjekt subject)
            {
                return;
            }

            var edit = new SubjectEdit(subject);
            edit.ShowDialog();
        }
    }
}