using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace VehicleDatabaseLib
{
    public class Truck : Vehicle, IGoodsTransportPrice
    {
        private double GoodsTransportPrice { get; set; }
        public Truck(double speed, double goodsTrasportPrice) : base(speed)
        {
            GoodsTransportPrice = goodsTrasportPrice;
        }
        public override string ToString()
        {
            StringBuilder sb = new();

            sb.Append("Nákladní auto s maximální rychlostí ");
            sb.Append(base.Speed);
            sb.Append("km/h a cenou přepravy zboží ");
            sb.Append(GoodsTransportPrice);
            sb.Append("Kč");
            return sb.ToString();
        }
        public double GetGoodsTransportPrice()
        {
            return GoodsTransportPrice;
        }
    }
}
