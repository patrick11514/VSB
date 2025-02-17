using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace C06
{
    internal class Secretary : MonthlySalaryEmployee
    {
        public Secretary(string name, int age) : base(name, age, 20000)
        {

        }

        public override void PrintInfo()
        {
            base.PrintInfo();
            Console.WriteLine("Pozice: sekretářka");
        }
    }
}
