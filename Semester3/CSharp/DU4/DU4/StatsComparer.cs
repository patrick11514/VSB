using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DU4
{
    internal class StatsComparer
    {
        public int Compare(Team a, Team b)
        {
            int diff = (b.WinCount - b.LossCount) - (a.WinCount - a.LossCount);

            if (diff != 0)
            {
                return diff;
            }

            return b.TieCount - a.TieCount;
           // return 0;
        }
    }
}
