using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Test2
{
    internal class CandidateComparer : IComparer<Candidate>
    {
        public int Compare(Candidate? a, Candidate? b)
        {
            if (a == null && b == null) return 0;
            if (a == null) {
                return -1;
            }
            if (b == null) {
                return 1;
            }
            return b.VoteCount - a.VoteCount;
        }
    }
}
