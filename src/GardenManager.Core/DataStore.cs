using System;
using System.Collections.Generic;
using Sider;
using System.Text;
using System.Configuration;

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

		public string GetValue(DeviceId id, int sensorNumber)
		{
			var client = new RedisClient();
			var fullKey = Prefix + "-Device-" + id.ToString () + "-Sensor-" + sensorNumber;
			return client.Get (fullKey);
		}

		public Dictionary<string, double> GetValues(DeviceId id, int sensorNumber)
		{
			var stringValue = GetValue (id, sensorNumber);

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

		public Dictionary<string, double> GetValues(DeviceId id, int sensorNumber, DateTime startTime, DateTime endTime)
		{
			var data = GetValues (id, sensorNumber);

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

		public void AddData(DeviceId id, int sensorNumber, DateTime dateTime, double value)
		{
			if (!SensorNumberExists (id, sensorNumber))
				AddSensorNumber (id, sensorNumber);

			var fullKey = Prefix + "-Device-" + id.ToString () + "-Sensor-" + sensorNumber;

			var fullLine = dateTime + "," + value + ";\n";

			var client = new RedisClient (); // TODO: Store this client for reuse
			client.Append (fullKey, fullLine);
		}

		// TODO: Remove if not neede
		/*public void AddData(DeviceId id, string dateTime, Dictionary<string, string> data)
		{

			foreach (var key in data.Keys) {
				AddData (id, dateTime, data [key]);
			}
		}*/

		public void AddSensorNumber(DeviceId id, int sensorNumber)
		{
			if (!SensorNumberExists (id, sensorNumber)) {
				var client = new RedisClient ();
				client.Append (Prefix + "-Device-" + id.ToString () + "-SensorNumbers", sensorNumber + ";\n");
			}
		}

		public bool SensorNumberExists(DeviceId id, int sensorNumber)
		{
			var keys = new List<int> (GetSensorNumbers (id));
			return keys.Contains (sensorNumber);
		}

		public int[] GetSensorNumbers(DeviceId id)
		{

			var client = new RedisClient();
			var sensorNumbersString = client.Get (Prefix + "-Device-" + id.ToString() + "-SensorNumbers");
			var sensorNumbers = new List<int> ();

			if (!String.IsNullOrEmpty (sensorNumbersString)) {
				foreach (var sensorNumber in sensorNumbersString.Trim().Trim(';').Split(';')) {
					sensorNumbers.Add (Convert.ToInt32(sensorNumber));
				}
			}

			return sensorNumbers.ToArray ();
		}

		public double GetLatestValue(DeviceId id, int sensorNumber)
		{
			var values = GetValues (id, sensorNumber);

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
				var sensorNumbers = GetSensorNumbers (deviceId);

				foreach (var sensorNumber in sensorNumbers) {
					DeleteData (deviceId, sensorNumber);
				}

				DeleteKeys (deviceId);
			}

			DeleteDevices ();
		}

		public void DeleteData(DeviceId id, int sensorNumber)
		{
			var client = new RedisClient ();
			client.Del (Prefix + "-Device-" + id.ToString() + "-Sensor-" + sensorNumber);
		}

		public void DeleteKeys(DeviceId id)
		{
			var client = new RedisClient ();
			client.Del (Prefix + "-Device-" + id.ToString() + "-SensorNumbers");
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

