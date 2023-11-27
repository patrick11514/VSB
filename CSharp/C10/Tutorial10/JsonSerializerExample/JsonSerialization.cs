using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Text.Json;
using Tutorial10.Commons;

namespace Tutorial10.JsonSerializerExample
{
    internal class JsonSerialization
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

            using FileStream fs = new("data4.json", FileMode.Create);
            JsonSerializer.Serialize(fs, customer, new JsonSerializerOptions() 
            { 
                WriteIndented = true,
                Encoder = System.Text.Encodings.Web.JavaScriptEncoder.UnsafeRelaxedJsonEscaping,
                PropertyNamingPolicy = JsonNamingPolicy.CamelCase 
            });

            fs.Seek(0, SeekOrigin.Begin);
            Customer c = JsonSerializer.Deserialize<Customer>(fs, new JsonSerializerOptions()
            {
                //PropertyNamingPolicy = JsonNamingPolicy.CamelCase
                PropertyNameCaseInsensitive = true
            });
            CustomerHelper.Print(c);
            
        }
    }

}
