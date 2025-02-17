using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.Json.Serialization;
using System.Threading.Tasks;

namespace Tutorial10.Auticko
{
    public class Car
    {
        [JsonPropertyName("car-name")]
        public string CarName { get; set; }
        public Wheel[] Wheels { get; set; } 
    }
}
