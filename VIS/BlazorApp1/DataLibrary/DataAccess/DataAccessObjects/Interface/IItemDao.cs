using DataLibrary.DataTransferObjects;

namespace DataLibrary.DataAccess.DataAccessObjects.Interface;

public interface IItemDao
{
    List<ItemDTO> SelectAll();
}