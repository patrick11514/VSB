using System;

namespace C03
{
    internal class Program
    {
        static void Main(string[] args)
        {
            string str = Console.ReadLine();

            //Console.WriteLine(IntParser.ParseInt(str);

            /*int? cislo = IntParser.ParseIntOrNull(str);
            if (cislo == null)
            {
                Console.WriteLine("Neni cislo");
            } else
            {
                Console.WriteLine(cislo);
            }*/

            /*int num;
            if (IntParser.TryParseInt(str, out num))
            {
                Console.Write(num);
            } else
            {
                Console.WriteLine("Neni cislo");
            }
                
            nebo
             
             

            if (IntParser.TryParseInt(str, out int num))
            {
                Console.Write(num);
            }
            else
            {
                Console.WriteLine("Neni cislo");
            }
            */

            if (IntParser.TryParseInt(str, ParseIntOption.ALLOW_NEGATIVE_NUMBERS | ParseIntOption.IGNORE_INVALID_CHARACTERS, out int num))
            {
                Console.Write(num);
            }
            else
            {
                Console.WriteLine("Neni cislo");
            }


        }
    }
}