using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.Json;
using System.Threading.Tasks;

namespace Tutorial10.Auticko
{
    public class Serialize
    {
        public static void Run()
        {
            using FileStream fs = new("test.json", FileMode.Open);
            List<Car> car = JsonSerializer.Deserialize<List<Car>>(fs);
            foreach (Car carItem in car)
            {
                Console.WriteLine($"Car name: {carItem.CarName}");
                foreach (Wheel wheel in carItem.Wheels)
                {
                    Console.WriteLine($"Wheel size: {wheel.Size}");
                }
            }
        }
    }
}
