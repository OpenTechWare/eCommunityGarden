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
			var keys = new Hashtable ();
			keys.Add ("Tmp", "Temperature");
			//keys.Add ("Hm", "Humidity");
			keys.Add ("Lt", "Light");
			//keys.Add ("Fl", "Flow");
			keys.Add ("Mst", "SoilMoisture");

			var file = Path.GetFullPath ("serialLog.txt");

			var data = File.ReadAllText (file);
			File.WriteAllText (file, "");

			ConvertStringToData (data);

			//var parser = new DataLogParser ();

			//var values = parser.Parse ();//.GetValues (data, keys.Keys.Cast<string>().ToArray());

			/*var store = new DataStore ();

			foreach (var key in keys.Keys) {
				var d = values [key.ToString()];

				store.Add (key.ToString(), d);
			}*/

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

			//var dateTime = GetDateTime (parts);

			//var id = DeviceId.Parse(GetID (parts));

			var values = GetValues (parts);

			var dateTime = values ["T"];

			var id = DeviceId.Parse (values ["Id"]);

			// Remove the ID and time stamp from the data collection 
			values.Remove ("T");
			values.Remove ("Id");

			if (!Store.DeviceExists(id))
				Store.AddDeviceId (id);

			Store.AddData (id, dateTime, values);
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

						var valueString = trimmedPart.Replace (key + ":", "");//part.Substring (key.Length, part.Length - key.Length);

						var value = valueString;

						data.Add (key, value);
				}
			}

			return data;
		}

		public bool IsValidPart(string part)
		{
			return part.Contains(":");
			//part.StartsWith ("ID:") // ID
			//&& !part.StartsWith ("T:") // Time
			//&& 
		}
	}
}

