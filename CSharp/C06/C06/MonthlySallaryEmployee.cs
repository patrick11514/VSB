using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace C06
{
    internal abstract class MonthlySalaryEmployee : Employee
    {
        public double MonthlySalary { get; set; }
        public MonthlySalaryEmployee(string name, int age, double monthlySalary) 
            : base(name, age)
        {
            this.MonthlySalary = monthlySalary;
        }

        override public double GetSalary()
        {
            return this.MonthlySalary + this.GetBonus();
        }
    }
}
