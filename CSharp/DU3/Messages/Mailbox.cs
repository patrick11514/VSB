using Messages;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Messages
{
    public class Mailbox
    {
        private MessageQueue<string> Queue;
        private List<MessageSerializeBase> MessageSerializers = new();
        public Mailbox(MessageQueue<string> queue)
        {
            Queue = queue;
        }

        public void SendMessage<T>(MessageSerializer<T> serializer, T obj) where T : IMessage, new()
        {
            Queue.Enqueue(serializer.Serialize(obj));
        }

        public void RegisterSerializer(MessageSerializeBase serializer)
        {
            MessageSerializers.Add(serializer);
        }

        public bool TryReceive<T>(string serialized, out T obj) where T : IMessage, new()
        {
            foreach (MessageSerializeBase serializer in MessageSerializers)
            {
                if (serializer is MessageSerializer<T> msgSerializer)
                {
                    try
                    {
                        obj = msgSerializer.Deserialize(serialized);
                        return true;
                    }
                    catch (InvalidMessageException)
                    {

                    }
                }
            }

            obj = default;

            return false;
        }
    }
}
