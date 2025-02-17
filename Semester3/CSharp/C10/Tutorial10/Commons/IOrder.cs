using System;
using System.Collections.Generic;
using System.Text;

namespace Tutorial10.Commons
{
    public interface IOrder
    {
        string Name { get; set; }
        double Price { get;set;}
    }
}
