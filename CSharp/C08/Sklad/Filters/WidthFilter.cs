using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace StockLib.Filters
{
    public class WidthFilter : IFilter
    {
        public int? MinWidth { get; set; }
        public int? MaxWidth { get; set; }
        public bool isMatch(IProduct item)
        {
            if (item is IPhysicalProduct physicalItem)
            {
                if (!MinWidth.HasValue && !MaxWidth.HasValue)
                {
                    throw new InvalidFilterArgumentException("MinWidth nor MaxWidth is not set");
                }

                if (MinWidth.HasValue)
                {
                    if (physicalItem.Width < MinWidth.Value)
                    {
                        return false;
                    }
                }

                if (MaxWidth.HasValue)
                {
                    if (physicalItem.Width > MaxWidth.Value)
                    {
                        return false;
                    }
                }

                return true;
            }

                return false;
        }
    }
}
