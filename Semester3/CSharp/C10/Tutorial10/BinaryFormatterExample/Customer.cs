using System;
using System.Collections.Generic;
using System.Text;
using Tutorial10.Commons;

namespace Tutorial10.BinaryFormatterExample
{
    [Serializable]
    public class Customer : ICustomer<Order>
    {
        public string Name { get; set; }
        public int Age { get; set; }

        public string Email { get; set; }


        public double? Weight { get; set; }

        public bool IsAlive { get; set; }

        public Order[] Orders { get; set; }
    }
}
