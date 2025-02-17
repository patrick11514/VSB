using System;
using System.Collections.Generic;
using System.IO;
using System.Runtime.Serialization;
using System.Text;
using Tutorial10.Commons;

namespace Tutorial10.DataContractExample
{
    internal class DataContractSerialization
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

            using FileStream fs = new("data1.xml", FileMode.Create);
            DataContractSerializer serializer = new(typeof(Customer));
            serializer.WriteObject(fs, customer);

            fs.Seek(0, SeekOrigin.Begin);
            Customer c = (Customer) serializer.ReadObject(fs);
            CustomerHelper.Print(c);
        }
    }
}
