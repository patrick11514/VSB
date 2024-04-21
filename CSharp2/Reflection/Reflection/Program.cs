namespace Reflection;

class Program
{
    static void Main(string[] args)
    {
        using var dapper = new MyDapper("db.sqlite");
        dapper.getType<string>();
        dapper.getType<int>();
        dapper.getType<double>();
        dapper.getType<float>();
        dapper.getType<decimal>();
        dapper.getType<long>();
        dapper.getType<short>();
        dapper.getType<byte>();
        dapper.getType<char>();
        dapper.getType<uint>();
        dapper.getType<ulong>();
        dapper.getType<ushort>();
        dapper.getType<sbyte>();
        dapper.getType<bool>();
        dapper.getType<DateTime>();
        dapper.getType<TimeSpan>();
        //dapper.CreateTable<Address>();
        //dapper.Insert(new Address() { City = null, Number = 459, ZipCode = "42000", Street = "Zlatá"});
        //dapper.SelectAll(new Address() { City = null, Number = 459, ZipCode = "42000", Street = "Zlatá"});
        //dapper.SelectAll<ExampleClass>();

        //dapper.Update(new ExampleClass() { Bool = true, Byte = 1, Char = 'c'});
    }
}