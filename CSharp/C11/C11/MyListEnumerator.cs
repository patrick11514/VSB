using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace C11
{
    internal class MyListEnumerator : IEnumerator<int>
    {
        private List<int> list;
        private int index = -1;
        public MyListEnumerator(List<int> list) 
        {
            this.list = list;
        }

        public int Current
        {
            get
            {
                return this.list[this.index];
            }
        }

        object IEnumerator.Current
        {
            get
            {
                return this.list[this.index];
            }
        }

        public void Dispose()
        {
            
        }

        public bool MoveNext()
        {
            this.index++;
            return this.list.Count > this.index;
        }

        public void Reset()
        {
            this.index = -1;
        }
    }
}
