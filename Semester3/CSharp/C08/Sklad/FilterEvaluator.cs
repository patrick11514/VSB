using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace StockLib
{
    public class FilterEvaluator
    {
        private List<IFilter> filters = new();

        public void Add(IFilter filter)
        {
            filters.Add(filter);
        }

        public bool IsMatch(IProduct product) 
        {
            foreach (IFilter filter in filters)
            {
                if (!filter.isMatch(product))
                {
                    return false;
                }
            }

            return true;
        }
    }
}
