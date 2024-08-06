namespace Web_Service_Parameters.Models
{
    public class ResponseModel
    {
        public bool IsSuccess { get; set; } = true;
        public object Result { get; set; }
        public string DisplayMessages { get; set; } = "";

        public List<string> ErrorMessages { get; set; }
    }
}
