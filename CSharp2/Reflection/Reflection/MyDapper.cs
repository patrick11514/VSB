using System.Reflection;
using System.Text;

namespace Reflection;

public class MyDapper
{
    private void AddProperty(ref StringBuilder sb, PropertyInfo? property, object? obj)
    {
        sb.Append($"{property?.Name} = ");
        AddValue(ref sb, property?.GetValue(obj));
    }
    private void AddValue(ref StringBuilder sb, object? value)
    {
        switch (value)
        {
            case null:
                sb.Append("null");
                break;
            case DateTime:
            case TimeSpan:
            case string:
                sb.Append($"\"{value}\"");
                break;
            case char:
                sb.Append($"'{value}'");
                break;
            default:
            {
                if (value is bool boolVal)
                {
                    sb.Append(boolVal ? "true" : "false");
                }
                else
                {
                    sb.Append($"{value}");
                }

                break;
            }
        }
    } 
    
    public void SelectAll<T>(T obj)
    {
        var type = typeof(T);
        var properties = type.GetProperties();

        StringBuilder sb = new();
        sb.Append($"UPDATE {type.Name} SET ");

        int i = 0;

        PropertyInfo? idAttr = null;
       
        foreach (var property in properties)
        {
            if (Attribute.IsDefined(property, typeof(PrimaryKeyAttribute)))
            {
                idAttr = property;
                continue;
            }
            
            AddProperty(ref sb, property, obj);
            
            if (i < properties.Length - 1)
            {
                sb.Append(", ");
            }

            ++i;
        }

        if (idAttr == null)
        {
            return;
        }

        sb.Append($" WHERE ");
        AddProperty(ref sb, idAttr, obj);
        
        Console.WriteLine(sb.ToString());
    }
}