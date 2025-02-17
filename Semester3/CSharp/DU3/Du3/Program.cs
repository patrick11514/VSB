using System.Security.Cryptography.X509Certificates;

namespace Messages
{
    internal class Program
    {
        static void Main(string[] args)
        {
            PasswordBasedEncryption passwordEnc = new PasswordBasedEncryption("moje heslo");
            Rot13Encryption rot13Enc = new Rot13Encryption();


            MessageSerializer<Message> passwordEnv = new MessageSerializer<Message>(passwordEnc);
            MessageSerializer<Message> rot13Env = new MessageSerializer<Message>(rot13Enc);
            MessageSerializer<EmailMessage> emailRot13Env = new MessageSerializer<EmailMessage>(rot13Enc);


            MessageQueue<string> queue = new MessageQueue<string>();

            Mailbox mailbox = new Mailbox(queue);
            mailbox.RegisterSerializer(emailRot13Env);
            mailbox.RegisterSerializer(passwordEnv);
            mailbox.RegisterSerializer(rot13Env);



            mailbox.SendMessage(passwordEnv, new Message() { Text = "Text první zprávy." });
            mailbox.SendMessage(emailRot13Env, new EmailMessage("jan.janousek@vsb.cz", "Email"));
            mailbox.SendMessage(rot13Env, new Message() { Text = "Poslední zpráva." });



            while (!queue.IsEmpty)
            {
                string txt = queue.Dequeue();
                if (mailbox.TryReceive(txt, out Message message))
                {
                    Console.WriteLine("TEXT MESSAGE:");
                    Console.WriteLine(message.Text);
                }
                else if (mailbox.TryReceive(txt, out EmailMessage emailMessage))
                {
                    Console.WriteLine("EMAIL MESSAGE:");
                    Console.WriteLine(emailMessage.Email);
                    Console.WriteLine(emailMessage.Text);
                }
                else
                {
                    Console.WriteLine("Nelze přečíst zprávu: " + txt);
                }
                Console.WriteLine();
            }



            Console.WriteLine();
        }
    }
}