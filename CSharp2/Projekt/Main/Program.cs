using DataLayer;
using DataLayer.Structures;

namespace Main
{
	internal class Program
	{
		static void Main(string[] args)
		{
			using var dapper = new MyDapper("db.sqlite");

			Thread app = new(() =>
			{
				var application = new System.Windows.Application();
				application.Run(new DesktopApp.MainWindow(dapper));
			});
			app.SetApartmentState(ApartmentState.STA);
			app.Start();

			Thread web = new(() =>
			{
				Web.Models.DataLayer.dapper = dapper;
				Web.Program.Main([]);
			});

			web.Start();

			app.Join();
			web.Join();
		}
	}
}
