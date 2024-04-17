namespace Reflection;

public class User
{
    [PrimaryKey]
    public int Id { get; set; }
    public string Name { get; set; }
    public string Surname { get; set; }
    public int Age { get; set; }
    public string Email { get; set; }
}