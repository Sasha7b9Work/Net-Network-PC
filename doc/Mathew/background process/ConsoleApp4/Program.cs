using ConsoleApp4;
using ConsoleApp4.Models;
using ConsoleApp4.Server;

Config config = new Config();
Server_PC server = new Server_PC(config.Ip, config.Port, config.ConnectionString, config.lengthOfOscilliator);

while (true)
{
    IEnumerable<ParameterModel> model = await server.Start();

    foreach (var parameter in model)
        await server.Add(parameter);
}