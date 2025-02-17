using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.Json.Serialization;
using System.Threading.Tasks;
using System.Xml.Serialization;

namespace DU4
{
    public class Team
    {
        [XmlAttribute]
        public int Id { get; set; }
        public string ShortName { get; set; }
        [JsonPropertyName("Name")]
        [XmlElement(ElementName = "Name")]
        public string TeamName { get; set; }
        [JsonIgnore]
        [XmlIgnore]
        public int TieCount { get; set; }
        [JsonIgnore]
        [XmlIgnore]
        public int LossCount { get; set; }
        [JsonIgnore]
        [XmlIgnore]
        public int WinCount { get; set; }
    }
}
