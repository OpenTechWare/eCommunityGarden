using System;
using System.IO;
using System.Collections;
using System.Linq;

namespace GardenManager.Core
{
	public class DataConversion
	{
		public DataConversion ()
		{
		}

		public void ConvertFileToData()
		{
			var keys = new Hashtable ();
			keys.Add ("Tmp", "Temperature");
			keys.Add ("Hm", "Humidity");
			keys.Add ("Lt", "Light");
			keys.Add ("Fl", "Flow");
			keys.Add ("Mst", "SoilMoisture");

			var file = Path.GetFullPath ("serialLog.txt");

			var data = File.ReadAllText (file);
			File.WriteAllText (file, "");

			var parser = new DataLogParser ();

			var values = parser.GetValues (data, keys.Keys.Cast<string>().ToArray());

			var store = new DataStore ();

			foreach (var key in keys.Keys) {
				var d = values [key.ToString()];

				store.Add (key.ToString(), d);
			}

		}
	}
}

