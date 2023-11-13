using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace VehicleDatabaseLib
{ 
    public class Train : Vehicle, ITicketPrice, IGoodsTransportPrice
    {
        private double? TicketPrice { get; set; }
        private double GoodsTransportPrice { get; set; }

        public Train(double speed, double? ticketPrice, double goodsTransportPrice) : base(speed)
        {
            TicketPrice = ticketPrice;
            GoodsTransportPrice = goodsTransportPrice;
        }

        public override string ToString()
        {
            StringBuilder sb = new();

            sb.Append("Vlak s maximální rychlostí ");
            sb.Append(base.Speed);
            sb.Append("km/h, cenou lístku ");
            sb.Append(TicketPrice == null ? "???" : TicketPrice.ToString());
            sb.Append("Kč a cenou přepravy zboží ");
            sb.Append(GoodsTransportPrice);
            sb.Append("Kč");
            return sb.ToString();
        }

        public double? GetTicketPrice()
        {
            return TicketPrice;
        }

        public double GetGoodsTransportPrice()
        {
            return GoodsTransportPrice;
        }
    }
}
