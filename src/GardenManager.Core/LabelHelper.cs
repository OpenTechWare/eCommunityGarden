using System;

namespace GardenManager.Core
{
	public class LabelHelper
	{
		public LabelHelper ()
		{
		}

		static public string GetLabel(string key)
		{
			switch (key) {
			case "Lt":
				return "Light";
			case "Mst":
				return "Soil Moisture";
			case "Tmp":
				return "Temperature";
			case "Hm":
				return "Humidity";
			}

			return "Unknown";
		}
	}
}

