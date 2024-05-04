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

			this.list = new(dapper.SelectAll<HighSchoolProgram>());

			this.DataContext = this;
		}

		private void EditProgram(object sender, RoutedEventArgs e)
		{
			Button btn = sender as Button;
			var program = btn.DataContext as HighSchoolProgram;

			var form = new ProgramForm(program);
			form.ShowDialog();

			dapper.Update(form.program);
		}

		private void RemoveProgram(object sender, RoutedEventArgs e)
		{

		}
		private void AddProgram(object sender, RoutedEventArgs e)
		{
			var form = new ProgramForm();
			form.ShowDialog();

			dapper.Insert(form.program);
			this.list.Add(form.program);
		}
	}
}
