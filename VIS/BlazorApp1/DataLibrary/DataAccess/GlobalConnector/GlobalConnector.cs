using DataLibrary.DataAccess.Connectors;

namespace DataLibrary.DataAccess.GlobalConnector;

public class GlobalConnector
{
    public IDataConnector Connector { get; set; } = new SqlConnector();
}