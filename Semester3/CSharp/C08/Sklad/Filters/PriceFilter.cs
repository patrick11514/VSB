using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace StockLib.Filters
{
    public class PriceFilter : IFilter
    {
        private int? minPrice { get; set; }
        public int? MinPrice { 
            get 
            {
                return minPrice;
            } 

            set {
                if (value < 0) {
                    minPrice = 0;
                } else
                {
                    minPrice = value;
                }
            } 
        }
        private int? maxPrice { get; set; }
        public int? MaxPrice
        {
            get
            {
                return maxPrice;
            }

            set
            {
                if (value < 0)
                {
                    maxPrice = 0;
                }
                else
                {
                    maxPrice = value;
                }
            }
        }
        public bool isMatch(IProduct item)
        {
            if (!MinPrice.HasValue && !MaxPrice.HasValue)
            {
                throw new InvalidFilterArgumentException("Price is not set");
            }

            if (MinPrice.HasValue)
            {
                if (item.Price < MinPrice)
                {
                    return false;
                }
            }

            if (MaxPrice.HasValue)
            {
                if (item.Price > MaxPrice)
                {
                    return false;
                }
            }

            return true;
        }
    }
}
