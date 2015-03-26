using System;
using System.Configuration;

namespace GardenManager.Core
{
	public class SensorConfig
	{
		public SensorConfig ()
		{
		}

		public static string GetValueText(int sensorNumber, double value)
		{
			var postfix = GetPostfix(sensorNumber);

			return value + postfix;
		}

		public static string GetName(int sensorNumber)
		{
			return ConfigurationSettings.AppSettings ["Sensor." + sensorNumber];
		}

		public static string GetColor(int sensorNumber)
		{
			var value = ConfigurationSettings.AppSettings ["Sensor." + sensorNumber + ".Color"];

			if (String.IsNullOrEmpty (value))
				return ConfigurationSettings.AppSettings ["Sensor.Default.Color"];
			else
				return value; 
		}

		public static string GetPostfix(int sensorNumber)
		{
			var value = ConfigurationSettings.AppSettings ["Sensor." + sensorNumber + ".Postfix"];

			return value; 
		}
	}
}

