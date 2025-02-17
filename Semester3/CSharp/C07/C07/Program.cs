namespace C07
{
    internal class Program
    {
        static void Main(string[] args)
        {
            /*int[] array1 = { 1, 2, 3 };
            string[] array2 = { "a", "b", "c" };
            ArrayHelper.Swap(array1, 0, 2);
            Console.WriteLine(string.Join(", ", array1));

            ArrayHelper.Swap(array2, 0, 2);
            Console.WriteLine(string.Join(", ", array2));

            int[] array3 = { 1, 2, 3 };
            string[] array4 = { "a", "b", "c" };

            Console.WriteLine(string.Join(", ", ArrayHelper.Concat(array1, array3)));
            Console.WriteLine(string.Join(", ", ArrayHelper.Concat(array2, array4)));*/


            TreeMap<int, string> map = new();

            map[0] = "Pepa";
            map[2] = "Karel";
            map[1] = "Honza";
            map[10] = "Jirka";
            map[5] = "Pavel";
            map[3] = "Jana";
            map[4] = "Janouš";

            Console.WriteLine(map[10]);
            Console.WriteLine(map.Count());

            List<KeyValuePair<int, string>> keyValuePairs = map.ToList();

            foreach (KeyValuePair<int, string> pair in keyValuePairs)
            {
                Console.WriteLine(pair.Key + " " + pair.Value);
            }

            KeyValuePair<int, string>[] keyValuePairs1 = map.ToArray();

            foreach (KeyValuePair<int, string> pair in keyValuePairs1)
            {
                Console.WriteLine(pair.Key + " " + pair.Value);
            }
        }
    }
}