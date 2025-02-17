using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MyLib
{
    public class TestAttribute : Attribute
    {
        public string Name { get; set; }
        public TestAttribute(string name)
        {
            Name = name;
        }
    }
}
