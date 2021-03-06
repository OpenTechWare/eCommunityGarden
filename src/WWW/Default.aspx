﻿<%@ Page Language="C#" %>
<%@ Import Namespace="GardenManager.Core" %>
<%@ Import Namespace="GardenManager.Web.UI" %>
<!DOCTYPE html>
<html>
<head runat="server">
	<title>eCommunityGarden</title>
	<link rel="stylesheet" type="text/css" href="css/style.css">
	<link rel="stylesheet" type="text/css" href="css/istyle.css">
	<script runat="server">
	DeviceId[] deviceIds;
	DataStore Store = new DataStore();

	void Page_Load(object sender, EventArgs e)
	{
		var store = new DataStore();

		deviceIds = store.GetDeviceIds();
	}

	int[] GetSensorNumbers(DeviceId deviceId)
	{
		return Store.GetSensorNumbers(deviceId);
	}

	string GetLatestValue(DeviceId deviceId, int sensorNumber)
	{
		var value = Store.GetLatestValue(deviceId, sensorNumber);

		return SensorConfig.GetValueText(Store.GetSensorCode(deviceId, sensorNumber), value);
	}

	string GetDeviceLabel(DeviceId deviceId)
	{
		var title = "";
		var description = "";

		var name = Store.GetDeviceName(deviceId);

		if (name != String.Empty)
		{
			title = name;
			description = "Device " + deviceId.ToString();
		}
		else
			title = "Device " + deviceId.ToString();

		return "<div class='rhd'>" + title + @"</div>
			<div class='desc'>" + description + "</div>";
	}

	</script>
    <script type="text/javascript" src='Chart.min.js'></script>
    <script type="text/javascript" src='script.js'></script>
</head>
<body>
	<form id="form1" runat="server">
		<div class="pghd">eCommunityGarden</div>
		<div class="buttons">
			<div id="RefreshButton" class="ref button" onclick="window.location.reload()">
		  		Refresh
			</div>
			<div class="ref button" onclick="toggleFullScreen()">
			  Full Screen
			</div>
		</div>
	<% foreach (var deviceId in deviceIds) { %>
	<div class="row">
		<div class="rowlbl" onclick="window.location.href = 'Device.aspx?id=<%= deviceId.ToString() %>';">
			<%= GetDeviceLabel(deviceId) %>
		</div>
		<div class="rowbtn">
			[<a href="EditDevice.aspx?id=<%= deviceId.ToString() %>">edit</a>]
		</div>
		<div>
		<% foreach (var sensorNumber in GetSensorNumbers(deviceId)) { %>
		<div class="mpnl" onclick="window.location.href = 'Graph.aspx?id=<%= deviceId.ToString() %>&s=<%= sensorNumber %>';">
		<div class="shd"><%= sensorNumber %>. <%= SensorConfig.GetName(Store.GetSensorCode(deviceId, sensorNumber)) %></div>

		<span class="mval"><%= GetLatestValue(deviceId, sensorNumber) %></span>
		<span class="mgrph">
		<%= new Grapher{Height=80, Width=50, ScaleShowLabels=false}.GetGraphScript(deviceId, sensorNumber, 2) %>
		</span>
		</div>
		<% } %>
		</div>
	</div>
	<% } %>
	<% if (deviceIds.Length == 0){ %>
	<p>No devices detected. Please ensure they're running.</p>
	<% } %>
	</form>
</body>
</html>

