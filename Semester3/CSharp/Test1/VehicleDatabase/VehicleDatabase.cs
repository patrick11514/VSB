using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace VehicleDatabaseLib
{
    public class VehicleDatabase
    {
        private readonly List<Vehicle> vehicles = new();

        public void Add(Vehicle vehicle)
        {
            vehicles.Add(vehicle);
        }

        public List<T> GetByType<T>()
        {
            List<T> newVehicles = new();

            foreach (Vehicle vehicle in vehicles)
            {
                if (vehicle is T myVehicle) {
                    newVehicles.Add(myVehicle);
                }
            }

            return newVehicles;
        }

        public double GetAverageTicketPrice(int kilometers)
        {
            double price = 0;
            int vehicleCount = 0;

            foreach (Vehicle vehicle in vehicles)
            {
                if (vehicle is ITicketPrice ticketPriceVehicle)
                {
                    double? vPrice = ticketPriceVehicle.GetTicketPrice();

                    if (vPrice != null)
                    {
                        price += vPrice.Value * kilometers;
                        vehicleCount++;
                    }
                }
            }

            if (vehicleCount == 0) return 0;
            return price / vehicleCount;
        }   
    }
}
