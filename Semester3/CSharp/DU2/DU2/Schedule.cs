using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DU2
{
    public class Schedule
    {
        private List<Event> events = new();

        public List<Event> this[int year, int month, int day]
        {
            get
            {
                List<Event> events = new();
                foreach (Event e in this.events)
                {
                    if (e.Start.Year == year && e.Start.Month == month && e.Start.Day == day)
                    {
                        events.Add(e);
                    }   
                }

                return events;
            }
        }
        public void Add(Event e)
        {
            this.events.Add(e);
        }

        public List<Event> GetAllUpcoming()
        {
            List<Event> events = new();

            foreach (Event e in this.events)
            {
                if (e.IsUpcoming)
                {
                    events.Add(e);
                }
            }

            return events;
        }
    }
}
