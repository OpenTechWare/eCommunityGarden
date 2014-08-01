<%@ Page Language="C#" %>
<%@ Import Namespace="System.Text" %>
<%@ Import Namespace="System.IO" %>
<%@ Import Namespace="GardenManager.Core" %>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html>
	<head runat="server">
		<title>Garden Monitor - Charts</title>
		<script runat="server">
		
			int[] temperatures = new int[]{};
			int[] humidity = new int[]{};
			int[] light = new int[]{};
			int[] moisture = new int[]{};
			
			string data = "";

			void Page_Load(object sender, EventArgs e)
			{				
				var fileName = Server.MapPath("serialLog.txt");
				
				var lines = File.ReadAllText(fileName);
				
				data = lines;
			}
		
			string GetDataValues(string label)
			{
				var builder = new StringBuilder();
			
				builder.Append("type: \"line\",");
				builder.Append("dataPoints: [");
				
				var parser = new DataLogParser();
				
				int i = 0;
				
				var values = parser.GetValues(data, label);
				
				foreach (var temp in values)
				{
					i++;
					builder.Append("{ ");
					builder.Append(String.Format("x: {0}, y: {1}", i, temp));
					builder.Append("}");
					if (i < values.Length)
						builder.Append(",");
					builder.Append(Environment.NewLine);
				}
				
				builder.Append("]");
	            return builder.ToString();
			}
			
			string GetChartScript(string label, string containerId)
			{
				var scr = @"
			    var " + containerId + @"Chart = new CanvasJS.Chart(""" + containerId + @""",
			    {
			      theme: ""theme2"",
			      title:{
			        text: """ + label + @"""
			      },
			      axisX: {
			        valueFormatString: """",
			        interval:1000,
			        intervalType: ""month""
			        
			      },
			      axisY:{
			        includeZero: false
			        
			      },
			      data: [
			      {        
			        " + GetDataValues(label) + @"
			      }	 
			      ]
			    });
			    
				" + containerId + "Chart.render();";
				
				return scr;
			}
		</script>
        <script src="js/jQuery/jquery-2.1.1.min.js"></script>
        <script src="js/canvasjs/canvasjs.min.js"></script>
        <script>
			  window.onload = function () {
			  
			  	<%= GetChartScript("Temperature", "temperatureContainer") %>
			  	<%= GetChartScript("Humidity", "humidityContainer") %>
			  	<%= GetChartScript("Moisture", "moistureContainer") %>
			  	<%= GetChartScript("Light", "lightContainer") %>
			}
        </script>
	</head>
	<body>
		<form id="form1" runat="server">
		<table width="100%">
			<tr>
				<td>
					<div id="lightContainer" style="height: 300px; width: 100%;">
		  			</div>
		  		</td>
		  		<td>
		  			<div id="moistureContainer" style="height: 300px; width: 100%;">
		  			</div>
		  		</td>
			</tr>
			<tr>
				<td>
					<div id="temperatureContainer" style="height: 300px; width: 100%;">
		  			</div>
		  		</td>
		  		<td>
		  			<div id="humidityContainer" style="height: 300px; width: 100%;">
		  			</div>
		  		</td>
			</tr>
		</table>
		</form>
	</body>
</html>
