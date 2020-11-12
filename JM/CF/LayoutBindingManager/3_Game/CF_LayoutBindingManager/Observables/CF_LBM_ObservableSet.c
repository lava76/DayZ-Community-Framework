class CF_LBM_ObservableSet<Class TValue> : CF_LBM_Observable
{
	private	ref set<TValue> _data = new set<TValue>();

	int Insert(TValue value)
	{
		int index = _data.Insert(value);
		if (index != -1)
		{
			CollectionChanged(new CF_LBM_CollectionChangedEventArgs(this, CF_LBM_NotifyCollectionChangedAction.Add, index, new Param1<TValue>(value)));
		}

		return index;
	}

	int InsertAt(TValue value, int index)
	{
		int new_index = _data.InsertAt(value, index);
		CollectionChanged(new CF_LBM_CollectionChangedEventArgs(this, CF_LBM_NotifyCollectionChangedAction.Set, index, new Param1<TValue>(value)));
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
		CollectionChanged(new CF_LBM_CollectionChangedEventArgs(this, CF_LBM_NotifyCollectionChangedAction.Set, index, new Param1<TValue>(value)));
		_data.Set(index, value);
	}

	int Move(int index, int moveindex)
	{
		int new_index = _data.MoveIndex(index, moveindex);
		CollectionChanged(new CF_LBM_CollectionChangedEventArgs(this, CF_LBM_NotifyCollectionChangedAction.Move, index, new Param1<int>(new_index)));
		return new_index;
	}

	void Clear()
	{
		_data.Clear();
		CollectionChanged(new CF_LBM_CollectionChangedEventArgs(this, CF_LBM_NotifyCollectionChangedAction.Clear, -1, null));
	}

	TValue Get(int index)
	{
		TValue v = _data.Get(index);
		if (!v)
		{
			CF_LBM.Error(string.Format("CF_LBM_ObservableCollection.Get returned null! Index: %1 Name: %2", index, m_VariableName));
		}

		return v;
	}

	override int Count()
	{
		return _data.Count();
	}

	int Find(TValue value)
	{
		return _data.Find(value);
	}

	override typename GetType()
	{
		return TValue;
	}
};