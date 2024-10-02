using DataLibrary.DataAccess.DataAccessObjects.Interface;
using DataLibrary.DataAccess.DataAccessObjects.Text;

namespace DataLibrary.DataAccess.Connectors;

public class TextConnector : IDataConnector
{
    public IItemDao ItemDao { get; } = new ItemTextDao();
}