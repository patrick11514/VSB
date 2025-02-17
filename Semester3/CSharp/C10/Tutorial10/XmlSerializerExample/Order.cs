using System;
using System.Collections.Generic;
using System.Text;
using Tutorial10.Commons;

namespace Tutorial10.XmlSerializerExample
{
    public class Order : IOrder
    {
        public string Name { get; set; }
        public double Price { get; set; }
    }
}
