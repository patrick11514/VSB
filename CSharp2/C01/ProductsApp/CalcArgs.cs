using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ProductsApp
{
    internal class CalcArgs : EventArgs
    {
        public int output;
        public CalcArgs(int output) {
            this.output = output;
        }
    }
}
