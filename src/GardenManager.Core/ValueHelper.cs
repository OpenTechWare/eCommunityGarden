using System;

namespace GardenManager.Core
{
	public class ValueHelper
	{
		public ValueHelper ()
		{
		}

		public static string GetValue(string key, double value)
		{
			var postfix = GetPostFix(key);

			return value + postfix;
		}

		public static string GetPostFix(string key)
		{
			switch (key) {
			case "Tmp":
				return "c";
			case "Mst":
			case "Hm":
			case "Lt":
				return "%";
			}

			return String.Empty;
		}
	}
}

