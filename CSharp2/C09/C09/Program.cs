using System.Net;
using System.Net.Http.Headers;
using System.Net.Http.Json;
using System.Net.Mail;
using System.Text;
using System.Text.Json;
using MimeKit;
using SmtpClient = MailKit.Net.Smtp.SmtpClient;

namespace C09;

class Program
{
    static async Task Main(string[] args)
    {
        HttpListener listener = new();
        listener.Prefixes.Add("http://localhost:8080/");
        
        listener.Start();

        while (true)
        {
            HttpListenerContext ctx = listener.GetContext();

            StreamReader reader = new(ctx.Request.InputStream);
            string txt = reader.ReadToEnd();
            
            Console.WriteLine(txt);

            using (StreamWriter sw = new(ctx.Response.OutputStream))
            {
                if (ctx.Request.RawUrl.EndsWith("test"))
                {
                    sw.WriteLine("Neco");
                }
                else
                {
                    sw.WriteLine("Ahoj!");
                }
                
                

                
            }
            
            ctx.Response.OutputStream.Close();

        }


        /*using MimeMessage msg = new();
        msg.Subject = "Testovaci email";
        msg.Sender = new MailboxAddress("Csharp", "atnet2019@seznam.cz ");
        msg.To.Add(new MailboxAddress("Já", "ja@patrick115.eu"));

        BodyBuilder builder = new();
        //builder.TextBody = "";
        builder.HtmlBody = "<h1>Nadpis</h1><p>Test mailu.</p><i style=\"color:red;background:blue;\">UwU ^^</i>";

        msg.Body = builder.ToMessageBody();

        using SmtpClient client2 = new();
        await client2.ConnectAsync("smtp.seznam.cz", 465);
        await client2.AuthenticateAsync("atnet2019@seznam.cz", "Csharp2023");
        await client2.SendAsync(msg);

        await client2.DisconnectAsync(true);*/



        //HTTPClient
        return;
        
        using HttpClient client = new();
    

        using HttpRequestMessage request = new(HttpMethod.Post /*Get*/,
            "https://www.7timer.info/bin/astro.php?lon=18.160005506399536&lat=49.831015379859586&ac=0&unit=metric&output=json&tzshift=0");

        request.Headers.Authorization = AuthenticationHeaderValue.Parse("Bearer gsdgsdggsggdsggsgds");
        request.Headers.Add("X-Name", "bla");
        request.Content = new StringContent("{\"a\":5}", Encoding.UTF8, "application/json");
        /*request.Content = new FormUrlEncodedContent(new Dictionary<string,string>()
        {
            {"a", "1"},
            {"jmeno", "patrik"}
        });*/

        using HttpResponseMessage response = await client.SendAsync(request);
        /*await client.GetAsync(
            "https://www.7timer.info/bin/astro.php?lon=18.160005506399536&lat=49.831015379859586&ac=0&unit=metric&output=json&tzshift=0");
*/


        if (response.StatusCode == HttpStatusCode.OK)
        {
            /*using Stream s1 = response.Content.ReadAsStream();
            using FileStream fs = new FileStream("data.json", FileMode.Create);
            s1.CopyTo(fs);*/
            //string json = await response.Content.ReadAsStringAsync();
            //Console.WriteLine(json);

            //JSON result = JsonSerializer.Deserialize<JSON>(json);

            JSON result = await response.Content.ReadFromJsonAsync<JSON>();

            Console.WriteLine(result.init);
            foreach (var item in result.dataSeries)
            {
                Console.WriteLine(item.timePoint);
                Console.WriteLine(item.temp);
                Console.WriteLine("");
            }
        }
    }
}
