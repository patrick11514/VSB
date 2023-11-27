using System;
using System.Collections.Generic;
using System.Text;
using System.Text.Json.Serialization;
using Tutorial10.Commons;

namespace Tutorial10.JsonSerializerExample
{
    public class Customer : ICustomer<Order>
    {
        public string Name { get; set; }
        public int Age { get; set; }
        [JsonPropertyName("ContactEmail")]
        public string Email { get; set; }
        [JsonIgnore]
        public double? Weight { get; set; }

        public bool IsAlive { get; set; }
        public Order[] Orders { get; set; }
    }
}
