using Microsoft.AspNetCore.Mvc;
using Newtonsoft.Json;
using System.Collections.Generic;
using System.Diagnostics;
using Web_Service_Diploma.Models;
using Web_Service_Diploma.Services.Interfaces;
using Web_Service_Diploma.ViewModels;

namespace Web_Service_Diploma.Controllers
{
    public class HomeController : Controller
    {
        private readonly ILogger<HomeController> _logger;
        private readonly IParameterService _parameterService;

        private static ListOfViewModels models = new();

        public HomeController(ILogger<HomeController> logger, IParameterService parameterService)
        {
            _logger = logger;
            _parameterService = parameterService;
        }

        public IActionResult Index()
            => View();

        public IActionResult ChangeSchema(string schema)
        {
            models.Schema = schema;

            return RedirectToAction("Index");
        }

        [HttpGet]
        public async Task<IActionResult> GetSchemas()
        {
            var response = await _parameterService.GetSchemas();

            if(response is not null && response.IsSuccess)
            {
                models.Schemas = new List<string> { "Nothing" };

                var schemas = JsonConvert
                    .DeserializeObject<List<string>>(Convert.ToString(response.Result));

                foreach(string schema in schemas)
                    models.Schemas.Add(schema);

                return Ok(models);
            }

            return BadRequest(response);
        }

        [HttpGet]
        public async Task<IActionResult> GetParameters()
        {
            if(models.Schema != "Nothing")
            {
                var responseCount = await _parameterService
                    .GetCount(models.Schema);

                if (responseCount is not null && responseCount.IsSuccess)
                {
                    int lengthOfOscilliator = Int32.Parse(responseCount.Result.ToString());

                    var responseTemp = await _parameterService
                        .GetParameter("Temperature", models.Schema);

                    var responsePre = await _parameterService
                        .GetParameter("Pressure", models.Schema);

                    var responseHum = await _parameterService
                        .GetParameter("Humidity", models.Schema);

                    var responseDp = await _parameterService
                        .GetParameter("Dew point", models.Schema);


                    var responseChartTemp = await _parameterService
                        .GetChartParameters(models.Schema, "Temperature");

                    var responseChartPre = await _parameterService
                        .GetChartParameters(models.Schema, "Pressure");

                    var responseChartHum = await _parameterService
                        .GetChartParameters(models.Schema, "Humidity");

                    var responseChartDp = await _parameterService
                        .GetChartParameters(models.Schema, "Dew point");

                    if (responseTemp is not null && responseTemp.IsSuccess)
                    {
                        if (responseChartTemp is not null && responseChartTemp.IsSuccess)
                        {
                            List<ParameterViewModel>? list = JsonConvert
                                .DeserializeObject<List<ParameterViewModel>>(Convert.ToString(responseTemp.Result));

                            var isAdding = (bool)responseChartTemp.Result;
                            ParameterViewModel? viewModel = null;

                            if (isAdding)
                            {
                                if (list.Count >= models.LastCountTemperature)
                                {
                                    models.Temperature = list;
                                    models.LastCountTemperature = list.Count;
                                    viewModel = models.Temperature
                                        .LastOrDefault();
                                }
                            }
                            else
                            {
                                if (list.Count == lengthOfOscilliator)
                                    models.Temperature = list;

                                viewModel = models.Temperature
                                        .FirstOrDefault();
                            }

                            if (viewModel is not null)
                                models.InstantParameters.Temperature = viewModel.Parameter;
                        }
                    }

                    if (responsePre is not null && responsePre.IsSuccess)
                    {
                        if (responseChartPre is not null && responseChartPre.IsSuccess)
                        {
                            List<ParameterViewModel>? list = JsonConvert
                                .DeserializeObject<List<ParameterViewModel>>(Convert.ToString(responsePre.Result));

                            var isAdding = (bool)responseChartPre.Result;
                            ParameterViewModel? viewModel = null;

                            if (isAdding)
                            {
                                if (list.Count >= models.LastCountPressure)
                                {
                                    models.Pressure = list;
                                    models.LastCountPressure = list.Count;
                                    viewModel = models.Pressure
                                        .LastOrDefault();
                                }
                            }
                            else
                            {
                                if (list.Count == lengthOfOscilliator)
                                    models.Pressure = list;

                                viewModel = models.Pressure
                                        .FirstOrDefault();
                            }

                            if (viewModel is not null)
                                models.InstantParameters.Pressure = viewModel.Parameter;
                        }
                    }

                    if (responseHum is not null && responseHum.IsSuccess)
                    {
                        if (responseChartHum is not null && responseChartHum.IsSuccess)
                        {
                            List<ParameterViewModel>? list = JsonConvert
                                .DeserializeObject<List<ParameterViewModel>>(Convert.ToString(responseHum.Result));

                            var isAdding = (bool)responseChartHum.Result;
                            ParameterViewModel? viewModel = null;

                            if (isAdding)
                            {
                                if (list.Count >= models.LastCountHumidity)
                                {
                                    models.Humidity = list;
                                    models.LastCountHumidity = list.Count;
                                    viewModel = models.Humidity
                                        .LastOrDefault();
                                }
                            }
                            else
                            {
                                if (list.Count == lengthOfOscilliator)
                                    models.Humidity = list;

                                viewModel = models.Humidity
                                        .FirstOrDefault();
                            }

                            if (viewModel is not null)
                                models.InstantParameters.Humidity = viewModel.Parameter;
                        }
                    }

                    if (responseDp is not null && responseDp.IsSuccess)
                    {
                        if (responseChartDp is not null && responseChartDp.IsSuccess)
                        {
                            List<ParameterViewModel>? list = JsonConvert
                                .DeserializeObject<List<ParameterViewModel>>(Convert.ToString(responseDp.Result));

                            var isAdding = (bool)responseChartDp.Result;
                            ParameterViewModel? viewModel = null;

                            if (isAdding)
                            {
                                if (list.Count >= models.LastCountDewPoint)
                                {
                                    models.DewPoint = list;
                                    models.LastCountDewPoint = list.Count;
                                    viewModel = models.DewPoint
                                        .LastOrDefault();
                                }
                            }
                            else
                            {
                                if (list.Count == lengthOfOscilliator)
                                    models.DewPoint = list;

                                viewModel = models.DewPoint
                                        .FirstOrDefault();
                            }

                            if (viewModel is not null)
                                models.InstantParameters.DewPoint = viewModel.Parameter;
                        }
                    }

                    return Ok(models);
                }
            }

            return BadRequest();
        }

        public IActionResult Privacy()
        {
            return View();
        }

        [ResponseCache(Duration = 0, Location = ResponseCacheLocation.None, NoStore = true)]
        public IActionResult Error()
        {
            return View(new ErrorViewModel { RequestId = Activity.Current?.Id ?? HttpContext.TraceIdentifier });
        }
    }
}
