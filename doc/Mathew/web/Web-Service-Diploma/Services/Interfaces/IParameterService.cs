using Web_Service_Diploma.Models.HttpModels;

namespace Web_Service_Diploma.Services.Interfaces
{
    public interface IParameterService: IBaseService
    {
        Task<ResponseViewModel> GetSchemas();
        Task<ResponseViewModel> GetParameter(string tableName, string schema);
        Task<ResponseViewModel> GetChartParameters(string schema, string parameter);
        Task<ResponseViewModel> GetCount(string schema);
    }
}
