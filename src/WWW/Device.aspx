<%@ Page Language="C#" %>
<%@ Import Namespace="System.Collections.Generic" %>
<%@ Import Namespace="System.IO" %>
<%@ Import Namespace="GardenManager.Core" %>
<%@ Import Namespace="GardenManager.Web.UI" %>
<!DOCTYPE html>
<html>
<head>
	<title>eCommunityGarden - Device</title>
	<link rel="stylesheet" type="text/css" href="css/style.css">
	<link rel="stylesheet" type="text/css" href="css/dstyle.css">
	<script runat="server">

		DeviceId deviceId;

		DataStore Store = new DataStore();

		void Page_Load(object sender, EventArgs e)
		{

			deviceId = DeviceId.Parse(Request.QueryString["id"]);

			if (!IsPostBack)
				DataBind();
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
				<% foreach (var sensorNumber in Store.GetSensorNumbers(deviceId)) { %>
				<div class="pnl" onclick="window.location.href = 'Graph.aspx?id=<%= deviceId.ToString() %>&s=<%= sensorNumber %>';">
				  <div class="hd"><%= sensorNumber %>. <%= SensorConfig.GetName(Store.GetSensorCode(deviceId, sensorNumber)) %></div>
				  <%= new Grapher{Width=150,Height=230}.GetGraphScript(deviceId, sensorNumber, 4) %>
				  <div class="bdy">
				    <span class="lval"><%= SensorConfig.GetValueText(Store.GetSensorCode(deviceId, sensorNumber), Store.GetLatestValue(deviceId, sensorNumber)) %></span>
				  </div>
				</div>
				<% } %>
		  </div>
  </form>
</body>
</html>
