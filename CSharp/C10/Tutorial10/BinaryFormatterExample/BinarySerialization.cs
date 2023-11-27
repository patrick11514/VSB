using System;
using System.Collections.Generic;
using System.IO;
using System.Runtime.Serialization.Formatters.Binary;
using System.Text;
using Tutorial10.Commons;

namespace Tutorial10.BinaryFormatterExample
{
    internal class BinarySerialization
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

            using FileStream fs = new("data5.bin", FileMode.Create);

            BinaryFormatter bf = new();
#pragma warning disable SYSLIB0011 // Typ nebo člen je zastaralý.
            bf.Serialize(fs, customer);


            fs.Seek(0, SeekOrigin.Begin);

            Customer c = (Customer)bf.Deserialize(fs);

#pragma warning restore SYSLIB0011 // Typ nebo člen je zastaralý.

            CustomerHelper.Print(c);
        }
    }
}
