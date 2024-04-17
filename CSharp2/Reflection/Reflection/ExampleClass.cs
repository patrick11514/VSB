namespace Reflection;

public class ExampleClass
{
    public string String { get; set; }
    [PrimaryKey]
    public int Int { get; set; }    
    public double Double { get; set; }
    public float Float { get; set; }
    public decimal Decimal { get; set; }
    public long Long { get; set; }
    public short Short { get; set; }
    public byte Byte { get; set; }
    public char Char { get; set; }
    public uint Uint { get; set; }
    public ulong Ulong { get; set; }
    public ushort Ushort { get; set; }
    public sbyte Sbyte { get; set; }
    public bool Bool { get; set; }
    public DateTime DateTime { get; set; }
    public TimeSpan TimeSpan { get; set; }
}