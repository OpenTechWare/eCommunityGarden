<%@ Page Language="C#" %>
<%@ Import namespace="GardenManager.Core" %>
<!DOCTYPE html>
<html>
<head runat="server">
	<link rel="stylesheet" type="text/css" href="css/style.css">
	<title>EditDevice</title>
	<script runat="server">
	DeviceId deviceId;
	string name = String.Empty;

	DataStore Store = new DataStore();

	void Page_Load(object sender, EventArgs e)
	{
		deviceId = DeviceId.Parse(Request.QueryString["id"]);

		name = Store.GetDeviceName(deviceId);

		DataBind();
	}

	void SubmitButton_Click(object sender, EventArgs e)
	{
		Store.SetDeviceName(deviceId, Name.Text);

		Response.Redirect("Default.aspx");
	}
	</script>
</head>
<body>
	<form id="form1" runat="server">
		<table width="400px">
			<tr><td colspan="2" class="hd">Edit Device</td></tr>
			<tr>
				<td>Device ID:</td>
				<td><%= deviceId.ToString() %></td>
			</tr>
			<tr>
				<td>Name:</td>
				<td><asp:textbox runat="Server" value='<%# name.ToString() %>' id="Name"></asp:textbox></td>
			</tr>
			<tr>
				<td></td><td><asp:button runat="server" id="SubmitButton" onClick="SubmitButton_Click" text="Save"></asp:button></td>
			</tr>
		</table>
	</form>
</body>
</html>

