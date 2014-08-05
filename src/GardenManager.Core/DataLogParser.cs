using System;
using System.Collections.Generic;
using System.Text;

namespace GardenManager.Core
{
	public class DataLogParser
	{
		public int MaxPoints = 1000;
		public int TotalPoints = 0;

		public DataLogParser ()
		{
		}
		
		public Dictionary<string, Dictionary<string, int>> GetValues(string data, params string[] labels)
		{
			var dict = new Dictionary<string, Dictionary<string, int>> ();

			foreach (var label in labels) {
				dict.Add (label, GetValues (data, label));
			}

			return dict;
		}

		public Dictionary<string, int> GetValues(string data, string label)
		{
			var lines = data.Split (Environment.NewLine.ToCharArray(), StringSplitOptions.None);

			return GetValues (lines, label);
		}		

		public Dictionary<string, int> GetValues(string[] dataLines, string label)
		{
			var dict = new Dictionary<string, int> ();

			string fullLabel = label + ": ";

			int interval = 1;

			// If there are more lines of data than set in MaxPoints
			if (dataLines.Length > MaxPoints)
				// Determine an interval that will show the correct number of points
				interval = dataLines.Length / MaxPoints;

			// Define a counter for the current line
			int i = 0;

			foreach (var line in dataLines) {
				if (!String.IsNullOrEmpty(line.Trim()))
				{
					// Split the line into parts using ; character
					var parts = line.Split (';');

					// If the current line starts with "Data" then parse it
					if (parts.Length > 0
						&& !String.IsNullOrEmpty(parts[0])
						&& parts [0].StartsWith ("Data")) {
						// Increment the counter
						i++;

						var dateTimeString = GetDateTimeString (parts);

						if (!String.IsNullOrEmpty(dateTimeString.Trim()))
						{
							// If the current line count matches the interval (ie. the current line count
							// is a factor of the interval with no remainder)
							if ((i % interval) == 0) { 
								// Loop through each part of the line
								foreach (var part in parts) {
									// Remove whitespace from the current part
									var fixedPart = part.Trim ();
									if (fixedPart.StartsWith (fullLabel)) {
										// Parse the value as an int
										var value = Convert.ToInt32 (
											fixedPart.Replace (fullLabel, "").Trim ()
										);
		
										// Add the value to the list
										if (!dict.ContainsKey (dateTimeString))
											dict.Add (dateTimeString, value);
									}
								}
							}
						}
					}
				}
			}

			TotalPoints = i;

			return dict;
		}

		public string GetDateTimeString(string[] lineParts)
		{
			var output = String.Empty;

			foreach (var part in lineParts) {
				if (part.Trim().StartsWith ("DateTime"))
					output = part.Replace ("DateTime: ", "").Trim ();
			}

			return output;
		}
	}
}

