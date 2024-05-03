using System.Reflection;
using System.Text;
using Microsoft.Data.Sqlite;

namespace Reflection;

public class MyDapper : IDisposable, IAsyncDisposable
{
    private SqliteConnection connection;
    public MyDapper(string source)
    {
        var connectionString = $"Data Source={source};";
        connection = new SqliteConnection(connectionString);
        connection.Open();
    }
    
    ////////////////////////////// DISPOSE ////////////////////////////// 
    public void Dispose()
    {
        connection.Dispose();
    }

    public async ValueTask DisposeAsync()
    {
        await connection.DisposeAsync();
    }
    
    ////////////////////////////// PRIVATE ////////////////////////////// 
    
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

    //Convert type to SQLite type
    public string GetType<T>()
    {
        switch (typeof(T).Name)
        {
            case "String":
                return "TEXT";   
            case "Int16":
            case "Int32":
            case "Int64":
                return "INTEGER";
            case "Double":
            case "Single":
            case "Decimal":
                return "REAL";
            default:
                Console.WriteLine(typeof(T).Name);
                return "";
        }
        
        return "aaa";
    }

    private PropertyInfo? GetPrimaryProperty<T>()
    {
        var type = typeof(T);

        foreach (var property in type.GetProperties())
        {
            if (Attribute.IsDefined(property, typeof(PrimaryKeyAttribute)))
            {
                return property;
            }
        }

        return null;
    }

    ////////////////////////////// CREATE //////////////////////////////

    public void CreateTable<T>()
    {
        var type = typeof(T);
        var properties = type.GetProperties();
        
        StringBuilder sb = new();
        sb.Append($"CREATE TABLE {type.Name} (");

        int i = 0;
        
        foreach (var property in properties)
        {
            if (!Attribute.IsDefined(property, typeof(NullableAttribute)))
            {
                continue;
            }
            
            var attr = (NullableAttribute?)Attribute.GetCustomAttribute(property, typeof(NullableAttribute));

            if (attr == null)
            {
                continue;
            }

            if (Attribute.IsDefined(property, typeof(PrimaryKeyAttribute)))
            {
                sb.Append(" PRIMARY KEY");
            }

            if (i < properties.Length - 1)
            {
                sb.Append(", ");
            }

            ++i;
        }

        sb.Append(')');
        
        var cmd = connection.CreateCommand();
        cmd.CommandText = sb.ToString();
        cmd.ExecuteNonQuery();
    }
    public List<T> SelectAll<T>()
    {
        var type = typeof(T);

        StringBuilder sb = new();
        sb.Append($"SELECT * FROM {type.Name}");
        
        var cmd = connection.CreateCommand();
        cmd.CommandText = sb.ToString();
        var reader = cmd.ExecuteReader();

        /*while (reader.Read())
        {
            
        }*/

        return new List<T>();
    }

    public void Select<T>(uint? limit = null)
    {
        var type = typeof(T);

        StringBuilder sb = new();
        sb.Append($"SELECT * FROM {type.Name}");
        if (limit != null)
        {
            sb.Append(" LIMIT ");
            sb.Append(limit);
        }
        
        var cmd = connection.CreateCommand();
        cmd.CommandText = sb.ToString();
        cmd.ExecuteNonQuery();
    }
    public bool Update<T>(T obj)
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
            return false;
        }

        sb.Append($" WHERE ");
        AddProperty(ref sb, idAttr, obj);

        var cmd = connection.CreateCommand();
        cmd.CommandText = sb.ToString();
        cmd.ExecuteNonQuery();

        return true;
    }

    public void Insert<T>(T obj)
    {
        var type = typeof(T);
        var properties = type.GetProperties();

        StringBuilder sb = new();
        sb.Append($"INSERT INTO {type.Name} (");
        
        int i = 0;
        foreach (var property in properties)
        {
            sb.Append($"{property.Name}");

            if (i < properties.Length - 1)
            {
                sb.Append(", ");
            }

            ++i;
        }

        sb.Append(") VALUES (");

        i = 0;
        foreach (var property in properties)
        {
            AddValue(ref sb, property.GetValue(obj));
            if (i < properties.Length - 1)
            {
                sb.Append(", ");
            }

            ++i;
        }

        sb.Append(")");
        
        Console.WriteLine(sb.ToString());
    }

}