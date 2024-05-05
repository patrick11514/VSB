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
	/// Interakční logika pro ProgramForm.xaml
	/// </summary>
	public partial class ProgramForm : Window
	{
		public HighSchoolProgram program { get; set; }
		public ProgramForm(HighSchoolProgram program = null)
		{
			this.program = program ?? new HighSchoolProgram();
			InitializeComponent();

			this.DataContext = this.program;
		}

		private void Save(object sender, RoutedEventArgs e)
		{
			this.Close();
		}
	}
}
