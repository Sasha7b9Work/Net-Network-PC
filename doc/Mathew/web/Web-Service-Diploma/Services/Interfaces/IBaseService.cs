using Web_Service_Diploma.Models.HttpModels;

namespace Web_Service_Diploma.Services.Interfaces
{
    public interface IBaseService : IDisposable
    {
        Task<ResponseViewModel> SendAsync(RequestViewModel request);
    }
}
