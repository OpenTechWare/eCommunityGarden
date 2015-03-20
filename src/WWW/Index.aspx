<%@ Page Language="C#" %>
<%@ Import Namespace="System.Collections.Generic" %>
<%@ Import Namespace="System.IO" %>
<%@ Import Namespace="GardenManager.Core" %>
<!DOCTYPE html>
<html>
<head>
	<link rel="stylesheet" type="text/css" href="style.css">
	<script runat="server">
		
			int[] temperatures = new int[]{};
			int[] humidity = new int[]{};
			int[] light = new int[]{};
			int[] moisture = new int[]{};

			int maxPoints = 10000;
			int totalPoints = 0;
			
			Dictionary<string, Dictionary<string, double>> data = new Dictionary<string, Dictionary<string, double>>();

			void Page_Load(object sender, EventArgs e)
			{

				var fileName = Server.MapPath("serialLog.txt");
				
				var rawData = "";
				
				if (File.Exists(fileName))
					rawData = File.ReadAllText(fileName).Trim();

				var parser = new DataLogParser();
				parser.MaxPoints = maxPoints;
				
				data = parser.GetValues(rawData, "Tmp", "Hm", "Lt", "Mst", "Fl");

				totalPoints = parser.TotalPoints;
			}

			string getDataScript(string key, string id)
			{
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

				var data = getLatestData(key);

				int i = 0;
				var builder = new StringBuilder();
				builder.AppendLine(
@"<canvas id=""" + id + @""" width=""150"" height=""230"" style=""float:right""></canvas>"
				);
				builder.AppendLine("<script>");
				builder.AppendLine("var " + id + @"Data = {");
          		builder.Append("	labels : [");
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
						if (i <= 3)
						{
							latestData.Add(t, selectedData[t]);
						}

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
<script type="text/javascript">
$(document).ready(
  function() {
    var colorOrig=$('.pnl').css('background-color');
    $('.pnl').hover(
    function() {
        //mouse over
        $(this).css('background', 'lightgray');
    }, function() {
        //mouse out
        $(this).css('background', colorOrig);
    });
}); 
</script>
<div class="buttons">
	<div class="ref button" onclick="window.location.reload()">
  		Refresh
	</div>
</div>
	<div class="panels">
		<div class="pnl" onclick="window.location.href = 'Graph.aspx?k=Tmp&t=Temperature';">
		  <div class="hd">Temperature</div>
		  <%= getDataScript("Tmp", "temperature") %>
		  <div class="bdy">
		    <span class="lval"><%= getLatestValue("Tmp") %>c</span>
		  </div>
		</div>
		<div class="pnl" onclick="window.location.href = 'Graph.aspx?k=Hm&t=Humidity';">
		<div class="hd">Humidity</div>
		  <%= getDataScript("Hm", "humidity") %>
		  <div class="bdy"><span class="lval"><%= getLatestValue("Hm") %>%</span></div>
		</div>
		<div class="pnl" onclick="window.location.href = 'Graph.aspx?k=Lt&t=Light';">
		<div class="hd">Light</div>
		  <%= getDataScript("Lt", "light") %>
		  <div class="bdy"><span class="lval"><%= getLatestValue("Lt") %>%</span></div>
		</div>
		<div class="pnl" onclick="window.location.href = 'Graph.aspx?k=Mst&t=Soil+Moisture';">
		<div class="hd">Soil Moisture</div>
		  <%= getDataScript("Mst", "moisture") %>
		  <div class="bdy"><span class="lval"><%= getLatestValue("Mst") %>%</span></div>
		</div>
  </div>
</body>
</html>
