using Microsoft.AspNetCore.Mvc;
using System.Text.RegularExpressions;
using Web_Service_Parameters.Data;
using Web_Service_Parameters.Models;

namespace Web_Service_Parameters.Controllers
{
    [Route("[controller]/[action]")]
    public class ParameterController : ControllerBase
    {
        private ResponseModel _response;
        private readonly AppDbContext _context;

        public ParameterController(AppDbContext context)
            => (_response, _context) = (new(), context);

        [HttpGet]
        public async Task<object> GetSchema()
        {
            try
            {
                var schemas = await _context.GetSchema();

                _response.Result = schemas
                    .Where(schema => Regex.IsMatch(schema, @"^[A-Z0-9]+$"))
                    .Distinct();
            }
            catch (Exception ex)
            {
                _response.IsSuccess = false;
                _response.ErrorMessages = new List<string> { ex.Message };
            }

            return _response;
        }

        [HttpPost]
        public async Task<object> GetCount([FromBody] RequestModel request)
        {
            try
            {
                _response.Result = await _context
                    .Count(request.schema);
            }
            catch (Exception ex)
            {
                _response.IsSuccess = false;
                _response.ErrorMessages = new List<string> { ex.Message };
            }

            return _response;
        }

        [HttpPost]
        public async Task<object> GetParameter([FromBody]RequestModel request)
        {
            try
            {
                _response.Result = await _context
                    .GetParameter(request.tableName, request.schema);
            }
            catch (Exception ex)
            {
                _response.IsSuccess = false;
                _response.ErrorMessages = new List<string> { ex.Message };
            }

            return _response;
        }

        [HttpPut()]
        public async Task<object> GetChartParameters([FromBody] RequestModel request)
        {
            try
            {
                _response.Result = await _context
                    .GetChartParameters(request.schema, request.tableName);
            }
            catch (Exception ex)
            {
                _response.IsSuccess = false;
                _response.ErrorMessages = new List<string> { ex.Message };
            }

            return _response;
        }
    }
}
