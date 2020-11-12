
// Check JM/CF/GUI/layouts/sample_LayoutBindingManager2.layout
class SampleScriptViewController: CF_LBM_Controller
{
	Widget content_host;
	ref CF_LBM_ObservableCollection<ref SampleScriptView> script_view = new CF_LBM_ObservableCollection<ref SampleScriptView>(this);
}

class SampleRelayCommand: CF_LBM_RelayCommand
{
	override bool Execute(Class sender, CF_LBM_CommandArgs args)
	{ 
		Print("SampleRelayCommand Execute!");
		SampleScriptViewController controller = SampleScriptViewController.Cast(m_Controller);
				
		CF_LBM_ButtonCommandArgs commandArgs = args;
		if (commandArgs.GetButtonState()) {
			controller.script_view.Insert(new SampleScriptView());
		} else {
			delete controller.script_view;
			controller.script_view = new CF_LBM_ObservableCollection<ref SampleScriptView>(controller);
		}
		
		return true; // We dont want to keep calling up the Execution tree
	}
}


// This is just a wrapper for JM/CF/GUI/layouts/sample_LayoutBindingManager.layout
class SampleScriptView: CF_LBM_ScriptView
{
	override string GetLayoutFile() {
		return "JM/CF/GUI/layouts/sample_LayoutBindingManager.layout";
	}
	
	override typename GetControllerType() {
		return SampleScriptViewController;
	}
}