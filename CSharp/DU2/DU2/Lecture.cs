using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DU2
{
    public class Lecture : Event
    {
        public Lecture(string name, DateTime start, DateTime end) : base(name, start, end)
        {
        }

        public override DateTime? GetReminderTime()
        {
            return this.Start.AddMinutes(-30);
        }
    }
}
