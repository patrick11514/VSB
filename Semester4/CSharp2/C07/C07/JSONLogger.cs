using System.Text.Json;

namespace C07
{
    public class JSONLogger : IMyLogger
    {
        public async Task Log(string exception)
        {
            await File.WriteAllTextAsync("log.json", JsonSerializer.Serialize(new
            {
                Error = "Došlo k chybě: " + exception
            }));
        }
    }
}
