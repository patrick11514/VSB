using DataLibrary.DataAccess.DataAccessObjects.Interface;

namespace DataLibrary.DataAccess.Connectors;

public interface IDataConnector
{
    IItemDao ItemDao { get; }
}