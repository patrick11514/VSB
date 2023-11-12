using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.AccessControl;
using System.Text;
using System.Threading.Tasks;

namespace Messages
{
    public class MessageSerializer<T> : MessageSerializeBase where T : IMessage, new()
    {
        public IEncryption Encryption { get; set; }
        public MessageSerializer(IEncryption encryption) 
        {
            Encryption = encryption;
        }

        public string Serialize(T obj)
        {
            string str = obj.Serialize();
            string encrypted = Encryption.Encrypt(str);

            StringBuilder sb = new();
            sb.Append(obj.GetMark());
            sb.Append(Encryption.GetMark());
            sb.Append(encrypted);

            return sb.ToString();
        }

        public T Deserialize(string encrypted) 
        {
            T obj = new();

            if (encrypted[0] != obj.GetMark())
            {
                throw new InvalidMessageException();
            }

            if (encrypted[1] != Encryption.GetMark())
            {
                throw new InvalidMessageException();
            }

            string decrypted = Encryption.Decrypt(encrypted.Substring(2));

            obj.Deserialize(decrypted);

            return obj;
        }
    }
}
