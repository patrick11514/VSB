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
	/// Interakční logika pro HightSchoolList.xaml
	/// </summary>
	public partial class HightSchoolList : Window
	{
		private MyDapper dapper;
		public ObservableCollection<HighSchool> list { get; set; }
		public HightSchoolList(MyDapper dapper)
		{
			this.dapper = dapper;
			InitializeComponent();

			this.FetchData();

			this.DataContext = this;
		}

		private async void FetchData()
		{
			this.list = new(await this.dapper.SelectAll<HighSchool>());
		}

		private async void EditHighSchool(object sender, RoutedEventArgs e)
		{
			if (sender is not Button btn)
			{
				return;
			}

			if (btn.DataContext is not HighSchool highSchool)
			{
				return;
			}

			var highSchoolForm = new HighSchoolForm(highSchool);
			highSchoolForm.ShowDialog();

			await dapper.Update(highSchoolForm.highSchool);
		}

		private async void RemoveHighSchool(object sender, RoutedEventArgs e)
		{
			if (sender is not Button btn)
			{
				return;
			}

			if (btn.DataContext is not HighSchool highSchool)
			{
				return;
			}

			var result = MessageBox.Show("Opravu chcete smazat tento záznam?", "Odstranění záznamu", MessageBoxButton.YesNo, MessageBoxImage.Question);
			if (result != MessageBoxResult.Yes)
			{
				return;
			}

			await dapper.Delete(highSchool);
			list.Remove(highSchool);
		}

		private void ShowPrograms(object sender, RoutedEventArgs e)
		{
			if (sender is not Button btn)
			{
				return;
			}
			if (btn.DataContext is not HighSchool highSchool)
			{
				return;
			}
			var schoolProgram = new ProgramList(dapper, highSchool);
			schoolProgram.ShowDialog();
		}
	}
}
