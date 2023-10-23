using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace C06
{
    internal abstract class HourlySalaryEmployee : Employee
    {
        public double HourlySalary { get; set; }
        public int HoursWorked { get; set; }
        public HourlySalaryEmployee(string name, int age, double hourlySalary) : base(name, age)
        {
            this.HourlySalary = hourlySalary;
        }

        public override double GetSalary()
        {
            return this.HoursWorked * this.HourlySalary + this.GetBonus();
        }


    }
}
