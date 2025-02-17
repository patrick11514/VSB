using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace VehicleDatabaseLib
{
    public class Bus : Vehicle, ITicketPrice
    {
        private double? TicketPrice { get; set; }

        public Bus(double speed, double? ticketPrice = null) : base(speed)
        {
            TicketPrice = ticketPrice;
        }

        public override string ToString()
        {
            StringBuilder sb = new();

            sb.Append("Autobus s maximální rychlostí ");
            sb.Append(base.Speed);
            sb.Append("km/h a cenou lístku ");
            sb.Append(TicketPrice == null ? "???" :  TicketPrice.ToString());
            sb.Append("Kč");
            return sb.ToString();
        }

        public double? GetTicketPrice()
        {
            return TicketPrice;
        }
    }
}
