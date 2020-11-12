
// Essentially the same as CF_LBM_ScriptView except GetType is now a Template
class CF_LBM_ScriptViewTemplate<Class T> : CF_LBM_ScriptView
{
	protected T m_TemplateController;

	T GetTemplateController()
	{
		return m_TemplateController;
	}

	void CF_LBM_ScriptViewTemplate()
	{
		if (!Class.CastTo(m_TemplateController, m_Controller))
		{
			Error("Error in casting of Template Controller %1", T.ToString());
			return;
		}
	}

	private	override typename GetControllerType()
	{
		return T;
	}
};