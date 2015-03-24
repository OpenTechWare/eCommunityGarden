<%@ Page Language="C#" %>
<%@ Import Namespace="System.Collections.Generic" %>
<%@ Import Namespace="System.IO" %>
<%@ Import Namespace="GardenManager.Core" %>
<%@ Import Namespace="GardenManager.Web.UI" %>
<!DOCTYPE html>
<html>
<head>
	<link rel="stylesheet" type="text/css" href="css/style.css">
	<link rel="stylesheet" type="text/css" href="css/dstyle.css">
	<script runat="server">

			int maxPoints = 10000;
			int totalPoints = 0;
			bool autoRefresh = true;

			DeviceId deviceId;

			DataStore Store = new DataStore();
			
			Dictionary<string, Dictionary<string, double>> data = new Dictionary<string, Dictionary<string, double>>();

			void Page_Load(object sender, EventArgs e)
			{

				//var keys = new string[]{ "Tmp", "Hm", "Lt", "Mst", "Fl" };

				var keys = new string[]{ "Tmp", "Lt", "Mst" };

				deviceId = DeviceId.Parse(Request.QueryString["id"]);

				var store = new DataStore();

				var conversion = new DataConversion(store);
				conversion.ConvertFileToData();

				foreach (var key in keys)
					data.Add(key, store.GetValues(deviceId, key));

				//totalPoints = parser.TotalPoints;

				if (!IsPostBack)
					DataBind();
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


		string[] getKeys(DeviceId deviceId)
		{
			return Store.GetDataKeys(deviceId);
		}
	</script>
    <script type="text/javascript" src='script.js'></script>
    <script type="text/javascript" src='Chart.min.js'></script>
    <script type="text/javascript" src='jquery-2.1.3.min.js'></script>
</head>
<body id="body">
    <form runat="server">
		<script type="text/javascript">
		$(document).ready(
		  function() {
		    var colorOrig=$('.pnl').css('background-color');
		    $('.pnl').hover(
		    function() {
		        //mouse over
		        $(this).css('background', '#F0F0F5');
		    }, function() {
		        //mouse out
		        $(this).css('background', colorOrig);
		    });
		}); 
		</script>
		<div class="pghd">eCommunityGarden  &raquo; <a href="Default.aspx">Home</a>  &raquo; Device <%= deviceId.ToString() %></div>
		<div class="buttons">
			<div class="bkpnl button" onclick="window.location.href='Default.aspx'">
			  &laquo; Back
			</div>
			<div id="RefreshButton" class="ref button" onclick="window.location.reload()">
		  		Refresh
			</div>
			<div class="ref button" onclick="toggleFullScreen()">
			  Full Screen
			</div>
		</div>
			<div class="panels">
				<% foreach (var key in getKeys(deviceId)) { %>
				<div class="pnl" onclick="window.location.href = 'Graph.aspx?id=<%= deviceId.ToString() %>&k=<%= key %>';">
				  <div class="hd"><%= LabelHelper.GetLabel(key) %></div>
				  <%= new Grapher{Width=150,Height=230}.GetGraphScript(deviceId, key, 4) %>
				  <div class="bdy">
				    <span class="lval"><%= getLatestValue(key) %>c</span>
				  </div>
				</div>
				<% } %>
				<div id="options">
					Auto refresh: <asp:CheckBox runat="server" id="AutoRefreshCheckBox" Checked='<%# autoRefresh %>' /><br/>
				</div>
		  </div>
  </form>
</body>
</html>
