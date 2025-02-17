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
    /// Interakční logika pro ApplicationInfo.xaml
    /// </summary>
    public partial class ApplicationInfo : Window
    {
        private MyDapper dapper;
        private Student student;
        public DataLayer.Structures.Application application { get; set; }

        //High schools
        public List<HighSchool> highSchools { get; set; }
        public ObservableCollection<string> highSchoolNames { get; set; }

        //Program1
        public int highSchool1 { get; set; } = -1;
        public int program1 { get; set; } = -1;
        public List<HighSchoolProgram> programs1 { get; set; } = new();
        public ObservableCollection<string> programNames1 { get; set; } = new();
        //Program2
        public int highSchool2 { get; set; } = -1;
        public int program2 { get; set; } = -1;
        public List<HighSchoolProgram> programs2 { get; set; } = new();
        public ObservableCollection<string> programNames2 { get; set; } = new();
        //Program2
        public int highSchool3 { get; set; } = -1;
        public int program3 { get; set; } = -1;
        public List<HighSchoolProgram> programs3 { get; set; } = new();
        public ObservableCollection<string> programNames3 { get; set; } = new();

        public ApplicationInfo(MyDapper dapper, Student student)
        {
            this.dapper = dapper;
            this.student = student;
            InitializeComponent();

            _ = FetchData();

            this.DataContext = this;
        }

        public async Task FetchData()
        {
            var applications = await this.dapper.Select<DataLayer.Structures.Application, int>(
                new Dictionary<string, object> { { "Student", student.Id } }
            );

            this.application = applications.First();

            this.highSchools = new(await dapper.SelectAll<HighSchool>());
            this.highSchoolNames = new(highSchools.Select((h) => h.Name));

            //PROGRAM 1
            var program1 = await dapper.Select<HighSchoolProgram>((object)application.StudyProgram1);
            this.highSchool1 = this.highSchools.FindIndex((item) => item.Id == program1.HighSchool);
            //program2
            if (application.StudyProgram2 != null)
            {
                var program2 = await dapper.Select<HighSchoolProgram>((object)application.StudyProgram2);
                this.highSchool2 = this.highSchools.FindIndex((item) => item.Id == program2.HighSchool);
            }
            //program 3
            if (application.StudyProgram3 != null)
            {
                var program3 = await dapper.Select<HighSchoolProgram>((object)application.StudyProgram3);
                this.highSchool3 = this.highSchools.FindIndex((item) => item.Id == program3.HighSchool);
            }

            //var school1 = await this.dapper<HighSchool>(application.StudyProgram1.)

        }

        private async void Save(object sender, RoutedEventArgs e)
        {
            if (program1 == -1)
            {
                MessageBox.Show("Program 1 musí být zvolen!", "Chyba", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            application.StudyProgram1 = programs1[program1].Identifier;

            if (program2 > -1)
            {
                application.StudyProgram2 = programs2[program2].Identifier;
            }
            else
            {
                application.StudyProgram2 = null;
            }

            if (program3 > -1)
            {
                application.StudyProgram3 = programs3[program3].Identifier;
            }
            else
            {
                application.StudyProgram3 = null;
            }

            await dapper.Update(application);

            this.Close();
        }

        private async Task<int> Update(List<HighSchoolProgram> programs, ObservableCollection<string> names, string? studyProgram, int selectedItem)
        {
            programs.Clear();
            foreach (var program in await dapper.Select<HighSchoolProgram, int>(new Dictionary<string, object>() { { "HighSchool", highSchools[selectedItem].Id } }))
            {
                programs.Add(program);
            }

            names.Clear();
            foreach (var program in programs.Select((program) => program.Name))
            {
                names.Add(program);
            }

            return programs.FindIndex((item) => item.Identifier == studyProgram);
        }

        private async void School1Changed(object sender, SelectionChangedEventArgs e)
        {
            if (sender is not ComboBox comboBox)
            {
                return;
            }

            this.program1 = await Update(programs1, programNames1, application.StudyProgram1, comboBox.SelectedIndex);
        }

        private async void School2Changed(object sender, SelectionChangedEventArgs e)
        {
            if (sender is not ComboBox comboBox)
            {
                return;
            }

            this.program2 = await Update(programs2, programNames2, application.StudyProgram2, comboBox.SelectedIndex);
        }

        private async void School3Changed(object sender, SelectionChangedEventArgs e)
        {
            if (sender is not ComboBox comboBox)
            {
                return;
            }

            this.program3 = await Update(programs3, programNames3, application.StudyProgram3, comboBox.SelectedIndex);
        }

        private void DeleteProgram2(object sender, RoutedEventArgs e)
        {
            program2 = -1;
            programNames2.Clear();
            programs2.Clear();
        }

        private void DeleteProgram3(object sender, RoutedEventArgs e)
        {
            program3 = -1;
            programNames3.Clear();
            programs3.Clear();
        }
    }
}
