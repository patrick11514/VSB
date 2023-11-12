using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Messages
{
    public class Message : IMessage
    {
        public string Text { get; set; }

        public Message()
        {
        }

        public Message(string text)
        {
            Text = text;
        }
        public void Deserialize(string text)
        {
            Text = text;
        }

        public char GetMark()
        {
            return 'T';
        }

        public string Serialize()
        {
            return Text;
        }
    }
}
