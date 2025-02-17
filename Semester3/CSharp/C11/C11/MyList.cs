using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace C11
{
    internal class MyList : IEnumerable<int>
    {
        private List<int> list = new();

        public void Add(int x)
        {
            this.list.Add(x);
        }

        public IEnumerator<int> GetEnumerator()
        {
            for (int i = 0; i < list.Count; i+=2)
            {
                if (list[i] == 3)
                {
                    yield break;
                }
                yield return this.list[i];
            }
            //return new MyListEnumerator(this.list);
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            for (int i = 0; i < list.Count; i += 2)
            {
                if (list[i] == 3)
                {
                    yield break;
                }
                yield return this.list[i];
            }
            //return new MyListEnumerator(this.list);
        }

        public IEnumerable<int> Multiply(int multiply)
        {
            for (int i = 0; i < list.Count; ++i)
            {
                yield return this.list[i] * multiply;
            }
            //return new MyListEnumerator(this.list);
        }

        public int this[int x]
        {
            get
            {
                return this.list[x];
            }
            set
            {
                this.list[x] = value;
            }
        }
    }
}
