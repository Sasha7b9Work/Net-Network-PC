using Dapper;
using Npgsql;
using System.Data;
using Web_Service_Parameters.Models;

namespace Web_Service_Parameters.Data
{
    public class AppDbContext: IAppDatabase
    {
        private readonly IConfiguration _configuration;
        private readonly string _connection;

        public AppDbContext(IConfiguration configuration)
        {
            _configuration = configuration;
            _connection = _configuration.GetConnectionString("DbConnectionString");
        }

        public async Task<IEnumerable<string>> GetSchema()
        {
            using (IDbConnection connection = new NpgsqlConnection(_connection))
            {
                string sql = $"SELECT table_schema FROM INFORMATION_SCHEMA.TABLES ";

                return await connection.QueryAsync<string>(sql);
            }
        }

        public async Task<IEnumerable<ParameterModel>> GetParameter(string tableName, string schema)
        {
            using (IDbConnection connection = new NpgsqlConnection(_connection))
            {
                string sql = $"SELECT * FROM \"{schema}\".\"{tableName}\"";

                return await connection.QueryAsync<ParameterModel>(sql);
            }
        }

        public async Task<bool> GetChartParameters(string schema, string parameter)
        {
            using (IDbConnection connection = new NpgsqlConnection(_connection))
            {
                string sql = $"SELECT \"{parameter}\" FROM \"{schema}\".\"ChartParameters\"";

                return await connection.ExecuteScalarAsync<bool>(sql);
            }
        }

        public async Task<int> Count(string schema)
        {
            using (IDbConnection connection = new NpgsqlConnection(_connection))
            {
                string sql = $"SELECT \"Length\" FROM \"{schema}\".\"ChartParameters\"\r\nWhere id = 1";

                return await connection.ExecuteScalarAsync<int>(sql);
            }
        }
    }
}
