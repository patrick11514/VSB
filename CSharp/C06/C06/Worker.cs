using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace C06
{
    internal class Worker : HourlySalaryEmployee
    {
        public Worker(string name, int age) : base(name, age, 130)
        {
        }

        public override double GetBonus()
        {
            if (this.HoursWorked > 160)
            {
                return 5000 + base.GetBonus();
            }
            return base.GetBonus();
        }
    }
}
