using System;
using System.Collections.Generic;
using Sider;
using System.Text;

namespace GardenManager.Core
{
	public class DataStore
	{
		public string Prefix = "eCG";

		public DataStore ()
		{
		}

		public DataStore (string prefix)
		{
			Prefix = prefix;
		}


		/*public void Add(Dictionary<string, Dictionary<string, double>> data)
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
		}*/

		public string GetValue(DeviceId id, string key)
		{
			var client = new RedisClient();
			var fullKey = Prefix + "-Device-" + id.ToString () + "-" + key;
			return client.Get (fullKey);
		}

		public Dictionary<string, double> GetValues(DeviceId id, string key)
		{
			var stringValue = GetValue (id, key);

			var lines = stringValue.Trim().Trim(';').Split (';');

			var data = new Dictionary<string, double> ();

			foreach (var line in lines) {
				var parts = line.Split (',');

				if (parts.Length > 1) {
					var date = parts [0];
					var value = Convert.ToDouble (parts [1]);

					if (!data.ContainsKey (date))
						data.Add (date, value);
				} else
					throw new Exception (line);
			}

			return data;
		}

		public Dictionary<string, double> GetValues(DeviceId id, string key, DateTime startTime, DateTime endTime)
		{
			var data = GetValues (id, key);

			var keptData = new Dictionary<string, double>();

			foreach (var timeKey in data.Keys)
			{
				DateTime keyTime = DateTime.MinValue;

				if (DateTime.TryParse (timeKey, out keyTime)) {
					if (keyTime > startTime
						&& keyTime < endTime
						&& !keptData.ContainsKey(timeKey)) {
						keptData.Add (timeKey, data [timeKey]);
					}
				} else
					Console.WriteLine ("Invalid date time format: " + timeKey);
			}

			return keptData;
		}

		public DeviceId[] GetDeviceIds()
		{

			var client = new RedisClient();
			var devicesString = client.Get (Prefix + "-Devices");
			var deviceIds = new List<DeviceId> ();

			if (!String.IsNullOrEmpty (devicesString)) {
				foreach (var deviceString in devicesString.Trim().Trim(';').Split(';')) {
					deviceIds.Add (DeviceId.Parse (deviceString.Trim()));
				}
			}

			return deviceIds.ToArray ();
		}

		public void AddDeviceId(DeviceId id)
		{
			if (!DeviceExists (id)) {
				var client = new RedisClient ();
				client.Append (Prefix + "-Devices", id.ToString () + ";\n");
			}
		}

		public bool DeviceExists(DeviceId id)
		{

			var devices = new List<DeviceId> (GetDeviceIds ());
			return devices.Contains (id);
		}

		public void AddData(DeviceId id, string dateTime, Dictionary<string, string> data)
		{
			var client = new RedisClient ();

			foreach (var key in data.Keys) {

				if (!DataKeyExists (id, key))
					AddDataKey (id, key);

				var fullKey = Prefix + "-Device-" + id.ToString () + "-" + key;

				var value = data [key];

				var fullLine = dateTime + "," + value + ";\n";

				client.Append (fullKey, fullLine);
			}
		}

		public void AddDataKey(DeviceId id, string key)
		{
			if (!DataKeyExists (id, key)) {
				var client = new RedisClient ();
				client.Append (Prefix + "-Device-" + id.ToString () + "-Keys", key + ";\n");
			}
		}

		public bool DataKeyExists(DeviceId id, string key)
		{
			var keys = new List<string> (GetDataKeys (id));
			return keys.Contains (key);
		}

		public string[] GetDataKeys(DeviceId id)
		{

			var client = new RedisClient();
			var keysString = client.Get (Prefix + "-Device-" + id.ToString() + "-Keys");
			var keys = new List<string> ();

			if (!String.IsNullOrEmpty (keysString)) {
				foreach (var key in keysString.Trim().Trim(';').Split(';')) {
					keys.Add (key.Trim());
				}
			}

			return keys.ToArray ();
		}

		public double GetLatestValue(DeviceId id, string key)
		{
			var values = GetValues (id, key);

			var latestValue = 0.0;

			// TODO: Make this more efficient
			foreach (var entry in values) {
				latestValue = entry.Value;
			}

			return latestValue;
		}

		public void DeleteAll()
		{
			var deviceIds = GetDeviceIds ();

			foreach (var deviceId in deviceIds) {
				var keys = GetDataKeys (deviceId);

				foreach (var key in keys) {
					DeleteData (deviceId, key);
				}

				DeleteKeys (deviceId);
			}

			DeleteDevices ();
		}

		public void DeleteData(DeviceId id, string key)
		{
			var client = new RedisClient ();
			client.Del (Prefix + "-Device-" + id.ToString() + "-" + key);
		}

		public void DeleteKeys(DeviceId id)
		{
			var client = new RedisClient ();
			client.Del (Prefix + "-Device-" + id.ToString() + "-Keys");
		}

		public void DeleteDevices()
		{
			var client = new RedisClient ();
			client.Del (Prefix + "-Devices");
		}

		public void SetDeviceName(DeviceId id, string name)
		{
			var client = new RedisClient ();
			var key = GetDeviceNameKey (id);
			client.Set (key, name);
		}

		public string GetDeviceName(DeviceId id)
		{
			var client = new RedisClient ();
			var key = GetDeviceNameKey (id);
			if (!client.Exists (key))
				return String.Empty;
			return client.Get (key);
		}

		public string GetDeviceNameKey(DeviceId id)
		{
			var key = Prefix + "-" + id.ToString () + "-Name";
			return key;
		}
	}
}

