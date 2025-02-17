using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace C07
{
    public interface IKeyValuePair<K, V>
    {
        K Key { get; }
        V Value { get; }
    }
}
