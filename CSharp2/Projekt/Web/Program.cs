using DataLayer;
using DesktopApp;
using System.Collections;

namespace Web
{
    public class Program
    {
        public static void Main(string[] args)
        {
            using var dapper = new MyDapper("db.sqlite");

            Thread myApp = new(() =>
            {
                var application = new System.Windows.Application();
                application.Run(new DesktopApp.MainWindow(dapper));
            });
            myApp.SetApartmentState(ApartmentState.STA);
            myApp.Start();

            Environment.SetEnvironmentVariable("ASPNETCORE_URLS", "http://localhost:5146");
            Environment.SetEnvironmentVariable("ASPNETCORE_ENVIRONMENT", "Development");

            var builder = WebApplication.CreateBuilder(args);

            // Add services to the container.
            builder.Services.AddControllersWithViews();

            //builder.Services.AddSingleton<DataService>();

            var app = builder.Build();

            // Configure the HTTP request pipeline.
            if (!app.Environment.IsDevelopment())
            {
                app.UseExceptionHandler("/Home/Error");
            }

            app.UseStaticFiles();

            app.UseRouting();

            app.UseAuthorization();

            app.MapControllerRoute(
                name: "default",
                pattern: "{controller=Home}/{action=Index}/{id?}");


            app.Run();
        }
    }
}
