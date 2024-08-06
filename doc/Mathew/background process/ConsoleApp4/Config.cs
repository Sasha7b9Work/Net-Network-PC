using ConsoleApp4.Models;
using Newtonsoft.Json;
using System.Net.Http.Json;
using System.Text.Json;
using System.Text.Json.Serialization;

namespace ConsoleApp4
{
    internal class Config
    {
        public string Ip { get; }
        public int Port { get; }
        public string ConnectionString { get; }
        public int lengthOfOscilliator { get; }

        public Config()
        {
            ConfigModel config = GetConfigFromJson();

            Ip = config.ip;
            Port = config.port;
            ConnectionString = config.connectionString;
            lengthOfOscilliator = config.lengthOfOscilliator;
        }

        private ConfigModel GetConfigFromJson()
        {

            using (StreamReader r = new StreamReader(Path.GetFullPath("config.json")))
            {
                string json = r.ReadToEnd();
                var routes_list = JsonConvert.DeserializeObject<IDictionary<string, object>>(json);

                ConfigModel config = new ConfigModel
                {
                    ip = routes_list["ip"].ToString(),
                    port = Int32.Parse(routes_list["port"].ToString()),
                    connectionString = routes_list["connectionString"].ToString(),
                    lengthOfOscilliator = int.Parse(routes_list["lengthOfOscilliator"].ToString())
                };

                return config;
            }
        }
    }
}
