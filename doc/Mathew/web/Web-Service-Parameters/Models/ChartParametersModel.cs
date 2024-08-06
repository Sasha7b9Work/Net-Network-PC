namespace Web_Service_Parameters.Models
{
    public class ChartParametersModel
    {
        public int id { get; set; }
        public bool Temperature { get; set; }
        public bool Humidity { get; set; }
        public bool Pressure { get; set; }
        public bool Dew_Point { get; set; }
    }
}
