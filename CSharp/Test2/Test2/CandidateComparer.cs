using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Test2
{
    internal class CandidateComparer : IComparer<Candidate>
    {
        public int Compare(Candidate a, Candidate b)
        {
            return b.VoteCount - a.VoteCount;
        }
    }
}
