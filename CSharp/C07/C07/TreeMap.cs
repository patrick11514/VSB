using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace C07
{
    internal class TreeMap<K, V> where K : IComparable<K>
    {
        private Node<K, V> root;

        public V this[K key] { 
            get 
            {
                Node<K, V> current = root;

                while (true)
                {
                    if (current == null)
                    {
                        break;
                    }

                    if (current.Key.CompareTo(key) == 0)
                    {
                        return current.Value;
                    }

                    if (current.Key.CompareTo(key) < 0)
                    {
                        current = current.right;
                    } else
                    {
                        current = current.left;
                    }
                }
                return default(V);
            }
            set
            {

                Node<K, V> current = root;

                if (root == null)
                {
                    root = new Node<K, V>(key, value);
                    return;
                }

                while (true)
                {

                    if (current.Key.CompareTo(key) == 0)
                    {
                        current.Value = value;
                        return;
                    }

                    if (current.Key.CompareTo(key) < 0)
                    {
                        if (current.right == null)
                        {
                            current.right = new Node<K, V>(key, value);
                            return;
                        }
                        current = current.right;
                    }
                    else
                    {
                        if (current.left == null)
                        {
                            current.left = new Node<K, V>(key, value);
                            return;
                        }
                        current = current.left;
                    }
                }
            }
        }

        private int _NodeCount(Node<K, V> node)
        {
            if (node == null) return 0;

            int count = 1;

            count += _NodeCount(node.right);
            count += _NodeCount(node.left);

            return count;
        }

        public int Count()
        {
            return _NodeCount(this.root);
        }

        private void _ToList(Node<K, V> node, List<KeyValuePair<K, V>> list)
        {
            if (node == null) return;

            _ToList(node.left, list);
            list.Add(new KeyValuePair<K, V>(node.Key, node.Value));
            _ToList(node.right, list);
        }   

        public List<KeyValuePair<K, V>> ToList()
        {
            List<KeyValuePair<K, V>> list = new();

            _ToList(this.root, list);

            return list;
        }

        public KeyValuePair<K, V>[] ToArray()
        {
            List<KeyValuePair<K, V>> list = this.ToList();

            return list.ToArray();
        }
    }
}
