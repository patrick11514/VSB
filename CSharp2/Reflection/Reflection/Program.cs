namespace Reflection;

class Program
{
    static void Main(string[] args)
    {
        using var dapper = new MyDapper("db.sqlite");
        dapper.GetType<string>();
        dapper.GetType<int>();
        dapper.GetType<double>();
        dapper.GetType<float>();
        dapper.GetType<decimal>();
        dapper.GetType<long>();
        dapper.GetType<short>();
        dapper.GetType<byte>();
        dapper.GetType<char>();
        dapper.GetType<uint>();
        dapper.GetType<ulong>();
        dapper.GetType<ushort>();
        dapper.GetType<sbyte>();
        dapper.GetType<bool>();
        dapper.GetType<DateTime>();
        dapper.GetType<TimeSpan>();
        //dapper.CreateTable<Address>();
        //dapper.Insert(new Address() { City = null, Number = 459, ZipCode = "42000", Street = "Zlatá"});
        //dapper.SelectAll(new Address() { City = null, Number = 459, ZipCode = "42000", Street = "Zlatá"});
        //dapper.SelectAll<ExampleClass>();

        //dapper.Update(new ExampleClass() { Bool = true, Byte = 1, Char = 'c'});
    }
}