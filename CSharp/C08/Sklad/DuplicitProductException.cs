using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace StockLib
{
    public class DuplicitProductException : Exception
    {

        public IProduct Product { get; set; }
        public DuplicitProductException(IProduct product)
        {
            Product = product;
        }
    }
}
