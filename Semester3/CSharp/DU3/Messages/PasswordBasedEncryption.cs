using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Messages
{
    public class PasswordBasedEncryption : IEncryption
    {
        private readonly string password;

        public PasswordBasedEncryption(string password)
        {
            if (password == null || password.Length == 0)
            {
                throw new InvalidPasswordException();
            }
            this.password = password;
        }

        public string Decrypt(string data)
        {
            StringBuilder sb = new();

            int i = 0;
            foreach (char c in data)
            {
                char newC = (char)(c - this.password[i]);
                if ((int)newC < 0)
                {
                    throw new InvalidPasswordException("Unable to decrypt", data, this.password);
                }

                sb.Append(newC);

                ++i;
                if (i == this.password.Length)
                {
                    i = 0;
                }
            }

            return sb.ToString();
        }

        public string Encrypt(string data)
        {
            StringBuilder sb = new();

            int i = 0;
            foreach (char c in data)
            {
                sb.Append((char)(c + this.password[i]));

                ++i;
                if (i == this.password.Length)
                {
                    i = 0;
                }
            }

            return sb.ToString();
        }

        public char GetMark()
        {
            return 'P';
        }
    }
}
