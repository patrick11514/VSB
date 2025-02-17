namespace C07
{
    public class BrowserAuthMiddleware
    {
        private readonly RequestDelegate next;

        public BrowserAuthMiddleware(RequestDelegate next)
        {
            this.next = next;
        }

        public async Task Invoke(HttpContext ctx)
        {
            string ua = ctx.Request.Headers.UserAgent;

            if (ua.Contains("Chrome"))
            {
                await next(ctx);
            }
            else
            {
                ctx.Response.StatusCode = 403;
            }
        }
    }
}
