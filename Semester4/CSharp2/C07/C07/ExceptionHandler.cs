namespace C07
{
    public class ExceptionHandler
    {
        private readonly IMyLogger logger;

        public ExceptionHandler(IMyLogger logger)
        {
            this.logger = logger;
        }
        public async Task Handle(Exception exception)
        {
            await this.logger.Log(exception.StackTrace);
        }
    }
}
