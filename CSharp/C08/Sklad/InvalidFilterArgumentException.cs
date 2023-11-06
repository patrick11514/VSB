using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace StockLib
{
    public class InvalidFilterArgumentException : Exception
    {
        public InvalidFilterArgumentException(string? message) : base(message)
        {
        }
    }
}
