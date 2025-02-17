namespace C07
{
    public class FileServingMiddleware
    {
        private readonly RequestDelegate next;

        public FileServingMiddleware(RequestDelegate next)
        {
            this.next = next;
        }

        public async Task Invoke(HttpContext ctx)
        {
            if (ctx.Request.Path.Value.EndsWith(".jpg"))
            {
                string path = @"C:\Users\Patrik Mintěl\source\repos\2C07\C07\bin\Debug\net8.0";
                string fullPath = Path.Combine(path, ctx.Request.Path.Value.TrimStart('/'));
                if (File.Exists(fullPath))
                {
                    ctx.Response.ContentType = "image/jpeg";
                    await ctx.Response.SendFileAsync(fullPath);
                    return;
                }
                else
                {
                    ctx.Response.StatusCode = 404;
                    ctx.Response.Headers.ContentType = "text/plain";
                    await ctx.Response.WriteAsync("Nenalezeno");

                    return;
                }
            }

            await this.next(ctx);
        }
    }
}
