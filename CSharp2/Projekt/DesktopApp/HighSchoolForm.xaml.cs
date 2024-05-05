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
	/// Interakční logika pro AddHighSchool.xaml
	/// </summary>
	public partial class HighSchoolForm : Window
	{
		public HighSchool highSchool { get; set; }
		public HighSchoolForm(HighSchool? highSchool = null)
		{
			this.highSchool = highSchool ?? new HighSchool();
			InitializeComponent();
			this.DataContext = this.highSchool;
		}

		private void Save(object sender, RoutedEventArgs e)
		{
			this.Close();
		}
	}
}
