using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Xml.Serialization;
using Tutorial10.Commons;

namespace Tutorial10.XmlSerializerExample
{
    internal class XmlSerialization
    {

        public static void Run()
        {
            Customer customer = new Customer()
            {
                Name = "Jan",
                Age = 30,
                Email = "jan@vsb.cz",
                Weight = 75.4,
                IsAlive = true,
                Orders = new Order[] {
                    new Order() {
                        Price = 400,
                        Name = "Dort"
                    },
                    new Order() {
                        Price = 20.5,
                        Name = "Koláč"
                    }
                }
            };

            using FileStream fs = new("data3.xml", FileMode.Create);
            XmlSerializer serializer = new(typeof(Customer));
            serializer.Serialize(fs, customer);

            fs.Seek(0, SeekOrigin.Begin);
            Customer c = (Customer) serializer.Deserialize(fs);
            CustomerHelper.Print(c);

            //pokud chci do promene string
            using MemoryStream ms = new();
            XmlSerializer serializer2 = new(typeof(Customer));
            serializer2.Serialize(ms, customer);

            string xml = Encoding.UTF8.GetString(ms.ToArray());

            Console.WriteLine(xml);

        }

    }
}
