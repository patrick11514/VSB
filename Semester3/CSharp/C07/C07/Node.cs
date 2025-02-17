using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace C07
{
    internal class Node<K, V> : IKeyValuePair<K, V>
    {
        public K Key { get; set; }
        public V Value { get; set; }
        public Node<K, V> left { get; set; }
        public Node<K, V> right { get; set; }
        public Node(K key, V value)
        {
            this.Key = key;
            this.Value = value;
        }
    }
}
