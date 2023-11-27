using System;
using System.Collections.Generic;
using System.Text;

namespace Tutorial10.Commons
{
    public interface ICustomer<T> where T : IOrder
    {
        string Name { get; set; }
        int Age { get; set; }
        string Email { get; set; }
        double? Weight { get; set; }

        bool IsAlive { get; set; }

        T[] Orders { get; set; }
    }
}
