namespace C07
{
    public class TxTLogger : IMyLogger
    {
        public async Task Log(string exception)
        {
            await File.WriteAllTextAsync("log.txt", "Došlo k chybě: " + exception);
        }
    }
}
