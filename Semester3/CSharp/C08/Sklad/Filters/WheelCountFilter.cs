using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace StockLib.Filters
{
    public class WheelCountFilter : IFilter
    {
        public int? WheelCount { get; set; }

        public bool isMatch(IProduct item)
        {
            if (item is IWheelVehicle wheelItem)
            {
                if (!WheelCount.HasValue)
                {
                    throw new InvalidFilterArgumentException("WheelCount is not set");
                }

                if (wheelItem.WheelCount !=  WheelCount.Value)
                {
                    return false;
                }

                return true;
            }

            return false;
        }
    }
}
