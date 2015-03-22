using System;
using System.Collections.Generic;
using Sider;
using System.Text;

namespace GardenManager.Core
{
	public class DataStore
	{
		public DataStore ()
		{
		}


		public void Add(Dictionary<string, Dictionary<string, double>> data)
		{
			foreach (var key in data.Keys) {
				Add (key, data [key]);
			}
		}

		public void Add(string key, Dictionary<string, double> data)
		{
			var dataStringBuilder = new StringBuilder ();

			foreach (var k in data.Keys)
			{
				dataStringBuilder.Append(String.Format("{0},{1};\n", k, data[k]));
			}

			var client = new RedisClient();
			client.Append(key, dataStringBuilder.ToString());
		}

		public string GetValue(string key)
		{
			var client = new RedisClient();
			return client.Get (key);
		}

		public Dictionary<string, double> GetValues(string key)
		{
			var stringValue = GetValue (key);

			var lines = stringValue.Trim().Split ('\n');

			var data = new Dictionary<string, double> ();

			foreach (var line in lines) {
				var parts = line.Trim ().TrimEnd (';').Split (',');

				var date = parts [0];
				var value = Convert.ToDouble(parts[1]);

				data.Add(date, value);
			}

			return data;
		}
	}
}

