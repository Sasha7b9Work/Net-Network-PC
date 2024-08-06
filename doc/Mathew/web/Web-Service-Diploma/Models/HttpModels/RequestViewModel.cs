using System.Security.AccessControl;

namespace Web_Service_Diploma.Models.HttpModels
{
    public class RequestViewModel
    {
        public HttpMethod Method { get; set; }
        public string Url { get; set; }
        public object Data { get; set; }
    }
}
