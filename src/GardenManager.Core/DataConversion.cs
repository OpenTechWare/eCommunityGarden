using System;
using System.IO;
using System.Collections;
using System.Linq;
using System.Collections.Generic;

namespace GardenManager.Core
{
	public class DataConversion
	{
		public DataStore Store { get;set; }

		public DataConversion (DataStore store)
		{
			Store = store;
		}

		public void ConvertFileToData()
		{
			var file = Path.GetFullPath ("serialLog.txt");

			var data = File.ReadAllText (file);
			File.WriteAllText (file, "");

			ConvertStringToData (data);
		}

		public void ConvertStringToData(string dataString)
		{
			foreach (var line in dataString.Split('\n'))
			{
				// If the line starts with D; then it's a data line
				if (line.Trim().StartsWith ("D;")) {
					ConvertLineToData (line);
				}
			}
		}

		public void ConvertLineToData(string dataLine)
		{
			try
			{
				var parts = dataLine.Split (';');

				var values = GetValues (parts);

				var dateTime = DateTime.Parse(values ["T"]);

				var deviceId = DeviceId.Parse (values ["Id"]);

				if (deviceId.ToString() != "0.0.0")
				{
					var sensorNumber = Convert.ToInt32(values ["N"]);

					var sensorCode = Convert.ToInt32(values ["S"]);

					var value = Convert.ToDouble(values ["V"]);

					if (!Store.DeviceExists(deviceId))
						Store.AddDeviceId (deviceId);

					Store.AddData (deviceId, sensorNumber, sensorCode, dateTime, value);
				}
			}
			catch {
				Console.WriteLine ("Corrupt data line: " + dataLine);
			}
		}

		public string GetDateTime(string[] parts)
		{
			foreach (var part in parts) {
				if (part.StartsWith ("T:"))
					return part.Replace ("T:", "");
			}

			return String.Empty;
		}

		public string GetID(string[] parts)
		{
			foreach (var part in parts) {
				if (part.StartsWith ("ID:"))
					return part.Replace ("ID:", "");
			}

			return String.Empty;
		}

		public Dictionary<string, string> GetValues(string[] parts)
		{
			var data = new Dictionary<string, string> ();

			foreach (var part in parts) {
				var trimmedPart = part.Trim ();
				if (IsValidPart(trimmedPart)) {

					var key = trimmedPart.Substring (0, trimmedPart.IndexOf (":"));

						var valueString = trimmedPart.Replace (key + ":", "");

						var value = valueString;

						data.Add (key, value);
				}
			}

			return data;
		}

		public bool IsValidPart(string part)
		{
			return part.Contains(":");
		}
	}
}

