using System;
using System.Collections.Generic;
using System.Runtime.Serialization;
using System.Text;
using Tutorial10.Commons;

namespace Tutorial10.DataContractExample
{
    [DataContract]
    public class Customer : ICustomer<Order>
    {
        [DataMember]
        public string Name { get; set; }

        [DataMember]
        public int Age { get; set; }

        [DataMember(Name = "ContactEmail")]
        public string Email { get; set; }

        public double? Weight { get; set; }

        [DataMember]
        public bool IsAlive { get; set; }

        [DataMember]
        public Order[] Orders { get; set; }
    }
}
