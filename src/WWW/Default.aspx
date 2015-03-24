<%@ Page Language="C#" %>
<%@ Import Namespace="GardenManager.Core" %>
<%@ Import Namespace="GardenManager.Web.UI" %>
<!DOCTYPE html>
<html>
<head runat="server">
	<title>Default</title>
	<link rel="stylesheet" type="text/css" href="css/style.css">
	<link rel="stylesheet" type="text/css" href="css/istyle.css">
	<script runat="server">
	DeviceId[] deviceIds;
	DataStore Store = new DataStore();

	void Page_Load(object sender, EventArgs e)
	{
		var store = new DataStore();

		var conversion = new DataConversion(store);
		conversion.ConvertFileToData();

		deviceIds = store.GetDeviceIds();
	}

	string[] GetKeys(DeviceId deviceId)
	{
		return Store.GetDataKeys(deviceId);
	}

	string GetLatestValue(DeviceId deviceId, string key)
	{
		var value = Store.GetLatestValue(deviceId, key);

		return ValueHelper.GetValue(key, value);
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
</head>
<body>
	<form id="form1" runat="server">
		<div class="pghd">eCommunityGarden</div>
	<% foreach (var deviceId in deviceIds) { %>
	<div class="row">
		<div class="rowlbl" onclick="window.location.href = 'Device.aspx?id=<%= deviceId.ToString() %>';">
			<%= GetDeviceLabel(deviceId) %>
		</div>
		<div class="rowbtn">
			[<a href="EditDevice.aspx?id=<%= deviceId.ToString() %>">edit</a>]
		</div>
		<div>
		<% foreach (var key in GetKeys(deviceId)) { %>
		<div class="mpnl" onclick="window.location.href = 'Graph.aspx?id=<%= deviceId.ToString() %>&k=<%= key %>';">
		<div class="shd"><%= LabelHelper.GetLabel(key) %></div>

		<span class="mval"><%= GetLatestValue(deviceId, key) %></span>
		<span class="mgrph">
		<%= new Grapher{Height=80, Width=50, ScaleShowLabels=false}.GetGraphScript(deviceId, key, 2) %>
		</span>
		</div>
		<% } %>
		</div>
	</div>
	<% } %>
	</form>
</body>
</html>

