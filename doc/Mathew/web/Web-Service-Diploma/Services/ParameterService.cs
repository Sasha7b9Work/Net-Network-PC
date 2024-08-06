using Web_Service_Diploma.Models.HttpModels;
using Web_Service_Diploma.Services.Interfaces;

namespace Web_Service_Diploma.Services
{
    public class ParameterService: BaseService, IParameterService
    {
        private IConfiguration _configuration;

        public ParameterService(IHttpClientFactory http, IConfiguration configuration) : base(http)
            => _configuration = configuration;

        public async Task<ResponseViewModel> GetSchemas()
            => await this.SendAsync(new RequestViewModel
            {
                Method = HttpMethod.Get,
                Url = _configuration["ParameterUrl"] + "/parameter/GetSchema"
            });

        public async Task<ResponseViewModel> GetParameter(string tableName, string schema)
            => await this.SendAsync(new RequestViewModel
            {
                Method = HttpMethod.Post,
                Data = new { tableName = tableName, schema = schema },
                Url = _configuration["ParameterUrl"] + "/parameter/GetParameter"
            });

        public async Task<ResponseViewModel> GetChartParameters(string schema, string parameter)
            => await this.SendAsync(new RequestViewModel
            {
                Method = HttpMethod.Put,
                Data = new { schema = schema, tableName = parameter },
                Url = _configuration["ParameterUrl"] + "/parameter/GetChartParameters"
            });

        public async Task<ResponseViewModel> GetCount(string schema)
            => await this.SendAsync(new RequestViewModel
            {
                Method = HttpMethod.Post,
                Data = new { schema = schema },
                Url = _configuration["ParameterUrl"] + "/parameter/GetCount"
            });
    }
}
