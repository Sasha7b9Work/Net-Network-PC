namespace Web_Service_Diploma.Models.HttpModels
{
    public class ResponseViewModel
    {
        public bool IsSuccess { get; set; } = true;
        public object Result { get; set; }
        public string DisplayMessages { get; set; } = "";

        public List<string> ErrorMessages { get; set; }
    }
}
