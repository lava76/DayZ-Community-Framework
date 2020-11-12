// Abstract Class
class CF_LBM_RelayCommand
{
	private	bool m_CanExecute = true;

	protected CF_LBM_Controller m_Controller;
	void SetController(CF_LBM_Controller controller)
	{
		m_Controller = controller;
	}

	protected CF_LBM_ViewBinding m_ViewBinding;
	void SetViewBinding(CF_LBM_ViewBinding viewBinding)
	{
		m_ViewBinding = viewBinding;
	}

	// returns bool defining whether or not the Execute can be called
	bool CanExecute()
	{
		return m_CanExecute;
	}

	// sets whether or not CF_LBM_RelayCommand can be executed
	// depreciate if BI ever adds property Getters and Setters
	void SetCanExecute(bool state)
	{
		m_CanExecute = state;
		thread CanExecuteChanged(m_CanExecute);
	}

	/* Abstract Methods */

	// called when type is Clicked, Selected, or Changed
	// Return: Handled (if false, will continue to execute up the tree)
	bool Execute(Class sender, CF_LBM_CommandArgs args);

	// Abstract function called when execution ability is changed
	void CanExecuteChanged(bool state);
};