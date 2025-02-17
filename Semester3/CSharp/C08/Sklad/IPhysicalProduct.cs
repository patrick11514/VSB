using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace StockLib
{
    public interface IPhysicalProduct : IProduct
    {
        int Width { get; }
        int Height { get; }
        int Length { get; }
    }
}
