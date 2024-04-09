namespace C07
{
    public class ErrorHandleMiddleware
    {
        private readonly RequestDelegate next;

        public ErrorHandleMiddleware(RequestDelegate next)
        {
            this.next = next;
        }

        public async Task Invoke(HttpContext ctx, ExceptionHandler handler)
        {
            try
            {
                await this.next(ctx);
            }
            catch (Exception e)
            {
                await handler.Handle(e);
            }

        }
    }
}
