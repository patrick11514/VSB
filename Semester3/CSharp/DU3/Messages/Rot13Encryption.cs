using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Messages
{
    public class Rot13Encryption : IEncryption
    {
        public string Decrypt(string data)
        {
            StringBuilder sb = new();
            foreach (char c in data)
            {
                sb.Append(Rot13(c));
            }

            return sb.ToString();
        }

        public string Encrypt(string data)
        {
            StringBuilder sb = new();
            foreach (char c in data)
            {
                sb.Append(Rot13(c));
            }

            return sb.ToString();
        }

        public char GetMark()
        {
            return 'R';
        }

        private char Rot13(char letter)
        {
            if (letter >= 'A' && letter <= 'Z')
            {
                letter = (char)('A' + (letter - 'A' + 13) % 26);
            }
            else if (letter >= 'a' && letter <= 'z')
            {
                letter = (char)('a' + (letter - 'a' + 13) % 26);
            }
            return letter;
        }
    }
}
