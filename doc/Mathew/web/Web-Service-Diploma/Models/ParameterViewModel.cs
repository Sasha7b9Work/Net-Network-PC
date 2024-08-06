using System.ComponentModel.DataAnnotations;

namespace Web_Service_Diploma.Models
{
    public class ParameterViewModel
    {
        public int Id { get; set; }
        public decimal Parameter { get; set; }
        public DateTime Time { get; set; }
    }
}
