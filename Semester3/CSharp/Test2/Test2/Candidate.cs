using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.Json.Serialization;
using System.Threading.Tasks;

namespace Test2
{
    internal class Candidate
    {
        [JsonIgnore]
        public int Number { get; set; }
        public string Name { get; set; }
        [JsonIgnore]
        public int Age { get; set; }
        public int VoteCount { get; set; }
        [JsonIgnore]
        public string Party { get; set; }
        [JsonIgnore]
        public string Prislusnost { get; set; }
    }
}
