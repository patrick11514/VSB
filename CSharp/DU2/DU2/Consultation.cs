using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DU2
{
    public class Consultation : Event, IStudent
    {
        private Student[]? students { get; set; }
        public Student[] Students { 
            get 
            { 
                if (this.students == null)
                {
                    return new Student[0];
                }
                return this.students;
            } 
            set 
            { 
                this.students = value;
            }
        }
        private string? location { get; set; }
        public string? Location { 
            get 
            {
                if (this.location == null)
                {
                    return "Neznámé místo";
                }
                return this.location;
            } 
            set 
            {
                this.location = value;
            } 
        }
        public Consultation(string name, DateTime start, DateTime end) : base(name, start, end)
        {
        }

        public override DateTime? GetReminderTime()
        {
            return null;
        }

        public override string ToString()
        {
            StringBuilder sb = new();
            sb.Append(base.ToString());
            sb.Append(" (");
            sb.Append(this.Location);
            sb.Append(')');
            return sb.ToString(); 
        }
    }
}
