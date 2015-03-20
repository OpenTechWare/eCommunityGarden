using System;
using System.Collections.Generic;
using System.Text;
namespace GardenManager.Core
{
	public class DataLogParser
	{
		public int MaxPoints = 1000;
		public int TotalPoints = 0;
		public int IncludeLatestDataPoints = 5;

		public DataLogParser ()
		{
		}
		public Dictionary<string, Dictionary<string, double>> GetValues(string data, params string[] keys)
		{
			var dict = new Dictionary<string, Dictionary<string, double>> ();
			foreach (var key in keys) {
				dict.Add (key, GetValues (data, key));
			}
			return dict;
		}

		/*public Dictionary<string, Dictionary<string, double>> GetLatestValues(int positions, string data, params string[] keys)
		{
			var dict = new Dictionary<string, Dictionary<string, double>> ();
			foreach (var key in keys) {
				dict.Add (key, GetValues (data, key));
			}
			return dict;
		}*/
		public Dictionary<string, double> GetValues(string data, string key)
		{
			var lines = data.Split (Environment.NewLine.ToCharArray(), StringSplitOptions.None);
			return GetValues (lines, key);
		}
		public Dictionary<string, double> GetValues(string[] dataLines, string key)
		{
			// TODO: Reorganize this function and break up the mess of code into smaller bits/functions
			var dict = new Dictionary<string, double> ();
			string fullKey = key + ":";
			int interval = 1;
			// If there are more lines of data than set in MaxPoints
			if (dataLines.Length > MaxPoints)
				// Determine an interval that will show the correct number of points
				interval = dataLines.Length / MaxPoints;

			var validLines = GetValidLines(dataLines);


			for (int i = 0; i < validLines.Length; i++)
			{
				var line = validLines[i];

				var parts = line.Split (';');
				var dateTimeString = GetDateTimeString (parts);
				if (!String.IsNullOrEmpty (dateTimeString.Trim ())) {
					// If the current line count matches the interval (ie. the current line count
					// is a factor of the interval with no remainder)
					var mod = (i % interval);
					if (
						mod == 0
				    	|| i >= validLines.Length - IncludeLatestDataPoints // Always include the latest 3 data points
					) {
						TotalPoints++;

						// Loop through each part of the line
						foreach (var part in parts) {
							// Remove whitespace from the current part
							var fixedPart = part.Trim ();
							if (fixedPart.StartsWith (fullKey)) {
								var stringValue = fixedPart.Replace (fullKey, "").Trim ();
								// Parse the value as an int
								var value = Convert.ToDouble (
									stringValue
								);
								var yearString = dateTimeString.Substring (0, dateTimeString.IndexOf ("-"));
								var year = Convert.ToInt32 (yearString);
								// If the year is less than 2150 then it's a valid date time string (if it's above that it's a false value indicating the RTC module isn't plugged in)
								bool isValidDate = year < 2150;
								// Create the key (date if it's valid, or index if the date is invalid)
								var newKey = isValidDate ? dateTimeString : i.ToString ();
								// Add the value to the list
								if (!dict.ContainsKey (newKey))
									dict.Add (newKey, value);
							}
						}
					}
				}

			}
			return dict;
		}
		public string GetDateTimeString(string[] lineParts)
		{
			var output = String.Empty;
			foreach (var part in lineParts) {
				if (part.Trim().StartsWith("T:"))
					output = part.Replace ("T:", "").Trim ();
			}
			return output;
		}

		public string[] GetValidLines(string[] lines)
		{
			var list = new List<string> ();
			
			foreach (var line in lines) {
				if (!String.IsNullOrEmpty (line.Trim ())) {
					if (line.StartsWith ("D;")) {
						// Split the line into parts using ; character
						var parts = line.Split (';');
						// If the current line starts with "Data" then parse it
						if (parts.Length > 0
						    && !String.IsNullOrEmpty (parts [0])
						    && parts [0] == "D") {
			
							list.Add (line);
						}
					}
				}
			}

			return list.ToArray ();
		}
	}
}