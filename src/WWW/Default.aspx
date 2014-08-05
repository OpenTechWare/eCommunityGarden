<%@ Page Language="C#" %>
<%@ Import Namespace="System.Text" %>
<%@ Import Namespace="System.IO" %>
<%@ Import Namespace="GardenManager.Core" %>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html>
	<head runat="server">
		<title>Garden Monitor - Charts</title>
		<script runat="server">
		
			int maxPoints = 100;
			int autoRefreshMinutes = 1;
			int autoRefreshSeconds = 0;

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

				if (!IsPostBack)
					DataBind();
			}

			void RefreshButton_Click(object sender, EventArgs e)
			{
                autoRefreshMinutes = Convert.ToInt32(RefreshMinutesBox.Text);
                autoRefreshSeconds = Convert.ToInt32(RefreshSecondsBox.Text);
                maxPoints = Convert.ToInt32(MaxPointsBox.Text);
			}
		
			string GetDataValues(string label)
			{
				var builder = new StringBuilder();
			
				builder.Append("type: \"line\",");
				builder.Append("xValueType: \"dateTime\",");
				builder.Append("dataPoints: [");
				
				var parser = new DataLogParser();

				parser.MaxPoints = maxPoints;
				
				int i = 0;
				
				var values = parser.GetValues(data, label);
				
				foreach (var pair in values)
				{
					i++;
					
					var dateTime = DateTime.Parse(pair.Key);
					
					var jsTimeStamp = dateTime.Subtract(
						new DateTime(1970,1,1,0,0,0))
               			.TotalMilliseconds;
               		
               		//var jsTimeStamp = DateTime.Now.Subtract(new DateTime(1970, 1,1)).TotalMilliseconds;
					
					builder.Append("{ "); 
					builder.Append(String.Format("x: {0}, y: {1}", jsTimeStamp, pair.Value));
					builder.Append("}");
					if (i < values.Count)
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
            		valueFormatString: ""DD-MMM HH:mm:ss"",
			        interval:" + 2  + @",
            		labelAngle: -70,
			        intervalType: ""minute""
			      },
			      axisY:{
			        includeZero: true
			        
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

			string GetRefreshScript()
			{
				var scr = @"
	                // Auto Refresh Page with Time script
	    	        // By JavaScript Kit (javascriptkit.com)
	            	// Over 200+ free scripts here

	                //enter refresh time in ""minutes:seconds"" Minutes should range from 0 to inifinity. Seconds should range from 0 to 59
	    	        var limit='" + autoRefreshMinutes + ":" + autoRefreshSeconds + @"';

	                if (document.images){
	    	                var parselimit=limit.split("":"")
	            	        parselimit=parselimit[0]*60+parselimit[1]*1
	            }

	                function beginrefresh(){
	   	        	        if (!document.images)
	           		                return
	                        if (parselimit==1)
	                                document.getElementById('RefreshButton').click();
	    	                else{
	           	                	parselimit-=1
	                   		        curmin=Math.floor(parselimit/60)
	                    	        cursec=parselimit%60
	            	                if (curmin!=0)
	    	                                curtime=curmin+"" minutes and ""+cursec+"" seconds left until page refresh!""
	                                else
	                                        curtime=cursec+"" seconds left until page refresh!""
	                    	        window.status=curtime
	            	                setTimeout(""beginrefresh()"",1000)
	    	                }
	                }";

				return scr;
			}
		</script>
        <script src="js/canvasjs/canvasjs.min.js"></script>
        <script>
            <% if (AutoRefreshCheckBox.Checked) { %>
            	<%= GetRefreshScript() %>
            <% } %>

			 window.onload = function () {
			  
			  	<%= GetChartScript("Temperature", "temperatureContainer") %>
			  	<%= GetChartScript("Humidity", "humidityContainer") %>
			  	<%= GetChartScript("Moisture", "moistureContainer") %>
			  	<%= GetChartScript("Light", "lightContainer") %>

				<% if (AutoRefreshCheckBox.Checked) { %>
					beginrefresh();
				<% } %>
			}


        </script>
	<style>
	body
	{
		font-family: Verdana;
	}

	.Desc
	{
		font-size: 10px;
	}
	</style>
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
		<div><b>Settings:</b></div>
		<p>
			Maximum points:<br/>
			<span class="Desc">(The maximum number of points displayed on each graph. Choose fewer points to make the page load faster. Choose a greater number for more detail.)</span><br/>
			<asp:TextBox runat="server" id="MaxPointsBox" text='<%# maxPoints %>'></asp:TextBox><br/>
		</p>
		<p>
			Auto refresh: <asp:CheckBox runat="server" id="AutoRefreshCheckBox" Checked="true" onclientclick="document.getElementById('RefreshButton').click();" /><br/>
			[minutes:seconds]:
			 <asp:TextBox runat="server" id="RefreshMinutesBox" Text='<%# autoRefreshMinutes %>' width="30"></asp:TextBox>
			 :<asp:TextBox runat="server" id="RefreshSecondsBox" Text='<%# autoRefreshSeconds %>' width="30"></asp:TextBox>
		</p>
		<p>
			<asp:button runat="server" id="RefreshButton" text="Refresh" onclick="RefreshButton_Click" />
		</p>
		</form>
	</body>
</html>
