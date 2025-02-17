using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DU2
{
    public class Tutorial : Event, IStudent
    {
        private Student[]? students { get; set; }
        public Student[] Students
        {
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
        public Tutorial(string name, DateTime start, DateTime end, Student[] students) : base(name, start, end)
        {
            this.Students = students;
        }

        public override DateTime? GetReminderTime()
        {
            return this.Start.AddMinutes(-30 - (10 * this.Students.Length));
        }
    }
}
