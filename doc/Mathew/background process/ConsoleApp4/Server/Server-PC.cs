using ConsoleApp4.Data;
using ConsoleApp4.Models;
using System.Net;
using System.Net.Sockets;
using System.Reflection;
using System.Text;
using System.Text.RegularExpressions;

namespace ConsoleApp4.Server
{
    internal class Server_PC
    {
        private IPAddress ipAddr { get; set; }
        private IPEndPoint ipEndPoint { get; set; }
        private int lengthOfOscilliator { get; set; }
        private AppDatabase db { get; set; }

        private TcpListener Listener { get; }

        public Server_PC(string ip, int port, 
            string connectionString, 
            int lengthOfOscilliator)
        {
            this.lengthOfOscilliator = lengthOfOscilliator;
            ipAddr = IPAddress.Parse(ip);
            ipEndPoint = new IPEndPoint(ipAddr, port);

            db = new AppDatabase(connectionString);

            Listener = new TcpListener(ipEndPoint);
            Listener.Start();
        }

        public async Task<IEnumerable<ParameterModel>> Start()
        {
            TcpClient client = await Listener.AcceptTcpClientAsync();

            using NetworkStream stream = client.GetStream();

            byte[] buffer = new byte[1024];
            int bytesRead = await stream.ReadAsync(buffer, 0, buffer.Length);
            string data = Encoding.UTF8.GetString(buffer, 0, bytesRead);

            List<ParameterModel> models = new List<ParameterModel>();
            List<string> json_arr = data.Split("\n").ToList();
            json_arr = json_arr.Where(x => Regex.IsMatch(x, "{.+}")).ToList();

            foreach (string json_str in json_arr)
            {
                string id = Regex.Match(json_str.Split(',')[0], @"(?<=:"").+(?="")").Value;
                string nameOfParameter = Regex.Match(json_str.Split(',')[1], @"(?<="")\D+(?="":)").Value;
                string parametr = Regex.Match(json_str.Split(',')[1], @"(?<=:"").+(?="")").Value;
                string time = Regex.Match(json_str.Split(',')[2], @"(?<=:"").+(?="")").Value;

                ParameterModel model = new ParameterModel()
                {
                    Schema = id,
                    TableName = nameOfParameter,
                    Parametr = Decimal.Parse(parametr.Replace(".", ",")),
                    Time = DateTime.Parse(time)
                };

                models.Add(model);

                Console.WriteLine(json_str);
            }

            client.Close();

            return models;
        }

        public async Task Add(ParameterModel model)
        {
            if(!Regex.IsMatch(model.Schema, @"F+"))
            {
                bool isSchema = await db.IsSchemaExists(model.Schema);

                if (isSchema)
                {
                    bool isTable = await db.IsTableExists(model.TableName, model.Schema);
                    bool isChartParameters = await db.IsChartParameters(model.Schema);

                    if (!isChartParameters)
                        await db.CreateChartParameters(model.Schema, lengthOfOscilliator);

                    if (isTable)
                    {
                        int count = await db.Count(model.TableName, model.Schema);

                        if (count < lengthOfOscilliator)
                        {
                            await db.ChangeChartParameters(model.Schema, model.TableName, true);
                            await AddParametrToDb(model);
                        }
                        else
                        {
                            await db.ChangeChartParameters(model.Schema, model.TableName, false);
                            await MovingForwardParameters(model);
                        }
                    }
                    else
                    {
                        await db.CreateTable(model.TableName, model.Schema);
                        await Add(model);
                    }
                }
                else
                {
                    await db.CreateSchema(model.Schema);
                    await Add(model);
                }
            }
        }

        private async Task AddParametrToDb(ParameterModel model)
        {
            DbModel dbModel = new DbModel { Parameter = model.Parametr, Time = model.Time };

            await db.AddParameter(dbModel, model.TableName, model.Schema, false);
        }

        private async Task MovingForwardParameters(ParameterModel model)
        {
            IEnumerable<DbModel> ie = await db
                .GetParameters(model.TableName, model.Schema);
            List<DbModel> list = ie.ToList();
            List<DbModel> result = new List<DbModel>();

            foreach (DbModel dbm in list)
            {
                if (list.IndexOf(dbm) == 0)
                {
                    int lastId = Int32.Parse(list[list.Count - 1].id.ToString());
                    result.Add(new DbModel { id = lastId, Parameter = model.Parametr, Time = model.Time });
                }
                else
                    result.Add(list[list.IndexOf(dbm) - 1]);
            }

            await db.TruncateTable(model.TableName, model.Schema);

            for(int i = 0; i < lengthOfOscilliator; i++)
                await db.AddParameter(result[i], model.TableName, model.Schema, true);

        }
    }
}

