
/*

 Base class for CF_LBM_Observable Types
 The design behind them is you wont need to use NotifyPropertyChanged() upon setting a variable
 They are current used to support Collections (array, set, map)

Example: 


class TestController: CF_LBM_Controller
{
	// "LeftList" goes into Binding_Name
	ref CF_LBM_ObservableCollection<string> LeftList; 	
	
	// "LeftListSelection" goes into Selected_Item
	string LeftListSelection; 		
	
	void TestController()
	{
		LeftList = new CF_LBM_ObservableCollection<string>("LeftList", this);
		LeftList.Insert("Line1");
	}

	override void PropertyChanged(string propertyName)
	{
		switch (propertyName)
		{
			case "LeftListSelection": {
				Print("LeftList selection changed: " + LeftListSelection);
				break;
			}
		}
	}
}

*/

class CF_LBM_ObservableCollection<Class TValue> : CF_LBM_Observable
{
	private	ref array<autoptr TValue> _data = {};

	void CF_LBM_ObservableCollection(CF_LBM_Controller controller)
	{
		m_Type = CF_LBM_TemplateType<TValue>.GetType();
	}

	void ~CF_LBM_ObservableCollection()
	{
		delete _data;
	}

	int Insert(TValue value)
	{
		int index = _data.Insert(value);
		if (index != -1)
		{
			CollectionChanged(new CF_LBM_CollectionChangedEventArgs(this, CF_LBM_NotifyCollectionChangedAction.Insert, index, new Param1<TValue>(value)));
		}

		return index;
	}

	int InsertAt(TValue value, int index)
	{
		int new_index = _data.InsertAt(value, index);
		CollectionChanged(new CF_LBM_CollectionChangedEventArgs(this, CF_LBM_NotifyCollectionChangedAction.InsertAt, index, new Param1<TValue>(value)));
		return new_index;
	}

	// 0: TValue value
	// 1: int index
	int InsertAtEx(CF_LBM_TypeConverter typeConverter, int index)
	{
		TValue value; // scoofed
		g_Script.CallFunction(typeConverter, "GetData", value, null);
		Print(value);
		int new_index = _data.InsertAt(value, index);
		CollectionChanged(new CF_LBM_CollectionChangedEventArgs(this, CF_LBM_NotifyCollectionChangedAction.InsertAt, index, new Param1<TValue>(value)));
		return new_index;
	}

	void Remove(int index)
	{
		CollectionChanged(new CF_LBM_CollectionChangedEventArgs(this, CF_LBM_NotifyCollectionChangedAction.Remove, index, new Param1<TValue>(_data.Get(index))));
		_data.Remove(index);
	}

	void Remove(TValue value)
	{
		int index = _data.Find(value);
		if (index != -1)
			Remove(index);
	}

	void Set(int index, TValue value)
	{
		CollectionChanged(new CF_LBM_CollectionChangedEventArgs(this, CF_LBM_NotifyCollectionChangedAction.Replace, index, new Param1<TValue>(value)));
		_data.Set(index, value);
	}

	int MoveIndex(int index, int moveIndex)
	{
		TValue value = _data.Get(index);
		int new_index = _data.MoveIndex(index, moveIndex);
		CollectionChanged(new CF_LBM_CollectionChangedEventArgs(this, CF_LBM_NotifyCollectionChangedAction.Move, new_index, new Param1<TValue>(value)));
		return new_index;
	}

	void SwapItems(int itemA, int itemB)
	{
		_data.SwapItems(itemA, itemB);
		CollectionChanged(new CF_LBM_CollectionChangedEventArgs(this, CF_LBM_NotifyCollectionChangedAction.Swap, -1, new CF_LBM_CollectionSwapArgs(itemA, itemB)));
	}

	void Clear()
	{
		_data.Clear();
		CollectionChanged(new CF_LBM_CollectionChangedEventArgs(this, CF_LBM_NotifyCollectionChangedAction.Clear, -1, null));
	}

	TValue Get(int index)
	{
		return _data.Get(index);
	}

	override int Count()
	{
		return _data.Count();
	}

	int Find(TValue value)
	{
		Print(value);
		return _data.Find(value);
	}
};