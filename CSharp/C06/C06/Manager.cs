using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace C06
{
    internal class Manager : MonthlySalaryEmployee
    {
        public Manager(string name, int age) : base(name, age, 80_000)
        {

        }

        public override double GetSalary()
        {
            return this.MonthlySalary + this.Age * 500 + this.GetBonus();
        }

        public override void PrintInfo()
        {
            base.PrintInfo();
            Console.WriteLine("Pozice: manager");
        }
    }
}
