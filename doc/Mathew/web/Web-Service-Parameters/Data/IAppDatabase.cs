using Web_Service_Parameters.Models;

namespace Web_Service_Parameters.Data
{
    public interface IAppDatabase
    {
        Task<IEnumerable<string>> GetSchema();
        Task<IEnumerable<ParameterModel>> GetParameter(string tableName, string schema);
    }
}
