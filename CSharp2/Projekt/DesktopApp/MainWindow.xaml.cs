using DataLayer;
using DataLayer.Structures;
using System.Diagnostics;
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

namespace DesktopApp
{
	/// <summary>
	/// Interaction logic for MainWindow.xaml
	/// </summary>
	public partial class MainWindow : Window
	{
		MyDapper dapper;
		public MainWindow(MyDapper dapper)
		{
			this.dapper = dapper;
			InitializeComponent();
		}

		private async void AddHighSchoolList(object sender, RoutedEventArgs e)
		{
			var highSchoolForm = new HighSchoolForm();
			highSchoolForm.ShowDialog();

			await dapper.Insert(highSchoolForm.highSchool);
		}

		private void ShowHighSchoolList(object sender, RoutedEventArgs e)
		{
			var listForm = new HightSchoolList(dapper);
			listForm.ShowDialog();
		}

		private async void CreateTables(object sender, RoutedEventArgs e)
		{
			await dapper.CreateTable<HighSchool>();
			await dapper.CreateTable<HighSchoolProgram>();
			await dapper.CreateTable<Student>();
			await dapper.CreateTable<DataLayer.Structures.Application>();
		}
	}
}