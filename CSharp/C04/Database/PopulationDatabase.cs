using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Database
{
    public class PopulationDatabase
    {
        private Person[] persons = new Person[10];
        private int index = 0;
        public int Count { get
            {
                int count = 0;
                foreach (Person p in persons)
                {
                    if (p != null) {
                        count++;
                    }
                }
                return count;
                //return index;
            } 
        }

        public int AdultCount { get {
                int count = 0;
                foreach (Person p in persons)
                {
                     if (p?.IsAdult == true)
                     {
                         count++;
                     }
                }
                return count;
            } 
        }

        public void Add(Person person)
        {
            persons[index] = person;
            index++;
        }

        public double? GetAverageAge()
        {
            int sum = 0;
            int count = 0;
            foreach (Person p in persons) {
                if (p?.Age == null) continue;

                sum += p.Age.Value;
                count++;
            }

            if (count == 0) return null;

            return sum / (double)count;
        }

        public override string ToString()
        {
            StringBuilder sb = new();
            foreach (Person p in persons)
            {
                if (p == null) continue;
                sb.AppendLine(p.ToString());
            }
            return sb.ToString();
        }

    }
}
