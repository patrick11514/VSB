using System;
using Tutorial10.Auticko;
using Tutorial10.BinaryFormatterExample;
using Tutorial10.DataContractExample;
using Tutorial10.DataContractJsonExample;
using Tutorial10.JsonSerializerExample;
using Tutorial10.XmlSerializerExample;
using Tutorial10.Auticko;

namespace Tutorial10
{
    internal class Program
    {
        static void Main(string[] args)
        {
            DataContractSerialization.Run();

            DataContractJsonSerialization.Run();

            BinarySerialization.Run();

            XmlSerialization.Run();

            JsonSerialization.Run();

            Serialize.Run();

            Console.WriteLine();
        }


    }
}
