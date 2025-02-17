using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace C06
{
    internal class Person
    {
        public readonly string Name;

        public Person(string name, int age)
        {
            this.Name = name;
            this.Age = age;
        }

        public int Age { get; set; }

        public virtual void PrintInfo()
        {
            Console.WriteLine($"{Name}: {Age}");
        }
    }
}
