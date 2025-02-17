using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Messages
{
    public interface IEncryption
    {
        string Encrypt(string data);
        string Decrypt(string data);
        char GetMark();
    }
}
