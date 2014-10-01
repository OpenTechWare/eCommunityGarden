<%@ Page Language="C#" %>
<%@ Import Namespace="System.Text" %>
<%@ Import Namespace="System.IO" %>
<%@ Import Namespace="System.Collections.Generic" %>
<%@ Import Namespace="GardenManager.Core" %>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html>
	<head runat="server">
		<title>Garden Monitor - Charts</title>
		<script runat="server">
		
			int maxPoints = 100;
			bool autoRefresh = true;
			int autoRefreshMinutes = 1;
			int autoRefreshSeconds = 0;
			int totalPoints = 0;

			int[] temperatures = new int[]{};
			int[] humidity = new int[]{};
			int[] light = new int[]{};
			int[] moisture = new int[]{};
			
			Dictionary<string, Dictionary<string, double>> data = new Dictionary<string, Dictionary<string, double>>();

			void Page_Load(object sender, EventArgs e)
			{				
				var fileName = Server.MapPath("serialLog.txt");
				
				var rawData = "";
				
				if (File.Exists(fileName))
					rawData = File.ReadAllText(fileName);
				
				
				if (!IsPostBack)
				{
					if (!String.IsNullOrEmpty(Request.QueryString["MaxPoints"]))
						maxPoints = Convert.ToInt32(Request.QueryString["MaxPoints"]);
						
					if (!String.IsNullOrEmpty(Request.QueryString["AutoRef"]))
						autoRefresh = Convert.ToBoolean(Request.QueryString["AutoRef"]);
						
					if (!String.IsNullOrEmpty(Request.QueryString["RefMin"]))
						autoRefreshMinutes = Convert.ToInt32(Request.QueryString["RefMin"]);
						
					if (!String.IsNullOrEmpty(Request.QueryString["RefSec"]))
						autoRefreshSeconds = Convert.ToInt32(Request.QueryString["RefSec"]);
					
					var parser = new DataLogParser();
					parser.MaxPoints = maxPoints;
					
					data = parser.GetValues(rawData, "Tmp", "Hm", "Lt", "Mst", "Fl");
					
					totalPoints = parser.TotalPoints;
					
					DataBind();
				}
			}

			void RefreshButton_Click(object sender, EventArgs e)
			{
				Refresh();
			}
			
			void CaptureButton_Click(object sender, EventArgs e)
			{
				new CaptureStarter().Start();
				
				Application["IsCapturing"] = true;
			
				Refresh();
			}
			
			void Refresh()
			{
			
				autoRefresh = AutoRefreshCheckBox.Checked;
				
				if (!String.IsNullOrEmpty(RefreshMinutesBox.Text))
                	autoRefreshMinutes = Convert.ToInt32(RefreshMinutesBox.Text);
                	
				if (!String.IsNullOrEmpty(RefreshMinutesBox.Text))
                	autoRefreshSeconds = Convert.ToInt32(RefreshSecondsBox.Text);
                
				if (!String.IsNullOrEmpty(MaxPointsBox.Text))
                	maxPoints = Convert.ToInt32(MaxPointsBox.Text);
                
                Response.Redirect(
                	String.Format(
                		"{0}?MaxPoints={1}&AutoRef={2}&RefMin={3}&RefSec={4}",
                		Path.GetFileName(Request.PhysicalPath),
                		maxPoints,
                		autoRefresh,
                		autoRefreshMinutes,
                		autoRefreshSeconds
                	)
                );
			}
		
			string GetDataValues(string key)
			{
				var builder = new StringBuilder();
			
				builder.Append("type: \"line\",");
				builder.Append("xValueType: \"dateTime\",");
				builder.Append("dataPoints: [");
				
				int i = 0;
				
				if (!data.ContainsKey(key))
					throw new ArgumentException("No data found with the key: " + key);
				
				var currentData = data[key];
				
				foreach (var pair in currentData)
				{
					i++;
					
					var dateTime = DateTime.Now.AddSeconds(i);
					
					builder.Append("{ "); 
					
					try
					{
						dateTime = DateTime.Parse(pair.Key);
						
						var jsTimeStamp = dateTime.Subtract(
							new DateTime(1970,1,1,0,0,0))
	               			.TotalMilliseconds;
						
						builder.Append(String.Format("x: {0}, y: {1}", jsTimeStamp, pair.Value));
					}
					catch (Exception ex)
					{
						builder.Append(String.Format("x: {0}, y: {1}", i, pair.Value));
					//throw ex;
					}
					
					builder.Append("}");
					
					if (i < currentData.Count)
						builder.Append(",");
					builder.Append(Environment.NewLine);
				}
				
				builder.Append("]");
	            		return builder.ToString();
			}
			
			string GetChartScript(string label, string key, string containerId)
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
            		labelAngle: -60,
			        intervalType: ""minute""
			      },
			      axisY:{
			        includeZero: true
			        
			      },
			      data: [
			      {
			        " + GetDataValues(key) + @"
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
			  
			  	<%= GetChartScript("Temperature", "Tmp", "temperatureContainer") %>
			  	<%= GetChartScript("Humidity", "Hm", "humidityContainer") %>
			  	<%= GetChartScript("Moisture", "Mst", "moistureContainer") %>
			  	<%= GetChartScript("Light", "Lt", "lightContainer") %>
			  	<%= GetChartScript("Flow", "Fl", "flowContainer") %>

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
					<div id="flowContainer" style="height: 300px; width: 100%;">
		  			</div>
		  		</td>
			</tr>
			<tr>
				<td>
		  			<div id="humidityContainer" style="height: 300px; width: 100%;">
		  			</div>
		  		</td>
		  		<td>
		  		</td>
			</tr>
		</table>
		<div><b>Settings:</b></div>
		<p>
			Total data points available: <%= totalPoints %>
		</p>
		<p>
			Maximum data points to display:<br/>
			<span class="Desc">(The maximum number of points displayed on each graph. Choose fewer points to make the page load faster. Choose a greater number for more detail.)</span><br/>
			<asp:TextBox runat="server" id="MaxPointsBox" text='<%# maxPoints %>'></asp:TextBox><br/>
		</p>
		<p>
			Auto refresh: <asp:CheckBox runat="server" id="AutoRefreshCheckBox" Checked='<%# autoRefresh %>' onclientclick="document.getElementById('RefreshButton').click();" /><br/>
			[minutes:seconds]:
			 <asp:TextBox runat="server" id="RefreshMinutesBox" Text='<%# autoRefreshMinutes %>' width="30"></asp:TextBox>
			 :<asp:TextBox runat="server" id="RefreshSecondsBox" Text='<%# autoRefreshSeconds %>' width="30"></asp:TextBox>
		</p>
		<p>
			<asp:button runat="server" id="RefreshButton" text="Refresh" onclick="RefreshButton_Click" />
		</p>
		<p>
			Data capture running: <%= Application["IsCapturing"] != null && (bool)Application["IsCapturing"] ? "Yes" : "No" %><br/>
			<asp:button runat="server" id="CaptureButton" text="Start Data Capture" onclick="CaptureButton_Click" Enabled='<%# Application["IsCapturing"] == null || (bool)Application["IsCapturing"] == false %>'/><br/>
			<span class="Desc">(Launches 'captureSerial.sh' script to start the serial monitor, saving all data to the 'serialLog.txt' file. This page will load that data and display it each time it refreshes.)</span>
		</p>
		</form>
	</body>
</html>
