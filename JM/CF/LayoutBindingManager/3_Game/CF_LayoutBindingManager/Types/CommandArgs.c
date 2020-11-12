
// Base class for all CF_LBM_CommandArgs
class CF_LBM_CommandArgs
{
	CF_LBM_ScriptedViewBase Context;
};

class CF_LBM_CommandArgsT<Class T>: CF_LBM_CommandArgs
{
	T Source;
	
	void CF_LBM_CommandArgsT(T source)
	{
		Source = source;
	}
};

// 0: Source Widget
// 1: Mouse Button
class CF_LBM_ButtonCommandArgs: CF_LBM_CommandArgsT<ButtonWidget>
{
	private int m_MouseButton;
	
	void CF_LBM_ButtonCommandArgs(ButtonWidget source, int mouseButton)
	{
		m_MouseButton = mouseButton;
	}
	
	int GetMouseButton()
	{
		return m_MouseButton;
	}
	
	bool GetButtonState()
	{
		return Source.GetState();
	}
};


// 0: Source Widget
// 1: CheckBox State
class CF_LBM_CheckBoxCommandArgs: CF_LBM_CommandArgsT<CheckBoxWidget>
{	
	bool GetCheckBoxState()
	{
		return Source.IsChecked();
	}
};

// 0: Source Widget
// 1: Selected Item
class CF_LBM_XComboBoxCommandArgs: Param2<XComboBoxWidget, int>
{
	XComboBoxWidget GetComboBoxWidget()
	{
		return param1;
	}
	
	int GetSelectedIndex()
	{
		return param2;
	}
};

// 0: Source Widget
// 1: Selected Widget
class CF_LBM_WrapSpacerCommandArgs: Param2<WrapSpacerWidget, Widget>
{
	WrapSpacerWidget GetWrapSpacerWidget()
	{
		return param1;
	}
	
	Widget GetSelectedWidget()
	{
		return param2;
	}
};
