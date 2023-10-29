using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DU2
{
    public class Student
    {
        public string Name { get; private set; }

        public override string ToString()
        {
            return Name;
        }
        public Student(string name)
        {
            Name = name;
        }
    }
}
