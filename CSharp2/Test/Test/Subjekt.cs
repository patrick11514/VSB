using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Serialization;

namespace Test
{
    [XmlRoot(ElementName = "subjekt")]
    public class Subjekt
    {
        [XmlElement(ElementName = "nazev")]
        public string Name { get; set; }
        [XmlElement(ElementName = "ico")]
        public int Ico { get; set; }
        [XmlElement(ElementName = "zapisDatum")]
        public string Date { get; set; }
    }
}
