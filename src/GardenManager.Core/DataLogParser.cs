using System;
using System.Collections.Generic;
using System.Text;

namespace GardenManager.Core
{
	public class DataLogParser
	{
		public DataLogParser ()
		{
		}

		public int[] GetValues(string data, string label)
		{
			var lines = data.Split (Environment.NewLine.ToCharArray(), StringSplitOptions.None);

			return GetValues (lines, label);
		}		

		public int[] GetValues(string[] dataLines, string label)
		{
			var list = new List<int> ();

			string fullLabel = label + ": ";

			foreach (var line in dataLines) {
				var parts = line.Split (';');
				if (parts [0].StartsWith ("Data")) {
					foreach (var part in parts) {
						var fixedPart = part.Trim ();
						if (fixedPart.StartsWith (fullLabel)) {
							var value = 
							Convert.ToInt32 (
								fixedPart.Replace (fullLabel, "").Trim ()
							);

							list.Add (value);
						}
					}
				}
			}

			return list.ToArray ();
		}
	}
}

