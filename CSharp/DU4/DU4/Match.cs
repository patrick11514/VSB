using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DU4
{
    public class Match
    {
        public int AwayGoals { get; set; }
        public Team AwayTeam { get; set; }
        public DateTime Date { get; set; }
        public int HomeGoals { get; set; }
        public Team HomeTeam { get; set; }
        public string Season { get; set; }
    }
}
