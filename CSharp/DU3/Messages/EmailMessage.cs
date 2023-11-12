using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Messages
{
    public class EmailMessage : IMessage
    {
        public string Email { get; set; }
        public string Text { get; set; }

        public EmailMessage()
        {
        }
        public EmailMessage(string email, string text)
        {
            Email = email;
            Text = text;
        }
        public void Deserialize(string text)
        {
            string[] splitted = text.Split(":");
            Email = splitted[0];
            Text = splitted[1];
        }

        public char GetMark()
        {
            return 'E';
        }

        public string Serialize()
        {
            StringBuilder sb = new();
            sb.Append(this.Email);
            sb.Append(':');
            sb.Append(this.Text);

            return sb.ToString();
        }
    }
}
