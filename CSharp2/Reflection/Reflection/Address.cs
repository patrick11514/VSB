namespace Reflection;

public class Address
{
    [PrimaryKey]
    public int Id { get; set; }
    
    [Nullable]
    public string City { get; set; }
    [Nullable]
    public string? Street { get; set; }
    public int Number { get; set; }
    [Nullable]
    public string? ZipCode { get; set; }
    public DateTime Date { get; set; }
}