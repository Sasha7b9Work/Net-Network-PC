using Microsoft.AspNetCore.Html;
using Microsoft.AspNetCore.Mvc.Rendering;

namespace Web_Service_Diploma.HtmlHelpers
{
    public static class SchemaHelper
    {
        public static HtmlString Schemas(this IHtmlHelper html, List<string> schemas)
        {
            string result = "<select class=\"form-select\" id=\"schema\" required=\"\">";

            foreach (var schema in schemas)
                result += $"<option value=\"{schema}\">{schema}</option>";

            return new HtmlString($"{result}</select>");
        }
    }
}
