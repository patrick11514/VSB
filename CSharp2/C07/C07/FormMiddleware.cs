namespace C07
{
    public class FormMiddleware
    {
        private readonly RequestDelegate next;

        public FormMiddleware(RequestDelegate next)
        {
            this.next = next;
        }

        public async Task Invoke(HttpContext ctx)
        {
            if (!ctx.Request.Path.StartsWithSegments("/form"))
            {
                await this.next(ctx);
                return;
            }

            if (ctx.Request.Method == "POST")
            {
                string txt = ctx.Request.Form["text"];
                Console.WriteLine(txt);
                return;
            }

            ctx.Response.Headers.ContentType = "text/html; charset=UTF-8";

            await ctx.Response.WriteAsync(@"
                <form method=""POST"">
                    <input type=""text"" name=""text"" />
                    <button type=""submit"">odeslat</button>
                </form>
            ");

        }
    }
}
