using ConsoleApp4.Models;
using Dapper;
using Npgsql;
using System.Data;

namespace ConsoleApp4.Data
{
    internal class AppDatabase
    {
        private string connectionString;

        public AppDatabase(string connectionString)
            => this.connectionString = connectionString;

        public async Task<bool> IsSchemaExists(string schema)
        {
            using (IDbConnection connection = new NpgsqlConnection(connectionString))
            {
                string sql = $"SELECT EXISTS (\r\n  \tSELECT schema_name \r\n\tFROM information_schema.schemata\r\n\tWhere schema_name = '{schema}'\r\n) AS table_exists";

                return await connection.ExecuteScalarAsync<bool>(sql);
            }
        }
        public async Task<bool> IsTableExists(string tableName, string schema)
        {
            using(IDbConnection connection = new NpgsqlConnection(connectionString))
            {
                string sql = $"SELECT EXISTS (\r\n  SELECT *\r\n  " +
                             $"FROM INFORMATION_SCHEMA.TABLES \r\n  WHERE TABLE_SCHEMA = '{schema}'\r\n  AND TABLE_NAME = '{tableName}'\r\n) " +
                             $"AS table_exists";

                return await connection.ExecuteScalarAsync<bool>(sql);
            }
        }
        public async Task<bool> IsChartParameters(string schema)
        {
            using (IDbConnection connection = new NpgsqlConnection(connectionString))
            {
                string sql = $"SELECT EXISTS (\r\n  SELECT *\r\n  " +
                             $"FROM INFORMATION_SCHEMA.TABLES \r\n  WHERE TABLE_SCHEMA = '{schema}'\r\n  AND TABLE_NAME = 'ChartParameters'\r\n) " +
                             $"AS table_exists";

                return await connection.ExecuteScalarAsync<bool>(sql);
            }
        }

        public async Task<bool> CreateSchema(string schema)
        {
            try
            {
                using (IDbConnection connection = new NpgsqlConnection(connectionString))
                {
                    string sql = $"CREATE SCHEMA \"{schema}\"";

                    await connection.QueryAsync(sql);
                }

                return true;
            }
            catch (Exception ex)
            {
                return false;
            }
        }
        public async Task<bool> CreateTable(string tableName, string schema)
        {
            try
            {
                using (IDbConnection connection = new NpgsqlConnection(connectionString))
                {
                    string sql = $"CREATE TABLE \"{schema}\".\"{tableName}\"(\r\n\tId SERIAL PRIMARY KEY,\r\n\tParameter DECIMAL,\r\n\t\"Time\" TIMESTAMP WITH TIME ZONE\r\n)";

                    await connection.QueryAsync(sql);
                }

                return true;
            }
            catch (Exception ex)
            {
                return false;
            }
        }
        public async Task<bool> CreateChartParameters(string schema, int lengthOfOscilliator)
        {
            try
            {
                using (IDbConnection connection = new NpgsqlConnection(connectionString))
                {
                    string sql = $"create table \"{schema}\".\"ChartParameters\" (\r\n\tid SERIAL PRIMARY KEY,\r\n\t\"Temperature\" bool,\r\n\t\"Humidity\" bool,\r\n\t\"Pressure\" bool,\r\n\t\"Dew point\" bool,\r\n\t\"Length\" int\r\n)";
                    await connection.QueryAsync(sql);

                    sql = $"insert into \"D5E0B863\".\"ChartParameters\" (\"Dew point\", \"Humidity\", \"Pressure\", \"Temperature\", \"Length\") values (true,true,true,true, {lengthOfOscilliator})";
                    await connection.QueryAsync(sql);
                }

                return true;
            }
            catch (Exception ex)
            {
                return false;
            }
        }

        public async Task<bool> AddParameter(DbModel model, string tableName, string schema, bool addId)
        {
            using (IDbConnection connection = new NpgsqlConnection(connectionString))
            {
                string sql = "";

                if(addId)
                    sql = $"Insert into \"{schema}\".\"{tableName}\" (\"id\", \"parameter\", \"Time\")\r\nValues ({model.id}, {model.Parameter.ToString().Replace(",", ".")}, '{model.Time.ToString("yyyy-MM-dd H:mm:ss")}')";
                else
                    sql = $"INSERT INTO \"{schema}\".\"{tableName}\" (\"parameter\", \"Time\") \r\nVALUES ('{model.Parameter.ToString().Replace(",", ".")}', '{model.Time.ToString("yyyy-MM-dd H:mm:ss")}')";

                await connection.QueryAsync(sql);
            }

            return true;
        }
        public async Task<IEnumerable<DbModel>> GetParameters(string tableName, string schema)
        {
            using (IDbConnection connection = new NpgsqlConnection(connectionString))
            {
                string sql = $"SELECT * FROM \"{schema}\".\"{tableName}\"";

                return await connection.QueryAsync<DbModel>(sql);
            }
        }
        public async Task<int> Count(string tableName, string schema)
        {
            using (IDbConnection connection = new NpgsqlConnection(connectionString))
            {
                string sql = $"SELECT COUNT(*) FROM \"{schema}\".\"{tableName}\"";

                return await connection.ExecuteScalarAsync<int>(sql);
            }
        }
        public async Task<bool> ChangeChartParameters(string schema, string parameter, bool change)
        {
            try
            {
                using (IDbConnection connection = new NpgsqlConnection(connectionString))
                {
                    string sql = $"UPDATE \"{schema}\".\"ChartParameters\"\r\nSET \"{parameter}\" = {change}\r\nWhere id = 1";

                    await connection.QueryAsync(sql);
                }

                return true;
            }
            catch (Exception ex)
            {
                return false;
            }
        }
        public async Task<bool> ChangeChartParameters(string schema, int lenght)
        {
            try
            {
                using (IDbConnection connection = new NpgsqlConnection(connectionString))
                {
                    string sql = "SELECT \"Length\" FROM \"D5E0B863\".\"ChartParameters\"\r\nWhere id = 1";
                    int lastLength = await connection.ExecuteScalarAsync<int>(sql);

                    if (lastLength != lenght)
                    {
                        sql = $"UPDATE \"{schema}\".\"ChartParameters\"\r\nSET \"Length\" = {lenght}\r\nWhere id = 1";

                        await connection.QueryAsync(sql);
                    }
                }

                return true;
            }
            catch (Exception ex)
            {
                return false;
            }
        }
        public async Task<bool> TruncateTable(string tableName, string schema)
        {
            try
            {
                using (IDbConnection connection = new NpgsqlConnection(connectionString))
                {
                    string sql = $"truncate \"{schema}\".\"{tableName}\"";

                    await connection.ExecuteScalarAsync(sql);
                }

                return true;
            }
            catch (Exception ex)
            {
                return false;
            }
        }
    }
}
