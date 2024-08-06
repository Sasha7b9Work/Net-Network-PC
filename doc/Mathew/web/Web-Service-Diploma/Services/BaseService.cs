using Newtonsoft.Json;
using System.Net.Http.Headers;
using System.Text;
using Web_Service_Diploma.Models.HttpModels;
using Web_Service_Diploma.Services.Interfaces;

namespace Web_Service_Diploma.Services
{
    public class BaseService: IBaseService
    {
        private IHttpClientFactory _http;

        public BaseService(IHttpClientFactory http)
            => _http = http;

        public async Task<ResponseViewModel> SendAsync(RequestViewModel request)
        {
            try
            {
                var client = _http.CreateClient("Client");
                var message = new HttpRequestMessage();
                message.Headers.Add("Accept", "application/json");
                message.RequestUri = new Uri(request.Url);
                client.DefaultRequestHeaders.Clear();

                message.Method = request.Method;

                if (request.Data is not null)
                    message.Content = new StringContent(JsonConvert.SerializeObject(request.Data),
                        Encoding.UTF8, "application/json");

                var res = await client.SendAsync(message);
                var content = await res.Content.ReadAsStringAsync();
                var response = JsonConvert.DeserializeObject<ResponseViewModel>(content);

                return response;

            }
            catch (Exception ex)
            {
                var response = new ResponseViewModel
                {
                    IsSuccess = false,
                    ErrorMessages = new List<string> { ex.Message }
                };

                return response;
            }
        }

        public void Dispose()
        {
            GC.SuppressFinalize(this);
        }
    }
}
