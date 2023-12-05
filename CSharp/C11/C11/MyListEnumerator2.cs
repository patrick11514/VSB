using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace C11
{
    internal class MyListEnumerator2 : IEnumerator<int>
    {
        //private List<int> list;
        private int index = -1;
        public MyListEnumerator2(List<int> list) 
        {
            //this.list = list;
        }

        public int Current
        {
            get
            {
                return (int) Math.Pow(2, this.index);
            }
        }

        object IEnumerator.Current
        {
            get
            {
                return (int)Math.Pow(2, this.index);
            }
        }

        public void Dispose()
        {
            
        }

        public bool MoveNext()
        {
            this.index++;
            return this.index < 32;
        }

        public void Reset()
        {
            this.index = -1;
        }
    }
}
