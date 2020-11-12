static ref CF_LBM g_LayoutBindingManager;

class CF_LBM
{
	static int CF_LBM_LogLevel;
	
	private static void CheckLayoutBindingManager()
	{
		if (!g_LayoutBindingManager)
		{ 
			g_LayoutBindingManager = new CF_LBM();
		}
	}
	
	protected static ref CF_LBM_TypenameHashMap m_WidgetControllerHashMap;
	static CF_LBM_WidgetController GetWidgetController(Widget data) 
	{
		CheckLayoutBindingManager();
	
		CF_LBM_WidgetController widgetController = CF_LBM_WidgetController.Cast(m_WidgetControllerHashMap.Get(data.Type()).Spawn());
		g_Script.Call(widgetController, "SetWidget", data);
		return widgetController;
	}	
	
	protected static ref CF_LBM_TypeConversionHashMap m_TypeConverterHashMap;
	static CF_LBM_TypeConverter GetTypeConversion(typename type) 
	{
		CheckLayoutBindingManager();
				
		return CF_LBM_TypeConverter.Cast(m_TypeConverterHashMap[type].Spawn()); 
	}
	
	void CF_LBM()
	{
		Log("CF_LBM");
		
		if (!m_TypeConverterHashMap)
		{
			m_TypeConverterHashMap = new CF_LBM_TypeConversionHashMap();
			RegisterConversionTemplates(m_TypeConverterHashMap);
		}
		
		if (!m_WidgetControllerHashMap)
		{
			m_WidgetControllerHashMap = new CF_LBM_TypenameHashMap();
			RegisterWidgetControllers(m_WidgetControllerHashMap);
		}
	}
	
	void ~CF_LBM() 
	{ 
		Log("~CF_LBM"); 
	}
	
	// Override THIS to add your own Custom Conversion Templates
	// this determines how data is represented in the UI itself
	// i.e. you can assign a TextWidget to float, due to the TypeConversion's GetString()
	void RegisterConversionTemplates(out CF_LBM_TypeConversionHashMap type_conversions)
	{
		Log("CF_LBM::RegisterConversionTemplates");
		type_conversions.Insert(bool, CF_LBM_TypeConversionBool);
		type_conversions.Insert(int, CF_LBM_TypeConversionInt);
		type_conversions.Insert(float, CF_LBM_TypeConversionFloat);
		type_conversions.Insert(string, CF_LBM_TypeConversionString);
		type_conversions.Insert(vector, CF_LBM_TypeConversionVector);
		type_conversions.Insert(Widget, CF_LBM_TypeConversionWidget);
		type_conversions.Insert(Object, CF_LBM_TypeConversionObject);
		type_conversions.Insert(CF_LBM_Observable, CF_LBM_TypeConversionObservable);
		type_conversions.Insert(CF_LBM_ScriptedViewBase, CF_LBM_TypeConversionScriptView);
	}
	
	// Override THIS to add your own Widget widget_controllers 
	// this determins how the Widget controls the data sent to it
	// Great for prefabs
	void RegisterWidgetControllers(out CF_LBM_TypenameHashMap widget_controllers)
	{
		Log("CF_LBM::RegisterWidgetControllers");
		
		widget_controllers.Insert(Widget, CF_LBM_WidgetBaseController);
		widget_controllers.Insert(SpacerBaseWidget, CF_LBM_SpacerBaseWidgetController);
		widget_controllers.Insert(WrapSpacerWidget, CF_LBM_SpacerBaseWidgetController);
		widget_controllers.Insert(GridSpacerWidget, CF_LBM_SpacerBaseWidgetController);
		widget_controllers.Insert(ScrollWidget, CF_LBM_SpacerBaseWidgetController);

		widget_controllers.Insert(ButtonWidget, CF_LBM_ButtonWidgetController);
		widget_controllers.Insert(CheckBoxWidget, CF_LBM_CheckBoxWidgetController);
		widget_controllers.Insert(EditBoxWidget, CF_LBM_EditBoxWidgetController);
		widget_controllers.Insert(SliderWidget, CF_LBM_SliderWidgetController);
		widget_controllers.Insert(ProgressBarWidget, CF_LBM_ProgressBarController);
		widget_controllers.Insert(TextWidget, CF_LBM_TextWidgetController);
		widget_controllers.Insert(MultilineEditBoxWidget, CF_LBM_MultilineEditBoxWidgetController);
		widget_controllers.Insert(XComboBoxWidget, CF_LBM_XComboBoxWidgetController);
		widget_controllers.Insert(ImageWidget, CF_LBM_ImageWidgetController);
		widget_controllers.Insert(TextListboxWidget, CF_LBM_TextListboxController);
		widget_controllers.Insert(RichTextWidget, CF_LBM_TextWidgetController);
		widget_controllers.Insert(ItemPreviewWidget, CF_LBM_ItemPreviewWidgetController);
	}
	
	static void Trace(string message, string param1 = "", string param2 = "", string param3 = "", string param4 = "", string param5 = "", string param6 = "", string param7 = "", string param8 = "", string param9 = "")
	{
		if (CF_LBM_LogLevel <= 0)
			PrintFormat("LayoutBindingManagerLog::Trace %1", string.Format(message, param1, param2, param3, param4, param5, param6, param7, param8, param9));
	}
	
	static void Log(string message, string param1 = "", string param2 = "", string param3 = "", string param4 = "", string param5 = "", string param6 = "", string param7 = "", string param8 = "", string param9 = "")
	{
		if (CF_LBM_LogLevel <= 1)
			PrintFormat("LayoutBindingManagerLog::Log %1", string.Format(message, param1, param2, param3, param4, param5, param6, param7, param8, param9));
	}

	static void Error(string message, string param1 = "", string param2 = "", string param3 = "", string param4 = "", string param5 = "", string param6 = "", string param7 = "", string param8 = "", string param9 = "")
	{
		string msg = string.Format(message, param1, param2, param3, param4, param5, param6, param7, param8, param9);
		//PrintFormat("LayoutBindingManagerLog::Error %1", msg);
		Error2("CF_LBM Error", msg);
		
#ifdef COMPONENT_SYSTEM
		Workbench.Dialog("CF_LBM Error", msg);
#endif
	}

	// Loads .layout file Widgets into Properties of context (when they are the same name)
	/*

	Example:

	.layout file:
	MenuBarRoot		FrameWidget
		MenuBarFile   	ButtonWidget
			MenuBarFileLabel	TextWidget


	.c file:
	class TestClass
	{
		ButtonWidget MenuBarFile; //<-- these properties will be assigned
		private TextWidget MenuBarFileLabel;
	}

	*/
	static void LoadViewProperties(Class context, CF_LBM_PropertyTypeHashMap property_map, Widget root_widget)
	{
		foreach (string propertyName, typename property_type : property_map)
		{
			if (!property_type.IsInherited(Widget))
				continue;

			Widget target = root_widget.FindAnyWidget(propertyName);

			// fixes bug that breaks everything
			if (target && root_widget.GetName() != propertyName)
			{
				EnScript.SetClassVar(context, propertyName, 0, target);
				continue;
			}

			// Allows you to define the layout root aswell within it
			if (!target && root_widget.GetName() == propertyName)
			{
				EnScript.SetClassVar(context, propertyName, 0, root_widget);
				continue;
			}
		}
	}

	// 0: Context of Start Scope, out is context of final scope
	// 1: Name of variable string Ex: m_Binding.Value.Root
	// return: Final variable name
	static CF_LBM_PropertyInfo GetSubScope(out Class context, string name)
	{
		if (name == string.Empty)
			return null;

		TStringArray variable_scope = {};
		name.Split(".", variable_scope);

		for (int i = 0; i < variable_scope.Count() - 1; i++)
		{
			EnScript.GetClassVar(context, variable_scope[i], 0, context);
		}

		if (variable_scope.Count() == 1)
		{
			return CF_LBM_PropertyInfo.GetFromClass(context, name);
		}

		return CF_LBM_PropertyInfo.GetFromClass(context, variable_scope[variable_scope.Count() - 1]);
	}

	static Widget GetWidgetChildAtIndex(Widget widget, int index)
	{
		Widget result = widget.GetChildren();
		while (index > 0)
		{
			result = result.GetSibling();
			index--;
		}

		return result;
	}

	static bool IsWidgetChild(Widget widget, Widget child)
	{
		Widget result = widget.GetChildren();
		while (result != null)
		{
			if (result == child)
				return true;
				
			result = result.GetSibling();
		}

		return false;
	}
};

// Workbench safe variant of GetGame()
// https://feedback.bistudio.com/T153287
DayZGame GetWorkbenchGame() 
{
	// dont want to call this while actually in game
#ifdef COMPONENT_SYSTEM 
	if (!g_Game) {
		g_Game = new DayZGame();
	}
#endif
	
	return g_Game;
}