using DataLayer;
using DataLayer.Structures;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
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
    /// Interakční logika pro StudentList.xaml
    /// </summary>
    public partial class StudentList : Window
    {
        private MyDapper dapper;
        public ObservableCollection<Student> list { get; set; }
        public StudentList(MyDapper dapper)
        {
            this.dapper = dapper;
            InitializeComponent();

            this.FetchData();

            this.DataContext = this;
        }

        private async void FetchData()
        {
            this.list = new(await this.dapper.SelectAll<Student>());
        }

        private async void EditStudent(object sender, RoutedEventArgs e)
        {
            if (sender is not Button btn)
            {
                return;
            }

            if (btn.DataContext is not Student student)
            {
                return;
            }

            var studentForm = new StudentForm(student);
            studentForm.ShowDialog();

            await dapper.Update(studentForm.student);
        }

        private async void RemoveStudent(object sender, RoutedEventArgs e)
        {
            if (sender is not Button btn)
            {
                return;
            }

            if (btn.DataContext is not Student student)
            {
                return;
            }

            var result = MessageBox.Show("Opravu chcete smazat tento záznam?", "Odstranění záznamu", MessageBoxButton.YesNo, MessageBoxImage.Question);
            if (result != MessageBoxResult.Yes)
            {
                return;
            }

            await dapper.Delete(student);
            await dapper.Delete<DataLayer.Structures.Application>(new Dictionary<string, object> { { "Student", student.Id } });
            list.Remove(student);
        }

        private void ShowApplication(object sender, RoutedEventArgs e)
        {
            if (sender is not Button btn)
            {
                return;
            }

            if (btn.DataContext is not Student student)
            {
                return;
            }

            var applicationInfo = new ApplicationInfo(dapper, student);
            applicationInfo.ShowDialog();
        }
    }
}
