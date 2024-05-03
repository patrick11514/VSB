namespace Reflection;

public class PrimaryKeyAttribute : Attribute
{
    public readonly bool AutoIncrement;

    public PrimaryKeyAttribute(bool autoIncrement = true)
    {
        this.AutoIncrement = autoIncrement;
    }
}