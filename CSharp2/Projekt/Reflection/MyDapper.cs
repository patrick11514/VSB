using Microsoft.Data.Sqlite;
using System.ComponentModel;
using System.Reflection;
using System.Text;

namespace DataLayer;

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

	private void AddProperty(ref StringBuilder sb, PropertyInfo property, object? obj)
	{
		sb.Append($"{property.Name} = ");
		AddValue(ref sb, property, obj);
	}

	private void AddValue(ref StringBuilder sb, PropertyInfo property, object? obj)
	{
		var value = property.GetValue(obj);
		if (value == null)
		{
			sb.Append("NULL");
		}
		else
		{
			if (this.GetType(property.PropertyType) == "TEXT")
			{
				sb.Append($"\"{value}\"");
			}
			else
			{
				sb.Append(value);
			}
		}
	}

	public bool IsNullable<T>()
	{
		return Nullable.GetUnderlyingType(typeof(T)) != null;
	}

	//Convert type to SQLite type
	public string? GetType(Type t)
	{
		string name = t.Name;
		Type? nullableType = Nullable.GetUnderlyingType(t);

		if (nullableType != null)
		{
			name = nullableType.Name;
		}

		switch (name)
		{
			case "String":
			case "DateTime":
			case "TimeSpan":
				return "TEXT";
			case "Int16":
			case "Int32":
			case "Int64":
				return "INTEGER";
			case "Double":
			case "Single":
			case "Decimal":
				return "REAL";
		}

		return null;
	}

	//https://stackoverflow.com/a/49627291
	public object? ConvertTo<T>(T value, Type to)
	{
		if (Nullable.GetUnderlyingType(typeof(T)) != null)
		{
			TypeConverter converter = TypeDescriptor.GetConverter(typeof(T));
			return converter.ConvertTo(value, to);
		}
		return Convert.ChangeType(value, to);
	}

	private PropertyInfo? GetPrimaryProperty<T>(T value)
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

	private PropertyInfo? GetPrimaryProperty(Type t)
	{
		foreach (var property in t.GetProperties())
		{
			if (Attribute.IsDefined(property, typeof(PrimaryKeyAttribute)))
			{
				return property;
			}
		}

		return null;
	}

	private IEnumerable<T> Iterate<T>(SqliteDataReader r)
	{
		using var reader = r;
		Type type = typeof(T);

		while (reader.Read())
		{
			object? instance = Activator.CreateInstance(type);

			if (instance == null)
			{
				yield break;
			}

			var properties = type.GetProperties();
			for (int i = 0; i < reader.FieldCount; i++)
			{
				var property = properties[i];

				var value = reader.GetValue(i);
				if (reader.IsDBNull(i))
				{
					property.SetValue(instance, null);
				}
				else
				{
					property.SetValue(instance, ConvertTo(value, property.PropertyType));
				}
			}

			yield return (T)instance;
		}
	}

	////////////////////////////// CREATE //////////////////////////////

	public void CreateTable<T>()
	{
		var type = typeof(T);
		var properties = type.GetProperties();

		StringBuilder sb = new();
		sb.Append($"CREATE TABLE IF NOT EXISTS {type.Name} (");

		int i = 0;

		foreach (var property in properties)
		{
			//get type from property
			string? typeName = this.GetType(property.PropertyType);
			if (typeName == null)
			{
				throw new Exception("Type not supported");
			}

			sb.Append($"\"{property.Name}\" {typeName}");

			if (!Attribute.IsDefined(property, typeof(NullableAttribute)))
			{
				sb.Append(" NOT NULL");
			}

			if (Attribute.IsDefined(property, typeof(PrimaryKeyAttribute)))
			{
				sb.Append(" PRIMARY KEY");

				PrimaryKeyAttribute attr = (PrimaryKeyAttribute)property.GetCustomAttribute(typeof(PrimaryKeyAttribute))!;
				if (attr.AutoIncrement)
				{
					sb.Append(" AUTOINCREMENT");
				}
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

	////////////////////////////// SELECT //////////////////////////////

	public IEnumerable<T> SelectAll<T>(uint? limit = null)
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

		return Iterate<T>(cmd.ExecuteReader());

	}

	public T Select<T>(object pk)
	{
		var type = typeof(T);
		//get primary key
		PropertyInfo? pkProperty = GetPrimaryProperty(typeof(T));
		if (pkProperty == null)
		{
			throw new Exception("Primary key not found");
		}

		StringBuilder sb = new();
		sb.Append($"SELECT * FROM {type.Name} WHERE {pkProperty.Name} = ");

		if (this.GetType(pkProperty.PropertyType) == "TEXT")
		{
			sb.Append($"\"{pk}\"");
		}
		else
		{
			sb.Append(pk);
		}

		var cmd = connection.CreateCommand();
		cmd.CommandText = sb.ToString();
		return Iterate<T>(cmd.ExecuteReader()).First();

	}

	////////////////////////////// UPDATE //////////////////////////////

	public bool Update<T>(T obj)
	{
		var type = typeof(T);
		var properties = type.GetProperties();

		var pkProperty = GetPrimaryProperty(type);
		if (pkProperty == null)
		{
			return false;
		}

		StringBuilder sb = new();
		sb.Append($"UPDATE {type.Name} SET ");

		int i = 0;

		foreach (var property in properties)
		{
			if (property == pkProperty)
			{
				continue;
			}

			AddProperty(ref sb, property, obj);

			if (i < properties.Length - 2 /* -1 + -1 because of primary key*/)
			{
				sb.Append(", ");
			}

			++i;
		}

		sb.Append($" WHERE ");
		AddProperty(ref sb, pkProperty, obj);

		var cmd = connection.CreateCommand();
		cmd.CommandText = sb.ToString();
		cmd.ExecuteNonQuery();

		return true;
	}

	////////////////////////////// INSERT //////////////////////////////

	public bool Insert<T>(T obj)
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
			bool added = false;

			if (Attribute.IsDefined(property, typeof(PrimaryKeyAttribute)))
			{
				PrimaryKeyAttribute attr = (PrimaryKeyAttribute)property.GetCustomAttribute(typeof(PrimaryKeyAttribute))!;
				if (attr.AutoIncrement)
				{
					if (property.GetValue(obj).Equals(Activator.CreateInstance(property.PropertyType)))
					{
						sb.Append("NULL");
						added = true;
					}
				}
			}

			if (!added)
			{
				AddValue(ref sb, property, obj);
			}


			if (i < properties.Length - 1)
			{
				sb.Append(", ");
			}

			++i;
		}

		sb.Append(")");

		try
		{
			var cmd = connection.CreateCommand();
			cmd.CommandText = sb.ToString();
			cmd.ExecuteNonQuery();
		}
		catch
		{
			return false;
		}

		return true;
	}

	////////////////////////////// DELETE //////////////////////////////

	public void Delete<T>(T obj)
	{
		var type = typeof(T);
		var properties = type.GetProperties();

		StringBuilder sb = new();
		sb.Append($"DELETE FROM {type.Name} WHERE ");

		var pkProperty = GetPrimaryProperty(type);

		if (pkProperty == null)
		{
			//list each property
			int i = 0;
			foreach (var property in properties)
			{
				sb.Append($"{property.Name} = ");
				AddValue(ref sb, property, obj);

				if (i < properties.Length - 1)
				{
					sb.Append(" AND ");
				}

				++i;
			}
		}
		else
		{
			sb.Append($"{pkProperty.Name} = ");
			AddValue(ref sb, pkProperty, obj);
		}

		var cmd = connection.CreateCommand();
		cmd.CommandText = sb.ToString();
		cmd.ExecuteNonQuery();
	}
}