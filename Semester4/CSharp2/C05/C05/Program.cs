using System.Text.RegularExpressions;
using System.Xml;

namespace C05
{
    internal class Program
    {
        static void Main(string[] args)
        {
            Regex regex = new("^[A-Z]{3}[0-9]{3,4}$");

            bool ok = regex.IsMatch("75a");

            Console.WriteLine(ok);

            Regex emailRegex = new(@"^[a-zA-Z\.\-]+@[a-zA-Z]+\.[a-zA-Z]{2,}$");
            Console.WriteLine(emailRegex.IsMatch("patrik.mintel@seznam.cz"));
            Console.WriteLine(emailRegex.IsMatch("patrik.mintel@seznam.z"));
            Console.WriteLine(emailRegex.IsMatch("patrik.m@intel@seznam.cz"));
            Console.WriteLine(emailRegex.IsMatch("patrik.mintel@seznam.cz"));

            Regex parserReg = new(@"^(https?):\/\/(([a-zA-Z]+)\.)?([a-zA-Z]+\.cz)");
            string[] urls = new[]
            {
                "https://katedrainformatiky.cz/cs/pro-uchazece/zamereni-studia",
                "http://katedrainformatiky.cz/",
                "https://katedrainformatiky.cz?page=5" ,
                "https://page.katedrainformatiky.cz?url=http://test.cz/"
            };

            foreach (string url in urls)
            {
                Match match = parserReg.Match(url);
                if (!match.Success)
                {
                    Console.WriteLine("Není url");
                    continue;
                }

                Console.WriteLine(url);
                Console.WriteLine(match.Groups[1].Value);
                Console.WriteLine(match.Groups[2].Value);
                Console.WriteLine(match.Groups[3].Value);
                Console.WriteLine(match.Groups[4].Value);
            }

            Dictionary<string, string> dict = new()
            {
                { "name", "Patrik" },
                {"orderName", "Lolko kartičky" },
                {"price", "99999$" }
            };

            string text = "Ahoj {name}. Tvá objednávka „{orderName}“ v ceně {price} byla úspěšně uhrazena. ";
            Regex placeholderRegex = new(@"\{([a-zA-Z]+)\}", RegexOptions.Compiled);

            string finalTxt = placeholderRegex.Replace(text, (Match match) =>
            {
                string key = match.Groups[1].Value;
                if (dict.TryGetValue(key, out string value))
                {
                    return value;
                }
                return key.ToUpperInvariant();
            });

            Console.WriteLine(finalTxt);


            Console.WriteLine("=================================================");

            /*XmlDocument doc = new();
            doc.AppendChild(doc.CreateXmlDeclaration("1.0", "UTF-8", "yes"));
            XmlNode root = doc.CreateElement("koren");
            doc.AppendChild(root);

            XmlNode child1 = doc.CreateElement("child");
            root.AppendChild(child1);

            XmlNode child2 = doc.CreateElement("child");
            root.AppendChild(child2);

            XmlNode child3 = doc.CreateTextNode("Můj super text");
            child1.AppendChild(child3);

            XmlAttribute childAttr = doc.CreateAttribute("id");
            childAttr.Value = "789";
            child2.Attributes.Append(childAttr);

            doc.Save("test.xml");*/

            /*XmlDocument doc = new();
            doc.Load("test.xml");

            Console.WriteLine(doc.DocumentElement.Name);
            doc.DocumentElement.RemoveChild(doc.DocumentElement.ChildNodes[0]);
            doc.Save("test2.xml");*/

            XmlDocument doc = new();
            doc.Load("rss.xml");

            foreach (XmlNode node in doc.SelectNodes("/rss/channel/item"))
            {
                Console.WriteLine(node.SelectSingleNode("title/text()").Value);
                Console.WriteLine(node.SelectSingleNode("pubDate/text()").Value);
            }
        }
    }
}
