using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace _2C03
{

    class SimpleStack<T>
    {

        private List<T> data = new();

        private object lockObject = new();

        public T Top
        {
            get
            {
                lock (lockObject)
                {
                    int idx = this.data.Count - 1;
                    if (idx == -1)
                    {
                        throw new StackEmptyException();
                    }
                    return data[idx];
                }
            }
        }


        public bool IsEmpty
        {
            get
            {
                return this.data.Count == 0;
            }
        }


        public void Push(T val)
        {
            this.data.Add(val);
        }


        public T Pop()
        {
            lock (lockObject)
            {
                int idx = this.data.Count - 1;
                if (idx == -1)
                {
                    throw new StackEmptyException();
                }
                T val = this.data[idx];
                this.data.RemoveAt(idx);
                return val;
            }
        }


        public class StackEmptyException : Exception
        {

        }
    }


}
