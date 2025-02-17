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
    /// Interakční logika pro ProgramList.xaml
    /// </summary>
    public partial class ProgramList : Window
    {
        private MyDapper dapper;
        private HighSchool highSchool;
        public ObservableCollection<HighSchoolProgram> list { get; set; }
        public ProgramList(MyDapper dapper, HighSchool highSchool)
        {
            this.dapper = dapper;
            this.highSchool = highSchool;

            InitializeComponent();

            _ = this.FetchData();

            this.DataContext = this;
        }

        private async Task FetchData()
        {
            this.list = new(await dapper.Select<HighSchoolProgram, int>(
                new Dictionary<string, object>() { { "HighSchool", highSchool.Id } })
                );
        }

        private async void EditProgram(object sender, RoutedEventArgs e)
        {
            if (sender is not Button btn)
            {
                return;
            }

            if (btn.DataContext is not HighSchoolProgram program)
            {
                return;
            }

            var form = new ProgramForm(program);
            form.ShowDialog();

            await dapper.Update(form.program);
        }

        private async void RemoveProgram(object sender, RoutedEventArgs e)
        {
            if (sender is not Button btn)
            {
                return;
            }

            if (btn.DataContext is not HighSchoolProgram program)
            {
                return;
            }

            var result = MessageBox.Show("Opravu chcete smazat tento záznam?", "Odstranění záznamu", MessageBoxButton.YesNo, MessageBoxImage.Question);
            if (result != MessageBoxResult.Yes)
            {
                return;
            }

            await dapper.Delete(program);
            list.Remove(program);
        }
        private async void AddProgram(object sender, RoutedEventArgs e)
        {
            var form = new ProgramForm();
            form.ShowDialog();

            form.program.HighSchool = highSchool.Id;

            await dapper.Insert(form.program);
            this.list.Add(form.program);
        }
    }
}
