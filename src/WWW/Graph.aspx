<%@ Page Language="C#" %>
<%@ Import Namespace="System.Collections.Generic" %>
<%@ Import Namespace="System.IO" %>
<%@ Import Namespace="GardenManager.Core" %>
<%@ Import Namespace="GardenManager.Web.UI" %>
<!DOCTYPE html>
<html>
<head>
	<title>eCommunityGarden - Device - Graph</title>
	<link rel="stylesheet" type="text/css" href="css/style.css">
	<link rel="stylesheet" type="text/css" href="css/gstyle.css">
	<script runat="server">
			int maxPoints = 10;
			int totalPoints = 0;
			
			Dictionary<string, Dictionary<string, double>> data = new Dictionary<string, Dictionary<string, double>>();

			string sensorKey = "";
			string sensorTitle = "";
			DeviceId deviceId;

			DateTime startTime = DateTime.MinValue;
			DateTime endTime = DateTime.MinValue;

			void Page_Load(object sender, EventArgs e)
			{
				sensorKey = Request.QueryString["k"];
				sensorTitle = Request.QueryString["t"];
				deviceId = DeviceId.Parse(Request.QueryString["id"]);

				var store = new DataStore();

				var conversion = new DataConversion(store);
				conversion.ConvertFileToData();


				startTime = GetStartTime();
				endTime = GetEndTime();

				data.Add(sensorKey, store.GetValues(deviceId, sensorKey, startTime, endTime));
			}

			DateTime GetStartTime()
			{
				var time = DateTime.Now;
				switch (DateRange.SelectedValue)
				{
					case "today":
						return new DateTime (DateTime.Now.Year, DateTime.Now.Month, DateTime.Now.Day);
						break;
					case "yesterday":
						return new DateTime (DateTime.Now.Year, DateTime.Now.Month, DateTime.Now.Day).AddDays(-1);
						break;
					case "week":
						return GetStartOfWeek(DateTime.Now, DayOfWeek.Monday);
						break;
					case "month":
						return new DateTime (DateTime.Now.Year, DateTime.Now.Month, 1);
						break;
					case "year":
						return new DateTime (DateTime.Now.Year, 1, 1);
						break;
				}

				return time;
			}

			DateTime GetEndTime()
			{
				var time = DateTime.Now;
				switch (DateRange.SelectedValue)
				{
					case "today":
						return new DateTime (DateTime.Now.Year, DateTime.Now.Month, DateTime.Now.Day).AddDays(1).AddSeconds(-1);
						break;
					case "yesterday":
						return new DateTime (DateTime.Now.Year, DateTime.Now.Month, DateTime.Now.Day).AddSeconds(-1);
						break;
					case "week":
						return GetStartOfWeek(DateTime.Now, DayOfWeek.Monday).AddDays(7).AddSeconds(-1);
						break;
					case "month":
						return new DateTime (DateTime.Now.Year, DateTime.Now.Month, 1).AddMonths(1).AddSeconds(-1);
						break;
					case "year":
						return new DateTime (DateTime.Now.Year, 1, 1).AddYears(1).AddSeconds(-1);
						break;
				}

				return time;
				
			}

			DateTime GetStartOfWeek(DateTime dt, DayOfWeek startOfWeek)
		    {
		        int diff = dt.DayOfWeek - startOfWeek;
		        if (diff < 0)
		        {
		            diff += 7;
		        }

		        return dt.AddDays(-1 * diff).Date;
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
    <script type="text/javascript" src='script.js'></script>
    <script type="text/javascript" src='Chart.min.js'></script>
    <script type="text/javascript" src='jquery-2.1.3.min.js'></script>
</head>
<body id="body">
    <form runat="server">
		<div class="pghd">eCommunityGarden &raquo; <a href="Default.aspx">Home</a>  &raquo; <a href="Device.aspx?id=<%= deviceId.ToString() %>">Device <%= deviceId.ToString() %></a>  &raquo; <%= LabelHelper.GetLabel(sensorKey) %></div>
		<div class="buttons">
			<div class="bkpnl button" onclick="window.location.href='Device.aspx?id=<%= deviceId.ToString() %>'">
			  &laquo; Back
			</div>
			<div class="bkpnl button" onclick="window.location.href='Default.aspx'">
			  Home
			</div>
			<div class="ref button" onclick="window.location.reload()">
			  Refresh
			</div>
		  <asp:DropDownList runat="server" cssclass="ddl" id="DateRange" AutoPostBack="true">
		  	<asp:ListItem Text="Today" Value="today"></asp:ListItem>
		  	<asp:ListItem Text="Yesterday" Value="yesterday"></asp:ListItem>
		  	<asp:ListItem Text="This Week" Value="week"></asp:ListItem>
		  	<asp:ListItem Text="This Month" Value="month"></asp:ListItem>
		  	<asp:ListItem Text="This Year" Value="year"></asp:ListItem>
		  </asp:DropDownList>
			<div class="ref button" onclick="toggleFullScreen()">
			  Full Screen
			</div>
		</div>
		<div class="grphpnl">
		  <div class="hd"><%= LabelHelper.GetLabel(sensorKey) %></div>
		  <%= new Grapher{Width=800,Height=500,StartTime=startTime,EndTime=endTime}.GetGraphScript(deviceId, sensorKey) %>
		  <div class="times">
		  <span style="float:left">From: <%= DateFormatter.Format(startTime) %></span>
		  <span style="float:right">To: <%= DateFormatter.Format(endTime) %></span></div>
		</div>
  </form>
</body>
</html>
