using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DU2
{
    public abstract class Event
    {
        public DateTime? End { get; set; }
        public DateTime Start { get; set; }
        public bool IsUpcoming { 
            get {
                return this.Start > DateTime.Now;
            } 
        }
        public string Name { get; set; }

        public Event(string name, DateTime start, DateTime end)
        {
            this.Name = name;

            if (start > end)
            {
                this.End = start;
                this.Start = end;
            } else
            {
                this.Start = start;
                this.End = end;
            }
        }

        public abstract DateTime? GetReminderTime();

        public override string ToString()
        {
            StringBuilder sb = new();
            sb.Append(this.Name);
            sb.Append(": ");
            sb.Append(this.Start);
            sb.Append(" - ");
            sb.Append(this.End);

            return sb.ToString();
        }

    }
}
