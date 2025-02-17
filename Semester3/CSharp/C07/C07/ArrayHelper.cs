using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace C07
{
    public class ArrayHelper
    {
        public static void Swap<T>(T[] array, int index1, int index2)
        {
            T temp = array[index1];
            array[index1] = array[index2];
            array[index2] = temp;
        }

        public static T[] Concat<T>(T[] array1, T[] array2) {
            T[] array = new T[array1.Length + array2.Length];

            int i = 0;
            foreach (T item in array1)
            {
                array[i] = item;
                i++;
            }

            foreach (T item in array2)
            {
                array[i] = item;
                i++;
            }

            return array;
        }
    }
}
