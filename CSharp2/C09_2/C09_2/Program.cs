namespace C09_2;

class Program
{
    static async Task Main(string[] args)
    {
        using HttpClient client = new();

        {
            using HttpResponseMessage response = await client.GetAsync("http://localhost:8080/test");
            string content = await response.Content.ReadAsStringAsync();
            Console.WriteLine("/test -> " + content);
        }

        {
            using HttpRequestMessage request = new(HttpMethod.Post, "http://localhost:8080");
            request.Content = new FormUrlEncodedContent(new Dictionary<string, string>()
            {
                { "name", "Jan" },
                {"age", "10"}
            });
            
            using HttpResponseMessage response = await client.SendAsync(request);
            string content = await response.Content.ReadAsStringAsync();
            Console.WriteLine("/ -> " + content);
        }

    }
}