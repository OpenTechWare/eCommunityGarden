<%@ Page Language="C#" %>
<%@ Import Namespace="System.Collections.Generic" %>
<%@ Import Namespace="System.IO" %>
<%@ Import Namespace="GardenManager.Core" %>
<!DOCTYPE html>
<html>
<head>
	<link rel="stylesheet" type="text/css" href="style.css">
	<script runat="server">
			int maxPoints = 50;
			int totalPoints = 0;
			
			Dictionary<string, Dictionary<string, double>> data = new Dictionary<string, Dictionary<string, double>>();

			string sensorKey = "";
			string sensorTitle = "";

			void Page_Load(object sender, EventArgs e)
			{
				sensorKey = Request.QueryString["k"];
				sensorTitle = Request.QueryString["t"];

				var conversion = new DataConversion();
				conversion.ConvertFileToData();

				var store = new DataStore();

				data.Add(sensorKey, store.GetValues(sensorKey));
			}

			string getDataScript(string key, string id)
			{
				var data = getLatestData(key);

				var color = "lightblue";

				switch (key)
				{
					case "Tmp":
						color = "darkred";
						break;
					case "Lt":
						color = "orange";
						break;
					case "Hm":
						color = "green";
						break;
					case "Mst":
						color = "navy";
						break;
				}

				int i = 0;
				var builder = new StringBuilder();
				builder.AppendLine(
@"<canvas id=""" + id + @""" width=""800px"" height=""500px"" style=""margin: 5px;""></canvas>"
				);
				builder.AppendLine("<script>");
				builder.AppendLine("var " + id + @"Data = {");
          		builder.Append("	labels : [");
				foreach (var t in data.Keys)
				{
					builder.Append("\"");
          			builder.Append(DateTime.Parse(t).ToShortTimeString());
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
			pointColor : """ + color + @""",
			pointStrokeColor : """ + color + @""",
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
        scaleSteps: 10,
        scaleStepWidth: Math.ceil(100 / 10),
        scaleStartValue: 0
      };

      // get bar chart canvas
      var " + id + @" = document.getElementById(""" + id + @""").getContext(""2d"");
      // draw bar chart
      new Chart(" + id + @").Line(" + id + @"Data, options);");
      builder.AppendLine("<" + "/script>");

				return builder.ToString();
			}

			Dictionary<string, double> getLatestData(string key)
			{
				var selectedData = data[key];

				var latestData = new Dictionary<string, double>();

				int i = selectedData.Keys.Count;
				if (data.Count > 0)
				{
					foreach (var t in selectedData.Keys)
					{
						//if (i <= 3)
						//{
							latestData.Add(t, selectedData[t]);
						//}

						i--;
					}
				}
				return latestData;
			}
				
		double getLatestValue(string sensorKey)
		{
			double val = 0;

			foreach (var timeKey in data[sensorKey].Keys)
			{
				val = data[sensorKey][timeKey];
			}

			return val;
		}
	</script>
    <script type="text/javascript" src='Chart.min.js'></script>
    <script type="text/javascript" src='jquery-2.1.3.min.js'></script>
</head>
<body>
<div class="buttons">
<div class="bkpnl button" onclick="window.location.href='Index.aspx'">
  &laquo; Back
</div>
<div class="ref button" onclick="window.location.reload()">
  Refresh
</div>
</div>
<div class="grphpnl">
  <div class="hd"><%= sensorTitle %></div>
  <%= getDataScript(sensorKey, "graph") %>
</div>
</body>
</html>
