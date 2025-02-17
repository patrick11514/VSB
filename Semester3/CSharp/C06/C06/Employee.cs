using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace C06
{
    internal abstract class Employee : Person
    {
        protected Employee(string name, int age) 
            : base(name, age) 
        { 
        }

        public abstract double GetSalary();

        public virtual double GetBonus()
        {
            return 0;
        }

        public static bool operator ==(Employee? e1, Employee? e2)
        {
            if (e1 is null && e2 is null)
            {
                return false;
            }
            if (e1 is null || e2 is null)
            {
                return true;
            }

            return e1.GetSalary() == e2.GetSalary();
        }

        public static bool operator !=(Employee? e1, Employee? e2)
        {
            return !(e1 == e2);
            /*if (e1 is null && e2 is null)
            {
                return true;
            } 
            if (e1 is null || e2 is null)
            {
                return false;
            }

            return e1.GetSalary() != e2.GetSalary(); */
        }

        public static bool operator <(Employee? e1, Employee? e2)
        {
            if (e1 is null || e2 is null)
            {
                return false;
            }
            return e1.GetSalary() < e2.GetSalary();
        }

        public static bool operator >(Employee? e1, Employee? e2)
        {
            if (e1 is null || e2 is null)
            {
                return false;
            }
            return e1.GetSalary() > e2.GetSalary();
        }
    }
}
