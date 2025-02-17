using System;
using System.Collections.Generic;
using System.Text;
using System.Xml.Serialization;
using Tutorial10.Commons;

namespace Tutorial10.XmlSerializerExample
{
    public class Customer : ICustomer<Order>
    {
        public string Name { get; set; }

        public int Age { get; set; }

        [XmlElement(ElementName = "ContactEmail")]
        public string Email { get; set; }

        [XmlIgnore]
        public double? Weight { get; set; }

        [XmlAttribute]
        public bool IsAlive { get; set; }

        public Order[] Orders { get; set; }
    }
}
