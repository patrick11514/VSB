using DataLibrary.DataAccess.DataAccessObjects.Interface;
using DataLibrary.DataAccess.DataAccessObjects.Sql;

namespace DataLibrary.DataAccess.Connectors;

public class SqlConnector : IDataConnector
{
    public IItemDao ItemDao { get; } = new ItemSqlDao();
}