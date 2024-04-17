namespace Reflection;

public enum Type
{
    INTEGER,
    TEXT,
    NULL,
    REAL,
    BOOLEAN
}

public class TypeAttribute : Attribute
{
    public readonly Type type;
    public readonly bool nullable;

    public TypeAttribute(Type type, bool nullable = false)
    {
        this.type = type;
        this.nullable = nullable;
    }

    public string getType()
    {
        switch (type)
        {
            case Type.INTEGER:
                return "INTEGER";
            case Type.TEXT:
                return "TEXT";
            case Type.NULL:
                return "NULL";
            case Type.REAL:
                return "REAL";
            case Type.BOOLEAN:
                return "BOOLEAN";
            default:
                throw new ArgumentOutOfRangeException();
        }
    }
}