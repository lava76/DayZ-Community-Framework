
// Abstract Class
// Inherit from this if you want to create UI elements with 100% script
/*

Example:

class CustomDialogWindow: CF_LBM_ScriptView
{
	override string GetLayoutFile() {
		return "MyMod/gui/Layouts/dialogs/Dialog.layout";
	}
}

	CustomDialogWindow window = new CustomDialogWindow();
	window.Show();
	....
	....
	window.Close();

*/

class CF_LBM_ScriptView : CF_LBM_ScriptedViewBase
{
	protected ref CF_LBM_Controller m_Controller;
	CF_LBM_Controller GetController()
	{
		return m_Controller;
	}

	// Maybe one day we'll get constructor overloading :)
	void CF_LBM_ScriptView()
	{
		m_LayoutRoot = CreateWidget(null);

		CF_LBM.LoadViewProperties(this, CF_LBM_PropertyTypeHashMap.FromType(Type()), m_LayoutRoot);

		m_LayoutRoot.GetScript(m_Controller);

		// If no Controller is specified in the WB Root
		if (!m_Controller || !m_Controller.IsInherited(CF_LBM_Controller))
		{
			Log("Controller not found on %1, creating...", m_LayoutRoot.GetName());
			if (!GetControllerType().IsInherited(CF_LBM_Controller))
			{
				Error("%1 is invalid. Must inherit from Controller!", GetControllerType().ToString());
				return;
			}

			if (!Class.CastTo(m_Controller, GetControllerType().Spawn()))
			{
				Error("Could not create Controller %1", GetControllerType().ToString());
				return;
			}

			// Since its not loaded in the WB, needs to be called here
			CF_LBM.LoadViewProperties(m_Controller, CF_LBM_PropertyTypeHashMap.FromType(GetControllerType()), m_LayoutRoot);
			m_Controller.OnWidgetScriptInit(m_LayoutRoot);
		}

		m_Controller.Debug_Logging = Debug_Logging;
		m_Controller.SetParent(this);
		//m_LayoutRoot.SetHandler(this);
	}

	void ~CF_LBM_ScriptView()
	{
		delete m_Controller;

		if (m_LayoutRoot)
		{
			Print("~" + m_LayoutRoot.GetName());
			m_LayoutRoot.Unlink();
		}
	}

	void SetParent(Widget parent)
	{
		m_LayoutRoot = CreateWidget(parent);
	}

	private	Widget CreateWidget(Widget parent)
	{
		Widget result;
		if (!GetLayoutFile())
		{
			Error("Layout file not found! Are you overriding GetLayoutFile?");
			return result;
		}

		WorkspaceWidget workspace = GetWorkbenchGame().GetWorkspace();
		if (!workspace)
		{
			Error("Workspace was null, try reloading Workbench");
			return result;
		}

		Log("Loading %1", GetLayoutFile());
		result = workspace.CreateWidgets(GetLayoutFile(), parent);
		if (!result)
		{
			Error("Invalid layout file %1", GetLayoutFile());
			return result;
		}

		return result;
	}

	// Useful if you want to set to an existing controller
	void SetController(CF_LBM_Controller controller)
	{
		m_Controller = controller;
		m_Controller.Debug_Logging = Debug_Logging;
		m_Controller.OnWidgetScriptInit(m_LayoutRoot);
		m_Controller.SetParent(this);
	}

	// Virtual Methods
	protected string GetLayoutFile();

	protected typename GetControllerType()
	{
		return CF_LBM_Controller;
	}
};