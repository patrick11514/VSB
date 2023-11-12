using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Messages
{
    public class MessageQueue<T>
    {
        private LinkedList<T> list = new();
        public bool IsEmpty { get
            {
                return list.Count == 0;
            } 
        }

        public void Enqueue(T item)
        {
            list.AddLast(item);
        }

        public T Dequeue()
        {
            if (this.IsEmpty)
            {
                throw new EmptyQueueException();
            }

            T last = list.First.Value;
            list.RemoveFirst();

            return last;
        }
    }
}
