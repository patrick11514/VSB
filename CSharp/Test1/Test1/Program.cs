using System.Diagnostics;
using System.Globalization;
using VehicleDatabaseLib;

namespace Test1
{
    internal class Program
    {
        static void Main(string[] args)
        {
            CultureInfo.DefaultThreadCurrentCulture = CultureInfo.InvariantCulture;
            CultureInfo.DefaultThreadCurrentUICulture = CultureInfo.InvariantCulture;


            VehicleDatabase db = new VehicleDatabase();

            db.Add(new Train(180 /* rychlost */, 4 /* cena lístku */, 10 /* cena přepravy zboží */));
            db.Add(new Bus(60 /* rychlost */, 3 /* cena lístku */ ));
            db.Add(new Truck(50 /* rychlost */, 5 /* cena přepravy zboží */ ));
            db.Add(new Bus(80, null));
            db.Add(new Train(90, 2, 5));


            var averageTicketPrice = db.GetAverageTicketPrice(150 /* vzdálenost */);
            Console.WriteLine("Průměrná cena lístku na 150km: " + averageTicketPrice + "Kč");

            Console.WriteLine();

            Console.WriteLine("Vlaky:");
            foreach (var vehicle in db.GetByType<Train>())
            {
                Console.WriteLine(vehicle);
            }

            Console.WriteLine();
            Console.WriteLine("Autobusy:");
            foreach (var vehicle in db.GetByType<Bus>())
            {
                Console.WriteLine(vehicle);
            }


            // vyvolání výjimky
            // db.Add(new Bus(-20, null));
        }
    }
}