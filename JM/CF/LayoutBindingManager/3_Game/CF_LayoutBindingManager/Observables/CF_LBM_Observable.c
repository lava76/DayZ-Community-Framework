
// Base class for all CF_LBM_Observable Types (CF_LBM_ObservableCollection, CF_LBM_ObservableSet, CF_LBM_ObservableDictionary)
class CF_LBM_Observable
{
	// Assigned on Constructor of super
	private	typename m_Type;

	protected CF_LBM_Controller m_Controller;

	void CF_LBM_Observable(CF_LBM_Controller controller)
	{
		m_Controller = controller;
	}

	protected void CollectionChanged(CF_LBM_CollectionChangedEventArgs args)
	{
		m_Controller.NotifyCollectionChanged(args);
	}

	typename GetType()
	{
		return m_Type;
	}

	CF_LBM_TypeConverter GetTypeConverter()
	{
		return CF_LBM.GetTypeConversion(m_Type);
	}

	// Abstract
	int Count();
};