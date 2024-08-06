using Web_Service_Diploma.Models;

namespace Web_Service_Diploma.ViewModels
{
    public class ListOfViewModels
    {
        public string Schema { get; set; } = "Nothing";
        public List<string> Schemas { get; set; } = new List<string>();

        public InstantParameterViewModel InstantParameters { get; set; } = new InstantParameterViewModel();

        public List<ParameterViewModel> Temperature { get; set; } = new List<ParameterViewModel>();
        public int LastCountTemperature { get; set; } = 0; 

        public List<ParameterViewModel> Pressure { get; set; } = new List<ParameterViewModel>();
        public int LastCountPressure { get; set; } = 0;

        public List<ParameterViewModel> Humidity { get; set; } = new List<ParameterViewModel>();
        public int LastCountHumidity { get; set; } = 0;

        public List<ParameterViewModel> DewPoint { get; set; } = new List<ParameterViewModel>();
        public int LastCountDewPoint { get; set; } = 0;
    }
}
