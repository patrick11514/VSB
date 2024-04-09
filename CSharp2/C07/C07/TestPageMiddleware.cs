namespace C07
{
    public class TestPageMiddleware
    {
        private readonly RequestDelegate next;

        public TestPageMiddleware(RequestDelegate next)
        {
            this.next = next;
        }

        public async Task Invoke(HttpContext ctx)
        {
            if (ctx.Request.Path == "/test")
            {
                throw new NotImplementedException(); ;
                ctx.Response.Headers.ContentType = "text/html; charset=UTF-8";
                await ctx.Response.WriteAsync("<h1>Hello world</h1><p>testovací stránka!</p>");
            }
            else
            {
                await this.next(ctx);
            }
        }
    }
}
