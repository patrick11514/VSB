using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Messages
{
    public class InvalidPasswordException : Exception
    {
        public string HashedText { get;  private set; }
        public string Password { get; private set; }

        public InvalidPasswordException(string message = null, string hashedText = null, string password = null) : base(message) 
        {
            HashedText = hashedText;
            Password = password;
        }
    }
}
