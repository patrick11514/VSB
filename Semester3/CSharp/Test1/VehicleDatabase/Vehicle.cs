using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace VehicleDatabaseLib
{
    public abstract class Vehicle
    {
        public double Speed { get; set; }

        public Vehicle(double speed)
        {
            if (speed < 0)
            {
                throw new InvalidSpeedException(speed);
            }

            Speed = speed;
        }
    }
}
