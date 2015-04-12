using System;
using System.Text;
using GardenManager.Core;
using System.Collections.Generic;
using System.Web;

namespace GardenManager.Web.UI
{
	public class Grapher
	{
		public DataStore Store = new DataStore ();

		public int Height = 300;
		public int Width = 300;
		public int ScaleSteps = 10;
		public int ScaleStepDivider = 10;
		public bool ScaleShowLabels = true;

		public DateTime StartTime = DateTime.MinValue;
		public DateTime EndTime = DateTime.MaxValue;

		public Grapher()
		{
		}

		public string GetGraphScript(DeviceId id, int sensorNumber)
		{
			return GetGraphScript (id, sensorNumber, 0);
		}

		public string GetGraphScript(DeviceId id, int sensorNumber, int points)
		{
			var color = "lightblue";

			var canvasId = "dev" + id.ToString ().Replace (".", "_") + "_S" + sensorNumber;
			var dataId = "dev" + id.ToString ().Replace (".", "_") + "_S" + sensorNumber + "Data";

			color = SensorConfig.GetColor (sensorNumber);

			var data = GetLatestData(id, sensorNumber, points);

			int i = 0;
			var builder = new StringBuilder();
			builder.AppendLine(
				@"<canvas id=""" + canvasId + @""" width=""" + Width + @""" height=""" + Height + @""" style=""float:right""></canvas>
					<script>
					var " + dataId + @" = {
						labels : ["
			);

			foreach (var t in data.Keys)
			{
				builder.Append("\"");
				builder.Append("");
				//builder.Append(DateTime.Parse(t).ToShortTimeString());
				builder.Append("\"");
				if (i < data.Count-1)
					builder.Append(",");

				i++;
			}

			builder.Append(@"],
	datasets : [
              	{
                  fillColor : """ + color + @""",
                  strokeColor : """ + color + @""",
                  ");

			builder.Append("data : [");
			i=0;
			foreach (var t in data.Keys)
			{
				builder.Append(data[t]);
				if (i < data.Count-1)
					builder.Append(",");

				i++;
			}
			builder.Append(@"]
              }
          ]
      }

      var options = {      
        scaleOverride: true,
        scaleSteps: " + ScaleSteps + @",
        scaleStepWidth: Math.ceil(100 / " + ScaleStepDivider + @"),
        scaleStartValue: 0,
		scaleShowLabels: " + ScaleShowLabels.ToString().ToLower() + @",
		animation: false
      };

      // get bar chart canvas
      var " + canvasId + @" = document.getElementById(""" + canvasId + @""").getContext(""2d"");
      // draw bar chart
      new Chart(" + canvasId + @").Line(" + dataId + @", options);");

			builder.AppendLine("<" + "/script>");

			return builder.ToString();
		}

		public Dictionary<string, double> GetLatestData(DeviceId id, int sensorNumber, int points)
		{
			var selectedData = Store.GetValues(id, sensorNumber, StartTime, EndTime);

			var latestData = new Dictionary<string, double>();

			int i = selectedData.Keys.Count;
			if (selectedData.Count > 0)
			{
				foreach (var t in selectedData.Keys)
				{
					if (points == 0
						|| i <= points)
					{
						latestData.Add(t, selectedData[t]);
					}

					i--;
				}
			}
			return latestData;
		}
	}
}

