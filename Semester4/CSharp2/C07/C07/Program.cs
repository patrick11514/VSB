namespace C07
{
    public class Program
    {
        public static void Main(string[] args)
        {
            var builder = WebApplication.CreateBuilder(args);

            builder.Services.AddSingleton<ExceptionHandler>();
            builder.Services.AddSingleton<IMyLogger, TxTLogger>();

            var app = builder.Build();

            app.UseMiddleware<ErrorHandleMiddleware>();
            //app.UseMiddleware<BrowserAuthMiddleware>();

            app.UseMiddleware<FormMiddleware>();

            app.UseMiddleware<FileServingMiddleware>();
            //app.UseStaticFiles();
            app.UseMiddleware<FirstPageMiddleware>();
            app.UseMiddleware<TestPageMiddleware>();

            app.Run();
        }
    }
}
