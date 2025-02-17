using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace C09
{
    internal class InvalidAgeException : Exception
    {
        public Contact Contact { get; set; }

        public InvalidAgeException(Contact contact)
        {
            this.Contact = contact;
        }
    }
}
